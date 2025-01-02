/*
 * task_motorCtr.c
 *
 *  Created on: Dec 24, 2024
 *      Author: BaoThuan
 */
#include "stm32f3xx_hal.h"
#include "lib_encoder.h"
#include "task_enc.h"
// #include "define.h"
extern osMutexId_t mutex_encHandle;
/**
  * @brief  Function implementing the TaskEncoder thread.
  * @param  argument: Not used
  * @retval None
  */
int16_t speed_db_r = 0;
int16_t speed_db_l = 0;

int16_t speed_db_r_fill = 0;
int16_t speed_db_l_fill = 0;

encoder_user_t* enc_user_M;

/* USER CODE END Header_EncoderProcessor */
void EncoderProcessor(void *argument)
{
  /* USER CODE BEGIN 5 */

	eEncoder_taskState eState = ENC_INIT;

	move_aver_instance_int16 fillter_enc_right;
	move_aver_instance_int16 fillter_enc_left;
  /* Infinite loop */
  for(;;)
  {
	switch (eState) {
		case ENC_INIT:
			Average_filter_reset(&fillter_enc_right);
			Average_filter_reset(&fillter_enc_left);

			ENC_Init(&htim2, &htim3);
			/*Start Encoders*/
			HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
			HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

			eState = ENC_IDLE;
			break;
/*-----------------------------------------------------------*/
		case ENC_IDLE:

			eState = ENC_RUNNING;
			break;
/*-----------------------------------------------------------*/
		case ENC_RUNNING:
			// encoder
			enc_user_M = ENCM_Handl();
			// debug
			speed_db_r = enc_user_M->enc_vel;
			speed_db_l = (enc_user_M + 1)->enc_vel;

			osMutexAcquire(mutex_encHandle, 100);
			speed_db_r_fill = Average_filter_apply(&fillter_enc_right, enc_user_M->enc_vel);
			speed_db_l_fill = Average_filter_apply(&fillter_enc_left, (enc_user_M + 1)->enc_vel);
			osMutexRelease(mutex_encHandle);
			break;
		default:
			break;
	}
	// Sampling time
    osDelay(SAMP_TIME);
  }
  /* USER CODE END 5 */
}
