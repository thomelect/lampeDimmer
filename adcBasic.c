/**
@file 		adcBasic.c
@brief 		Librairie ADC simplifié qui comprend une fonction d'initialisation et de lecture en mode manuel du ADC0.
@author 	Thomas Desrosiers
@version 	1.0
@date 		2021/05/04
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adcBasic.h"

void adcInit()
{
	//ADMUX = REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0
	//ADCSRA = ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
	//ADCSRB = ADHSM ACME MUX5 – ADTS3 ADTS2 ADTS1 ADTS0
	//DIDR0 = ADC7D ADC6D ADC5D ADC4D - - ADC1D ADC0D
	//DIDR2 = - - ADC13D ADC12D ADC11D ADC10D ADC9D ADC8D
	
	ADMUX = (1<<REFS0); //Référence sur AVcc avec un condensateur sur la broche AREF.
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //ADC enable / ADC diviseur = 128 (le plus lent possible).
	ADCSRB = (1<<ADHSM);
	DIDR0 = (1<<ADC0D); //entrée numérique désactivée pour le ADC0.
}

uint16_t adcRead()
{
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC)); //On attend que la lecture du ADC soir terminée.
	return ADC;
}