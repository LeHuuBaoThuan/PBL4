/*
 * moving_average_int_16.c
 *
 *  Created on: Mar 9, 2024
 *      Author: BaoThuan
 */
#include <moving_average_int_16.h>

void Average_filter_reset(move_aver_instance_int16* instance)
{
	instance->counter = 0;
	instance->sum = 0;
	instance->out = 0;
	for(int i = 0; i < MOVING_AVEGE_LENGTH; i++)
	{
		instance->bufffer[i] = 0;
	}
}

int16_t Average_filter_apply(move_aver_instance_int16* instance, int16_t input)
{
	instance->sum += input - instance->bufffer[instance->counter];
	instance->bufffer[instance->counter] = input;
	instance->counter++;
	if(instance->counter == MOVING_AVEGE_LENGTH)
	{
		instance->counter = 0;
	}
	instance->out = instance->sum / MOVING_AVEGE_LENGTH;
	// normalizition
	return instance->out;
}
