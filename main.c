
#include <stdio.h>
#include "GPIO.h"

int main(void)
{
	RCC_APB2ENR |= RCC_IOPAEN;
	GPIO_InitTypeDef led_pin;
	led_pin.Pin = PIN5;
	led_pin.Mode = GPIO_MODE_OUTPUT_PP_2MHZ;
	GPIO_Init(GPIOA, &led_pin);
	GPIO_SetPin(GPIOA, PIN5);

while(1)
{}	
	return 0;
}

