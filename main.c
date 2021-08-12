/**
@file 		main.c
@brief		Laboratoire qui vise � exp�rimenter la lecture d'un clavier matriciel. La m�thode choisie est dans le cas pr�sent la lecture par ADC. Cette m�thode est plus complexe au montage mais offre comme avantage de n'utiliser qu'une seule broche.
@author 	Thomas Desrosiers
@version 	1.0
@date 		2021/05/05

@mainpage 	lab_7_integration
@author 	Thomas Desrosiers
@section 	MainSection1 Description
Laboratoire qui vise � exp�rimenter la lecture d'un clavier matriciel. La m�thode choisie est dans le cas pr�sent la lecture par ADC. Cette m�thode est plus complexe au montage mais offre comme avantage de n'utiliser qu'une seule broche.
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include "usart.h"
#include "adcBasic.h"
#include <avr/interrupt.h>
#include <stdio.h>

#define _KEY_AVERAGE_SIZE 4 //Utilis� comme r�f�rence au compteur pour le moyennage.
#define TIMER_CNT_CYCLE 25 //Nombre de cycle compt�s en interruption.

volatile uint16_t msCnt = 0; //Compteur utilis�s pour compter 25 fois un d�lai de 1ms.
volatile uint8_t msFlag = 0; //Flags qui est mis � 1 � chaques 25ms.

char msg[5];

//Prototypes de fonctions.
/**
*@brief  Fonction d'initialisation des diff�rents I/O et fonctions.
*/
void miscInit(void);

/**
*@brief  Fonction d'initialisation du Timer #0.
*/
void timer0Init(void);

int main(void)
{
	miscInit();
	
	while (1)
	{
		if (msFlag)
		{
			msFlag = 0;
			sprintf(msg, "%d\n\r", adcRead8());
			usartSendString(msg);
		}
	}
}

/**
*@brief Interruption de la comparaison sur OCR0A (timer0) qui met msFlag � 1 lorsque msCnt atteint TIMER_CNT_CYCLE qui est d�finie dans le define. msCnt est incr�ment� � chaques 1ms.
*/
ISR(TIMER0_COMPA_vect)
{
	msCnt++;
	if (msCnt >= TIMER_CNT_CYCLE)
	{
		msCnt -= TIMER_CNT_CYCLE;
		msFlag = 1;
	}
}

void miscInit(void)
{
	usartInit(1000000, F_CPU); //Initialisation du USART � 1Mbps.
	timer0Init(); //Initialisation de timer 0.
	adcInit(); //Appel de la fonction d'initialisation du ADC.
}

void timer0Init()
{
	//TCCR0A : COM0A1 COM0A0 COM0B1 COM0B0 � � WGM01 WGM00
	//TCCR0B : FOC0A FOC0B � � WGM02 CS02 CS01 CS00
	//TIMSK0 : � � � � � OCIE0B OCIE0A TOIE0
	uint8_t top = 250; //Valeur de OCR0A.
	TCCR0A = 0b00000010; //Mode CTC.
	TCCR0B = 0b00000011; //Prescaler de 64.
	TIMSK0 = 0b00000010; //Output compare match A interrupt enable.
	OCR0A = top-1; //62.5ns * 64 * 250 = 1ms.
	sei();
}