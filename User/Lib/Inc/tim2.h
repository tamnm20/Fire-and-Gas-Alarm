/*
 * tim2.h
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 */

#ifndef TIM2_H_
#define TIM2_H_

#include "stm32f10x.h"

void tim2_init(void);
void tim2_delay_1us(void);
void tim2_delay_us(uint32_t us);
void tim2_delay(uint32_t ms);
void TIM2_IRQHandler(void);

#endif /* INC_TIM2_H_ */
