/*
 * motor_control.c
 *
 *  Created on: 20 Feb 2024
 *      Author: BaoThuan
 */
#include <motor_control.h>

/**
  * @brief 	Nen khai bao truoc khi su dung cac ham ben duoi
  * @param	M_contr
  * @retval	None
  */
param_motor MOTOR_Contr_Init(
						TIM_HandleTypeDef *M_htim,		\
						uint32_t M_Channel)
{
	param_motor M_contr = {0, M_htim, M_Channel};

    HAL_TIM_PWM_Start(M_contr.htim, M_contr.Channel);

	return M_contr;
}

/**
  * @brief	Ham quay DC1 dieu khien Driver SMART PID - Thegiocchip
  * @param	M_contr
  *
  * MotorLeft
  */
#define CONTROL_DIR 1

func_state MOTOR_rotation(param_motor motor_param, int16_t speed_dis)
{
	//check
	if(motor_param.speed > SPEED_MAX)
		return ERR;

#if CONTROL_DIR

	if(motor_param.speed >= 0)
	{
		// Quay thuan ma toc do tinh tu pid < 0 => khong chap nhan
		if(speed_dis < 0)
		{
			__HAL_TIM_SET_COMPARE(motor_param.htim, motor_param.Channel, 0);
		}
		else
		{
			if(motor_param.id_motor == 2)
				DIR2_FOWARD;
			else
				DIR1_FOWARD;
			__HAL_TIM_SET_COMPARE(motor_param.htim, motor_param.Channel, motor_param.speed);
		}
	}
	else
	{
		// Quay thuan ma toc do tinh tu pid > 0 => khong chap nhan
		if(speed_dis >= 0)
		{
			__HAL_TIM_SET_COMPARE(motor_param.htim, motor_param.Channel, 0);
		}
		else
		{
			if(motor_param.id_motor == 2)
				DIR2_BACK;
			else
				DIR1_BACK;

			__HAL_TIM_SET_COMPARE(motor_param.htim, motor_param.Channel, - motor_param.speed);
		}
	}
#endif

	return OKE;
}

#ifdef V2

#if MULTI_MOTOR
/**
  * @brief	Ham quay DC2 dieu khien Driver SMART PID - Thegiocchip
  * @param	M_contr		: Chứa thông số bộ timer đang được sử dụng
  * 		speed_m2	: Tốc độ motor 1 mong muốn
  * @retval	uint16_t	: Tốc độ hiện tại
  *
  * MotorRight
  */
uint16_t MOTOR2_rotation(in4_motor_struct* M_contr, int16_t speed_m2)
{
	static int16_t speed_old_M2 = SPEED_STOP;

	if(speed_m2 < SPEED_STOP)			// Kiem tra
	{
		speed_m2 = SPEED_STOP;
		__HAL_TIM_SET_COMPARE(M_contr->M2_htim, M_contr->M2_Channel, SPEED_STOP);
	}
	else
	{
		if(speed_m2 - speed_old_M2 > 20)														// decelerate
		{
			__HAL_TIM_SET_COMPARE(M_contr->M2_htim, M_contr->M2_Channel, speed_old_M2 ++);
		}
		else if(speed_m2 - speed_old_M2 < -20)												// accelerate
		{
			__HAL_TIM_SET_COMPARE(M_contr->M2_htim, M_contr->M2_Channel, speed_old_M2 --);
		}
		else																				// run mode
		{
			speed_old_M2 = speed_m2;
			__HAL_TIM_SET_COMPARE(M_contr->M2_htim, M_contr->M2_Channel, speed_old_M2);
		}
	}
	return speed_old_M2;
}



/**
  * @brief	Ham Dung dong co 2
  * @param	M_contr		: Chứa thông số bộ timer đang được sử dụng
  * @retval	none
  */
void MOTOR2_Stop(in4_motor_struct* M_contr)
{
	__HAL_TIM_SET_COMPARE(M_contr->M2_htim, M_contr->M2_Channel, SPEED_STOP);
}

#else

/**
  * @brief 	Nen khai bao truoc khi su dung cac ham ben duoi
  * @param	M_contr		: Chứa thông số bộ timer
  * 		M1_htim	: Nặc danh thanh ghi Timer được sd để đk tốc độ ĐC Trái
  * 		M2_htim	: Nặc danh thanh ghi Timer được sd để đk tốc độ ĐC Phải
  * 		M1_Channel	: Nặc danh Channel được sd để đk tốc độ ĐC Trái
  * 		M2_Channel	: Nặc danh Channel được sd để đk tốc độ ĐC Phải
  * @retval	None
  */
void MOTOR_Contr_Init(	Motor_struct* M_contr,
						TIM_HandleTypeDef *M1_htim,
						uint32_t M1_Channel)
{
	M_contr->M1_htim = M1_htim;
	M_contr->M1_Channel = M1_Channel;
}

#endif


/**
  * @brief	Ham quay DC1 dieu khien Driver SMART PID - Thegiocchip
  * @param	M_contr		: Chứa thông số bộ timer đang được sử dụng
  * 		speed_m1	: Tốc độ motor 1 mong muốn	[-99; 99]
  * @retval	uint16_t	: Tốc độ hiện tại
  *
  * MotorLeft
  */
uint16_t MOTOR1_rotation(Motor_struct* M_contr, int16_t speed_m1)
{
	static int16_t speed_old_M1 = SPEED_STOP;

	if(speed_m1 < SPEED_STOP)			// Kiem tra
	{
		speed_m1 = SPEED_STOP;
		__HAL_TIM_SET_COMPARE(M_contr->M1_htim, M_contr->M1_Channel, SPEED_STOP);
	}
	else
	{
		if(speed_m1 - speed_old_M1 >= 0)														// decelerate
		{
			__HAL_TIM_SET_COMPARE(M_contr->M1_htim, M_contr->M1_Channel, speed_old_M1 ++);
		}
		else if(speed_m1 - speed_old_M1 < 0)												// accelerate
		{
			__HAL_TIM_SET_COMPARE(M_contr->M1_htim, M_contr->M1_Channel, speed_old_M1 --);
		}
		else																				// run mode
		{
			speed_old_M1 = speed_m1;
			__HAL_TIM_SET_COMPARE(M_contr->M1_htim, M_contr->M1_Channel, speed_old_M1);
		}
	}
	return speed_old_M1;
}



/**
  * @brief	Ham Dung dong co 1
  * @param	M_contr		: Chứa thông số bộ timer đang được sử dụng
  * @retval	none
  */
void MOTOR1_Stop(Motor_struct* M_contr)
{
	__HAL_TIM_SET_COMPARE(M_contr->M1_htim, M_contr->M1_Channel, SPEED_STOP);
}
#endif

