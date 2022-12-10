/**
@file 		adc.c
@brief 		Librairie ADC qui comprend les fonctions d'initialisation et de transmission de la valeur lu vers le main (getter). La converssion se déclanche manuellement larsque l'on appel la fonction adcGetValue().
@author 	Thomas Desrosiers
@version 	1.0
@date 		2021/02/28
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adcBasic2.h"

uint16_t _adcVal[2];

void adcInit2()
{
	//ADMUX = REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0
	//ADCSRA = ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
	//ADCSRB = ADHSM ACME MUX5 – ADTS3 ADTS2 ADTS1 ADTS0
	//DIDR0 = ADC7D ADC6D ADC5D ADC4D - - ADC1D ADC0D
	//DIDR2 = - - ADC13D ADC12D ADC11D ADC10D ADC9D ADC8D

	ADMUX = 0b01000000;		//VREF=2.56V
	ADCSRA = 0b10000111;	//div128
	ADCSRB = 0b10000000;
	DIDR0 = 0b00000001; //entrée numérique désactivée pour le ADC0.
}

uint16_t adcGetValue2(uint8_t canal)
{
	ADMUX &= ~5; //ADC0
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC)); //Ligne qui permet d'attendre qui la mesure soit terminée.
		_adcVal[0] = (ADCL | (ADCH<<8))>>2; //Mesure de ADC0
	ADMUX |= 5; //ADC1
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC)); //Ligne qui permet d'attendre qui la mesure soit terminée.
		_adcVal[1] = (ADCL | (ADCH<<8)); //Mesure de ADC1
	return _adcVal[canal];  //Retourne la valeur de ADC 0 ou 1 en fopnction de la valeur de canal.
}

uint8_t adcRead82(uint8_t canal)
{
	return (adcGetValue2(canal) >> 2);
}