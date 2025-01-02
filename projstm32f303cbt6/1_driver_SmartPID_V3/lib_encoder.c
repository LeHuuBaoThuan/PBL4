/*
 * Created on
 *
 * @Author:     Le Huu Bao Thuan
 * @Date:
 * @LastEditors:
 * @LastEditTime:
 */

#include<lib_encoder.h>

/* Private variables ---------------------------------------------------------*/
static encoder_instance_t enc_instance_M1 = {0, 0, 0, 0};
static encoder_instance_t enc_instance_M2 = {0, 0, 0, 0};
// static encoder_user_t enc_user_M2 = {0, 0, 0};

/* Private func macro---------------------------------------------------------*/
static void update_encoder(encoder_instance_t *encoder_value, TIM_HandleTypeDef *htim);
static int16_t calculator_enc_rpm (int16_t vel_Pul_per_SamplTime, uint8_t samplTime_ExpMin1, uint16_t pulse_enc);
#if NEED_RESET
static void reset_encoder(encoder_instance_t *encoder_value);
#endif




/**
  * @brief		Init htime cho thu vien
  * @param		tim_E1: htime encoder tren motor 1
  * @param		tim_E2: htime encoder tren motor 2
  * @retval
  */
void ENC_Init(TIM_HandleTypeDef *tim_E1, TIM_HandleTypeDef *tim_E2)
{
	enc_instance_M1.tim_enc = tim_E1;
	enc_instance_M2.tim_enc = tim_E2;
}




/**
  * @brief		pulse/s -> rpm
  * @param
  * @retval
  */
encoder_user_t* ENCM_Handl(void)
{
//	static encoder_user_t enc_user_M1 = {0, 0, 0};
//	static encoder_user_t enc_user_M2 = {0, 0, 0};

	static encoder_user_t enc_user_M[] =
	{
			{0, 0, 0},
			{0, 0, 0}
	};
	// Encoder debug varriables

	// Motor 1
	// Nhan lai encoder de tinh velocity
	enc_user_M[0].count_enc = __HAL_TIM_GET_COUNTER(enc_instance_M1.tim_enc);
	update_encoder(&enc_instance_M1, enc_instance_M1.tim_enc);
	enc_user_M[0].enc_pos = enc_instance_M1.position;
	// Van toc (rpm)
	enc_user_M[0].enc_vel = calculator_enc_rpm(enc_instance_M1.velocity, SAMP_TIME_EXP_MIN1, ENC_PULSE);

	// Motor 2
	// Nhan lai encoder de tinh velocity
	enc_user_M[1].count_enc = __HAL_TIM_GET_COUNTER(enc_instance_M2.tim_enc);
	update_encoder(&enc_instance_M2, enc_instance_M2.tim_enc);
	enc_user_M[1].enc_pos = enc_instance_M2.position;
	// Van toc (rpm)
	enc_user_M[1].enc_vel = calculator_enc_rpm(enc_instance_M2.velocity, SAMP_TIME_EXP_MIN1, ENC_PULSE);

	return enc_user_M;
}




/**
  * @brief
  * @param
  * @retval
  */
static void update_encoder(encoder_instance_t *encoder_value, TIM_HandleTypeDef *htim)
{
	uint32_t temp_counter = __HAL_TIM_GET_COUNTER(htim);
	static uint8_t first_time = 0;
	if(!first_time)
	{
		encoder_value->velocity = 0;
		first_time = 1;
	} /*END if(!first_time)*/
	else
	{
		/*
		 * GIAI THICH CHI TIET:
		 * -
		 *
		 *
		 * */
		if(temp_counter == encoder_value->last_counter_value)		// Neu couter_gia? = couter_cu~ => Xe dung yen
		{
			encoder_value->velocity = 0;
		}
		else if(temp_counter > encoder_value->last_counter_value)	// Neu couter_gia? > couter_cu~ => Xe chay
		{
			if(__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
			{
				encoder_value->velocity = -encoder_value->last_counter_value - (__HAL_TIM_GET_COUNTER(htim) - temp_counter);
			}
			else
			{
				encoder_value->velocity = temp_counter - encoder_value->last_counter_value;
			}
		}
		else														// Neu couter_gia? <= couter_cu~ => Xe chay nguoc chieu lai
		{
			if(__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
			{
				encoder_value->velocity = temp_counter - encoder_value->last_counter_value;
			}
			else
			{
				encoder_value->velocity = temp_counter + (__HAL_TIM_GET_COUNTER(htim) - encoder_value->last_counter_value);
			}
		}
	} /*END else if(first_time)*/
	encoder_value->position += encoder_value ->velocity;
	encoder_value->last_counter_value = temp_counter;
}


#if NEED_RESET
static void reset_encoder(encoder_instance_t *encoder_value)
{
	encoder_value->velocity = 0;
	encoder_value->position = 0;
	encoder_value->last_counter_value = 0;
}
#endif


/**
  * @brief  	Tinh toan van toc : pulse/s -> rpm
  * @param		vel_Pul_per_SamplTime	: velocity n (pulse/samplingTime (pulse/s))
  * @param		samplTime_ExpMin1		: 1 / Thời gian lấy mẫu (sec)
  * @param		pulse_enc				: Số xung trên nhãn encoder
  * @retval		rpm						: Số vòng/phút
  */
static int16_t calculator_enc_rpm (int16_t vel_Pul_per_SamplTime, uint8_t samplTime_ExpMin1, uint16_t pulse_enc)
{
	int16_t rpm = 0; // (round/min)
	// pulse 1 round enc = pulse_enc * 4
	// rpm = (vel_pul_per_sampl_time * 60 * samplTime_ExpMin1) / (4 * pulse_enc)	// (round/min)
	rpm = (vel_Pul_per_SamplTime * 15 * samplTime_ExpMin1) / (pulse_enc);
	return rpm;
}
