/**
 ******************************************************************************
 * @file    as7341.c
 * @brief   Driver for AMS AS7341 11-channel spectral sensor (STM32 HAL)
 ******************************************************************************
 */
#include "as7341.h"
#include <string.h>

/* ===========================================================================
 * Register map (high bank, accessible when CONFIG0.REG_BANK = 0)
 * =========================================================================== */
#define REG_ENABLE          0x80
#define REG_ATIME           0x81
#define REG_WTIME           0x83
#define REG_AUXID           0x90
#define REG_REVID           0x91
#define REG_ID              0x92
#define REG_STATUS          0x93
#define REG_ASTATUS         0x94
#define REG_CH0_DATA_L      0x95   /* CH0..CH5 ADC data, 12 bytes total */
#define REG_STATUS2         0xA3
#define REG_CFG0            0xA9
#define REG_CFG1            0xAA
#define REG_CFG6            0xAF
#define REG_ASTEP_L         0xCA
#define REG_ASTEP_H         0xCB

/* ENABLE register bits */
#define EN_PON              (1U << 0)   /* power on              */
#define EN_SP_EN            (1U << 1)   /* spectral measurement  */
#define EN_SMUXEN           (1U << 4)   /* start SMUX command    */

/* CFG0 bits */
#define CFG0_REG_BANK       (1U << 4)   /* 1 -> low bank (0x60..0x74) */

/* CFG6: SMUX command at bits [4:3] */
#define CFG6_SMUX_CMD_WRITE (0x02U << 3)

/* STATUS2 */
#define STATUS2_AVALID      (1U << 6)   /* spectral measurement complete */

#define PART_ID_EXPECTED    0x09        /* AS7341 device ID, in bits[7:2] of REG_ID */

#define I2C_TIMEOUT_MS      100
#define DATA_READY_TIMEOUT  500

/* ===========================================================================
 * Low-level I2C helpers
 * =========================================================================== */

static AS7341_Status_t i2c_write_u8(AS7341_Handle_t *dev, uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = { reg, val };
    if (HAL_I2C_Master_Transmit(dev->hi2c, AS7341_I2C_ADDR_8BIT,
                                buf, 2, I2C_TIMEOUT_MS) != HAL_OK) {
        return AS7341_ERR_I2C;
    }
    return AS7341_OK;
}

static AS7341_Status_t i2c_read(AS7341_Handle_t *dev, uint8_t reg,
                                uint8_t *buf, uint16_t len)
{
    if (HAL_I2C_Master_Transmit(dev->hi2c, AS7341_I2C_ADDR_8BIT,
                                &reg, 1, I2C_TIMEOUT_MS) != HAL_OK) {
        return AS7341_ERR_I2C;
    }
    if (HAL_I2C_Master_Receive (dev->hi2c, AS7341_I2C_ADDR_8BIT,
                                buf, len, I2C_TIMEOUT_MS) != HAL_OK) {
        return AS7341_ERR_I2C;
    }
    return AS7341_OK;
}

static AS7341_Status_t i2c_read_u8(AS7341_Handle_t *dev, uint8_t reg, uint8_t *val)
{
    return i2c_read(dev, reg, val, 1);
}

/* Read-modify-write a single bit/mask in a register */
static AS7341_Status_t reg_update(AS7341_Handle_t *dev, uint8_t reg,
                                  uint8_t mask, uint8_t value)
{
    uint8_t v;
    AS7341_Status_t s = i2c_read_u8(dev, reg, &v);
    if (s != AS7341_OK) return s;
    v = (v & ~mask) | (value & mask);
    return i2c_write_u8(dev, reg, v);
}

/* ===========================================================================
 * SMUX configuration (mapping of photodiodes to the 6 ADC channels).
 * These byte sequences come straight from the AS7341 application note;
 * do not edit unless you know what you're doing.
 * =========================================================================== */

