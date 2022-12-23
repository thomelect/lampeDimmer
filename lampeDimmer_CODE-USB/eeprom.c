/**
@file 		i2c.c
@brief 		Librairie I2C qui prmet l'utilisation complète de la lecture et de l'écriture à un module I²C en interruption.
@author 	Thomas Desrosiers
@version 	1.1
@date 		2021/03/07
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "eeprom.h"

volatile uint16_t _eeAdd = 0; // Adresse du module I²C.
// uint8_t _i2cModeRead = 1;			   // Indique si le I²C effectu actuellement une lecture ou une écriture.
volatile uint8_t _eeData[EE_DATA_SIZE_MAX]; // Buffer du I²C.
// uint8_t _i2cRxData[I2C_DATA_SIZE_MAX]; // Buffer du I²C.
volatile uint8_t _eeDataSize = 0; // Nombre d'octet envoyé.
// volatile uint8_t _i2cErrorCode = 0;	   // Variable contenant le code d'erreur.
volatile uint8_t _eeDataCnt = 0; // Donnée disponique.
// volatile uint8_t _i2cRxDataCnt = 0;	   // Donnée disponique.
volatile uint8_t _eeBusy = 0; // Variable indiquand l'état actuel du I²C.
volatile uint8_t _eeCnt = 0;

void eepromClear(void)
{
	_eeData[0] = 0;
	for (uint16_t i = 0; i < EE_ADDR_MAX; i++)
	{
		eepromWriteByte(i, _eeData[0]);
	}
}

uint8_t eepromIsBusy(void)
{
	return _eeBusy; // Retourne l'état du I²C [Occupé / Disponible].
}

uint8_t eepromReadByte(uint16_t add)
{
	while (_eeBusy || (EECR & (1 << EEPE))); // On s'assure que l'oppération précédente est terminé.

	_eeAdd = add;

	EEAR = _eeAdd;
	EECR |= (1 << EERE);

	return EEDR;
}

void eepromWriteByte(uint16_t add, uint8_t data)
{
	while (_eeBusy || (EECR & (1 << EEPE))); // On s'assure que l'oppération précédente est terminé.

	//_eeDataCnt = 0; //Aucune données de disponible.
	_eeAdd = add; // On conserve l'adresse en mémoire.
	_eeData[0] = data; // on Conserve la donnée envoyée en mémoire.
	_eeCnt = 1;

	//_eeDataSize = 1;
	_eeBusy = 1; // On est occupé.
	
	#if !EEPROM_INT_
	EEAR = _eeAdd;
	EEDR = _eeData[0];

	EECR &= ~(1 << EEPE);
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
	_eeBusy = 0; // On est occupé.
	#endif
	#if EEPROM_INT_
	EECR |= (1 << EERIE); // Le bit TWINT est effacé | Bit start | TWI activé | TWI interrupt enable.
	#endif
}

#if EEPROM_INT_
/**
* @brief  Interruption nécessaires à la transmission. Le buffer de transmission est vidé en donnant à UDR1 la valeur de l'octet à envoyer.
*/
ISR(EE_READY_vect)
{
	if (!_eeCnt)
	EECR &= ~(1 << EERIE); // Permet de sortir de l'interruption lorsqu'on sort du if de transmission c'est à dire lorsqu'il n'y a plus aucun caractère à transmettre.
	// 	else
	// 	{
	// EEARH = (_eeAdd>>8);
	// EEARH = _eeAdd;
	EEAR = _eeAdd;
	EEDR = _eeData[0];

	EECR &= ~(1 << EEPE);
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
	_eeBusy = 0; // On est occupé.
	_eeCnt--;
	// EEDR = _txBuffer[_txBufferOut++];
	//	}
}
#endif