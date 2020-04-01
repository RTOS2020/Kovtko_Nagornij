#include "stm32f10x.h"

void delay(int ms) {
	long i;
	for (i = 0; i < 10000 * ms; i++) {
		__NOP();
	}
}

void main(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	
	for (;;) {
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) != 0) {
			GPIO_SetBits(GPIOB, GPIO_Pin_8);
			delay(1000);
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
			delay(1000);
		} else {
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		}
	}
}
