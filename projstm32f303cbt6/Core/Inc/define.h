/*
 * define.h
 *
 *  Created on: Dec 24, 2024
 *      Author: BaoThuan
 */

#ifndef INC_DEFINE_H_
#define INC_DEFINE_H_

#define 	DIR_FORWARD  	0
#define 	DIR_BACK		1

#define 	FILLTER_ENC		1

#include "main.h"

//Enum task state----------------------------------------------------------
typedef enum
{
	ENC_IDLE = 0,
	ENC_INIT,
	ENC_RUNNING,
} eEncoder_taskState;

#define MOTOR1_FOWA 0x01
#define MOTOR1_BACK 0x10

#define MOTOR2_FOWA (0x01 << 2)
#define MOTOR2_BACK (0x10 << 2)

typedef enum
{
	// state basic
	MOTOR_IDLE = 0x00,
	MOTOR_INIT,
	// state drive robot
	MOTOR_DRIVE_FORWARD = MOTOR1_FOWA | MOTOR2_FOWA,
	MOTOR_DRIVE_BACK 	= MOTOR1_BACK | MOTOR2_BACK,
	MOTOR_DRIVING,
	// state turn robot
	MOTOR_TURN_LEFT		= MOTOR1_FOWA | MOTOR2_BACK,
	MOTOR_TURN_RIGHT	= MOTOR2_FOWA | MOTOR1_BACK,
	MOTOR_TURNNING,
} eMotor_taskState;

typedef enum
{
	LED_INIT = 0,
	LED_IDLE,
	LED_RUN
} eLed_taskState;

typedef enum
{
	PID_INIT = 0,
	PID_IDLE,
	PID_RUN,
	// PID_SEND
} ePID_taskState;


//Enum Queue transmiter----------------------------------------------------------
typedef enum
{
	idCAN = 0,
	idEXTI,
	idROUNT,
	idPID
} eID;

typedef struct
{
	eID id;							// can_isr hay exti_isr
	eMotor_taskState state_robot;			// Motor ctr
	uint16_t speed_right;
	uint16_t speed_left;
	uint16_t time_blink_D4;					// led ctr
	uint16_t time_blink_D5;
	uint16_t time_blink_D6;
} smsQ_buff_exti;

typedef struct
{
	eID id;
	eMotor_taskState estate;
	int16_t speed_right;	// speed M1 trung gian
	int16_t speed_left;	// speed M2 trung gian
} smsQ_buff_motor;

typedef struct
{
	uint16_t time_blink_D4;	// 	back - 1000; forward - 2000; stop - 0
	uint16_t time_blink_D5;	// turnning right - 100
	uint16_t time_blink_D6; // turnning left - 100
} smsQ_buff_led;

typedef struct
{
	int16_t speed_disM1;	// speed M1 - right mong muon
	int16_t speed_disM2;	// speed M2 - left mong muon
} smsQ_buff_pid;


//Define PIN --------------------------------------------------------------
// Direction Motor Control
#define DIR1_FOWARD		HAL_GPIO_WritePin(DIR_M1_GPIO_Port, DIR_M1_Pin, DIR_FORWARD)
#define DIR2_FOWARD     HAL_GPIO_WritePin(DIR_M2_GPIO_Port, DIR_M2_Pin, !DIR_FORWARD)

#define DIR1_BACK		HAL_GPIO_WritePin(DIR_M1_GPIO_Port, DIR_M1_Pin, DIR_BACK)
#define DIR2_BACK       HAL_GPIO_WritePin(DIR_M2_GPIO_Port, DIR_M2_Pin, !DIR_BACK)

// State LED control
#define LG_LED_ON		GPIO_PIN_SET
#define LG_LED_OFF		GPIO_PIN_RESET

#define LED4_ON     	HAL_GPIO_WritePin(LED_D4_GPIO_Port, LED_D4_Pin, LG_LED_ON)
#define LED5_ON     	HAL_GPIO_WritePin(LED_D5_GPIO_Port, LED_D5_Pin, LG_LED_ON)
#define LED6_ON     	HAL_GPIO_WritePin(LED_D6_GPIO_Port, LED_D6_Pin, LG_LED_ON)

#define LED4_OFF       	HAL_GPIO_WritePin(LED_D4_GPIO_Port, LED_D4_Pin, LG_LED_OFF)
#define LED5_OFF       	HAL_GPIO_WritePin(LED_D5_GPIO_Port, LED_D5_Pin, LG_LED_OFF)
#define LED6_OFF       	HAL_GPIO_WritePin(LED_D6_GPIO_Port, LED_D6_Pin, LG_LED_OFF)

#define LED4_TOGGLE     HAL_GPIO_TogglePin(LED_D4_GPIO_Port, LED_D4_Pin)
#define LED5_TOGGLE     HAL_GPIO_TogglePin(LED_D5_GPIO_Port, LED_D5_Pin)
#define LED6_TOGGLE     HAL_GPIO_TogglePin(LED_D6_GPIO_Port, LED_D6_Pin)

// even flag btn
#define BTN_SW4			0x11
#define BTN_SW5			0x12
#define BTN_SW6			0x13
#endif /* INC_DEFINE_H_ */
