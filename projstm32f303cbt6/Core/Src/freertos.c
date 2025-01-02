/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* Definitions for TaskEncoder */
osThreadId_t TaskEncoderHandle;
const osThreadAttr_t TaskEncoder_attributes = {
  .name = "TaskEncoder",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for TaskRobotCtr */
osThreadId_t TaskRobotCtrHandle;
const osThreadAttr_t TaskRobotCtr_attributes = {
  .name = "TaskRobotCtr",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskLED */
osThreadId_t TaskLEDHandle;
const osThreadAttr_t TaskLED_attributes = {
  .name = "TaskLED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_PIDCtr */
osThreadId_t Task_PIDCtrHandle;
const osThreadAttr_t Task_PIDCtr_attributes = {
  .name = "Task_PIDCtr",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_BtnDebouce */
osThreadId_t Task_BtnDebouceHandle;
const osThreadAttr_t Task_BtnDebouce_attributes = {
  .name = "Task_BtnDebouce",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_CANPrs */
osThreadId_t Task_CANPrsHandle;
const osThreadAttr_t Task_CANPrs_attributes = {
  .name = "Task_CANPrs",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_ReLog */
osThreadId_t Task_ReLogHandle;
const osThreadAttr_t Task_ReLog_attributes = {
  .name = "Task_ReLog",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for smsQ_CtrM */
osMessageQueueId_t smsQ_CtrMHandle;
const osMessageQueueAttr_t smsQ_CtrM_attributes = {
  .name = "smsQ_CtrM"
};
/* Definitions for smsQ_CtrLED */
osMessageQueueId_t smsQ_CtrLEDHandle;
const osMessageQueueAttr_t smsQ_CtrLED_attributes = {
  .name = "smsQ_CtrLED"
};
/* Definitions for smsQ_EXTI */
osMessageQueueId_t smsQ_EXTIHandle;
const osMessageQueueAttr_t smsQ_EXTI_attributes = {
  .name = "smsQ_EXTI"
};
/* Definitions for smsQ_PIDVel */
osMessageQueueId_t smsQ_PIDVelHandle;
const osMessageQueueAttr_t smsQ_PIDVel_attributes = {
  .name = "smsQ_PIDVel"
};
/* Definitions for mutex_enc */
osMutexId_t mutex_encHandle;
const osMutexAttr_t mutex_enc_attributes = {
  .name = "mutex_enc"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void EncoderProcessor(void *argument);
void RobotCtr_Processor(void *argument);
void LED_Processor(void *argument);
void PIDCtr_Processor(void *argument);
void BtnDebouce_Prc(void *argument);
void CANPrs_Process(void *argument);
void ReLog_prs(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of mutex_enc */
  mutex_encHandle = osMutexNew(&mutex_enc_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of smsQ_CtrM */
  smsQ_CtrMHandle = osMessageQueueNew (16, sizeof(smsQ_buff_motor), &smsQ_CtrM_attributes);

  /* creation of smsQ_CtrLED */
  smsQ_CtrLEDHandle = osMessageQueueNew (16, sizeof(smsQ_buff_led), &smsQ_CtrLED_attributes);

  /* creation of smsQ_EXTI */
  smsQ_EXTIHandle = osMessageQueueNew (16, sizeof(smsQ_buff_exti), &smsQ_EXTI_attributes);

  /* creation of smsQ_PIDVel */
  smsQ_PIDVelHandle = osMessageQueueNew (16, sizeof(smsQ_buff_pid), &smsQ_PIDVel_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TaskEncoder */
  TaskEncoderHandle = osThreadNew(EncoderProcessor, NULL, &TaskEncoder_attributes);

  /* creation of TaskRobotCtr */
  TaskRobotCtrHandle = osThreadNew(RobotCtr_Processor, NULL, &TaskRobotCtr_attributes);

  /* creation of TaskLED */
  TaskLEDHandle = osThreadNew(LED_Processor, NULL, &TaskLED_attributes);

  /* creation of Task_PIDCtr */
  Task_PIDCtrHandle = osThreadNew(PIDCtr_Processor, NULL, &Task_PIDCtr_attributes);

  /* creation of Task_BtnDebouce */
  Task_BtnDebouceHandle = osThreadNew(BtnDebouce_Prc, NULL, &Task_BtnDebouce_attributes);

  /* creation of Task_CANPrs */
  Task_CANPrsHandle = osThreadNew(CANPrs_Process, NULL, &Task_CANPrs_attributes);

  /* creation of Task_ReLog */
  Task_ReLogHandle = osThreadNew(ReLog_prs, NULL, &Task_ReLog_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_EncoderProcessor */
#if CODE
/**
  * @brief  Function implementing the TaskEncoder thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_EncoderProcessor */
void EncoderProcessor(void *argument)
{
  /* USER CODE BEGIN EncoderProcessor */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
	osDelay(1000);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
	osDelay(1000);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
	osDelay(1000);
  }
  /* USER CODE END EncoderProcessor */
}

/* USER CODE BEGIN Header_RobotCtr_Processor */
/**
* @brief Function implementing the TaskRobotCtr thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RobotCtr_Processor */
void RobotCtr_Processor(void *argument)
{
  /* USER CODE BEGIN RobotCtr_Processor */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END RobotCtr_Processor */
}

/* USER CODE BEGIN Header_LED_Processor */
/**
* @brief Function implementing the TaskLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_Processor */
void LED_Processor(void *argument)
{
  /* USER CODE BEGIN LED_Processor */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END LED_Processor */
}

/* USER CODE BEGIN Header_PIDCtr_Processor */
/**
* @brief Function implementing the Task_PIDCtr thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PIDCtr_Processor */
void PIDCtr_Processor(void *argument)
{
  /* USER CODE BEGIN PIDCtr_Processor */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END PIDCtr_Processor */
}

/* USER CODE BEGIN Header_BtnDebouce_Prc */
/**
* @brief Function implementing the Task_BtnDebouce thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BtnDebouce_Prc */
void BtnDebouce_Prc(void *argument)
{
  /* USER CODE BEGIN BtnDebouce_Prc */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END BtnDebouce_Prc */
}

/* USER CODE BEGIN Header_CANPrs_Process */
/**
* @brief Function implementing the Task_CANPrs thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CANPrs_Process */
void CANPrs_Process(void *argument)
{
  /* USER CODE BEGIN CANPrs_Process */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CANPrs_Process */
}

/* USER CODE BEGIN Header_ReLog_prs */
/**
* @brief Function implementing the Task_ReLog thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ReLog_prs */
void ReLog_prs(void *argument)
{
  /* USER CODE BEGIN ReLog_prs */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ReLog_prs */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
#endif
/* USER CODE END Application */

