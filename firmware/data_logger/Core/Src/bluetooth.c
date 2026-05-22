/**
 * \file bluetooth.c
 * \brief Implementation file for RN4871 Bluetooth Low Energy functions.
 *
 * This file contains the implementation of functions for configuring and
 * communicating with the Microchip RN4871 Bluetooth® Low Energy Module.
 *
 * The RN4871 module is a fully certified Bluetooth Smart module that is
 * controlled primarily through ASCII commands sent from a host MCU to its UART.
 * The module operates in two main modes:
 * - Data mode: Acts as a data pipe, transparently transferring serial data.
 * - Command mode: Interprets UART data as ASCII commands for configuration.
 *
 * Default UART Settings:
 * Baud Rate: 115200
 * Data Bits: 8
 * Parity: None
 * Stop Bits: 1
 * Flow Control: Disabled
 */

#include <bluetooth.h>
#include "main.h"

extern UART_HandleTypeDef huart3;

// --- Helper Functions (Internal to this file) ---
// These functions are not meant to be called directly by the user.

static void enter_command_mode(void);
static void exit_command_mode(void);

// --- Public Function Implementations ---

/**
 * @brief Performs a hard reset of the BLE module via the MCU's reset pin.
 *
 * A hard reset is necessary to apply certain configuration changes and
 * to ensure the module is in a known, initial state before sending commands.
 */
void BLE_HardReset(void) {
    // Pull the reset pin low to activate the reset
    HAL_GPIO_WritePin(BLE_RESET_GPIO_Port, BLE_RESET_Pin, 0);
    HAL_Delay(1000); // Wait for the module to reset
    // Pull the reset pin high to exit the reset state
    HAL_GPIO_WritePin(BLE_RESET_GPIO_Port, BLE_RESET_Pin, 1);
}

/**
 * @brief Configures the RN4871 BLE Module at startup.
 *
 * This function sets up the device name, enables the Transparent UART service,
 * and enables the UART RX Indication pin functionality.
 */
void BLE_Initialize(void) {
    uint8_t reboot_response[9] = {0};
    uint8_t command_ok_response[100] = {0};

    // Perform a hard reset to get the module into a known state
    BLE_HardReset();
    HAL_UART_Receive(&huart3, reboot_response, sizeof(reboot_response), UART_TIMEOUT); // Read initial reboot string

    // Enter Command Mode to send configuration commands
    enter_command_mode();

    // Set the device name for easy identification
    uint8_t device_name[] = "SN,BLE_SW\r";
    BLE_SendData(device_name, sizeof(device_name) - 1);
    HAL_UART_Receive(&huart3, command_ok_response, sizeof(command_ok_response), UART_TIMEOUT); // Read 'AOK' response

    // Enable Transparent UART service (UUID: 49535343-FE7D-4AE5-8FA9-9FAFD205E455)
    uint8_t enable_transparent_uart[] = "SS,C0\r";
    BLE_SendData(enable_transparent_uart, sizeof(enable_transparent_uart) - 1);
    HAL_UART_Receive(&huart3, command_ok_response, sizeof(command_ok_response), UART_TIMEOUT);

    // Set Pin P16 to act as UART RX Indication, which is useful for waking up the module
    uint8_t enable_uart_rx_ind[] = "SW,0C,04\r";
    BLE_SendData(enable_uart_rx_ind, sizeof(enable_uart_rx_ind) - 1);
    HAL_UART_Receive(&huart3, command_ok_response, sizeof(command_ok_response), UART_TIMEOUT);

    // Reboot the module for the new settings to take effect
    uint8_t reboot_command[] = "R,1\r";
    BLE_SendData(reboot_command, sizeof(reboot_command) - 1);
    HAL_Delay(100);

    // Exit Command Mode and return to Data Mode
    exit_command_mode();
}

