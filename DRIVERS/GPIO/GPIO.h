#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIOA_MEM_BASE (0x40010800UL)
#define GPIOB_MEM_BASE (0x40010C00UL)
#define GPIOC_MEM_BASE (0x40011000UL)
#define GPIOD_MEM_BASE (0x40011400UL)
#define GPIOE_MEM_BASE (0x40011800UL)
#define GPIOF_MEM_BASE (0x40011C00UL)
#define GPIOG_MEM_BASE (0x40012000UL)

#define RCC_APB2ENR (*(volatile uint32_t *)(0x40021000UL + 0x18UL))
#define RCC_IOPAEN (1U<<2)
#define RCC_IOPBEN (1U<<3)
#define RCC_IOPCEN (1U<<4)
#define RCC_IOPDEN (1U<<5)
#define RCC_IOPEEN (1U<<6)
#define RCC_IOPFEN (1U<<7)
#define RCC_IOPGEN (1U<<8)

#define PIN0 (1U << 0)
#define PIN1 (1U << 1)
#define PIN2 (1U << 2)
#define PIN3 (1U << 3)
#define PIN4 (1U << 4)
#define PIN5 (1U << 5)
#define PIN6 (1U << 6)
#define PIN7 (1U << 7)


typedef struct {
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
	} GPIO_TypeDef;
	

#define GPIOA ((GPIO_TypeDef *) GPIOA_MEM_BASE)
#define GPIOB ((GPIO_TypeDef *) GPIOB_MEM_BASE)
#define GPIOC ((GPIO_TypeDef *) GPIOC_MEM_BASE)
#define GPIOD ((GPIO_TypeDef *) GPIOD_MEM_BASE)
#define GPIOE ((GPIO_TypeDef *) GPIOE_MEM_BASE)
#define GPIOF ((GPIO_TypeDef *) GPIOF_MEM_BASE)
#define GPIOG ((GPIO_TypeDef *) GPIOG_MEM_BASE)


	typedef enum {
	GPIO_MODE_INPUT_FLOATING = 0x04,
	GPIO_MODE_INPUT_PU_PD = 0x08,
	GPIO_MODE_OUTPUT_PP_2MHZ = 0x02,
	GPIO_MODE_OUTPUT_PP_10MHZ = 0x03,
	}GPIO_MODE_TypeDef;

typedef struct{
	uint16_t Pin;
	GPIO_MODE_TypeDef Mode;
	}GPIO_InitTypeDef;
	
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
void GPIO_SetPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
#endif 
