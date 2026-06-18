/**
 ******************************************************************************
 * @file    as7341.c
 * @brief   Clean, interrupt-driven driver for AMS AS7341 with explicit 
 * I2C register pointer synchronization resets.
 ******************************************************************************
 */
#include "as7341.h"

/* Registers */
#define REG_ENABLE          0x80
#define REG_ATIME           0x81
#define REG_STATUS          0x93
#define REG_STATUS2         0xA3
#define REG_CFG0            0xA9
#define REG_CFG1            0xAA
#define REG_CFG6            0xAF
#define REG_ASTEP_L         0xCA
#define REG_ASTEP_H         0xCB
#define REG_FD_STATUS       0xDB
#define REG_INTENAB         0xF9
#define REG_CH0_DATA_L      0x95

/* Register bits */
#define EN_PON              (1U << 0)
#define EN_SP_EN            (1U << 1)
#define EN_FDEN             (1U << 6)
#define EN_SMUXEN           (1U << 4)

#define STATUS2_AVALID      (1U << 6) /* Spectral Data Valid Flag */

#define INT_SIEN            (1U << 0) 
#define INT_SPIEN           (1U << 3) 
#define CFG6_SMUX_CMD_WRITE (0x02U << 3)

#define EXPECTED_PART_ID    0x09
#define I2C_TIMEOUT         100

/* Static SMUX Mapping Definitions */
static const uint8_t smux_low_map[20] = {
    0x30, 0x01, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x50, 0x00,
    0x00, 0x00, 0x20, 0x04, 0x00, 0x30, 0x01, 0x50, 0x00, 0x06
};

static const uint8_t smux_high_map[20] = {
    0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x10, 0x03, 0x00, 0x10,
    0x03, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x05
};

/* --- Low-level functional helpers --- */

static AS7341_Status_t write_reg(AS7341_Handle_t *dev, uint8_t reg, uint8_t val)
{
    uint8_t buffer[2] = { reg, val };
    if (HAL_I2C_Master_Transmit(dev->hi2c, AS7341_I2C_ADDR_8BIT, buffer, 2, I2C_TIMEOUT) != HAL_OK) {
        return AS7341_ERR_I2C;
    }
    return AS7341_OK;
}

static AS7341_Status_t read_regs(AS7341_Handle_t *dev, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (HAL_I2C_Mem_Read(dev->hi2c, AS7341_I2C_ADDR_8BIT, reg, I2C_MEMADD_SIZE_8BIT, data, len, I2C_TIMEOUT) != HAL_OK) {
        return AS7341_ERR_I2C;
    }
    return AS7341_OK;
}

static AS7341_Status_t configure_smux_and_start(AS7341_Handle_t *dev, const uint8_t *smux_map)
{
    AS7341_Status_t status;
    uint8_t base_enable = EN_PON | (dev->flicker_enabled ? EN_FDEN : 0);

    /* 1. Stop conversion engines safely */
    status = write_reg(dev, REG_ENABLE, base_enable);
    if (status != AS7341_OK) return status;
    
    for (volatile uint32_t i = 0; i < 800; i++) { __NOP(); }

    /* 2. Configure SMUX registers (0x00 to 0x13) directly */
    for (uint8_t i = 0; i < 20; i++) {
        uint8_t val = smux_map[i];
        if (i == 19) {
            if (dev->flicker_enabled) {
                // If flicker is enabled, ADC5 is dedicated to flicker (high nibble = 6)
                // Cycle 1: smux_low_map has 0x06. We want 0x60 (Flicker on ADC5, NIR disabled).
                // Cycle 2: smux_high_map has 0x05. We want 0x65 (Flicker on ADC5, NIR on ADC4).
                val = (smux_map == smux_low_map) ? 0x60 : 0x65;
            } else {
                // If flicker is disabled, Cycle 1: 0x06 (NIR on ADC5). Cycle 2: 0x05 (NIR on ADC4).
                val = (smux_map == smux_low_map) ? 0x06 : 0x05;
            }
        }
        if (write_reg(dev, i, val) != AS7341_OK) return AS7341_ERR_I2C;
    }

    /* 3. Apply SMUX configuration mapping */
    status = write_reg(dev, REG_CFG6, CFG6_SMUX_CMD_WRITE);
    if (status != AS7341_OK) return status;

    status = write_reg(dev, REG_ENABLE, base_enable | EN_SMUXEN);
    if (status != AS7341_OK) return status;

    /* Block briefly for configuration propagation */
    uint8_t reg_en = EN_SMUXEN;
    while (reg_en & EN_SMUXEN) {
        if (read_regs(dev, REG_ENABLE, &reg_en, 1) != AS7341_OK) return AS7341_ERR_I2C;
    }

    for (volatile uint32_t i = 0; i < 800; i++) { __NOP(); }

    /* 4. Arm integration step and start conversions */
    return write_reg(dev, REG_ENABLE, base_enable | EN_SP_EN);
}

static AS7341_Flicker_t read_flicker_status(AS7341_Handle_t *dev)
{
    uint8_t fd_status = 0;
    if (read_regs(dev, REG_FD_STATUS, &fd_status, 1) != AS7341_OK) {
        return AS734_FLICKER_UNKNOWN;
    }

    if (fd_status & (1U << 5)) { 
        write_reg(dev, REG_FD_STATUS, (1U << 5)); 

        if (fd_status & (1U << 0)) return AS734_FLICKER_100HZ; 
        if (fd_status & (1U << 1)) return AS734_FLICKER_120HZ; 
        
        return AS734_FLICKER_NONE;
    }
    return AS734_FLICKER_UNKNOWN;
}

/* --- Public Core API Functions --- */

