/*
 * temp.h
 *
 * Created: 2022-12-07 11:35:42
 *  Author: thoma
 */ 


#ifndef TEMP_H_
#define TEMP_H_

#define FILTRE_SIZE			100
#define FILTRE_ECART_MAX	10

#define OFFSET_TEMP_AMB		5.5

float arrondir(float unRound, float roundTo, uint8_t byPass);
float filtreFenetre(float tempRaw);
float tempHandler(void);
void tempInit(void);


#endif /* TEMP_H_ */