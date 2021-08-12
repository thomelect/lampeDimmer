/**
@file 		adcBasic.h
@brief 		Librairie ADC simplifi� qui comprend une fonction d'initialisation et de lecture en mode manuel du ADC0.
@author 	Thomas Desrosiers
@version 	1.0
@date 		2021/05/04
*/

#ifndef ADCBASIC_H_
#define ADCBASIC_H_

/**
*@brief  Fonction d'initialisation du ADC.
*/
void adcInit();

/**
*@brief  Fonction de lecture du ADC0 en mode manuel. Cette fonction est appel� � chaque fois qu'une lecture du ADC01 est requise.
*@return Valeur mesur�e sur le canal #0 du ADC (ADC0).
*/
uint16_t adcRead();

#endif /* ADCBASIC_H_ */