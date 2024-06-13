/*
 * led.h
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32f10x.h"

void led_toggle(void);
void leds_init(void);
void led_control(uint8_t state);

#endif /* INC_LED_H_ */
