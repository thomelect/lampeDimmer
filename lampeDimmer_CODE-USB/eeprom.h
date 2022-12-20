/**
@file 		i2c.h
@brief 		Librairie I2C qui prmet l'utilisation complète de la lecture et de l'écriture à un module I²C en interruption.
@author 	Thomas Desrosiers
@version 	1.1
@date 		2021/03/07
*/

#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_INT_ 1

#define EE_DATA_SIZE_MAX 128 // Taille du buffer.
#define EE_ADDR_MAX 0x03FF
#define EE_ADDR_MAX_CNT (((0x03FF + 1) / 4) - 1)

/**
 * @brief 
 * 
 */
void eepromClear(void);

/**
 * @brief 
 * 
 * @return 1 ou en fonction de la disponibilité du eeprom 
 */
uint8_t eepromIsBusy(void);

/**
 * @brief 
 * @param add 
 * @return uint8_t Valeur lue.
 */
uint8_t eepromReadByte(uint16_t add);

/**
 * @brief 
 * @param add 
 * @param data 
 */
void eepromWriteByte(uint16_t add, uint8_t data);

// #ifdef I2C_H_
// 
// /**
// *@brief  Fonction d'initialisation du I²C à une fréquence voulu.
// *@param  frequence Fréquence voulu.
// *@param  fcpu      Fréquence du CPU.
// */
// void i2cInit(uint32_t frequence, uint32_t fcpu);
// 
// /**
// *@brief  Fonction qui lis l'état des I/O du slave à l'adresse spécifié.
// *@param  add Adresse du slave duquel on souhaite lire l'état des I/O.
// */
// void i2cReadByte(uint8_t add);
// 
// /**
// *@brief  Fonction qui lis l'état des I/O du slave à l'adresse spécifié sur plusieurs octets.
// *@param  add  Adresse du module I²C.
// *@param  size Nombre d'octet à lire.
// */
// void i2cReadBytes(uint8_t add, uint8_t size);
// 
// /**
// *@brief  Fonction qui envoie plusieurs octet depuis un tableau d'octet.
// *@param  add    Adresse du module I²C.
// *@param  source Pointeur vers le tableau de donnés à envoyer.
// *@param  size   Nombre d'éléments du tableau qui seront transmis.
// *@return uint8_t Nombre d'octet envoyé.
// */
// uint8_t i2cWriteBytes(uint8_t add, uint8_t *source, uint8_t size);
// 
// #endif

#endif /* EEPROM_H_ */