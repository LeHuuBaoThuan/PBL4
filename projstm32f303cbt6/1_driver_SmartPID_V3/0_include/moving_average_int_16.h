/*
 * moving_average_int_16.h
 *
 *  Created on: Mar 9, 2024
 *      Author: BaoThuan
 */

#ifndef _INCLUDE_MOVING_AVERAGE_INT_16_H_
#define _INCLUDE_MOVING_AVERAGE_INT_16_H_

#include "stdio.h"
#include "stdint.h"

#define MOVING_AVEGE_LENGTH 30

typedef struct
{
	int16_t bufffer [MOVING_AVEGE_LENGTH];
	uint16_t counter;
	int16_t out;
	int32_t sum;
} move_aver_instance_int16;


/**
  * @brief
  * @param
  * @retval
  */
void Average_filter_reset(move_aver_instance_int16* instance);


/**
  * @brief
  * @param
  * @retval
  */
int16_t Average_filter_apply(move_aver_instance_int16* instance, int16_t input);

#endif /* 0_INCLUDE_MOVING_AVERAGE_INT_16_H_ */
