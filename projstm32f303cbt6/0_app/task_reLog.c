/*
 * task_reLog.c
 *
 *  Created on: Dec 31, 2024
 *      Author: BaoThuan
 */
#define MAXLOG 5

#include "task_reLog.h"
#include "0_strPrc.h"

#include "string.h"
// #include "usart.h"

// extern param_motor motor1_manager;

/**
* @brief Function implementing the Task_ReLog thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ReLog_prs */
void ReLog_prs(void *argument)
{
  /* USER CODE BEGIN ReLog_prs */
	uint8_t arr[MAXLOG] = {0};
  /* Infinite loop */
  for(;;)
  {
  	  // Hiện Log debug
//  	   memset(ArrSpeed_Send, 0, strlen(ArrSpeed_Send));
//  	   sprintf(ArrSpeed_Send, "%d%d", motor1_manager.speed, motor2_manager.speed);
//
//  	  HAL_UART_Transmit(&huart1, ArrSpeed_Send, strlen(ArrSpeed_Send), 100);

//	  if(motor1_manager.speed < 0)
//		  motor1_manager.speed = - motor1_manager.speed;
//
//	  memset(arr, 0, strlen(arr));
//
//	  sprintf(arr, "%d", motor1_manager.speed);
//
//	  HAL_UART_Transmit(&huart1, arr, strlen(arr), 100);

	  osDelay(200);
  }
  /* USER CODE END ReLog_prs */
}
