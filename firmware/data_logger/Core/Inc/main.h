/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../USB_Device/App/usb_device.h"
#include <bluetooth.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IMU_IS_INT1_Pin GPIO_PIN_13
#define IMU_IS_INT1_GPIO_Port GPIOC
#define IMU_IS_INT1_EXTI_IRQn EXTI13_IRQn
#define IMU_IS_INT2_Pin GPIO_PIN_0
#define IMU_IS_INT2_GPIO_Port GPIOA
#define IMU_IS_INT2_EXTI_IRQn EXTI0_IRQn
#define SPI3_CS_NAND_Pin GPIO_PIN_4
#define SPI3_CS_NAND_GPIO_Port GPIOA
#define USER_BUTTON_Pin GPIO_PIN_10
#define USER_BUTTON_GPIO_Port GPIOB
#define USER_BUTTON_EXTI_IRQn EXTI10_IRQn
#define BLE_P0_0_Pin GPIO_PIN_6
#define BLE_P0_0_GPIO_Port GPIOC
#define BLE_P3_6_Pin GPIO_PIN_7
#define BLE_P3_6_GPIO_Port GPIOC
#define BLE_UART_RX_IND_Pin GPIO_PIN_8
#define BLE_UART_RX_IND_GPIO_Port GPIOC
#define BLE_RESET_Pin GPIO_PIN_9
#define BLE_RESET_GPIO_Port GPIOC
#define BLE_CONFIG_Pin GPIO_PIN_15
#define BLE_CONFIG_GPIO_Port GPIOA
#define MCU_I_O_2_Pin GPIO_PIN_4
#define MCU_I_O_2_GPIO_Port GPIOB
#define MCU_I_O_2_EXTI_IRQn EXTI4_IRQn
#define MCU_I_O_1_Pin GPIO_PIN_5
#define MCU_I_O_1_GPIO_Port GPIOB
#define MCU_I_O_1_EXTI_IRQn EXTI5_IRQn
#define MCU_GREEN_LED_Pin GPIO_PIN_6
#define MCU_GREEN_LED_GPIO_Port GPIOB
#define MCU_RED_LED_Pin GPIO_PIN_7
#define MCU_RED_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define I2C_TIMEOUT 100

// This enum defines the possible states for the main application logic.
typedef enum {
    STATE_IDLE,          // The device is waiting for a command or event.
    STATE_ACQUISITION,   // The device is actively collecting sensor data.
    STATE_USB_CONNECTED, // The device is connected to a computer via USB.
    STATE_DOWNLOAD       // The device is in the process of downloading data.
} AppState;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
