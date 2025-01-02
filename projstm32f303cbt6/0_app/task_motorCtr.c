/*
 * task_motorCtr.c
 *
 *  Created on: Dec 24, 2024
 *      Author: BaoThuan
 */
#include "task_motorCtr.h"

extern osMessageQueueId_t smsQ_CtrMHandle;
extern osMessageQueueId_t smsQ_PIDVelHandle;

//extern int16_t speed_db_r_fill;
//extern int16_t speed_db_l_fill;

// PID param
pid_instance_int16 pid_param_motor_left;
pid_instance_int16 pid_param_motor_right;

smsQ_buff_motor smsQ_ctrM_buff;

param_motor motor1_manager;
param_motor motor2_manager;

eMotor_taskState eState = MOTOR_INIT;

/**
* @brief Function implementing the TaskRobotCtr thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RobotCtr_Processor */
void RobotCtr_Processor(void *argument)
{
  /* USER CODE BEGIN RobotCtr_Processor */

  // eMotor_taskState eState = MOTOR_INIT;
	smsQ_buff_pid smsQ_PIDspeed = {0,0};
	eMotor_dir dir_old = FOWARD;

//	int16_t speed_m1_old_dis = 0;
  /* Infinite loop */
  for(;;)
  {
		switch (eState) {
			case MOTOR_INIT:
				motor1_manager = MOTOR_Contr_Init(&htim4, TIM_CHANNEL_2);
				motor2_manager = MOTOR_Contr_Init(&htim4, TIM_CHANNEL_1);

				motor1_manager.id_motor = 1;
				motor2_manager.id_motor = 2;

				eState = MOTOR_IDLE;
				break;

/*-----------------------------------------------------------*/
			case MOTOR_IDLE:
				// eState = eState_old;
				// Trạng thái Idle có thể được thực hiện dòng lệnh ch�?  Queue, flag,..
				// Ch�? lệnh để quyết định rẽ hay đi thẳng
				  if(osOK != osMessageQueueGet(smsQ_CtrMHandle, &smsQ_ctrM_buff, 0, osWaitForever))
				  {
					  __NOP();
				  }
				  else
				  {
					  eState = smsQ_ctrM_buff.estate;
				  }
				break;

/*-----------------------------------------------------------*/
			// Forward or Back -> Driving
			case MOTOR_DRIVE_FORWARD:
				// doi chieu dung co => return 0 speed de dao chieu dong co
				if(dir_old == BACK)
				{
					// return 0 position -> PID or --

//					motor1_manager.speed = 0;
//					motor2_manager.speed = 0;
//					MOTOR_rotation(motor1_manager, 0);
//					MOTOR_rotation(motor2_manager, 0);

					// returned 0
					dir_old = FOWARD;
				}
				eState = MOTOR_DRIVING;
				break;
			case MOTOR_DRIVE_BACK:
				// doi chieu dung co => return 0 speed de dao chieu dong co
				if(dir_old == FOWARD)
				{
					// return 0 position -> PID or --

//					motor1_manager.speed = 0;
//					motor2_manager.speed = 0;
//					MOTOR_rotation(motor1_manager, 0);
//					MOTOR_rotation(motor2_manager, 0);

					// returned 0
					dir_old = BACK;
				}
				eState = MOTOR_DRIVING;
				break;
			// Driving
			case MOTOR_DRIVING:
				// PID trien khai

				// smsQ_ctrM_buff 	: toc do mong muon
				// smsQ_PIDspeed 	: toc do truyen pid - ctr motor

				smsQ_PIDspeed.speed_disM1 = smsQ_ctrM_buff.speed_right;
				smsQ_PIDspeed.speed_disM2 = smsQ_ctrM_buff.speed_left;

				if(osOK != osMessageQueuePut(smsQ_PIDVelHandle, &smsQ_PIDspeed , 0, 0))
				  __NOP();

				eState = MOTOR_IDLE;
#if defV1
				int16_t err = speed_m1_old_dis - smsQ_ctrM_buff.speed_right;
				// tang toc -> err < 0
				// giam toc -> err > 0

				// tang toc
				if (err < -ERR_ALLOW)
				{
					smsQ_PIDspeed.speed_disM1 = smsQ_PIDspeed.speed_disM1 + 10;

					// dk dong co mong muon -> task pid
					if(osOK != osMessageQueuePut(smsQ_PIDVelHandle, &smsQ_PIDspeed , 0, 0))
					  __NOP();

					osDelay(1000);
				}
				// giam toc
				else if (err > ERR_ALLOW)
				{
					smsQ_PIDspeed.speed_disM1 = smsQ_PIDspeed.speed_disM1 - 10;

					// dk dong co mong muon -> task pid
					if(osOK != osMessageQueuePut(smsQ_PIDVelHandle, &smsQ_PIDspeed , 0, 0))
					  __NOP();

					osDelay(1000);
				}
				else
				{
					smsQ_PIDspeed.speed_disM1 = smsQ_ctrM_buff.speed_right;

					// dk dong co mong muon -> task pid
					if(osOK != osMessageQueuePut(smsQ_PIDVelHandle, &smsQ_PIDspeed , 0, 0))
					  __NOP();

					eState = MOTOR_IDLE;
				}
				speed_m1_old_dis = smsQ_PIDspeed.speed_disM1;
#endif
				break;

/*-----------------------------------------------------------*/
			// Turn left or right -> Turnning
			case MOTOR_TURN_LEFT:

				eState = MOTOR_TURNNING;
				break;
			case MOTOR_TURN_RIGHT:

				eState = MOTOR_TURNNING;
				break;
			// Turnning
			case MOTOR_TURNNING:
				// Robot turning
				eState = MOTOR_IDLE;
				break;

		}
		// Đảm bảo về lại smsQPend để kiểm tra dữ liệu có truyền đến hay không
		// if(eState != MOTOR_IDLE)
			// eState_old = eState;
  }
  /* USER CODE END RobotCtr_Processor */
}

