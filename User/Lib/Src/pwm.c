/*
 * pwm.c
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 * generate pwm with  f = 1Hz, pulse width = 50%
 */

#include "pwm.h"

void pwm_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA ->CRL &= ~(15u<<0);
	GPIOA ->CRL |= (0xb<<0);//AF push pull
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2 -> ARR = 1000;
	TIM2 -> PSC = 800 - 1;//CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1) T = 1ms
	
	TIM2 ->CCR1 = 0;
	
	// select channel 1 work in COMPARE (PWM mode 1) or capture mode
	TIM2 ->CCMR1 &= ~(3u<<0); //set channel 1 in output compare
	TIM2 ->CCMR1 &= ~(0x7<<4);
	TIM2 ->CCMR1 |= (0x6<<4);//set PWM mode 1
	//TIM2 ->CCMR1 |= (0x7<<4);//set PWM mode 2
	// enable channel 1 (compare/capture)
	TIM2 ->CCER |= (1u<<0);
	//enable counter
	TIM2 -> CR1 |= (1u<<0);
}

void pwm_pulse_ctrl(uint16_t pulse)
{
	TIM2 ->CCR1 = pulse;
}

/*
int main(void) {

		SysClock_Config();
		//leds_init();
		delay_init();
		pwm_init();
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		GPIOA ->CRL &= ~(15u<<24);
		GPIOA ->CRL |= (3<<24);
//		GPIOB -> ODR &= ~(1u<<12);
//		GPIOA -> ODR |= (1u<<0);
		GPIOA -> ODR &= ~(1u<<6);
		uint16_t cnt = 0;
    while (1) {
			if(++cnt > 100) cnt =0;
			pwm_pulse_ctrl(cnt);
//			led_toggle();
			delay_ms(20);
    }
}
*/
