/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define steering_engine_PWM_Pin GPIO_PIN_1
#define steering_engine_PWM_GPIO_Port GPIOA
#define motorA_PWM_Pin GPIO_PIN_6
#define motorA_PWM_GPIO_Port GPIOA
#define motorB_PWM_Pin GPIO_PIN_7
#define motorB_PWM_GPIO_Port GPIOA
#define infrared_sensor_left_Pin GPIO_PIN_0
#define infrared_sensor_left_GPIO_Port GPIOB
#define infrared_sensor_left_EXTI_IRQn EXTI0_IRQn
#define infrared_sensor_right_Pin GPIO_PIN_1
#define infrared_sensor_right_GPIO_Port GPIOB
#define infrared_sensor_right_EXTI_IRQn EXTI1_IRQn
#define TB6612_A1_Pin GPIO_PIN_12
#define TB6612_A1_GPIO_Port GPIOB
#define TB6612_A0_Pin GPIO_PIN_13
#define TB6612_A0_GPIO_Port GPIOB
#define TB6612_B1_Pin GPIO_PIN_14
#define TB6612_B1_GPIO_Port GPIOB
#define TB6612_B0_Pin GPIO_PIN_15
#define TB6612_B0_GPIO_Port GPIOB
#define DEBUG_T_Pin GPIO_PIN_9
#define DEBUG_T_GPIO_Port GPIOA
#define DEBUG_R_Pin GPIO_PIN_10
#define DEBUG_R_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
