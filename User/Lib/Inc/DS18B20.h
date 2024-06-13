/*
 * DS18B20.h
 *
 *  Created on: Apr 12, 2024
 *      Author: TamNM
 */
#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "stm32f10x.h"

#define DS18B20_CONVERT	0x44
#define DS18B20_READSCRAT 0xBE
#define DS18B20_SKIPROM 0xCC

uint8_t DS18B20_Start (void);
void DS18B20_Write (uint8_t data);
uint8_t DS18B20_Read (void);
void Set_Pin_Output(void);
void Set_Pin_Input(void);
float DS18B20_ReadTemp(void);

#endif /* INC_DS18B20_H_ */
