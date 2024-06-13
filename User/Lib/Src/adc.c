
/*
 * adc.c
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 */

#include "adc.h"

void adc_init(void)
{
	//ADC1 IN0: PA0
		RCC-> APB2ENR |= RCC_APB2ENR_IOPAEN;
		GPIOA -> CRL &= ~(0b1111<<0);// Analog mode
		
		RCC ->CFGR &= ~(0b11<<14);
		RCC ->CFGR |= (0b10<<14);//ADC prescaler 6 12Mhz(max 14Mhz)
	
    RCC-> APB2ENR |= RCC_APB2ENR_ADC1EN;
	
		ADC1-> CR1 |= (1<<8); //enable scan mode
		ADC1-> CR2 &= ~(1<<1); //Single conversion mode
	
	  ADC1-> JSQR &= ~(0b11<<20);
		ADC1-> JSQR |= (0b01<<20); //01: 2 conversions
	
    ADC1-> SMPR1 |= (0b111<<18); //sample time: 239.5 cycles IN16
		ADC1-> SMPR2 |= (0b111<<0); //sample time: 239.5 cycles IN0
	
		ADC1-> JSQR &= ~(1<<15); //set source for JSQ4 is PA0 (IN0)	
	  ADC1-> JSQR |= (16u<<10); //set source for JSQ3 is temp sensor (IN16)


    ADC1-> CR2 |= (1<<23); //enable temp sensor

    ADC1-> CR2 |= (1<<0); //Enable ADC and to start conversion
}
float adc_get_temp_ss(void)
{
    float temp = 0;
    float vin = 0;
    uint16_t raw_data = 0;

	
		ADC1->CR2 |= (0b111 << 12) ;//External event select for injected group: JSWSTART
		ADC1->CR2 |= (0b1 << 15) ;//1: Conversion on external event enabled
	
    // trigger ADC start convert
    ADC1-> CR2 |= (1u<<21);//Start conversion of injected channels

    //wait until end of conversion
    while(((ADC1-> SR>>2)&1) == 0);
    ADC1-> SR &= ~(1u<<2); //clear JEOC flag

    raw_data = ADC1-> JDR1; //read ADC data form JDR1 (data of JSQ4)

    vin = (raw_data*3300.0)/4095.0;

    temp = ((1430.0 - vin) / 4.3) + 25.0;

    return temp;
}

uint16_t adc_get_pa0(void)
{
    uint16_t raw_data = 0;
	
		ADC1->CR2 |= (0b111 << 12) ;//External event select for injected group: JSWSTART
		ADC1->CR2 |= (0b1 << 15) ;//1: Conversion on external event enabled
	
    // trigger ADC start convert
    ADC1-> CR2 |= (1u<<21);//Start conversion of injected channels

    //wait until end of conversion
    while(((ADC1-> SR>>2)&1) == 0);
    ADC1-> SR &= ~(1u<<2); //clear JEOC flag

    raw_data = ADC1-> JDR2; //read ADC data form JDR1 (data of JSQ4)

    //vin = (raw_data*3300.0)/4095.0;

    return raw_data;
}
