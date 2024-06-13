/*
 * DS18B20.c
 *
 *  Created on: Apr 12, 2024
 *      Author: TamNM
 * Pin: A1
 */

#include "DS18B20.h"
#include "tim2.h"

uint8_t DS18B20_Start (void)
{
	uint8_t Response = 0;
	Set_Pin_Output();   // set the pin as output
	GPIOA->ODR &= ~(1u<<1);
	//HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin low
	tim2_delay_us (480);   // delay according to datasheet

	Set_Pin_Input();    // set the pin as input
	tim2_delay_us (80);    // delay according to datasheet

	if(((GPIOA->IDR>>1)&1)==0) Response = 1;
	//if (!(HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = 0;

	tim2_delay_us (400); // 480 us delay totally.

	return Response;
}

void DS18B20_Write (uint8_t data)
{
	Set_Pin_Output();  // set as output

	for (int i=0; i<8; i++)
	{
		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1
			Set_Pin_Output();  // set as output
			GPIOA->ODR &= ~(1u<<1);
			//HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
			tim2_delay_us (1);  // wait for 1 us
			Set_Pin_Input();  // set as input
			tim2_delay_us (50);  // wait for 60 us
		}
		else  // if the bit is low
		{
			// write 0
			Set_Pin_Output();
			GPIOA->ODR &= ~(1u<<1);
			//HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
			tim2_delay_us (50);  // wait for 60 us
			Set_Pin_Input();
		}
	}
}

uint8_t DS18B20_Read (void)
{
	uint8_t value=0;

	Set_Pin_Input();

	for (int i=0;i<8;i++)
	{
		Set_Pin_Output();   // set as output
		GPIOA->ODR &= ~(1u<<1);
		//HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the data pin LOW
		tim2_delay_us (1);  // wait for > 1us

		Set_Pin_Input();  // set as input
		if (((GPIOA->IDR>>1)&1)==1)  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		tim2_delay_us (50);  // wait for 60 us
	}
	return value;
}

void Set_Pin_Output(void)
{
	//set PA1 is output push-pull
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA -> CRL |= (2u << 4);// set PA1 in OUTPUT mode speed 50MHz
	GPIOA -> CRL &= ~(3u << 6); //set PA1 output push-pull
}

void Set_Pin_Input(void)
{
	//set PA1 is input pull up
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	
	GPIOA -> CRL &= ~(3u << 4);// set PA1 in INPUT mode
	GPIOA -> CRL |= (2u << 6); //set PA1 output pupd
	GPIOA->ODR |= (1u<<1);//set PA1 output pull up
}

float DS18B20_ReadTemp(void)
{
	uint16_t Temp;
	uint8_t Temp1, Temp2;
	DS18B20_Start();
	tim2_delay(1);
	DS18B20_Write(DS18B20_SKIPROM);
	DS18B20_Write(DS18B20_CONVERT);
	//tim2_delay(200);
	tim2_delay(1);
	
	DS18B20_Start();
	tim2_delay(1);
	DS18B20_Write(DS18B20_SKIPROM);
	DS18B20_Write(DS18B20_READSCRAT);
	
	Temp1 = DS18B20_Read();
	Temp2 = DS18B20_Read();
	
	Temp = (uint16_t)((Temp2<<8)|Temp1);
	float DS18B20_Temp = (float)(Temp*0.0625);
	return DS18B20_Temp;
}
