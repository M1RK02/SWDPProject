/**
 ******************************************************************************
 * @file    as7341.h
 * @brief   Clean, interrupt-driven driver for AMS AS7341 with Flicker Detection
 * and optimized multi-cycle routing (F1-F8, CLEAR, NIR, FLICKER).
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
    AS7341_GAIN_32X,     
    AS7341_GAIN_64X,     
    AS7341_GAIN_128X,    
    AS7341_GAIN_256X,    
    AS7341_GAIN_512X     
} AS7341_Gain_t;

typedef enum {
    AS7341_OK       =  0,
    AS7341_ERR_I2C  = -1,
    AS7341_ERR_ID   = -2,
    AS7341_ERR_ARG  = -3
} AS7341_Status_t;

typedef enum {
    AS7341_STATE_IDLE = 0,
    AS7341_STATE_WAITING_LOW,  /* LOW bank spectral integration active */
    AS7341_STATE_WAITING_HIGH  /* HIGH bank spectral integration active */
} AS7341_State_t;

typedef enum {
    AS734_FLICKER_UNKNOWN = 0,
    AS734_FLICKER_NONE,
    AS734_FLICKER_100HZ,
    AS734_FLICKER_120HZ
} AS7341_Flicker_t;

typedef struct {
    uint16_t f1;   uint16_t f2;   uint16_t f3;   uint16_t f4;
    uint16_t f5;   uint16_t f6;   uint16_t f7;   uint16_t f8;
    uint16_t clear;
    uint16_t nir;
    AS7341_Flicker_t flicker; /* On-chip background calculation engine result flag */
} AS7341_SpectralData_t;

typedef struct {
    I2C_HandleTypeDef *hi2c;
    AS7341_State_t     state;
    uint8_t            atime;
    uint16_t           astep;
    AS7341_Gain_t      gain;
    bool               flicker_enabled;
} AS7341_Handle_t;

/* Public API */
AS7341_Status_t AS7341_Init(AS7341_Handle_t *dev, I2C_HandleTypeDef *hi2c, 
                            uint8_t atime, uint16_t astep, AS7341_Gain_t gain,
                            bool enable_flicker);

/**
 * @brief  Triggers the non-blocking read operation sequence.
 * Configures Cycle 1 (LOW Bank) and arm-triggers hardware execution.
 * @param  dev: Pointer to device handle structure.
 * @retval AS7341_Status_t: Initialization execution state status.
 */
AS7341_Status_t AS7341_StartRead(AS7341_Handle_t *dev);

/**
 * @brief  Processes the hardware interrupt. Call this inside your EXTI Callback loop.
 * @param  dev: Pointer to device handle structure.
 * @param  out: Pointer to destination spectral matrix telemetry storage struct.
 * @retval bool: true if BOTH Cycle 1 and Cycle 2 are complete and metrics are fully ready.
 */
bool AS7341_IRQHandler(AS7341_Handle_t *dev, AS7341_SpectralData_t *out);

#ifdef __cplusplus
}
#endif
#endif /* __AS7341_H__ */