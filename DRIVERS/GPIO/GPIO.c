
#include <stdio.h>
#include "GPIO.h"

void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
	uint32_t pinNumber =0;
	
	for (int i=0 ; i<8 ; i++){
		if (GPIO_InitStruct->Pin & (1U << i)){
			pinNumber = i;
			break;
		}
	}
	uint32_t bitPosition = pinNumber *4;
	GPIOx->CRL &= ~(0xFUL << bitPosition);
	GPIOx->CRL |= ((uint32_t)GPIO_InitStruct->Mode << bitPosition);
	
}

void GPIO_SetPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->ODR |= GPIO_Pin;
	}

void GPIO_ResetPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->ODR &= ~GPIO_Pin;
	}


void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->ODR ^= GPIO_Pin;
	}

