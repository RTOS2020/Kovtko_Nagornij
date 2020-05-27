#include "stm32f10x.h"
#include "cmsis_os.h"
#include "init_funcs.h"

#define WORD_TIMEOUT 1000

int state = 0;
int scaling = 1;
int time = 0;

typedef struct{
	int* state_ptr;
	int* scaling_ptr;
} Args;

void callback(){
	time++;
}

void delay(int ms) {
		time = 0;
    while(time < ms * 6000){
			
		}
}

void inputThreadFn(void const *arg){
	for(;;){
		delay(300);
		if(GPIOA->IDR & 0b100000000){
			*((Args*)arg)->state_ptr++;
		};
	}
}

void encoderThreadFn(void const *arg){ //simulate encoder //no actual driver
	for(;;){
		if(*((Args*)arg)->state_ptr>3){
			*((Args*)arg)->state_ptr = 0;
		}
		*((Args*)arg)->scaling_ptr = *((Args*)arg)->state_ptr % 3 + 1;
	}
}

void transmissionThreadFn(void const *arg){
	char arr[] = {'.','.','_','_ ',' ','.','_ ','.',' ','.','_'};
	int length = sizeof(arr) / sizeof(char);
	int j=0;
	for(;;j++) {
		char element = arr[j % length];
		if(element == '.') {
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			delay(350);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		} else if(element == '_') {
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			delay(350);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		} else if(element == ' ') {
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			delay(600);
		}
		if(j % length == length - 1) delay(WORD_TIMEOUT * *((Args*)arg)->scaling_ptr);
	}
}

osThreadId main_ID, input_ID1, encoder_ID2, transmission_ID3;
osThreadDef(transmissionThreadFn, osPriorityNormal,1, 0);
osThreadDef(inputThreadFn, osPriorityNormal,1, 0);
osThreadDef(encoderThreadFn, osPriorityNormal,1, 0);
osTimerDef(timer_handle,callback);

int main(void) {
    initGPIO();
		//initTIM();
		osTimerId timer = osTimerCreate(osTimer(timer_handle),osTimerPeriodic,0);
		osTimerStart(timer,1);
		initIRQ();
	
		static Args args;
		args.state_ptr = &state;
		args.scaling_ptr = &scaling;
	
    osKernelInitialize();
    input_ID1 = osThreadCreate(osThread(inputThreadFn), &args);
    encoder_ID2 = osThreadCreate(osThread(encoderThreadFn), &args);
		transmission_ID3 = osThreadCreate(osThread(transmissionThreadFn), &args);
    osKernelStart();
    while (1) { ; }
}
