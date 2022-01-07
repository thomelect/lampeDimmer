/**
@file 		main.c
@brief		Laboratoire qui vise à expérimenter la lecture d'un clavier matriciel. La méthode choisie est dans le cas présent la lecture par ADC. Cette méthode est plus complexe au montage mais offre comme avantage de n'utiliser qu'une seule broche.
@author 	Thomas Desrosiers
@version 	1.0
@date 		2021/05/05

@mainpage 	lab_7_integration
@author 	Thomas Desrosiers
@section 	MainSection1 Description
Laboratoire qui vise à expérimenter la lecture d'un clavier matriciel. La méthode choisie est dans le cas présent la lecture par ADC. Cette méthode est plus complexe au montage mais offre comme avantage de n'utiliser qu'une seule broche.
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include "usart.h"
#include "adcBasic.h"
#include <avr/interrupt.h>
#include <stdio.h>

#define OUTPUT_VALUE(val) (OCR4A = val) //Valeur PWM output R.
#define OUTPUT_INIT() DDRC |= (1 << 7)	//Init output R.
#define SWITCH_INIT() PORTB |= (1 << 1) //Bouton sur PD1.
#define SWITCH() ((PINB & (1 << 1)) == 0)

#define TIMER_CNT_CYCLE_ADC 25	//Nombre de cycle comptés en interruption.
#define TIMER_CNT_CYCLE_FADE 50 //Nombre de cycle comptés en interruption.
#define INCREMENT_STEP 1		//Incrément pour le fadding.
#define _MAX_RXDATASIZE_ 16

//enum des différents paramètres d'une réception
enum RX_STATES
{
	WAIT,
	RXSIZE,
	RXCOMMANDE,
	RXDATA,
	VALIDATE
};

//enum des différents paramètres d'une transmission
enum TX_COMMANDES
{
	VAL_POT,
	VAL_ACTU
};

//enum des différents commandes de transmission d'une réception
enum RX_COMMANDES
{
	GET_VAL_POT,
	SET_VAL,
	SET_SLEEP_MODE,
	GET_VAL_ACTU
};

//déclaration des enums
enum RX_STATES rxState;
enum TX_COMMANDES txCommande;
enum RX_COMMANDES rxCommande;

volatile uint16_t msCntAdc = 0;	 //Compteur utilisés pour compter 25 fois un délai de 1ms pour la mesure de l'ADC.
volatile uint8_t msFlagAdc = 0;	 //Flags qui est mis à 1 à chaques 25ms pour la mesure de l'ADC.
volatile uint16_t msCntFade = 0; //Compteur utilisés pour compter 50 fois un délai de 1ms pour le fade de la sortie.
volatile uint8_t msFlagFade = 0; //Flags qui est mis à 1 à chaques 50ms pour le fade de la sortie.
uint16_t valueAdcTbl[2] = {0, 0};
uint16_t valueOut = 0;
uint16_t valueAdc = 0;
uint8_t veilleMode = 0;
int increment = 5;
char msg[5];

//variables nécessaires à la communication avec l'interface
uint8_t rxDataSize;
uint8_t rxDataCnt = 0; //Compteur de donnés reçus.
uint8_t rxData[_MAX_RXDATASIZE_];
uint16_t rxErrorCommCnt = 0;
uint8_t moteurStopFlag = 0;
uint8_t seqAuto = 0;

//Prototypes de fonctions.

/**
 * @brief  Fonction de traitement des données et commandes reçues.
 */
void execRxCommand(void);

/**
 * @brief  Fonction de traitement pour l'envoie des donnés.
 */
void execTxCommand(void);

/**
 * @brief  Fonction d'initialisation des différents I/O et fonctions.
*/
void miscInit(void);

void outputVeille(uint8_t value);

/**
 * @brief		Fonction qui remplis la structure de donnés avec les paramètres correspondants qui lui sont envoyés en paramètre par la fonction usartRemRxData. Le switch case commence à l'état WAIT qui attend la réception de "<". RXDATA place alors les donnés reçus dans l'union de structure jusqu'à ce que la dernière donnée (ici, la longueur de la trame à été spécifié manuellement à 7 puisque nous n'envoyons pas l'octet qui contient la longueur de la trame. Finalement, VALIDATE s'assure que la trame se termine par ">"
 * @param  data Octet reçu par la fonction usartRemRxData
 */
