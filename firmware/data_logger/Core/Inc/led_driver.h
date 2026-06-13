#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__

/**
 * \file led_driver.h
 * \brief Driver library for controlling onboard LEDs.
 *
 * This header file defines the functions and data types used to manage
 * the state of the MCU's Green and Red LEDs. It provides a simple,
 * high-level interface for turning the LEDs on, off, or toggling their state.
 */

#include "main.h"

// --- Enumerations ---
// This enum provides a symbolic name for each LED.
typedef enum {
    LED_GREEN,
    LED_RED
} LedColor;

// --- Function Prototypes ---
// These are the public functions available for controlling the LEDs.

/**
 * @brief Turns a specified LED on.
 * @param led_color The color of the LED to turn on (LED_GREEN or LED_RED).
 */
void LED_On(LedColor led_color);

/**
 * @brief Turns a specified LED off.
 * @param led_color The color of the LED to turn off (LED_GREEN or LED_RED).
 */
void LED_Off(LedColor led_color);

/**
 * @brief Toggles the state of a specified LED (from ON to OFF, or OFF to ON).
 * @param led_color The color of the LED to toggle (LED_GREEN or LED_RED).
 */
void LED_Toggle(LedColor led_color);

#endif /* __LED_DRIVER_H__ */