AS7341_Status_t AS7341_Init(AS7341_Handle_t *dev, I2C_HandleTypeDef *hi2c, 
                            uint8_t atime, uint16_t astep, AS7341_Gain_t gain,
                            bool enable_flicker)
{
    if (!dev || !hi2c) return AS7341_ERR_ARG;

    dev->hi2c            = hi2c;
    dev->atime           = atime;
    dev->astep           = astep;
    dev->gain            = gain;
    dev->flicker_enabled = enable_flicker;
    dev->state           = AS7341_STATE_IDLE;

    HAL_Delay(5);

    uint8_t chip_id = 0;
    if (read_regs(dev, 0x92, &chip_id, 1) != AS7341_OK) return AS7341_ERR_I2C;
    if (((chip_id >> 2) & 0x3F) != EXPECTED_PART_ID) return AS7341_ERR_ID;

    if (write_reg(dev, REG_ATIME, atime) != AS7341_OK) return AS7341_ERR_I2C;
    if (write_reg(dev, REG_ASTEP_L, (uint8_t)(astep & 0xFF)) != AS7341_OK) return AS7341_ERR_I2C;
    if (write_reg(dev, REG_ASTEP_H, (uint8_t)(astep >> 8)) != AS7341_OK) return AS7341_ERR_I2C;
    if (write_reg(dev, REG_CFG1, (uint8_t)gain & 0x1F) != AS7341_OK) return AS7341_ERR_I2C;

    /* Route only Spectral Ready signal to the physical INT line */
    uint8_t int_mask = INT_SPIEN;
    if (write_reg(dev, REG_INTENAB, int_mask) != AS7341_OK) return AS7341_ERR_I2C;

    uint8_t initial_enable = EN_PON | (enable_flicker ? EN_FDEN : 0);
    return write_reg(dev, REG_ENABLE, initial_enable);
}

AS7341_Status_t AS7341_StartRead(AS7341_Handle_t *dev)
{
    if (!dev) return AS7341_ERR_ARG;
    
    dev->state = AS7341_STATE_WAITING_LOW;
    
    /* Ensure the data registers pointer bank is explicitly selected */
    write_reg(dev, REG_CFG0, 0x00);
    write_reg(dev, REG_STATUS, 0xFF); 
    
    return configure_smux_and_start(dev, smux_low_map);
}

bool AS7341_IRQHandler(AS7341_Handle_t *dev, AS7341_SpectralData_t *out)
{
    if (!dev || !out || dev->state == AS7341_STATE_IDLE) return false;

    // 1. Check data validity first to prevent empty reads
    uint8_t status2_reg = 0;
    if (read_regs(dev, REG_STATUS2, &status2_reg, 1) != AS7341_OK) return false;
    if (!(status2_reg & STATUS2_AVALID)) {
        return false;
    }

    uint8_t raw_data[13];
    uint16_t adc[6];

    if (dev->state == AS7341_STATE_WAITING_LOW) {
        /* --- CYCLE 1: LOW BANK DONE --- */
        // Read 13 bytes starting from ASTATUS (0x94) to trigger latching
        if (read_regs(dev, 0x94, raw_data, 13) == AS7341_OK) {
            for (int i = 0; i < 6; i++) {
                adc[i] = (uint16_t)raw_data[2 * i + 1] | ((uint16_t)raw_data[2 * i + 2] << 8);
            }
            out->f1    = adc[0];
            out->f2    = adc[1];
            out->f3    = adc[2];
            out->f4    = adc[3];
            out->clear = adc[4];
            // adc[5] is ignored here because ADC5 is dedicated to the flicker engine
        }
        
        /* Transition immediately to High Bank configuration */
        dev->state = AS7341_STATE_WAITING_HIGH;

        /* Clear the sensor's outstanding status flags to let the active-low INT pin go back high */
        uint8_t status_reg = 0;
        read_regs(dev, REG_STATUS, &status_reg, 1);
        write_reg(dev, REG_STATUS, status_reg);

        if (configure_smux_and_start(dev, smux_high_map) != AS7341_OK) {
            dev->state = AS7341_STATE_IDLE; 
        }
        
        return false; 
    } 
    else if (dev->state == AS7341_STATE_WAITING_HIGH) {
        /* --- CYCLE 2: HIGH BANK DONE --- */
        // Read 13 bytes starting from ASTATUS (0x94) to trigger latching
        if (read_regs(dev, 0x94, raw_data, 13) == AS7341_OK) {
            for (int i = 0; i < 6; i++) {
                adc[i] = (uint16_t)raw_data[2 * i + 1] | ((uint16_t)raw_data[2 * i + 2] << 8);
            }
            out->f5  = adc[0];
            out->f6  = adc[1];
            out->f7  = adc[2];
            out->f8  = adc[3];
            out->nir = adc[4]; // Successfully extract your isolated NIR channel from ADC4!
        }

        if (dev->flicker_enabled) {
            out->flicker = read_flicker_status(dev);
        } else {
            out->flicker = AS734_FLICKER_NONE;
        }

        /* CRUCIAL: Clear the sensor's outstanding status flags ONLY at the very end of Cycle 2 */
        uint8_t status_reg = 0;
        read_regs(dev, REG_STATUS, &status_reg, 1);
        write_reg(dev, REG_STATUS, status_reg);

        /* Stop conversion engines and reset state to IDLE */
        uint8_t base_enable = EN_PON | (dev->flicker_enabled ? EN_FDEN : 0);
        write_reg(dev, REG_ENABLE, base_enable);
        dev->state = AS7341_STATE_IDLE;
        
        return true; /* Complete loop finished cleanly */
    }

    return false;
}