void parseRxData(uint8_t data);

/*void sendPotValue(uint8_t data);*/

/**
 * @brief  Fonction d'initialisation du Timer #0.
 */
void timer0Init(void);

/**
 * @brief  Fonction d'initialisation du timer #4 utilisé pour le PWM.
 */
void timer4Init(void);

int main(void)
{
	miscInit();

	while (1)
	{
		if (usartRxAvailable())			   //Si un caractère est disponible...
			parseRxData(usartRemRxData()); //appel de la fonction parseRxData() avec en paramètre la valeur retournée par usartRemRxData().
		if (SWITCH())					   //Si l'interrupteur du potentiomètre est à la position "ON"...
		{
			if (msFlagAdc)
			{
				msFlagAdc = 0;
					for (uint8_t i = 0; i < 100; i++) //Une valeur moyenne sur un echantillon de 100 mesures est calculé afin d'éviter d'être entre deux valeurs.
					{
						valueAdcTbl[1] += adcRead8();
					}
					valueAdcTbl[1] /= 100;
					if (valueAdcTbl[1] >= 255) //Si valueOut dépasse 255..
						valueAdcTbl[1] = 255;  //valueOut est limité à 255.
					if (valueAdcTbl[1] != valueAdcTbl[0])
					{
						valueAdcTbl[0] = valueAdcTbl[1]; //La nouvelle valeur remplace l'ancienne.
						valueOut = valueAdcTbl[1];
						valueAdc = valueAdcTbl[1];
						txCommande = VAL_ACTU;
						execTxCommand();
					}
			}
		}
		else //Si l'interrupteur du potentiomètre est à la position "OFF"...
		{
			outputVeille(veilleMode);
			txCommande = VAL_ACTU;
			execTxCommand();
		}
		OUTPUT_VALUE(valueOut);
	}
}

/**
*@brief Interruption de la comparaison sur OCR0A (timer0) qui met msFlag à 1 lorsque msCnt atteint TIMER_CNT_CYCLE qui est définie dans le define. msCnt est incrémenté à chaques 1ms.
*/
ISR(TIMER0_COMPA_vect)
{
	msCntAdc++;
	msCntFade++;
	if (msCntAdc >= TIMER_CNT_CYCLE_ADC)
	{
		msCntAdc -= TIMER_CNT_CYCLE_ADC;
		msFlagAdc = 1;
	}
	if (msCntFade >= TIMER_CNT_CYCLE_FADE)
	{
		msCntFade -= TIMER_CNT_CYCLE_FADE;
		msFlagFade = 1;
	}
}

void execRxCommand(void)
{
	switch (rxCommande)
	{
	case GET_VAL_POT: //État non utilisé
		txCommande = VAL_POT;
		execTxCommand();
		break;
	case SET_VAL:	  //Réception depuis l'interface de la valeur de la sortie.
		if (SWITCH()) //Si l'interrupteur du potentiomètre est à la position "ON"...
			valueOut = rxData[0];
		break;
	case SET_SLEEP_MODE:
		veilleMode = rxData[0];
		break;
	case GET_VAL_ACTU:
		txCommande = VAL_ACTU;
		execTxCommand();
		break;
	}
}

void execTxCommand(void)
{
	char txData[5];
	switch (txCommande)
	{
	case VAL_POT:
		txData[0] = '<';
		txData[1] = 1;
		txData[2] = VAL_POT;
		txData[3] = valueAdc;
		txData[4] = '>';
		for (int x = 0; x <= 4; x++)
		{
			usartSendByte(txData[x]);
		}
		break;
	case VAL_ACTU:
		txData[0] = '<';
		txData[1] = 1;
		txData[2] = VAL_POT;
		txData[3] = valueOut;
		txData[4] = '>';
		for (int x = 0; x <= 4; x++)
		{
			usartSendByte(txData[x]);
		}
		break;
	}
}

void miscInit(void)
{
	adcInit();				   //Appel de la fonction d'initialisation du ADC.
	timer0Init();			   //Initialisation de timer 0.
	timer4Init();			   //Initialisation de timer 4.
	usartInit(1000000, F_CPU); //Initialisation du USART à 1Mbps.

	OUTPUT_INIT();
	SWITCH_INIT();
}

