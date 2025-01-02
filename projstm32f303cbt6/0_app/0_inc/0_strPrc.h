/**
* @file           :
* @brief          :
* @author         : Le Huu Bao Thuan
* @date           :
* @version        :
* @par Copyright (c):
*               /
* @par History: 1:Create
*               /
*/

#ifndef _STRPRC_0_STRPRC_H_
#define _STRPRC_0_STRPRC_H_

/* Define   ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Struct   ------------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Public func Macro ---------------------------------------------------------*/
/**
  * @addtogroup _Public_functions_Macro
  * @{
  */

extern char* strPrc_num2str(int i, char b[]);
extern void strPrc_strcpy_pos(char* dest, uint16_t adc, uint32_t pos);
extern char* strPrc_conv32B216B(char* b, char* rs); // "xyzt" (4 bytes)
extern void strPrc_strcpy10bytesADC(char* dest, uint16_t* adc, uint32_t pos);
extern void strPrc_strcpy4bytes_int16ADC(char* dest, int16_t* adc, uint32_t pos);
/**
  * @}  _Public_functions_Macro
  */

#endif /* 0_STRPRC_0_STRPRC_H_ */
