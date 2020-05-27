/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"

#include "cmsis_os.h"


void delay(int ms) {
    for (int i = 0; i < 5000 * ms; i++) {
    	__NOP();
    }
}

void led_thread1(void const *argument) // Flash LED 1
{
    for (;;) {
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
        delay(300);
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        delay(300);
    }
}

void led_thread2(void const *argument) // Flash LED 2

{
    for (;;) {
    	GPIO_SetBits(GPIOB, GPIO_Pin_9);
        delay(500);
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
        delay(500);
    }
}

osThreadId main_ID, led_ID1, led_ID2, led_ID3;
osThreadDef(led_thread2, osPriorityNormal,1, 0);
osThreadDef(led_thread1, osPriorityNormal,1, 0);

int main(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIOB->CRH = 0b11;
    osKernelInitialize();
    led_ID1 = osThreadCreate(osThread(led_thread1), NULL);
    led_ID2 = osThreadCreate(osThread(led_thread2), NULL);
    osKernelStart();
    while (1) { ; }
}
