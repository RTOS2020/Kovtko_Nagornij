#include "stm32f10x.h"

void delay(int ms) {
	long i;
	for (i = 0; i < 10000 * ms; i++) {
		__NOP();
	}
}

int main(void) {
	char ura[] = {'.', '.', '_', ' ', '.', '_', '.', ' ', '.', '_'};
	int length = sizeof(ura) / sizeof(char);

	int *GPIO_C_CRH = (int*)(0x40011004);
	int *GPIO_C_ODR = (int*)(GPIO_C_CRH + 0x8);

	int *apb2enr = (int*)0x40021018;
	int i = 0;

	*apb2enr |= 0x10;
	*GPIO_C_CRH = 0x00300000;

	for(;;) {
		char element = ura[i % length];
		if(element == '.') {
			*GPIO_C_ODR=0x00000100;
			delay(150);
		} else if(element == '_') {
			*GPIO_C_ODR=0x00000100;
			delay(300);
		} else if(element == ' ') {
			*GPIO_C_ODR=0x0000000;
			delay(600);
		}
		i++;
		if(i % length == length - 1) {
			delay(3000);
			i = 0;
		};
	}
}
