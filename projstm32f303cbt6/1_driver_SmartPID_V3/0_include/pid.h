/*
 * motor_control.h
 *
 *  Created on: 20 Feb 2024
 *      Author: BaoThuan
 */

#ifndef _PID_H_
#define _PID_H_

#include "stm32f3xx_hal.h"
#include <lib_encoder.h>

#define SAMPL_TMR 100

typedef struct
{
	struct
	{
		float p;
		float i;
		float d;
	}gain;

	struct
	{
		// discretiztion constant (s)
		float h;
		// discrete - time err and ctrl
		float errK;
		float errKm1;
		float errKm2;
		float ctlK;
		float ctlKm1;
	} disc;

	struct
	{
		// computed
		float a1;
		float a2;
		float a3;
	} comp;
} pid_instance_int16;


/**
  * @brief	Cài đặt thông số độ lợi PID
  * @param	pid_instance	: Địa chỉ tới biến struct chứa thông số pid cần thiết cho tính toán
  * @retval
  */
pid_instance_int16 set_pid_gain(float p, float i, float d, float delayP);

void PID_updateComp(pid_instance_int16* pid_instance);

void PID_ctrlInit (pid_instance_int16* pid_instance, uint8_t dSpeed, int16_t speed_measure);

int16_t PID_compute_ctrl_action (pid_instance_int16* pid_instance, uint8_t dSpeed, int16_t speed_measure);


#if V1
/**
  * @brief  Hàm tính toán PID đầu vào là tốc độ hiện tại và mong muốn, đầu ra là điện áp cấp cho động cơ giúp đạt đến xác lập
  * @param	pid_instance	: Địa chỉ tới biến struct chứa thông số pid cần thiết cho tính toán
  *			input_error		: Sai số tốc độ
  *			sampling_rate	: Thời gian lấy mẫu
  * @retval none
  */
void apply_pid(pid_instance_int16* pid_instance, int16_t input_error, float sampling_rate);
#endif
#endif /* _PID_H_ */
