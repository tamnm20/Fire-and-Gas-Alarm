/*
 * tim2.c
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 */

#include "tim2.h"
#define delay_INT

volatile static uint32_t tim2_cnt = 0;
void TIM2_IRQHandler(void)
{
	TIM2 -> SR &= ~(1u<<0);//clean UIF flag
	tim2_cnt++;
}

void tim2_init(void)
{
	//set 1ms for timer
	//rcc --> 72Mhz --> psc (3) --> 24MHz  
	// ARR = 1000
#ifdef delay_INT
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2 -> ARR = 72 - 1;
	TIM2 -> PSC =   1;//CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)


	TIM2 -> DIER |= (1u); //enable interrupt
	TIM2 -> CR1 |= (1u<<0);
	NVIC -> ISER[0] |= (1<<28);
	
//	TIM2 -> CR1 |= (1u<<0);
#else
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2 -> ARR = 72 - 1;
	TIM2 -> PSC =   0;//CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
	TIM2 -> CR1 |= (1u<<0);
#endif
}

void tim2_delay_1us(void)
{
	while(((TIM2 -> SR)&1) == 0); //wait UIF flag is set to 1
	TIM2 -> SR &= ~(1u<<0);//clean UIF flag
}

void tim2_delay_us(uint32_t us)
{
#ifdef delay_INT
	uint32_t current_cnt = tim2_cnt;
	while((uint32_t)(tim2_cnt-current_cnt) < us);
#else
	while(us--)
	{
		tim2_delay_1us();
	}
#endif
}

void tim2_delay(uint32_t ms)
{
	while(ms--)
	{
		for(int i=0;i<1000;i++)
		{
			tim2_delay_1us();
		}
	}
}
