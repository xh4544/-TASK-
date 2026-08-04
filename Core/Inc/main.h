/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define servo_Pin GPIO_PIN_2
#define servo_GPIO_Port GPIOA
#define TB6612_AIN1_Pin GPIO_PIN_12
#define TB6612_AIN1_GPIO_Port GPIOB
#define TB6612_AIN2_Pin GPIO_PIN_13
#define TB6612_AIN2_GPIO_Port GPIOB
#define TB6612_STBY_Pin GPIO_PIN_14
#define TB6612_STBY_GPIO_Port GPIOB
#define OLED_RES_Pin GPIO_PIN_5
#define OLED_RES_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_8
#define OLED_DC_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* LED：PC13（CubeMX 重生成时标签丢失，这里补上） */
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
