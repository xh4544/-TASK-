/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "board_config.h"
#include "gimbal_oled.h"
#include "gimbal.h"
#include "chassis.h"
#include "oled_app.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for startTask */
osThreadId_t startTaskHandle;
const osThreadAttr_t startTask_attributes = {
  .name = "startTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for gimbalTask */
osThreadId_t gimbalTaskHandle;
const osThreadAttr_t gimbalTask_attributes = {
  .name = "gimbalTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for chassisTask */
osThreadId_t chassisTaskHandle;
const osThreadAttr_t chassisTask_attributes = {
  .name = "chassisTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for oledTask */
osThreadId_t oledTaskHandle;
const osThreadAttr_t oledTask_attributes = {
  .name = "oledTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void GimbalTask(void *argument);
void ChassisTask(void *argument);
void OLED_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of startTask */
  startTaskHandle = osThreadNew(StartDefaultTask, NULL, &startTask_attributes);

  /* creation of gimbalTask */
  gimbalTaskHandle = osThreadNew(GimbalTask, NULL, &gimbalTask_attributes);

  /* creation of chassisTask */
  chassisTaskHandle = osThreadNew(ChassisTask, NULL, &chassisTask_attributes);

  /* creation of oledTask */
  oledTaskHandle = osThreadNew(OLED_Task, NULL, &oledTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the startTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* 双板共用工程：删除本板不需要的任务，节省 RAM/CPU */
#if BOARD_GIMBAL
  vTaskDelete(chassisTaskHandle);
  vTaskDelete(oledTaskHandle);
#else
  vTaskDelete(gimbalTaskHandle);
#endif

  /* 删除自身 */
  vTaskDelete(NULL);
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_GimbalTask */
/**
* @brief Function implementing the gimbalTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GimbalTask */
void GimbalTask(void *argument)
{
  /* USER CODE BEGIN GimbalTask */
  /* Infinite loop */
  for(;;)
  {
#if BOARD_GIMBAL
    gimbal_task();
    gimbal_oled_task();   /* 云台板 I2C OLED 显示 CAN 状态 */
    osDelay(5);
#else
    osDelay(1000);
#endif
  }
  /* USER CODE END GimbalTask */
}

/* USER CODE BEGIN Header_ChassisTask */
/**
* @brief Function implementing the chassisTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ChassisTask */
void ChassisTask(void *argument)
{
  /* USER CODE BEGIN ChassisTask */
  /* Infinite loop */
  for(;;)
  {
#if BOARD_CHASSIS
    chassis_task();
    osDelay(5);
#else
    osDelay(1000);
#endif
  }
  /* USER CODE END ChassisTask */
}

/* USER CODE BEGIN Header_OLED_Task */
/**
* @brief Function implementing the oledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLED_Task */
void OLED_Task(void *argument)
{
  /* USER CODE BEGIN OLED_Task */
  /* Infinite loop */
  for(;;)
  {
#if BOARD_CHASSIS
    oled_app_task();
    osDelay(50);
#else
    osDelay(1000);
#endif
  }
  /* USER CODE END OLED_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

