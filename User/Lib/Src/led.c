/*
 * led.c
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 */
 
#include "led.h"

#define LED_PB12

void leds_init(void)
{
#ifdef LED_PB12
	//set PB12 in output push-pull
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOB -> CRH |= (3u << 16);// set PB12 in OUTPUT mode speed 50MHz
	GPIOB -> CRH &= ~(3u << 18); //set PB12 output push-pull
#else
	//set PC13 in output push-pull
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC -> CRH |= (3u << 20);// set PC13 in OUTPUT mode speed 50MHz
	GPIOC -> CRH &= ~(3u << 22); //set PC13 output push-pull
#endif
}

void led_toggle(void){
#ifdef LED_PB12
	if(((GPIOB -> ODR >> 12)&1)==0)
	{
		GPIOB -> ODR |= (1u<<12);
	}
	else
	{
		GPIOB -> ODR &= ~(1u<<12);
	}
#else
	if(((GPIOC -> ODR >> 13)&1)==0)
	{
		GPIOC -> ODR |= (1u<<13);
	}
	else
	{
		GPIOC -> ODR &= ~(1u<<13);
	}
#endif
} 

void led_control(uint8_t state){
#ifdef LED_PB12
	if(state==0)
	{
		GPIOB -> ODR |= (1u<<12);
	}
	else
	{
		GPIOB -> ODR &= ~(1u<<12);
	}
#else
	if(state==0)
	{
		GPIOC -> ODR |= (1u<<13);
	}
	else
	{
		GPIOC -> ODR &= ~(1u<<13);
	}
#endif
}
