/*
 * task_GPIO.c
 *
 *  Created on: Dec 24, 2024
 *      Author: BaoThuan
 */
#include "task_GPIO.h"
#include "string.h"

extern osMessageQueueId_t smsQ_EXTIHandle;
extern osMessageQueueId_t smsQ_CtrLEDHandle;
extern osMessageQueueId_t smsQ_CtrMHandle;
/**
* @brief Function implementing the Task_BtnDebouce thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BtnDebouce_Prc */
void BtnDebouce_Prc(void *argument)
{
  /* USER CODE BEGIN BtnDebouce_Prc */
	smsQ_buff_exti smsQ_ctr;

	smsQ_buff_motor smsQ_ctrMotor;
	smsQ_buff_led smsQ_ctrLED;
  /* Infinite loop */
  for(;;)
  {
	memset(&smsQ_ctr, '\0', sizeof(smsQ_ctr));
	if(osOK != osMessageQueueGet(smsQ_EXTIHandle, &smsQ_ctr, 0, osWaitForever))
		__NOP();
/*-----------------------------------------------------------*/
	// turn of ISR Can interface
	if (idEXTI == smsQ_ctr.id)
	{
		if (smsQ_ctr.speed_left > SPEED_MAX || smsQ_ctr.speed_right > SPEED_MAX)
		{
			smsQ_ctr.speed_left = 0;
			smsQ_ctr.speed_right = 0;
		}
		else
		{
			smsQ_ctrMotor.estate = smsQ_ctr.state_robot;
			smsQ_ctrMotor.speed_left = smsQ_ctr.speed_left;
			smsQ_ctrMotor.speed_right = smsQ_ctr.speed_right;

			smsQ_ctrLED.time_blink_D4 = smsQ_ctr.time_blink_D4;
			smsQ_ctrLED.time_blink_D5 = smsQ_ctr.time_blink_D5;
			smsQ_ctrLED.time_blink_D6 = smsQ_ctr.time_blink_D6;
		}
	}
	else if (idCAN == smsQ_ctr.id)
	{
		__NOP();
	}
	else
	{
		break;
	}
/*-----------------------------------------------------------*/
	osMessageQueuePut(smsQ_CtrLEDHandle, (const void *)&smsQ_ctrLED, 0, 100);
	smsQ_ctrMotor.id = idROUNT;
	osMessageQueuePut(smsQ_CtrMHandle, (const void *)&smsQ_ctrMotor, 0, 100);

	// osDelay(1000);
  }
  /* USER CODE END BtnDebouce_Prc */
}


/**
* @brief Function implementing the TaskLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_Processor */
void LED_Processor(void *argument)
{
  /* USER CODE BEGIN LED_Processor */
	eLed_taskState eState = LED_INIT;

	smsQ_buff_led smsQ_ctr_led = {0, 0, 0};

	uint8_t countD4 = 0;
	uint8_t countD5 = 0;
	uint8_t countD6 = 0;
  /* Infinite loop */
  for(;;)
  {
	  switch (eState){

/*-----------------------------------------------------------*/
		  case LED_INIT:
			  for(int i = 0; i < 5; i++){
				  LED4_ON;
				  LED5_OFF;
				  LED6_OFF;
				  osDelay(100);
				  LED4_OFF;
				  LED5_ON;
				  LED6_OFF;
				  osDelay(100);
				  LED4_OFF;
				  LED5_OFF;
				  LED6_ON;
				  osDelay(100);
			  }
			  LED6_OFF;
			  eState = LED_IDLE;
			  break;
/*-----------------------------------------------------------*/
		  // Led control
		  case LED_IDLE:
			// Trạng thái Idle có thể được thực hiện dòng lệnh ch�? Queue, flag,..
			// Ch�? lệnh để quyết định nhays led
			  if(osErrorTimeout == osMessageQueueGet(smsQ_CtrLEDHandle, &smsQ_ctr_led, 0, 1))
			  {
				  eState = LED_RUN;
			  }
			  else
			  {
				  // Have smsQ data
				  LED4_OFF;
				  LED5_OFF;
				  LED6_OFF;
				  eState = LED_RUN;
			  }
			  break;
/*-----------------------------------------------------------*/
		  case LED_RUN:
			  if(smsQ_ctr_led.time_blink_D4 != 0)
			  {
				  if(countD4 >= smsQ_ctr_led.time_blink_D4)
				  {
					  countD4 = 0;
					  LED4_TOGGLE;
				  }
				  countD4++;
			  }

			  if(smsQ_ctr_led.time_blink_D5 != 0)
			  {
				  if(countD5 >= smsQ_ctr_led.time_blink_D5)
				  {
					  countD5 = 0;
					  LED5_TOGGLE;
				  }
				  countD5++;
			  }
			  if(smsQ_ctr_led.time_blink_D6 != 0)
			  {
				  if(countD6 >= smsQ_ctr_led.time_blink_D6)
				  {
					  countD6 = 0;
					  LED6_TOGGLE;
				  }
				  countD6++;
			  }

			  // return - change time blink leds
			  eState = LED_IDLE;
			  break;

		  default:
			  break;
	  }
	  osDelay(100);

  }
  /* USER CODE END LED_Processor */
}