static AS7341_Status_t smux_config_low_channels(AS7341_Handle_t *dev)
{
    /* F1-F4, CLEAR, NIR -> ADC0..ADC5 */
    static const uint8_t map[20] = {
        0x30, 0x01, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x50, 0x00,
        0x00, 0x00, 0x20, 0x04, 0x00, 0x30, 0x01, 0x50, 0x00, 0x06
    };
    for (uint8_t i = 0; i < 20; i++) {
        AS7341_Status_t s = i2c_write_u8(dev, i, map[i]);
        if (s != AS7341_OK) return s;
    }
    return AS7341_OK;
}

static AS7341_Status_t smux_config_high_channels(AS7341_Handle_t *dev)
{
    /* F5-F8, CLEAR, NIR -> ADC0..ADC5 */
    static const uint8_t map[20] = {
        0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x10, 0x03, 0x50, 0x10,
        0x03, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x50, 0x00, 0x06
    };
    for (uint8_t i = 0; i < 20; i++) {
        AS7341_Status_t s = i2c_write_u8(dev, i, map[i]);
        if (s != AS7341_OK) return s;
    }
    return AS7341_OK;
}

/* Issue SMUX_CMD_WRITE then trigger SMUXEN and wait for the chip to clear it. */
static AS7341_Status_t smux_apply(AS7341_Handle_t *dev)
{
    /* SMUX command = WRITE */
    AS7341_Status_t s = i2c_write_u8(dev, REG_CFG6, CFG6_SMUX_CMD_WRITE);
    if (s != AS7341_OK) return s;

    /* set SMUXEN; chip clears it when done */
    s = reg_update(dev, REG_ENABLE, EN_SMUXEN, EN_SMUXEN);
    if (s != AS7341_OK) return s;

    uint32_t t0 = HAL_GetTick();
    while ((HAL_GetTick() - t0) < DATA_READY_TIMEOUT) {
        uint8_t en;
        s = i2c_read_u8(dev, REG_ENABLE, &en);
        if (s != AS7341_OK) return s;
        if ((en & EN_SMUXEN) == 0) return AS7341_OK;
        HAL_Delay(1);
    }
    return AS7341_ERR_TO;
}

/* ===========================================================================
 * Spectral acquisition for the currently selected SMUX bank.
 * Reads 12 bytes starting at CH0_DATA_L into ch[0..5].
 * =========================================================================== */

static AS7341_Status_t spectral_acquire(AS7341_Handle_t *dev, uint16_t ch[6])
{
    /* enable spectral measurement */
    AS7341_Status_t s = reg_update(dev, REG_ENABLE, EN_SP_EN, EN_SP_EN);
    if (s != AS7341_OK) return s;

    /* poll AVALID */
    uint32_t t0 = HAL_GetTick();
    while ((HAL_GetTick() - t0) < DATA_READY_TIMEOUT) {
        uint8_t st2;
        s = i2c_read_u8(dev, REG_STATUS2, &st2);
        if (s != AS7341_OK) return s;
        if (st2 & STATUS2_AVALID) break;
        HAL_Delay(1);
    }
    if ((HAL_GetTick() - t0) >= DATA_READY_TIMEOUT) return AS7341_ERR_TO;

    /* read 12 bytes (CH0..CH5, little-endian) */
    uint8_t raw[12];
    s = i2c_read(dev, REG_CH0_DATA_L, raw, 12);
    if (s != AS7341_OK) return s;

    for (int i = 0; i < 6; i++) {
        ch[i] = (uint16_t)raw[2*i] | ((uint16_t)raw[2*i + 1] << 8);
    }

    /* disable spectral measurement before reconfiguring SMUX */
    return reg_update(dev, REG_ENABLE, EN_SP_EN, 0);
}

/* ===========================================================================
 * Public API
 * =========================================================================== */

AS7341_Status_t AS7341_ReadPartID(AS7341_Handle_t *dev, uint8_t *id)
{
    if (!dev || !id) return AS7341_ERR_ARG;
    return i2c_read_u8(dev, REG_ID, id);
}

