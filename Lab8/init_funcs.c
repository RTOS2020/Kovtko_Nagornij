#include "init_funcs.h"
void initGPIO(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIOB->CRH = 0b1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIOA->CRH = 0b1;
}

void initTIM(){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		// 72000000 / 71999 = 1000
		TIM_TimeBaseInitStruct.TIM_Prescaler = 719;
		TIM_TimeBaseInitStruct.TIM_Period = 99;
		TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
		
		// Enable TIM2 interrupt
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		
}

void initIRQ(){
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
}