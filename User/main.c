/*
 * main.c
 *
 *  Created on: May 6, 2024
 *      Author: TAMRD
 */

#include "led.h"
#include "adc.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "tim2.h"
#include "DS18B20.h"
#include "timers.h"

TaskHandle_t task_2 = NULL;
TaskHandle_t task_3 = NULL;

TimerHandle_t xMinuteTimer;
TimerHandle_t xThirtySecondTimer;

typedef struct  
{
	float ds18b20;
	uint32_t mq2;
} alarm;

float device_temp;
float cur_temp=0;
float temp_average = 0;
uint32_t mq2_value=0;
alarm room;
QueueHandle_t process_queue;
EventGroupHandle_t temp_event;
SemaphoreHandle_t uart_lock;

void vApplicationGetTimerTaskMemory(StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    /* Cung cap bo nho cho task Timer */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                        StackType_t ** ppxIdleTaskStackBuffer,
                                        uint32_t * pulIdleTaskStackSize )
{
	    /* Cung cap bo nho cho task IDLE */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void func_2(void* param)
{
	int measure_cnt = 0;
	uint16_t adc_value =0;
	float temp=0;
	while(1){
		device_temp = adc_get_temp_ss();
		room.mq2 = adc_get_pa0();
		room.ds18b20 = DS18B20_ReadTemp();
		xQueueSend(process_queue,&room,10000);
		vTaskDelay(100);
		if(++measure_cnt >= 10)
		{
			measure_cnt =0;
			//set event
			xEventGroupSetBits(temp_event, 1);
		}
	}
}

void func_3(void* param)
{	
	cur_temp = temp_average;
	float temp1 = 0;
	uint16_t adc_value =0;
	while(1){
		//wail until temp_event is set
		xEventGroupSync(temp_event,0,1,0xffffffff);	
		xSemaphoreTake(uart_lock, 0xffffffff);
		xQueueReceive(process_queue, &room,10000);
		temp_average = room.ds18b20;
		mq2_value = room.mq2;
		while(uxQueueMessagesWaiting(process_queue)>0)
		{
			xQueueReceive(process_queue, &room,0);
			temp_average += room.ds18b20;
			mq2_value += room.mq2;
		}
		temp_average /= 10.0;
		mq2_value /=10;
		//uart2_printf("[DS18B20]: temp: [%f]\t[MQ2]: mq2_value: [%d]\r\n",temp_average,mq2_value);
		if(((GPIOB -> ODR >> 12)&1)==0)	led_control(0);
		if(temp_average>50&&temp_average<100)
		{
			uart2_printf("Gui canh bao!!!!\r\n");
			uart_printf("AT+ALARM=1\n");
			led_control(1);
		}
		if(mq2_value>2000)
		{
			uart2_printf("Phat hien khi gas!!!!\r\n");
			uart_printf("AT+ALARM=2\n");
			led_control(1);
		}
		xSemaphoreGive(uart_lock);
	}
}

 void func_4(void* param)
 {	
 	while(1){
		xSemaphoreTake(uart_lock, 0xffffffff);
 		uart2_printf("[task 4]: xin chao\r\n");
		xSemaphoreGive(uart_lock);
 		vTaskDelay(500);
 	}
 }

void vMinuteTimerCallback(TimerHandle_t xTimer) {
	uart2_printf("Minute timer callback executed.\n");
	float pre_temp = cur_temp;
	//float pre_temp = 1;
	cur_temp = temp_average;
	uart2_printf("pre temp: %f, cur temp: %f\r\n",pre_temp,cur_temp);
	if(cur_temp>(pre_temp+8)&&(pre_temp!=0))
	{
		uart_printf("AT+ALARM=1\n");
		uart2_printf("Gui canh bao!!!!\r\n");
		led_control(1);
	}
	else
	{
		led_control(0);
	}
}

void vThirtySecondTimerCallback(TimerHandle_t xTimer) {
	xTaskCreate(func_2, "task 2", 128*4, NULL, 0, &task_2);
	xTaskCreate(func_3, "task 3", 128*4, NULL, 0, &task_3);
	uart2_printf("Thirty-second timer callback executed.\n");
    // Delete the timer after callback execution
  xTimerDelete(xTimer, 0);
}

void setup()
{
	leds_init();
	led_control(0);
	adc_init();
	UART1_init();
	UART2_init();
	tim2_init();
	uart2_printf("Initialize the system...\n");
	
	process_queue= xQueueCreate(11, sizeof(alarm));
	temp_event = xEventGroupCreate();
	uart_lock = xSemaphoreCreateMutex();

	xMinuteTimer= xTimerCreate(
			"MinuteTimer",                    // name timer, for debug
			pdMS_TO_TICKS(60000),       // Thoi gian timeout là 60 giây (60000 mili giây)
			pdTRUE,                     // Timer tu dong reload khi het han
			(void *)0,                  // ID cua timer, có the su dung de phân biet các timer khác nhau
			vMinuteTimerCallback              // Hàm callback duoc goi khi timer het han
	);
	if (xMinuteTimer != NULL) {
			// Bat dau timer, do tre ban dau la 0
			if (xTimerStart(xMinuteTimer, 0) != pdPASS) {
					uart2_printf("Failed to start timer.\n");
			}
	} else {
			uart2_printf("Failed to create timer.\n");
	}
	xThirtySecondTimer = xTimerCreate(
			"ThirtySecondTimer",
			pdMS_TO_TICKS(30000),
			pdTRUE,     
			(void *)1,      
			vThirtySecondTimerCallback
	);
	if (xThirtySecondTimer != NULL) {
			if (xTimerStart(xThirtySecondTimer, 0) != pdPASS) {
					uart2_printf("Failed to start timer.\n");
			}
	} else {
			uart2_printf("Failed to create timer.\n");
	}
	vTaskStartScheduler();
}

int main(void)
{
	setup();
	while (1)
	{
	}
	return 0;
}
