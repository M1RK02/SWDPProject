#ifndef __IMU_DRIVER_H__
#define __IMU_DRIVER_H__

/**
 * \file imu_driver.h
 * \brief Driver library for the LSM6DSO16IS IMU sensor.
 *
 * This header file provides the public interface for controlling the
 * LSM6DSO16IS, including configuration and data reading functions.
 */

#include "stdint.h"
#include "main.h"

// --- I2C Communication Settings ---
// The I2C slave address of the LSM6DSO16IS is 0x6B (7-bit format).
// This is the address to be used with the HAL library functions.
#define IMU_I2C_ADDRESS 0x6B
#define IMU_I2C_TIMEOUT 100 // Timeout in milliseconds for I2C operations

// --- Register Addresses ---
// These are the addresses of the key registers used to configure and read the IMU.
#define IMU_WHO_AM_I_REG        0x0F
#define IMU_WHO_AM_I_VALUE      0x22

// Accelerometer and Gyroscope Control Registers
#define IMU_ACC_CTRL1_REG       0x10 // ODR and FS selection for accelerometer
#define IMU_GYR_CTRL2_REG       0x11 // ODR and FS selection for gyroscope
#define IMU_ACC_CTRL6_REG       0x15 // Control register for accelerometer performance mode
#define IMU_GYR_CTRL7_REG       0x16 // Control register for gyroscope performance mode
#define IMU_CTRL10_REG          0x19 // Main control register

// Output Data Registers for Accelerometer and Gyroscope (contiguous)
#define IMU_GYR_OUT_X_L_REG     0x22 // Gyroscope X-axis LSB
#define IMU_ACC_OUT_X_L_REG     0x28 // Accelerometer X-axis LSB

// --- Register Configuration Values ---
// These preprocessor macros define the values for configuring ODR, Full Scale, and performance modes.

// Accelerometer Output Data Rate (ODR)
#define ACC_ODR_POWER_DOWN      0x00
#define ACC_ODR_12_5HZ          (1 << 4) // Bit 4-7
#define ACC_ODR_26HZ            (2 << 4)
#define ACC_ODR_52HZ            (3 << 4)
#define ACC_ODR_104HZ           (4 << 4)
#define ACC_ODR_208HZ           (5 << 4)

// Accelerometer Full Scale (FS)
#define ACC_FS_2G               (0 << 2) // Bit 2-3
#define ACC_FS_4G               (2 << 2)
#define ACC_FS_8G               (3 << 2)
#define ACC_FS_16G              (1 << 2)

// Gyroscope Output Data Rate (ODR)
#define GYR_ODR_POWER_DOWN      0x00
#define GYR_ODR_12_5HZ          (1 << 4) // Bit 4-7
#define GYR_ODR_26HZ            (2 << 4)
#define GYR_ODR_52HZ            (3 << 4)
#define GYR_ODR_104HZ           (4 << 4)
#define GYR_ODR_208HZ           (5 << 4)

// Gyroscope Full Scale (FS)
#define GYR_FS_250DPS           0x00     // Bit 2-3
#define GYR_FS_500DPS           (1 << 2)
#define GYR_FS_1000DPS          (2 << 2)
#define GYR_FS_2000DPS          (3 << 2)

// Performance Modes (High Performance mode disable bits)
#define ACC_HIGH_PERF_DISABLE_BIT   (1 << 4) // CTRL6_C[4]
#define GYR_HIGH_PERF_DISABLE_BIT   (1 << 7) // CTRL7_G[7]

// --- Data Structures ---
// This struct is used to store the 3-axis data from the IMU.
typedef struct {
    float x;
    float y;
    float z;
} IMU_Data;

// --- Public Function Prototypes ---
// The main API for the IMU driver.

/**
 * @brief Initializes the IMU sensor and performs a WHO_AM_I check.
 * @return 1 on success, 0 on failure.
 */
uint8_t IMU_Init(void);

/**
 * @brief Configures the accelerometer with specified ODR, Full Scale, and performance mode.
 * @param odr The desired Output Data Rate (e.g., ACC_ODR_104HZ).
 * @param scale The desired Full Scale range (e.g., ACC_FS_4G).
 * @param high_performance_mode 1 to enable high performance mode, 0 for low power.
 */
void IMU_ConfigAccelerometer(uint8_t odr, uint8_t scale, uint8_t high_performance_mode);

/**
 * @brief Configures the gyroscope with specified ODR, Full Scale, and performance mode.
 * @param odr The desired Output Data Rate (e.g., GYR_ODR_104HZ).
 * @param scale The desired Full Scale range (e.g., GYR_FS_250DPS).
 * @param high_performance_mode 1 to enable high performance mode, 0 for low power.
 */
void IMU_ConfigGyroscope(uint8_t odr, uint8_t scale, uint8_t high_performance_mode);

/**
 * @brief Reads the raw and converted accelerometer data.
 * @param acc_data Pointer to an IMU_Data struct where the results will be stored.
 */
void IMU_ReadAccelerometerData(IMU_Data *acc_data, uint8_t *raw_data);

/**
 * @brief Reads the raw and converted gyroscope data.
 * @param gyro_data Pointer to an IMU_Data struct where the results will be stored.
 */
void IMU_ReadGyroscopeData(IMU_Data *gyro_data, uint8_t *raw_data);

#endif /* __IMU_DRIVER_H__ */