/**
 * @brief Configures the RN4871 BLE Module to enter Dormant (Deep Sleep) mode.
 *
 * In this mode, the module consumes minimal power, and all RF communication is stopped.
 * The module can be woken up by toggling the UART_RX_IND pin.
 */
void BLE_EnterDormantMode(void) {
    uint8_t reboot_response[9] = {0};

    // Ensure the module is in a known state before sending commands
    BLE_HardReset();
    HAL_UART_Receive(&huart3, reboot_response, sizeof(reboot_response), UART_TIMEOUT);

    // Set the UART_RX_IND pin high to prepare for Dormant mode entry
    HAL_GPIO_WritePin(BLE_UART_RX_IND_GPIO_Port, BLE_UART_RX_IND_Pin, 1);
    HAL_Delay(10); // Small delay to allow the pin state to stabilize

    // Enter Command Mode
    enter_command_mode();

    // Send the command "O,0" to enter Dormant mode
    uint8_t dormant_mode_command[] = "O,0\r";
    BLE_SendData(dormant_mode_command, sizeof(dormant_mode_command) - 1);

    HAL_Delay(100); // Wait for the module to enter sleep
}

/**
 * @brief Wakes the RN4871 BLE Module from Dormant/Sleep mode.
 *
 * This function wakes the module by toggling the UART_RX_IND pin and then
 * re-initializes it to a functional state.
 */
void BLE_WakeUp(void) {
    // Toggle the UART_RX_IND pin to wake up the module
    // The module's 16 MHz clock restarts when this pin goes low.
    HAL_GPIO_WritePin(BLE_UART_RX_IND_GPIO_Port, BLE_UART_RX_IND_Pin, 0);
    HAL_Delay(5); // Wait for the clock to stabilize

    // After waking up, we need to re-enter command mode and return to data mode
    // to ensure the module is ready for communication.
    BLE_Initialize();
}

/**
 * @brief Configures the RN4871 BLE Module to enter Low-Power mode.
 *
 * In this mode, the module uses a 32kHz clock, significantly reducing power consumption.
 * A BLE connection can still be maintained, but the UART cannot receive data.
 */
void BLE_EnterLowPowerMode(void) {
    uint8_t command_ok_response[100] = {0};

    // Enter Command Mode
    enter_command_mode();

    // Send the command "SO,1" to enable Low-Power mode
    uint8_t low_power_command[] = "SO,1\r";
    BLE_SendData(low_power_command, sizeof(low_power_command) - 1);
    HAL_UART_Receive(&huart3, command_ok_response, sizeof(command_ok_response), UART_TIMEOUT);

    // Exit Command Mode
    exit_command_mode();
}

/**
 * @brief Exits the RN4871 BLE Module from Low-Power mode to Active mode.
 *
 * This function returns the module to its normal operating state (16MHz clock).
 */
void BLE_ExitLowPowerMode(void) {
    uint8_t command_ok_response[100] = {0};

    // Enter Command Mode
    enter_command_mode();

    // Send the command "SO,0" to exit Low-Power mode
    uint8_t exit_low_power_command[] = "SO,0\r";
    BLE_SendData(exit_low_power_command, sizeof(exit_low_power_command) - 1);
    HAL_UART_Receive(&huart3, command_ok_response, sizeof(command_ok_response), UART_TIMEOUT);

    // Exit Command Mode
    exit_command_mode();
}

/**
 * @brief Configures the RN4871 BLE Module for slow advertisements.
 *
 * This reduces power consumption by broadcasting less frequently.
 * The command "A,03E8,002F" sets the advertising interval to 1000ms.
 */
void BLE_SetSlowAdvertisements(void) {
    uint8_t command_ok_response[100] = {0};

    // Enter Command Mode
    enter_command_mode();

    // Send the command "A,03E8,002F" to set slow advertisement intervals
    // 03E8 is 1000 in hex (1000ms), 002F is 47ms (min interval)
    uint8_t slow_ads_command[] = "A,03E8,002F\r";
    BLE_SendData(slow_ads_command, sizeof(slow_ads_command) - 1);
    HAL_UART_Receive(&huart3, command_ok_response, sizeof(command_ok_response), UART_TIMEOUT);

    // Exit Command Mode
    exit_command_mode();
}

