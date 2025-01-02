/*
 * task_pid.c
 *
 *  Created on: Dec 24, 2024
 *      Author: BaoThuan
 */
#include "task_pid.h"
#include "pid.h"
#include "define.h"
// #include "lib_encoder.h"
#include "motor_control.h"
#include <moving_average_int_16.h>

#define USE_FILL (1)

extern osMessageQueueId_t smsQ_PIDVelHandle;
// extern osMessageQueueId_t smsQ_CtrMHandle;
extern osMutexId_t mutex_encHandle;

extern int16_t speed_db_r_fill;
extern int16_t speed_db_l_fill;

pid_instance_int16 pid_manager_motor1;
pid_instance_int16 pid_manager_motor2;

extern param_motor motor1_manager;
extern param_motor motor2_manager;

int16_t boost =0;
/**
* @brief Function implementing the Task_PIDCtr thread.
* @param argument: Not used
* @retval None
*/

smsQ_buff_pid smsQ_PIDspeed;

/* USER CODE END Header_PIDCtr_Processor */
void PIDCtr_Processor(void *argument)
{
  /* USER CODE BEGIN PIDCtr_Processor */
	ePID_taskState estate = PID_INIT;
	move_aver_instance_int16 fillter_pid_M1;
	move_aver_instance_int16 fillter_pid_M2;

	// smsQ_buff_motor smsQ_ctrM;

	int16_t ret_m1 = 0;
	int16_t ret_m2 = 0;

  /* Infinite loop */
  for(;;)
  {
	  switch (estate)
	  {
	  	  case PID_INIT:
	  		  // time pend wait periperal timer init
	  		  osDelay(500);

	  		  pid_manager_motor1 = set_pid_gain(4, 20, 0.2, H_DISC_CONSTA);
	  		  PID_ctrlInit(&pid_manager_motor1, 0, speed_db_l_fill);

	  		  pid_manager_motor2 = set_pid_gain(4, 20, 0.2, H_DISC_CONSTA);
	  		  PID_ctrlInit(&pid_manager_motor2, 0, speed_db_r_fill);

	  		  Average_filter_reset(&fillter_pid_M1);
	  		  Average_filter_reset(&fillter_pid_M2);

	  		  PID_updateComp(&pid_manager_motor1);
	  		  PID_updateComp(&pid_manager_motor2);

	  		  estate = PID_IDLE;
	  		  break;

	  	  case PID_IDLE:
			  if(osOK != osMessageQueueGet(smsQ_PIDVelHandle, &smsQ_PIDspeed, 0, TIME_SAMPLE))
			  {
				  __NOP();
			  }
			  else
			  {
				  // debug -> Khi thay doi Gain của PID
				  PID_updateComp(&pid_manager_motor1);
		  		  PID_updateComp(&pid_manager_motor2);
			  }

	  		  estate = PID_RUN;
	  		  break;

	  	  case PID_RUN:
	  		  // mutex encoder value
	  		  osMutexAcquire(mutex_encHandle, 100);
	  		  ret_m1 = PID_compute_ctrl_action(
	  				  &pid_manager_motor1, smsQ_PIDspeed.speed_disM1, speed_db_r_fill);

			  // mutex
	  		  ret_m2 = PID_compute_ctrl_action(
	  				  &pid_manager_motor2, smsQ_PIDspeed.speed_disM2, speed_db_l_fill);
	  		  osMutexRelease(mutex_encHandle);

#if USE_FILL

	  		  motor1_manager.speed = Average_filter_apply(&fillter_pid_M1, ret_m1);
	  		  motor2_manager.speed = Average_filter_apply(&fillter_pid_M2, ret_m2);

#else
	  		motor1_manager.speed = ret_m1;
#endif

	  		  // CTR
	  		  MOTOR_rotation(motor2_manager, smsQ_PIDspeed.speed_disM2);
	  		  MOTOR_rotation(motor1_manager, smsQ_PIDspeed.speed_disM1);

	  		  estate = PID_IDLE;
	  		  break;
	  }
  }
  /* USER CODE END PIDCtr_Processor */
}




//	  	  case PID_SEND:
////			   update dir of robot by @task_motorCtr.c
//	  		  smsQ_ctrM.id = idPID;
//	  		  smsQ_ctrM.estate &= 0x00;
//
//	  		// smsQ_ctrM.estate = MOTOR_DRIVE_FORWARD;
//	  		  // DIR
//	  		  if(motor2_manager.speed >= 0)
//	  			smsQ_ctrM.estate |= MOTOR2_FOWA;
//	  		   else
//	  			 smsQ_ctrM.estate |= MOTOR2_BACK;
//	  		  if(motor1_manager.speed >= 0)
//	  			smsQ_ctrM.estate |= MOTOR1_FOWA;
//	  		  else
//	  			 smsQ_ctrM.estate |= MOTOR1_BACK;
//
//	  		  // speed
//	  		  smsQ_ctrM.speed_right = motor1_manager.speed;
//	  		  smsQ_ctrM.speed_left  = motor2_manager.speed;
//	  		  // send
//			  if(osOK != osMessageQueuePut(smsQ_CtrMHandle, &smsQ_ctrM , 0, 0))
//				  __NOP();
//
//	  		  estate = PID_IDLE;
//	  		  break;
