/*
 * clock.c
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 * set up RCC to generate a clock 72Mhz (max)
 */

#include "clock.h"

void clock_init(void)
{
	// PLL clock (soure HSE 8Mhz) PLLSCR = 1, PLLMUL = 9
	RCC ->CR |= (1u<<16); //enable HSE
	while(((RCC->CR >> 17)& 1) == 0); //wait HSE ready
	
	FLASH->ACR |= 2u<<0;  // Bat bo dem Flash Prefetch
	
	RCC ->CFGR &= ~(1u<<17);  //PLLSCR = 1: PREDIV1 = 1 for HSE
	
	RCC ->CFGR |= (1u<<16);  //Clock from PREDIV1 (HSE) selected as PLL input clock
	
	RCC ->CFGR |= (7u<<18); //PLLMUL = 9
	
	RCC ->CR |= (1u<<24); //PLL enable
	while(((RCC->CR >> 25)& 1) == 0); //wait PLL ready

	RCC ->CFGR &= ~(3u<<0);
	RCC ->CFGR |= (2u<<0); //System clock Switch: PLL selected as system clock
	
	while (((RCC->CFGR>>2)&(3u)) != (2u)); // wait PLL used as system clock
	
	RCC ->CFGR &= ~(7u<<8);
	RCC ->CFGR |= (4u<<8); //set APB1 prescaler /2 (max APB1 = 36Mhz)
	
}
