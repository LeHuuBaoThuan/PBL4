/*
 * lib_encoder.h
 *
 *  Created on: Feb 16, 2024
 *      Author: BaoThuan
 */

#ifndef _LIB_ENCODER_H_
#define _LIB_ENCODER_H_

/* Define   ------------------------------------------------------------------*/
#define ENC_PULSE				(11*56)				// encode 1 vong bao nhieu xung
#define SAMP_TIME				(10)				// ms
#define SAMP_TIME_EXP_MIN1		(1000/SAMP_TIME) 	// 1 / Thời gian lấy mẫu (sec)


/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Struct   ------------------------------------------------------------------*/

/**
  * @brief	Thong tin tho^ (RAW) cua Encoder
  */
typedef struct
{
	int16_t velocity;					// Unit: Pulse / s
	int64_t position;					// Unit: Pulse
	uint32_t last_counter_value;		// Unit:
	TIM_HandleTypeDef* tim_enc;
} encoder_instance_t;


/**
  * @brief	Du lieu Encoder da xu ly de hien thi cho nguoi dung quan sat
  */
typedef struct
{
	int16_t enc_vel;                    // Unit: rpm
	int32_t enc_pos;                    // Unit:
	uint16_t count_enc;					// Unit: Pulse / s
} encoder_user_t;

/* Public variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private func macro---------------------------------------------------------*/

/* Public func ---------------------------------------------------------------*/
extern void ENC_Init(TIM_HandleTypeDef *tim_E1, TIM_HandleTypeDef *tim_E2);
extern encoder_user_t* ENCM_Handl(void);

#endif /* _LIB_ENCODER_H_ */