void outputVeille(uint8_t value)
{
	switch (value)
	{
	case 0:
		valueOut = 0;
		break;
	case 1:
		valueOut = 255;
		break;
	case 2:
		if (valueOut <= 1) //Lorsque oc4aValue à atteint son minimum.
		{
			increment = INCREMENT_STEP;
		}
		if (valueOut >= 255) //Lorsque oc4aValue à atteint son maximum.
		{
			increment = -INCREMENT_STEP;
		}
		if (msFlagFade)
		{
			msFlagFade = 0;
			valueOut += increment;
		}
		break;
	case 3:
		valueOut = 1;
		break;
	}
}

/**
 * @brief		Fonction qui sépare les données reçues sur le port série.
 * @param data  Donnée à traiter.
 */
void parseRxData(uint8_t data)
{
	//switch case des différents paramètres de la trame de réception
	switch (rxState)
	{
	//confirmation que la trame débute par '<'
	default:
		if (data == '<')
		{
			rxState = RXSIZE;
			rxDataCnt = 0;
		}
		break;
	//////////////////////////////////////////////////////////////////////////
	case RXSIZE:
		rxDataSize = data;
		if (rxDataSize >= _MAX_RXDATASIZE_)
			rxState = WAIT;
		else
			rxState = RXCOMMANDE;
		break;
	//////////////////////////////////////////////////////////////////////////
	case RXCOMMANDE:
		rxCommande = data;
		if (rxDataSize)
			rxState = RXDATA;
		else
			rxState = VALIDATE;
		break;
	//////////////////////////////////////////////////////////////////////////
	case RXDATA:
		rxData[rxDataCnt++] = data;
		if (rxDataCnt == rxDataSize)
			rxState = VALIDATE;
		break;
	//////////////////////////////////////////////////////////////////////////
	//confirmation que la trame se termine par '>'
	case VALIDATE:
		rxState = WAIT;
		if (data == '>')
			execRxCommand(); //si oui la fonction execRxCommand() est appelée
		else
			rxErrorCommCnt++; // sinon le nombre d'erreur augmente
		break;
	}
}

// void sendPotValue(uint8_t cmd, uint8_t data)
// {
// 	char txData[5];
// 	txData[0] = '<';
// 	txData[1] = 1;
// 	txData[2] = VAL_POT;
// 	txData[3] = data;
// 	txData[4] = '>';
// 	for (int x = 0; x <= 4; x++)
// 	{
// 		usartSendByte(txData[x]);
// 	}
// }

void timer0Init(void)
{
	//TCCR0A : COM0A1 COM0A0 COM0B1 COM0B0 – – WGM01 WGM00
	//TCCR0B : FOC0A FOC0B – – WGM02 CS02 CS01 CS00
	//TIMSK0 : – – – – – OCIE0B OCIE0A TOIE0
	uint8_t top = 250;	 //Valeur de OCR0A.
	TCCR0A = 0b00000010; //Mode CTC.
	TCCR0B = 0b00000011; //Prescaler de 64.
	TIMSK0 = 0b00000010; //Output compare match A interrupt enable.
	OCR0A = top - 1;	 //62.5ns * 64 * 250 = 1ms.
	sei();
}

void timer4Init(void)
{
	//TCCR4A: COM4A1 COM4A0 COM4B1 COM4B0 FOC4A FOC4B PWM4A PWM4B
	//TCCR4B: PWM4X PSR4 DTPS41 DTPS40 CS43 CS42 CS41 CS40
	//TCCR4C: COM4A1S COM4A0S COM4B1S COMAB0S COM4D1 COM4D0 FOC4D PWM4D
	//TCCR4D: FPIE4 FPEN4 FPNC4 FPES4 FPAC4 FPF4 WGM41 WGM40
	//TCCR4E: TLOCK4 ENHC4 OC4OE5 OC4OE4 OC4OE3 OC4OE2 OC4OE1 OC4OE0
	TCCR4A = 0b10000010; //PWM output OC4A.
	TCCR4B = 0b00000100; //Prescaler de 4.
	OCR4C = 255 - 1;	 //62.5ns * 4 * 255 = 127.5us.
	OUTPUT_VALUE(0);	 //Valeur de la sortie.
}

/*
60 0 62
60 127 62
60 255 62
*/

/*	<	SIZE	CMD		DATA	>	*/

//GET ÉTAT	<