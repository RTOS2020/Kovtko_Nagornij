#include "stm32f10x.h"

#define MAX_THREADS 4

typedef struct {
	void (*task)(int param);
	int executionPeriod; //ns
} Thread;
Thread* threads[MAX_THREADS];
volatile Thread* currentThread;
int* executionTimeCurrent;


void stepOne(int period){
	static char ledOn = 0;
	static long i = 1;
	if(period*40000>3600000){
		currentThread->executionPeriod = 0;
	}
	for(;;){
		if(ledOn < 1)
		{
			GPIOB->ODR |= 0b100000000;
			for(; i <3600000;i++){
				if(i%(period*43000)==0){
					return;
				}
			} //~1s
			currentThread->executionPeriod = 0;
			i=1;
			ledOn = 1;
		} else {
			GPIOB->ODR &= ~(0b1000000000);
			for(; i <3600000;i++){
				if(!i%(period*43000)){
					return;
				}
			} //~1s
			currentThread->executionPeriod = 0;
			i=1;
			ledOn=0;
		}
	}
}

void stepTwo(int period){
	static int i = 1;
	static char ledOn = 0;
	if(period*40000>3600000){
		currentThread->executionPeriod = 0;
	}
	for(;;){
		if(ledOn < 1)
		{
			GPIOB->ODR |= 0b1000000000;
			//GPIO_SetBits(GPIOB,GPIO_Pin_8);
			for(;i<7200000;i++){
				if(!i%(period*43000)){
					return;
				}
			};
			GPIOC->ODR |= 0b10000000000000;
			currentThread->executionPeriod = 0;
			i=1;
			ledOn=1;
		} else {
			GPIOB->ODR &= ~(0b1000000000);
			//GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			for(;i<7200000;i++){
				if(!i%(period*43000)){
					return;
				}
			};
			currentThread->executionPeriod = 0;
			i=1;
			ledOn=0;
		}
	}
}

void stepThree(int period){
	static int i = 1;
	static char ledOn = 0;
	if(period*40000>3600000){
		currentThread->executionPeriod = 0;
	}
	for(;;){
		if(!ledOn)
		{
			//GPIO_SetBits(GPIOB,GPIO_Pin_11);
			GPIOB->ODR |= 0b10000000000;
			for(;i<7200000;i++){
				if(!i%(period*43000)){
					return;
				}
			};
			currentThread->executionPeriod = 0;
			i=1;
			ledOn=1;
		} else {
			//GPIO_ResetBits(GPIOB,GPIO_Pin_11);
			GPIOB->ODR &= ~(0b10000000000);
			for(;i<7200000;i++){
				if(!i%(period*43000)){
					return;
				}
			};
			currentThread->executionPeriod = 0;
			i=1;
			ledOn=0;
		}
	}
}
void idle(int period){
	static long i =1;
	if(period*40000>3600000){
		currentThread->executionPeriod = 0;
	}
	for(;;){
		for(;i <1800000;i++){
			if(!i%(period*43000)){
				return;
			}
		}
		currentThread->executionPeriod = 0;
		i = 1;
	}
}

//40Hz (0.025s)
void TIM3_IRQHandler(){
	static int threadId = 0;
	currentThread = threads[threadId];
	threadId = (threadId + 1)%MAX_THREADS;
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	//currentThread.task(currentThread.executionPeriod);
}

Thread thread0 = (Thread){.task = stepOne, .executionPeriod = 0};
Thread thread1 = (Thread){.task = stepTwo, .executionPeriod = 0};
Thread thread2 = (Thread){.task = stepThree, .executionPeriod = 0};
Thread thread3 = (Thread){.task = idle, .executionPeriod = 0};
void init(){
	threads[0] = &thread0;
	threads[1] = &thread1;
	threads[2] = &thread2;
	threads[3] = &thread3;
}

void timerInit(){
	  TIM_TimeBaseInitTypeDef InitTIM;
	  NVIC_InitTypeDef InitNVIC;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// Налаштування виклику ФОП для TIM3 кожні 500ms
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1))
	// Update Event (Hz) = 72MHz / ((7199 + 1) * (249 + 1)) = 40Hz (0.025s)
	  InitTIM.TIM_Prescaler = 7199;
	  InitTIM.TIM_Period = 249;
	  InitTIM.TIM_ClockDivision = TIM_CKD_DIV1;
	  InitTIM.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM3, & InitTIM);

	  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	  TIM_Cmd(TIM3, ENABLE);

	  InitNVIC.NVIC_IRQChannel = TIM3_IRQn;
	  InitNVIC.NVIC_IRQChannelPreemptionPriority = 0x00;
	  InitNVIC.NVIC_IRQChannelSubPriority = 0x00;
	  InitNVIC.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&InitNVIC);
}

int main(void)
{
	init();
	RCC->APB2ENR = 0b11100;
	GPIOB->CRH = 0x3333;
	GPIOC->CRH = 0x300000;
	timerInit();
	for(;;){
		currentThread->task(currentThread->executionPeriod);
		for(int k;k<72000;k++){};
	}
}