/**
 * @brief Sends data to a connected external BLE device.
 *
 * This function uses the Transparent UART service to transmit data.
 * @param data Pointer to the data buffer to send.
 * @param data_length The number of bytes to send.
 */
void BLE_SendData(uint8_t* data, uint8_t data_length) {
    HAL_UART_Transmit(&huart3, data, data_length, UART_TIMEOUT);
}

/**
 * @brief Receives data from a connected external BLE device.
 *
 * This function reads data from the Transparent UART service.
 * @param data Pointer to the buffer where received data will be stored.
 * @param data_length The number of bytes to read.
 */
void BLE_ReceiveData(uint8_t* data, uint8_t data_length) {
    HAL_UART_Receive(&huart3, data, data_length, UART_TIMEOUT);
}

/**
 * @brief Sends a structured data packet with a specific type and value.
 *
 * This function creates a standardized packet format to send specific sensor data.
 * The packet format is: { | Type | MSB of Value | ... | LSB of Value | ... | }
 * @param type The type of data being sent (e.g., acceleration, gyroscope).
 * @param value The 32-bit value to be sent.
 */
void BLE_SendPacket(BLE_DataType ble_data_type, uint8_t* data_buffer) {
    uint8_t ble_packet[PACKET_LENGTH];

    // Initialize the packet buffer
    ble_packet[0] = '{';
    ble_packet[PACKET_LENGTH - 1] = '}';
    for (uint8_t i = 1; i < PACKET_LENGTH - 1; i++) {
        ble_packet[i] = 0;
    }

    // Byte 1: Data type identifier
    switch (ble_data_type) {
        case DATA_TYPE_IMU_ACCELERATION:
            ble_packet[1] = 'A';
            break;
        case DATA_TYPE_IMU_GYROSCOPE:
            ble_packet[1] = 'G';
            break;
        default:
            ble_packet[1] = 'U'; // Unknown data type
            break;
    }

    // Bytes 2-4: The 32-bit value, packed in big-endian format
    ble_packet[2] = data_buffer[0]; // X Axis LSB
    ble_packet[3] = data_buffer[1]; // X Axis MSB
    ble_packet[4] = data_buffer[2]; // Y Axis LSB
    ble_packet[5] = data_buffer[3]; // Y Axis MSB
    ble_packet[6] = data_buffer[4]; // Z Axis LSB
    ble_packet[7] = data_buffer[5]; // Z Axis MSB

    // Send the complete packet over UART
    BLE_SendData(ble_packet, sizeof(ble_packet));
}

// --- Helper Function Implementations ---
// These helper functions encapsulate common, repeated tasks to improve code clarity.

/**
 * @brief Helper function to enter Command Mode.
 *
 * Sends the `$$$` sequence to the module to switch from Data Mode to Command Mode.
 */
static void enter_command_mode(void) {
    uint8_t command_mode_sequence[] = "$$$";
    uint8_t command_prompt_response[5] = {0};
    BLE_SendData(command_mode_sequence, sizeof(command_mode_sequence) - 1);
    HAL_UART_Receive(&huart3, command_prompt_response, sizeof(command_prompt_response), UART_TIMEOUT);
    HAL_Delay(100);
}

/**
 * @brief Helper function to exit Command Mode.
 *
 * Sends the `---` command to the module to switch back to Data Mode.
 */
static void exit_command_mode(void) {
    uint8_t data_mode_command[] = "---\r";
    //uint8_t command_ok_response[100] = {0};
    BLE_SendData(data_mode_command, sizeof(data_mode_command) - 1);
    //HAL_UART_Receive(&huart3, command_ok_response, sizeof(command_ok_response), UART_TIMEOUT);
    HAL_Delay(100);
}
