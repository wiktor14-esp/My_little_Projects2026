


#include <stdio.h>

//ustawienie urzywanego obszaru pamieci
//to w sektoreze AHB1 znajduja sie rejestry GPIO ktorych urzyjemy

#define PERIPH_BASE			(0X40000000UL) //rejestr bazowy naszej pamieci
#define	AHB1PERIPH_OFFSET	(0X00200000UL) // komurka pamieci ktorej bedziemy urzywac
#define AHB1PERIPH_BASE		(PERIPH_BASE + AHB1PERIPH_OFFSET) 

//ustawienie rejestru naszego GPIO

#define GPIOA_OFFSET		(0x0000UL) //miejsce w komurce AHB1 gdzie znajduja sie rejestry GPIOA
#define GPIOA_BASE	(AHB1PERIPH_OFFSET + GPIOA_OFFSET) //adres docelowy

//deklarowanie adresu zegara , domyslnie jest wylaczony wiec musimy go odpalic
#define RCC_OFFSET			(0x3800UL)
#define RCC_BASE			(AHB1PERIPH_BASE + RCC_OFFSET)

//deklarujemy rejestr wlaczajacy port GPIO
#define AHB1EN_R_OFFSET		(0X30UL)
#define RCC_AHB1EN_R 		(*(volatile unsigned int *)(RCC_BASE + AHB1EN_R_OFFSET))

//deklaraja rejestru okreslajacego tryb portu GPIO
#define MODE_R_OFFSET		(0X00UL)

//zadeklarowanie rejestrow trybu portu GPIOA
#define GPIOA_MODE_R 		(*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))

//deklaracja ktory port chcemy urzyc
#define OD_R_OFFSET 		(0X14UL)

//adres danych wyjsciowych pinu GPIOA
#define GPIOA_OD_R 			(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))

//maski bitowe wlaczajace adres portu GPIOA 
#define GPIOAEN	(1U<<0)
#define PIN5 	(1U<<5)

//alias do maski bitowej pinu 5
#define LED_PIN PIN5

int main(void)
{
	//wlaczenie dostepu do zegara
	RCC_AHB1EN_R |= GPIOAEN;
	
	//przez deklaracje rejestru 10 na 1 i 11 na zero deklarujemy pin jako wyjscie
	GPIOA_MODE_R |= (1U<<10);
	GPIOA_MODE_R &= ~(1U<<11);
	
	while(1){
		GPIOA_OD_R |= LED_PIN;
		}
}

