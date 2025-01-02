/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <motor_control.h>
#include <pid.h>
#include <moving_average_int_16.h>
#include <define.h>
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_SW4_Pin GPIO_PIN_13
#define BTN_SW4_GPIO_Port GPIOC
#define BTN_SW4_EXTI_IRQn EXTI15_10_IRQn
#define BTN_SW5_Pin GPIO_PIN_14
#define BTN_SW5_GPIO_Port GPIOC
#define BTN_SW5_EXTI_IRQn EXTI15_10_IRQn
#define BTN_SW6_Pin GPIO_PIN_15
#define BTN_SW6_GPIO_Port GPIOC
#define BTN_SW6_EXTI_IRQn EXTI15_10_IRQn
#define LED_D4_Pin GPIO_PIN_11
#define LED_D4_GPIO_Port GPIOA
#define LED_D5_Pin GPIO_PIN_12
#define LED_D5_GPIO_Port GPIOA
#define LED_D6_Pin GPIO_PIN_15
#define LED_D6_GPIO_Port GPIOA
#define DIR_M2_Pin GPIO_PIN_4
#define DIR_M2_GPIO_Port GPIOB
#define DIR_M1_Pin GPIO_PIN_5
#define DIR_M1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
