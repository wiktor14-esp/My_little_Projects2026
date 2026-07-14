


#include <stdio.h>

//ustawienie urzywanego obszaru pamieci
//to w sektoreze AHB1 znajduja sie rejestry GPIO ktorych urzyjemy

#define PERIPH_BASE			(0X40000000UL) //rejestr bazowy naszej pamieci
#define	AHBPERIPH_OFFSET	(0X00020000UL) // komurka pamieci ktorej bedziemy urzywac
#define ABP2PERIPH_OFFSET	(0x00010000UL)
#define AHBPERIPH_BASE		(PERIPH_BASE + AHBPERIPH_OFFSET) 
#define ABP2PERIPH_BASE		(PERIPH_BASE + ABP2PERIPH_OFFSET) 

//ustawienie rejestru naszego GPIO

#define GPIOA_OFFSET		(0x0800UL) //miejsce w komurce AHB1 gdzie znajduja sie rejestry GPIOA
#define GPIOA_BASE			(ABP2PERIPH_BASE + GPIOA_OFFSET) //adres docelowy

//deklarowanie adresu zegara , domyslnie jest wylaczony wiec musimy go odpalic
#define RCC_OFFSET			(0x1000UL)
#define RCC_BASE			(AHBPERIPH_BASE + RCC_OFFSET)

//deklarujemy rejestr wlaczajacy port GPIO
#define AHBEN_R_OFFSET		(0X18UL)
#define RCC_AHBEN_R 		(*(volatile unsigned int *)(RCC_BASE + AHBEN_R_OFFSET))

//deklaraja rejestru okreslajacego tryb portu GPIO
#define CRL_R_OFFSET		(0X00UL)

//zadeklarowanie rejestrow trybu portu GPIOA
#define GPIOA_CRL_R 		(*(volatile unsigned int *)(GPIOA_BASE + CRL_R_OFFSET))

//deklaracja ktory port chcemy urzyc
#define OD_R_OFFSET 		(0X0CUL)

//adres danych wyjsciowych pinu GPIOA
#define GPIOA_OD_R 			(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))

//maski bitowe wlaczajace adres portu GPIOA 
#define GPIOAEN	(1U<<2)
#define PIN5 	(1U<<5)

//alias do maski bitowej pinu 5
#define LED_PIN PIN5

int main(void)
{
	//wlaczenie dostepu do zegara
	RCC_AHBEN_R |= GPIOAEN;
	
	//przez deklaracje rejestru 10 na 1 i 11 na zero deklarujemy pin jako wyjscie
	GPIOA_CRL_R &= ~(0xFUL<<20);
	GPIOA_CRL_R |= (0x2UL<<20);
	
	
	while(1){
		GPIOA_OD_R |= LED_PIN;
		for(volatile int i = 0; i <1000000; i++){}
		}
}

