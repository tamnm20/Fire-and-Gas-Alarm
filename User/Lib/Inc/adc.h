/*
 * adc.h
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f10x.h"

void adc_init(void);
float adc_get_temp_ss(void);
uint16_t adc_get_pa0(void);

#endif /* INC_ADC_H_ */
