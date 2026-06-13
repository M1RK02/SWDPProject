/**
 ******************************************************************************
 * @file    as7341.h
 * @brief   Driver for AMS AS7341 11-channel spectral sensor (STM32 HAL)
 *
 * Adapted from the ESP-IDF driver by Eric Gionet (MIT License) to the
 * STM32 HAL ecosystem.
 *
 * The AS7341 has 8 visible channels (F1-F8), CLEAR, NIR and a flicker
 * detector. The 8 visible channels share 6 ADCs through an internal SMUX,
 * so a full spectral acquisition requires TWO sequential measurements:
 * - LOW  channels  -> F1..F4, CLEAR, NIR
 * - HIGH channels  -> F5..F8, CLEAR, NIR
 *
 * I2C 7-bit address: 0x39
 ******************************************************************************
 */
#ifndef __AS7341_H__
#define __AS7341_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define AS7341_I2C_ADDR_7BIT   (0x39U)
#define AS7341_I2C_ADDR_8BIT   (AS7341_I2C_ADDR_7BIT << 1)

typedef enum {
    AS7341_GAIN_0_5X = 0,
    AS7341_GAIN_1X,
    AS7341_GAIN_2X,
    AS7341_GAIN_4X,
    AS7341_GAIN_8X,
    AS7341_GAIN_16X,
    AS7341_GAIN_32X,    /* recommended starting point */
    AS7341_GAIN_64X,
    AS7341_GAIN_128X,
    AS7341_GAIN_256X,
    AS7341_GAIN_512X
} AS7341_Gain_t;

typedef enum {
    AS7341_OK       =  0,
    AS7341_ERR_I2C  = -1,   /* HAL I2C transaction failed */
    AS7341_ERR_ID   = -2,   /* unexpected PART_ID         */
    AS7341_ERR_TO   = -3,   /* timeout waiting for data   */
    AS7341_ERR_ARG  = -4    /* invalid argument           */
} AS7341_Status_t;

/**
 * --- Stati per l'acquisizione Asincrona / Non Bloccante ---
 */
typedef enum {
    AS7341_ASYNC_START_LOW = 0,
    AS7341_ASYNC_WAIT_LOW,
    AS7341_ASYNC_START_HIGH,
    AS7341_ASYNC_WAIT_HIGH
} AS7341_AsyncState_t;

/**
 * Channel-to-wavelength mapping:
 * F1: 405-425 nm   F5: 545-565 nm
 * F2: 435-455 nm   F6: 580-600 nm
 * F3: 470-490 nm   F7: 620-640 nm
 * F4: 505-525 nm   F8: 670-690 nm
 */
typedef struct {
    uint16_t f1;
    uint16_t f2;
    uint16_t f3;
    uint16_t f4;
    uint16_t f5;
    uint16_t f6;
    uint16_t f7;
    uint16_t f8;
    uint16_t clear;
    uint16_t nir;
} AS7341_SpectralData_t;

typedef struct {
    I2C_HandleTypeDef   *hi2c;        /* HAL I2C bus handle */
    uint8_t              atime;       /* integration steps,    default 29  */
    uint16_t             astep;       /* integration step size, default 599 (~50 ms) */
    AS7341_Gain_t        gain;
    AS7341_AsyncState_t  async_state; /* Stato corrente della macchina asincrona */
    uint32_t             async_timer; /* Timer per i controlli di timeout asincroni */
} AS7341_Handle_t;

/* ----- Public API ----- */

AS7341_Status_t AS7341_Init(AS7341_Handle_t   *dev,
                            I2C_HandleTypeDef *hi2c,
                            uint8_t            atime,
                            uint16_t           astep,
                            AS7341_Gain_t      gain);

AS7341_Status_t AS7341_ReadSpectral(AS7341_Handle_t       *dev,
                                    AS7341_SpectralData_t *out);

/**
 * @brief  Legge i dati dello spettrometro in modalità non bloccante.
 * Esegue lo switch dei canali SMUX e l'attesa di integrazione hardware asincronamente.
 * @param  dev: Puntatore alla struttura di configurazione del dispositivo.
 * @param  out: Puntatore alla struttura dove salvare i dati aggiornati degli 11 canali.
 * @retval bool: true se un ciclo completo di campionamento (LOW + HIGH) è terminato e i dati sono pronti,
 * false se la conversione è ancora in corso.
 */
bool AS7341_ReadSpectral_NonBlocking(AS7341_Handle_t *dev, AS7341_SpectralData_t *out);

AS7341_Status_t AS7341_PowerOn   (AS7341_Handle_t *dev);
AS7341_Status_t AS7341_PowerOff  (AS7341_Handle_t *dev);
AS7341_Status_t AS7341_SetGain   (AS7341_Handle_t *dev, AS7341_Gain_t gain);
AS7341_Status_t AS7341_ReadPartID(AS7341_Handle_t *dev, uint8_t *id);

#ifdef __cplusplus
}
#endif
#endif /* __AS7341_H__ */