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
* @par Reference     :
*/

/* Define   ------------------------------------------------------------------*/
#define V1 0
#define V2 1

/* Includes ------------------------------------------------------------------*/
#include"0_strPrc.h"
#include "string.h"

/* Struct   ------------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private func macro---------------------------------------------------------*/


/**
  * @}  _Private_functions_Macro
  */

/* Public func ---------------------------------------------------------------*/
/**
  * @addtogroup _Public_functions
  * @{
  */

char* strPrc_num2str(int i, char b[]) // Ex: (uint16) 1234 --> (Arr 4 bytes) "1234"
{
    char const digit[] = "0123456789";
    char* p = b;

    if(i<0)
    {
        *p++ = '-';
        i *= -1;
    }

    p = p + 4;
    do
    { //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}



char* strPrc_conv32B216B(char* b, char* rs) // (Arr 4 bytes) "1234" --> (Arr 2 bytes)
{
	char* p = rs;

	*p = ((*(b + 1) << 4) & 0xF0) | (*b & 0x0F);
	*(p + 1) = ((*(b + 3) << 4) & 0xF0) | (*b & 0x0F);

	return rs;
}



void strPrc_strcpy_pos(char* dest, uint16_t adc, uint32_t pos) // v1: , char* src)
{

#if (V1)
uint8_t i = 0;
*src 		= adc & 0x00FF;
	*(src + 1) 	= (adc >> 8) & 0x00FF;

	dest = dest + pos;	// Dich den vi tri muon coppy src

	while(i <= 1)			// Coppy src -> des
	{
		*(dest++) = *(src++);
		i++;
	}
#endif /*V1*/

#if (V2)
	dest += pos;
	*(dest) 		= adc & 0x00FF;
	*(dest + 1) 	= (adc >> 8) & 0x00FF;
#endif /*V2*/
}


void strPrc_strcpy10bytesADC(char* dest, uint16_t* adc, uint32_t pos)
{
//	dest += pos;
//	for(uint8_t i = 0; i <= 3; i++)
//	{
//		*(dest + (i << 1)) 		= *(adc + i) & 0x00FF;
//		*(dest + (i << 1) + 1) 	= (*(adc + i) >> 8) & 0x00FF;
//	}

	// xxyyzztt\n
	dest += pos;

	*(dest) 		= *adc & 0x00FF;
	*(dest + 1) 	= *adc >> 8;

	*(dest + 2) 	= *(adc + 1) & 0x00FF;
	*(dest + 3) 	= *(adc + 1) >> 8;

	*(dest + 4) 	= *(adc + 2) & 0x00FF;
	*(dest + 5) 	= *(adc + 2) >> 8;

	*(dest + 6) 	= *(adc + 3) & 0x00FF;
	*(dest + 7) 	= *(adc + 3) >> 8;
}


void strPrc_strcpy4bytes_int16ADC(char* dest, int16_t* adc, uint32_t pos)
{
	// xxyyzztt\n
	dest += pos;

	*(dest) 		= *adc & 0x00FF;
	*(dest + 1) 	= *adc >> 8;

	*(dest + 2) 	= *(adc + 1) & 0x00FF;
	*(dest + 3) 	= *(adc + 1) >> 8;
}
/**
  * @}  _Public_functions
  */


/* Private func --------------------------------------------------------------*/
/**
  * @addtogroup HTTP_F_Private
  * @{
  */



/**
  * @}  _Private_functions
  */



/* ISR func ------------------------------------------------------------------*/

/* MAIN func -----------------------------------------------------------------*/

/* Task Init -----------------------------------------------------------------*/

/* Task func -----------------------------------------------------------------*/
/**
  * @brief
  * @param
  * @retval
  */


 /**
  * @brief
  *
  * @param      method :
  *                 @arg
  *                 @arg
  * @retval
  * @retval
  */


/* pulse variables */

/* State flag of motor flag */
