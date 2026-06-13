/**
 * \file led_driver.c
 * \brief Implementation file for the LED driver library.
 *
 * This file contains the implementation of functions for controlling the state
 * of the Green and Red LEDs connected to the MCU's GPIO pins. It uses the
 * HAL (Hardware Abstraction Layer) library to manage the GPIO operations.
 */

#include <led_driver.h>
#include "main.h"

// --- Public Function Implementations ---

/**
 * @brief Turns a specified LED on by setting its GPIO pin to a high state (logic 1).
 *
 * This function uses a switch statement to select the correct GPIO port and pin
 * based on the provided LedColor.
 *
 * @param led_color The color of the LED to turn on.
 */
void LED_On(LedColor led_color) {
    switch (led_color) {
        case LED_GREEN:
            // Set the GPIO pin for the Green LED to HIGH to turn it on
            HAL_GPIO_WritePin(MCU_GREEN_LED_GPIO_Port, MCU_GREEN_LED_Pin, GPIO_PIN_SET);
            break;
        case LED_RED:
            // Set the GPIO pin for the Red LED to HIGH
            HAL_GPIO_WritePin(MCU_RED_LED_GPIO_Port, MCU_RED_LED_Pin, GPIO_PIN_SET);
            break;
        default:
            // Handle any un-defined LED color gracefully
            break;
    }
}

/**
 * @brief Turns a specified LED off by setting its GPIO pin to a low state (logic 0).
 *
 * This function uses a switch statement to select the correct GPIO port and pin
 * based on the provided LedColor.
 *
 * @param led_color The color of the LED to turn off.
 */
void LED_Off(LedColor led_color) {
    switch (led_color) {
        case LED_GREEN:
            // Set the GPIO pin for the Green LED to LOW to turn it off
            HAL_GPIO_WritePin(MCU_GREEN_LED_GPIO_Port, MCU_GREEN_LED_Pin, GPIO_PIN_RESET);
            break;
        case LED_RED:
            // Set the GPIO pin for the Red LED to LOW
            HAL_GPIO_WritePin(MCU_RED_LED_GPIO_Port, MCU_RED_LED_Pin, GPIO_PIN_RESET);
            break;
        default:
            // Handle any un-defined LED color gracefully
            break;
    }
}

/**
 * @brief Toggles the state of a specified LED.
 *
 * This function changes the LED state from ON to OFF or from OFF to ON.
 * It uses the HAL_GPIO_TogglePin function for a simple state inversion.
 *
 * @param led_color The color of the LED to toggle.
 */
void LED_Toggle(LedColor led_color) {
    switch (led_color) {
        case LED_GREEN:
            // Toggle the state of the Green LED pin
            HAL_GPIO_TogglePin(MCU_GREEN_LED_GPIO_Port, MCU_GREEN_LED_Pin);
            break;
        case LED_RED:
            // Toggle the state of the Red LED pin
            HAL_GPIO_TogglePin(MCU_RED_LED_GPIO_Port, MCU_RED_LED_Pin);
            break;
        default:
            // Handle any un-defined LED color gracefully
            break;
    }
}
