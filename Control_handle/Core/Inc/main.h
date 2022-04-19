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
#include "FreeRTOS.h"
#include "cmsis_os.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef uint8_t   u8;
typedef uint32_t  u32; 
typedef uint16_t  u16;

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
#define OLED_CS_Pin GPIO_PIN_4
#define OLED_CS_GPIO_Port GPIOA
#define OLED_SCLK_Pin GPIO_PIN_5
#define OLED_SCLK_GPIO_Port GPIOA
#define OLED_SDIN_Pin GPIO_PIN_7
#define OLED_SDIN_GPIO_Port GPIOA
#define OLED_RET_Pin GPIO_PIN_0
#define OLED_RET_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_1
#define OLED_DC_GPIO_Port GPIOB
#define MPU_IIC_SCL_Pin GPIO_PIN_10
#define MPU_IIC_SCL_GPIO_Port GPIOB
#define MPU_IIC_SDA_Pin GPIO_PIN_11
#define MPU_IIC_SDA_GPIO_Port GPIOB
#define SPI_IRQ_Pin GPIO_PIN_3
#define SPI_IRQ_GPIO_Port GPIOB
#define SPI_CE_Pin GPIO_PIN_4
#define SPI_CE_GPIO_Port GPIOB
#define SPI_CSN_Pin GPIO_PIN_5
#define SPI_CSN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define delay_ms(x) osDelay(x)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