AS7341_Status_t AS7341_PowerOn(AS7341_Handle_t *dev)
{
    if (!dev) return AS7341_ERR_ARG;
    return reg_update(dev, REG_ENABLE, EN_PON, EN_PON);
}

AS7341_Status_t AS7341_PowerOff(AS7341_Handle_t *dev)
{
    if (!dev) return AS7341_ERR_ARG;
    return reg_update(dev, REG_ENABLE, EN_PON, 0);
}

AS7341_Status_t AS7341_SetGain(AS7341_Handle_t *dev, AS7341_Gain_t gain)
{
    if (!dev) return AS7341_ERR_ARG;
    AS7341_Status_t s = i2c_write_u8(dev, REG_CFG1, (uint8_t)gain & 0x1F);
    if (s == AS7341_OK) dev->gain = gain;
    return s;
}

AS7341_Status_t AS7341_Init(AS7341_Handle_t   *dev,
                            I2C_HandleTypeDef *hi2c,
                            uint8_t            atime,
                            uint16_t           astep,
                            AS7341_Gain_t      gain)
{
    if (!dev || !hi2c) return AS7341_ERR_ARG;

    dev->hi2c       = hi2c;
    dev->atime      = atime;
    dev->astep      = astep;
    dev->gain       = gain;
    dev->async_state = AS7341_ASYNC_START_LOW;
    dev->async_timer = 0;

    HAL_Delay(3);   /* power-up margin */

    /* sanity check: PART_ID upper 6 bits must equal 0x09 */
    uint8_t id;
    AS7341_Status_t s = AS7341_ReadPartID(dev, &id);
    if (s != AS7341_OK)               return s;
    if (((id >> 2) & 0x3F) != PART_ID_EXPECTED) return AS7341_ERR_ID;

    /* power on */
    s = AS7341_PowerOn(dev);                       if (s != AS7341_OK) return s;

    /* integration time */
    s = i2c_write_u8(dev, REG_ATIME,   atime);     if (s != AS7341_OK) return s;
    s = i2c_write_u8(dev, REG_ASTEP_L, (uint8_t)(astep & 0xFF));
    if (s != AS7341_OK) return s;
    s = i2c_write_u8(dev, REG_ASTEP_H, (uint8_t)(astep >> 8));
    if (s != AS7341_OK) return s;

    /* gain */
    return AS7341_SetGain(dev, gain);
}

AS7341_Status_t AS7341_ReadSpectral(AS7341_Handle_t       *dev,
                                    AS7341_SpectralData_t *out)
{
    if (!dev || !out) return AS7341_ERR_ARG;

    uint16_t ch[6];

    /* ---- LOW channels: F1, F2, F3, F4, CLEAR, NIR ---- */
    AS7341_Status_t s;
    s = reg_update(dev, REG_ENABLE, EN_SP_EN, 0);  if (s != AS7341_OK) return s;
    s = smux_config_low_channels(dev);             if (s != AS7341_OK) return s;
    s = smux_apply(dev);                           if (s != AS7341_OK) return s;
    s = spectral_acquire(dev, ch);                 if (s != AS7341_OK) return s;
    out->f1    = ch[0];
    out->f2    = ch[1];
    out->f3    = ch[2];
    out->f4    = ch[3];
    out->clear = ch[4];
    out->nir   = ch[5];

    /* ---- HIGH channels: F5, F6, F7, F8, CLEAR, NIR ---- */
    s = smux_config_high_channels(dev);            if (s != AS7341_OK) return s;
    s = smux_apply(dev);                           if (s != AS7341_OK) return s;
    s = spectral_acquire(dev, ch);                 if (s != AS7341_OK) return s;
    out->f5 = ch[0];
    out->f6 = ch[1];
    out->f7 = ch[2];
    out->f8 = ch[3];

    return AS7341_OK;
}