#if V2
/**
* @brief Function implementing the TaskRobotCtr thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RobotCtr_Processor */
void RobotCtr_Processor(void *argument)
{
  /* USER CODE BEGIN RobotCtr_Processor */

  in4_motor_struct M_contr_main;
  /*SPEED [0-99] THUC TE*/
  uint16_t speed_RT_ML = 0;
  uint16_t speed_RT_MR = 0;

  eMotor_taskState eState = MOTOR_INIT;
  eMotor_taskState eState_old = MOTOR_INIT;

  eMotor_dir dir = FOWARD;

  /* Infinite loop */
  for(;;)
  {
		switch (eState) {
			case MOTOR_INIT:
				M_contr_main = MOTOR_Contr_Init(&htim4, &htim4, TIM_CHANNEL_1, TIM_CHANNEL_2);
				// PID Init
				set_pid_gain(&pid_param_motor_left, kp_L, ki_L, kd_L);
				set_pid_gain(&pid_param_motor_right, kp_R, ki_R, kd_R);
				eState = MOTOR_IDLE;
				break;

/*-----------------------------------------------------------*/
			case MOTOR_IDLE:
				eState = eState_old;
				// Trạng thái Idle có thể được thực hiện dòng lệnh ch�?  Queue, flag,..
				// Ch�? lệnh để quyết định rẽ hay đi thẳng
				  if(osOK != osMessageQueueGet(smsQ_CtrMHandle, &smsQ_ctrM_buff, 0, 1))
				  {
					  __NOP();
				  }
				  else
				  {
					  eState = smsQ_ctrM_buff.id;
				  }
				break;

/*-----------------------------------------------------------*/
			// Forward or Back -> Driving
			case MOTOR_DRIVE_FORWARD:
				if(dir == BACK)
				{
					// return 0 position
					speed_RT_ML = MOTOR1_rotation(&M_contr_main, 0);
					speed_RT_MR = MOTOR2_rotation(&M_contr_main, 0);
					// returned 0
					if(speed_RT_ML == 0 && speed_RT_MR == 0)
						dir = FOWARD;
					eState = MOTOR_IDLE;
				}
				else
				{
					DIRL_FOWARD;
					DIRR_FOWARD;
					eState = MOTOR_DRIVING;
				}
				break;
			case MOTOR_DRIVE_BACK:
				if(dir == FOWARD)
				{
					// return 0 position
					speed_RT_ML = MOTOR1_rotation(&M_contr_main, 0);
					speed_RT_MR = MOTOR2_rotation(&M_contr_main, 0);
					// returned 0
					if(speed_RT_ML == 0 && speed_RT_MR == 0)
						dir = BACK;
					eState = MOTOR_IDLE;
				}
				else
				{
					DIRL_BACK;
					DIRR_BACK;
					eState = MOTOR_DRIVING;
				}
				break;
			// Driving
			case MOTOR_DRIVING:
				// PID Control
				// Add mutex
				if(speed_db_r_fill < 0)
					speed_db_r_fill = -speed_db_r_fill;
				if(speed_db_l_fill < 0)
					speed_db_l_fill = -speed_db_l_fill;

				apply_pid(	&pid_param_motor_right, \
							smsQ_ctrM_buff.speed_right - speed_db_r_fill, 	// Mong muon - hien tai
							(1.0/SAMPL_TMR));
				apply_pid(	&pid_param_motor_left,	\
							smsQ_ctrM_buff.speed_left - speed_db_l_fill, 		// Mong muon - hien tai
							(1.0/SAMPL_TMR));

				// straight moving robot
				if(pid_param_motor_left.output < 0)
					pid_param_motor_left.output = -pid_param_motor_left.output;
				if(pid_param_motor_right.output < 0)
					pid_param_motor_right.output = -pid_param_motor_right.output;

				// ConTrol Voltage
				speed_RT_ML = MOTOR1_rotation(&M_contr_main, pid_param_motor_left.output);
				speed_RT_MR = MOTOR2_rotation(&M_contr_main, pid_param_motor_right.output);
				eState = MOTOR_IDLE;
				osDelay(SAMPL_TMR);
				break;

/*-----------------------------------------------------------*/
			// Turn left or right -> Turnning
			case MOTOR_TURN_LEFT:
				DIRL_BACK;
				DIRR_FOWARD;
				eState = MOTOR_TURNNING;
				break;
			case MOTOR_TURN_RIGHT:
				DIRL_FOWARD;
				DIRR_BACK;
				eState = MOTOR_TURNNING;
				break;
			// Turnning
			case MOTOR_TURNNING:
				// Robot turning
				speed_RT_ML = MOTOR1_rotation(&M_contr_main, smsQ_ctrM_buff.speed_left);
				speed_RT_MR = MOTOR2_rotation(&M_contr_main, smsQ_ctrM_buff.speed_right);
				eState = MOTOR_IDLE;
				break;

		}
		// Đảm bảo về lại smsQPend để kiểm tra dữ liệu có truyền đến hay không
		if(eState != MOTOR_IDLE)
			eState_old = eState;

//  	  // Hiện Log debug
//  	  memset(ArrSpeed_Send, 0, strlen(ArrSpeed_Send));
//  	  sprintf(ArrSpeed_Send, "SpRT: %u, Sp: %u\n", speed_RT_ML, speed_ML);
//  	  HAL_UART_Transmit(&huart1, ArrSpeed_Send, strlen(ArrSpeed_Send), 100);
  }
  /* USER CODE END RobotCtr_Processor */
}
#endif
