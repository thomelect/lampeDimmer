/**
@file 		main.c
@brief		Laboratoire qui vise � exp�rimenter la lecture d'un clavier matriciel. La m�thode choisie est dans le cas pr�sent la lecture par ADC. Cette m�thode est plus complexe au montage mais offre comme avantage de n'utiliser qu'une seule broche.
@author 	Thomas Desrosiers
@version 	1.0
@date 		2021/05/05

@mainpage 	lab_7_integration
@author 	Thomas Desrosiers
@section 	MainSection1 Description
Laboratoire qui vise � exp�rimenter la lecture d'un clavier matriciel. La m�thode choisie est dans le cas pr�sent la lecture par ADC. Cette m�thode est plus complexe au montage mais offre comme avantage de n'utiliser qu'une seule broche.
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include "usart.h"
#include "adcBasic.h"
#include <avr/interrupt.h>
#include <stdio.h>

#define OUTPUT_VALUE(val)	(OCR4A = val) //Valeur PWM output R.
#define OUTPUT_INIT()		DDRC |= (1<<7) //Init output R.

#define TIMER_CNT_CYCLE 25 //Nombre de cycle compt�s en interruption.

enum COMM_STATES {WAIT, RXDATA, VALIDATE}; //Protocole de traitement de r�ception d'une trame d'octet.
enum COMM_STATES commState = WAIT;

volatile uint16_t msCnt = 0; //Compteur utilis�s pour compter 25 fois un d�lai de 1ms.
volatile uint8_t msFlag = 0; //Flags qui est mis � 1 � chaques 25ms.
uint8_t rxDataCnt = 0; //Compteur de donn�s re�us.
uint8_t rxData = 0; //Tableau des donn�s.
uint8_t valueAdc = 0;
uint8_t valueOut = 0;
char msg[5];

//Prototypes de fonctions.

/**
*@brief Fonction de traitement des donn�es et commandes re�ues.
*/
void execRxCommand(void);

/**
*@brief  Fonction d'initialisation des diff�rents I/O et fonctions.
*/
void miscInit(void);

/**
*@brief Fonction qui remplis la structure de donn�s avec les param�tres correspondants qui lui sont envoy�s en param�tre par la fonction usartRemRxData. Le switch case commence � l'�tat WAIT qui attend la r�ception de "<". RXDATA place alors les donn�s re�us dans l'union de structure jusqu'� ce que la derni�re donn�e (ici, la longueur de la trame � �t� sp�cifi� manuellement � 7 puisque nous n'envoyons pas l'octet qui contient la longueur de la trame. Finalement, VALIDATE s'assure que la trame se termine par ">"
*@param  data est l'octet re�u par la fonction usartRemRxData
*@return 1 si la trame comportait un chevron '<' au d�but de celle-ci, des donn�s et se terminait par un chevron '>'. Si elle ne se rend pas jusqu'� la validation elle retourne 0.
*/
uint8_t parseRxData(uint8_t data);

/**
*@brief  Fonction d'initialisation du Timer #0.
*/
void timer0Init(void);

/**
*@brief  Fonction d'initialisation du timer #4 utilis� pour le PWM.
*/
void timer4Init(void);

int main(void)
{
	miscInit();
	
	while (1)
	{
		if (usartRxAvailable()) //Si un caract�re est disponible:
			parseRxData(usartRemRxData()); //appel de la fonction parseRxData() avec en param�tre la valeur retourn�e par usartRemRxData().
		if (msFlag)
		{
			msFlag = 0;
			if (valueAdc != adcRead8())
			{
				valueAdc = adcRead8();
				valueOut = adcRead8();
			}			
			OUTPUT_VALUE(valueOut);
			sprintf(msg, "%d\n\r", valueOut);
			usartSendString(msg);			
		}
	}
}

/**
*@brief Interruption de la comparaison sur OCR0A (timer0) qui met msFlag � 1 lorsque msCnt atteint TIMER_CNT_CYCLE qui est d�finie dans le define. msCnt est incr�ment� � chaques 1ms.
*/
ISR(TIMER0_COMPA_vect)
{
	msCnt++;
	if (msCnt >= TIMER_CNT_CYCLE)
	{
		msCnt -= TIMER_CNT_CYCLE;
		msFlag = 1;
	}
}

void execRxCommand()
{
	valueOut = rxData;
}

void miscInit(void)
{
	adcInit(); //Appel de la fonction d'initialisation du ADC.
	timer0Init(); //Initialisation de timer 0.
	timer4Init(); //Initialisation de timer 4.
	usartInit(1000000, F_CPU); //Initialisation du USART � 1Mbps.
	
	OUTPUT_INIT();
}

uint8_t parseRxData(uint8_t data)
{
	// Switch case des diff�rents param�tres de la trame de r�ception
	switch(commState)
	{
		default :
			if(data == '<') //Confirmation que la trame d�bute par '<'
				commState = RXDATA; //Saut vers RXDATA.
			rxDataCnt = 0; //Remet rxDataSizeCnt � 0 pour �tre pr�t � recevoir la donn�e 0.
			break;
		case RXDATA:
			rxData = data; //Place les donn�es dans l'union de structure. Puisque les param�tres sont plac�s dans la m�me ordre dans la structure que dans celui qu'elle sont re�u, il est possible d'utiliser bytes[x] au lieu de devoir sp�cifier le nom complet. Ce qui permet aussi d'assembler les valeurs 16 bits qui avaient �t� s�par�s en 2 x 8bits.
			rxDataCnt++;
			if(rxDataCnt >= 1) //Le nombre de donn�es attendu est de 6.
				commState = VALIDATE; //Lorsque le nombre de donn�es anticip�s est re�u on sute � validate.
			break;
		case VALIDATE :
			if(data == '>') //On v�rifie que la trame se termine bien par un chevron de fermeture.
				execRxCommand(); //Appel de execRxCommand() pour executer divers opp�ration sur les donn�es re�ues.
			commState = WAIT;
			return 1; //Retourne 1 si il y a eu validation.
			break;
	}
	return 0;// Retourne si l'a validation n'a pas �t� faite.
}

void timer0Init(void)
{
	//TCCR0A : COM0A1 COM0A0 COM0B1 COM0B0 � � WGM01 WGM00
	//TCCR0B : FOC0A FOC0B � � WGM02 CS02 CS01 CS00
	//TIMSK0 : � � � � � OCIE0B OCIE0A TOIE0
	uint8_t top = 250; //Valeur de OCR0A.
	TCCR0A = 0b00000010; //Mode CTC.
	TCCR0B = 0b00000011; //Prescaler de 64.
	TIMSK0 = 0b00000010; //Output compare match A interrupt enable.
	OCR0A = top-1; //62.5ns * 64 * 250 = 1ms.
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
	OCR4C = 255-1; //62.5ns * 4 * 255 = 127.5us.
	OUTPUT_VALUE(0); //Valeur de la sortie.
}

/*
60 0 62
60 127 62
60 255 62
*/