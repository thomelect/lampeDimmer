/**
 * @file 		adcBasic.h
 * @brief 		Librairie ADC simplifié qui comprend une fonction d'initialisation et de lecture en mode manuel du ADC0.
 * @author 		Thomas Desrosiers
 * @version 	1.0
 * @date 		2021/05/04
 */

#ifndef ADCBASIC_H_
#define ADCBASIC_H_

/**
 * @brief  Fonction d'initialisation du ADC.
 */
void adcInit(void);

/**
 * @brief   Fonction de lecture du ADC0 en mode manuel. Cette fonction est appelé à chaque fois qu'une lecture du ADC01 est requise.
 * @return  Valeur mesurée sur le canal #0 du ADC (ADC0).
 */
uint16_t adcRead(void);

/**
 * @brief   Retourne une lecture de l'ADC en 8bits.
 * @return  Valeur de l'ADC convertie en 8bits.
 */
uint8_t adcRead8(void);

#endif /* ADCBASIC_H_ */