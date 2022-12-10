/*
 * temp.c
 *
 * Created: 2022-12-07 11:35:20
 *  Author: thoma
 */ 
#include <avr/io.h>
#include "temp.h"
#include "adcBasic2.h"

uint16_t _adcValTemp = 0;
float _adcValTemp1 = 0;
float _adcValTemp2 = 0;
uint8_t _tblFlag = 0;
float _tblData[FILTRE_SIZE];
uint8_t _tblIndex = 0;

/**
*@brief				Fonction qui arrondi une valeur un à la décimale la plus proche du paramètre roundTo.
*@param  unRound	Valeur brut à arrondir.
*@param  roundTo	Valeur décimale de 0 à 1 à laquelle on veut arrondir notre valeur brut (par exemple, 0.15, 0.25, 0.5, etc.).
*@param  byPass		Paramètre qui indique si l'on veut arrondir la valeur (byPass = 0) ou contourner la fonction en retournant la valeur brut (unRound) (byPass = 1).
*@return float		Valeur arrondi ou non en fonction de la valeur du paramètre byPass.
*/
float arrondir(float unRound, float roundTo, uint8_t byPass)
{
	if (byPass)
	return unRound;
	float roundToTmp = (roundTo / 2) + 0.001;
	float multipl =  (uint8_t)(1 / roundTo);
	return((uint8_t)((unRound + roundToTmp) * multipl)) / multipl;
}

float filtreFenetre(float tempRaw)
{
	float valFiltre = 0;
	if (_tblIndex >= FILTRE_SIZE)
	{
		_tblFlag = 1;
		_tblIndex = 0;
	}
	if (!_tblFlag)
	{
		_tblData[_tblIndex++] = tempRaw;
	}
	if (_tblFlag)
	{

		for (uint8_t i = 0; i < FILTRE_SIZE; i++)
		{
			valFiltre += _tblData[i];
		}
		valFiltre /= FILTRE_SIZE;
		if (((tempRaw - valFiltre) < FILTRE_ECART_MAX) && ((valFiltre - tempRaw) < FILTRE_ECART_MAX))
		{
			_tblData[_tblIndex++] = tempRaw;
		}
	}
	else
	{
		valFiltre = 0;
	}
	return valFiltre;
}

float tempHandler(void)
{
	_adcValTemp = adcGetValue2(1); // Lecture du canal 1 du ADC.
	_adcValTemp1 = (float)((_adcValTemp * 2560L) / 1024);
	_adcValTemp2 = (_adcValTemp1 / 28.45925926);
	_adcValTemp2 = (float)((_adcValTemp2 - 23.96617647) * 9.066666667);
	_adcValTemp2 -= OFFSET_TEMP_AMB;
	return filtreFenetre(_adcValTemp2);
}


void tempInit(void)
{
		for (uint8_t i = 0; i < FILTRE_SIZE; i++)
		{
			_tblData[i] = 0;
		}
}