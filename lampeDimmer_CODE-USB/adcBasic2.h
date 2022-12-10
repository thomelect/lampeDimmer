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
void adcInit2(void);

/**
 * @brief   Retourne une lecture de l'ADC en 8bits.
 * @return  Valeur de l'ADC convertie en 8bits.
 */
uint16_t adcGetValue2(uint8_t canal);

uint8_t adcRead82(uint8_t canal);

#endif /* ADCBASIC_H_ */