/*
 * pwm.h
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 */

#ifndef PWM_H_
#define PWM_H_

#include "stm32f10x.h"

void pwm_init(void);
void pwm_pulse_ctrl(uint16_t pulse);

#endif /* INC_PWM_H_ */
