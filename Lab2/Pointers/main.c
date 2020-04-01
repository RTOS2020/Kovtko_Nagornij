#include "stm32f10x.h"

void delay(int ms) {
	long i;
	for (i = 0; i < 10000 * ms; i++) {
		__NOP();
	}
}

int main(void) {
	int *GPIO_C_CRH = (int*) (0x40011004);
	int *GPIO_C_IDR = (int*) (0x40011008);
	int *GPIO_B_CRH = (int*) (0x40010c04);
	int *GPIO_B_ODR = (int*) (0x40010C0C);
	int *apb2enr = (int*) 0x40021018;
	*apb2enr = 0x00018; // |
	*GPIO_C_CRH = 0x30000000; //11 0000 0000 0000 0000 0000 0000 0000
	*GPIO_B_CRH = 0x00000033; //11

	for (;;) {
		if (*GPIO_C_IDR & 0x8000) {
			*GPIO_B_ODR = 0x100;     // for B8
			delay(1000);
			*GPIO_B_ODR = 0x00000000;
			delay(1000);
		} else {
			*GPIO_B_ODR = 0x00000000;
		}
	}
}
