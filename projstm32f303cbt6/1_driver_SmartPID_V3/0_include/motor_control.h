/*
 * motor_control.h
 *
 *  Created on: 20 Feb 2024
 *      Author: BaoThuan
 */

#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_

/* Define   ------------------------------------------------------------------*/
#define SPEED_STOP 0
#define MULTI_MOTOR (1)
#define SPEED_MAX (1000)

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include <lib_encoder.h>
#include "define.h"

typedef enum
{
	ERR = 0,
	OKE
} func_state;

typedef enum
{
	BACK = 0,
	FOWARD,
	STOP
} eMotor_dir;

/* Struct   ------------------------------------------------------------------*/

typedef struct
{
	uint8_t id_motor; 			// 1 motor1, 2 motor2
	TIM_HandleTypeDef *htim;
	uint32_t Channel;
	// speed > 0 -> DIRX_FOWARD
	// speed < 0 -> DIRX_BACK
	int16_t speed;				// thong so dk dong co tu PID
} param_motor;


/* Public func Macro ---------------------------------------------------------*/
param_motor MOTOR_Contr_Init(
						TIM_HandleTypeDef *M_htim,		\
						uint32_t M_Channel);

func_state MOTOR_rotation(param_motor motor_param, int16_t speed_dis);

#ifdef V2
#if MULTI_MOTOR
uint16_t MOTOR2_rotation(in4_motor_struct* M_contr, int16_t speed_m2);              /*Ham quay DC2*/
void MOTOR2_Stop(in4_motor_struct* M_contr);                                        /*Ham Dung dong co 2*/
#else
void MOTOR_Contr_Init(	in4_motor_struct* M_contr,
						TIM_HandleTypeDef *M1_htim,
						uint32_t M1_Channel);
#endif
uint16_t MOTOR1_rotation(in4_motor_struct* M_contr, int16_t speed_m1);				/*Ham quay DC1*/
void MOTOR1_Stop(in4_motor_struct* M_contr);										/*Ham Dung dong co 1*/
#endif


#endif /* 0_INCLUDE_MOTOR_CONTROL_H_ */