bool AS7341_ReadSpectral_NonBlocking(AS7341_Handle_t *dev, AS7341_SpectralData_t *out)
{
    if (!dev || !out) return false;

    uint8_t st2 = 0;
    uint8_t raw[12];
    uint16_t ch[6];

    switch (dev->async_state) {
        
        case AS7341_ASYNC_START_LOW:
            // Disabilita misura precedente per sicurezza, configura canali bassi ed applica lo SMUX
            reg_update(dev, REG_ENABLE, EN_SP_EN, 0);
            if (smux_config_low_channels(dev) != AS7341_OK) return false;
            if (smux_apply(dev) != AS7341_OK) return false;
            
            // Triggera la misura hardware reale
            if (reg_update(dev, REG_ENABLE, EN_SP_EN, EN_SP_EN) != AS7341_OK) return false;
            
            dev->async_timer = HAL_GetTick();
            dev->async_state = AS7341_ASYNC_WAIT_LOW;
            break;

        case AS7341_ASYNC_WAIT_LOW:
            // Interroghiamo il registro di stato (Zero cicli bloccanti)
            if (i2c_read_u8(dev, REG_STATUS2, &st2) == AS7341_OK && (st2 & STATUS2_AVALID)) {
                // I dati LOW sono pronti sul silicio, leggiamoli
                if (i2c_read(dev, REG_CH0_DATA_L, raw, 12) == AS7341_OK) {
                    for (int i = 0; i < 6; i++) {
                        ch[i] = (uint16_t)raw[2*i] | ((uint16_t)raw[2*i + 1] << 8);
                    }
                    out->f1    = ch[0];
                    out->f2    = ch[1];
                    out->f3    = ch[2];
                    out->f4    = ch[3];
                    out->clear = ch[4];
                    out->nir   = ch[5];
                }
                dev->async_state = AS7341_ASYNC_START_HIGH;
            } else if ((HAL_GetTick() - dev->async_timer) > DATA_READY_TIMEOUT) {
                dev->async_state = AS7341_ASYNC_START_LOW; // Timeout di emergenza, resetta macchina
            }
            break;

        case AS7341_ASYNC_START_HIGH:
            // Spegne misura precedente, configura canali alti ed applica lo SMUX
            reg_update(dev, REG_ENABLE, EN_SP_EN, 0);
            if (smux_config_high_channels(dev) != AS7341_OK) return false;
            if (smux_apply(dev) != AS7341_OK) return false;
            
            // Triggera la seconda misura hardware
            if (reg_update(dev, REG_ENABLE, EN_SP_EN, EN_SP_EN) != AS7341_OK) return false;
            
            dev->async_timer = HAL_GetTick();
            dev->async_state = AS7341_ASYNC_WAIT_HIGH;
            break;

        case AS7341_ASYNC_WAIT_HIGH:
            // Interroghiamo lo stato dei canali alti
            if (i2c_read_u8(dev, REG_STATUS2, &st2) == AS7341_OK && (st2 & STATUS2_AVALID)) {
                // I dati HIGH sono pronti, li leggiamo al volo
                if (i2c_read(dev, REG_CH0_DATA_L, raw, 12) == AS7341_OK) {
                    for (int i = 0; i < 6; i++) {
                        ch[i] = (uint16_t)raw[2*i] | ((uint16_t)raw[2*i + 1] << 8);
                    }
                    out->f5 = ch[0];
                    out->f6 = ch[1];
                    out->f7 = ch[2];
                    out->f8 = ch[3];
                }
                // Disabilita la misura spettrale per bloccare l'hardware fino al prossimo start
                reg_update(dev, REG_ENABLE, EN_SP_EN, 0);
                
                dev->async_state = AS7341_ASYNC_START_LOW; // Prepariamo la macchina per il prossimo giro
                return true; // !!! GIRO COMPLETO EFFETTUATO, DATI AGGIORNATI !!!
            } else if ((HAL_GetTick() - dev->async_timer) > DATA_READY_TIMEOUT) {
                dev->async_state = AS7341_ASYNC_START_LOW;
            }
            break;
    }

    return false; // Conversione ancora in corso
}