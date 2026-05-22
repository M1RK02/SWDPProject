#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

/**
 * \file bluetooth.h
 * \brief Library for interacting with the RN4871 Bluetooth Low Energy module.
 *
 * This header file defines the functions and data types used to control the
 * RN4871 BLE module.
 */

#include "stdio.h"
#include "stdint.h"

// --- Constants Definitions ---
// These are standard values used for the BLE communication packets and timeouts.
#define PACKET_LENGTH 20    // Maximum length of a data packet
#define TEXT_LENGTH 500     // Maximum length for text strings
#define UART_TIMEOUT 1000   // Timeout in milliseconds for UART operations

// --- Enumerations ---
// This enum defines the possible types of data packets that can be sent.
typedef enum {
    DATA_TYPE_IMU_ACCELERATION, // Indicates acceleration data from an IMU
    DATA_TYPE_IMU_GYROSCOPE     // Indicates gyroscope data from an IMU
} BLE_DataType;

// --- Function Prototypes ---
// These are the public functions available for use.
void BLE_HardReset(void);
void BLE_Initialize(void);
void BLE_EnterDormantMode(void);
void BLE_WakeUp(void);
void BLE_EnterLowPowerMode(void);
void BLE_ExitLowPowerMode(void);
void BLE_SetSlowAdvertisements(void);
void BLE_SendData(uint8_t* data, uint8_t data_length);
void BLE_ReceiveData(uint8_t* data, uint8_t data_length);
void BLE_SendPacket(BLE_DataType ble_data_type, uint8_t* data_buffer);

#endif /* INC_BLUETOOTH_H_ */
