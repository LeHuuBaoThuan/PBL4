/*
 * motor_control.c
 *
 *  Created on: 20 Feb 2024
 *      Author: BaoThuan
 */

#include <pid.h>
#include <motor_control.h>
#include <stdlib.h>


#define CCR_MOTOR_RIGHT_STOP(htim, channel)	 				__HAL_TIM_SET_COMPARE(htim, channel, SPEED_STOP)
#define CCR_MOTOR_LEFT_STOP(htim, channel)	 				__HAL_TIM_SET_COMPARE(htim, channel, SPEED_STOP)

#define CCR_MOTOR_RIGHT_RUN(htim, channel, CCR_MR) 			__HAL_TIM_SET_COMPARE(htim, channel, CCR_MR)
#define CCR_MOTOR_LEFT_RUN(htim, channel, CCR_ML) 			__HAL_TIM_SET_COMPARE(htim, channel, CCR_ML)

#define INTEGRAL_GAIN_MAX 	1000
#define PID_MAX				100

/**
  * @brief	Cài đặt thông số độ lợi PID
  * @param	pid_instance	: Địa chỉ tới biến struct chứa thông số pid cần thiết cho tính toán
  * @retval
  */
pid_instance_int16 set_pid_gain(float ep, float ei, float ed, float delayP)
{
	pid_instance_int16 pid_instance =
	{
			{ep, ei, ed},
			{delayP, 0.0, 0.0, 0.0, 0.0, 0.0},
			{0, 0, 0},
	};

	pid_instance.comp.a1 = 	pid_instance.gain.p + pid_instance.gain.i* pid_instance.disc.h 							\
							+  pid_instance.gain.d / pid_instance.disc.h;
	pid_instance.comp.a2 = -( pid_instance.gain.p + 2* pid_instance.gain.d / pid_instance.disc.h);
	pid_instance.comp.a3 =  pid_instance.gain.d / pid_instance.disc.h;

	return pid_instance;
}

void PID_updateComp(pid_instance_int16* pid_instance)
{
	pid_instance->comp.a1 = 	pid_instance->gain.p + pid_instance->gain.i* pid_instance->disc.h 							\
							+  pid_instance->gain.d / pid_instance->disc.h;
	pid_instance->comp.a2 = -( pid_instance->gain.p + 2* pid_instance->gain.d / pid_instance->disc.h);
	pid_instance->comp.a3 =  pid_instance->gain.d / pid_instance->disc.h;
}


void PID_ctrlInit (pid_instance_int16* pid_instance, uint8_t dSpeed, int16_t speed_measure)
{
	pid_instance->disc.errK = dSpeed - speed_measure;
	pid_instance->disc.errKm1 = pid_instance->disc.errK;
	pid_instance->disc.errKm2 = pid_instance->disc.errK;
}

int16_t PID_compute_ctrl_action (pid_instance_int16* pid_instance, uint8_t dSpeed, int16_t speed_measure)
{
	pid_instance->disc.errK = dSpeed - speed_measure;	// > 0 tang toc

	if(abs(pid_instance->disc.errK) < 3.0)
	{
		return pid_instance->disc.ctlKm1;
		// pid_instance->disc.ctlK = 0;
	}
	else
	{
			pid_instance->disc.ctlK = 	(pid_instance->disc.ctlKm1
										+ pid_instance->comp.a1 * pid_instance->disc.errK
										+ pid_instance->comp.a2 * pid_instance->disc.errKm1
										+ pid_instance->comp.a3 * pid_instance->disc.errKm2);
	}

	if(pid_instance->disc.ctlK > SPEED_MAX)		// tăng tốc
	{
		if(pid_instance->disc.errK > 0)
		{
			pid_instance->disc.ctlK = SPEED_MAX;
		}
		else
		{
			pid_instance->disc.ctlK = 0;
		}
	}

	if(pid_instance->disc.ctlK < -SPEED_MAX)	// giảm tốc
	{
		if(pid_instance->disc.errK > 0)
		{
			pid_instance->disc.ctlK = 0;
		}
		else
		{
			pid_instance->disc.ctlK = -SPEED_MAX;
		}
	}


	pid_instance->disc.ctlKm1 = pid_instance->disc.ctlK;
	pid_instance->disc.errKm2 = pid_instance->disc.errKm1;
	pid_instance->disc.errKm1 = pid_instance->disc.errK;

	return (int16_t)pid_instance->disc.ctlK;
}


#if V1
/**
  * @brief  Hàm tính toán PID đầu vào là tốc độ hiện tại và mong muốn, đầu ra là điện áp cấp cho động cơ giúp đạt đến xác lập
  * @param	pid_instance	: Địa chỉ tới biến struct chứa thông số pid cần thiết cho tính toán
  *			input_error		: Sai số tốc độ
  *			sampling_rate	: Thời gian lấy mẫu
  * @retval none
  */
void apply_pid(pid_instance_int16* pid_instance, int16_t input_error, float sampling_rate)
{
	pid_instance->error_integeral += input_error;
	if(pid_instance->error_integeral > INTEGRAL_GAIN_MAX)
	{
		pid_instance->error_integeral = INTEGRAL_GAIN_MAX;
	}
	if(pid_instance->error_integeral < -INTEGRAL_GAIN_MAX)
	{
		pid_instance->error_integeral = -INTEGRAL_GAIN_MAX;
	}


	pid_instance->output = 		pid_instance->p_gain 	* input_error
							+ 	(pid_instance->i_gain 	* pid_instance->error_integeral) / sampling_rate
							+	pid_instance->d_gain 	* sampling_rate * (input_error - pid_instance->last_error);

// sampling_rate = 0.01
	if(pid_instance->output >= PID_MAX)
	{
		pid_instance->output = PID_MAX;
	}
	if(pid_instance->output <= -PID_MAX)
	{
		pid_instance->output = -PID_MAX;
	}
	pid_instance->last_error = input_error;
}
#endif
