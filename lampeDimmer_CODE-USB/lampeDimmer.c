/**
  * @file       lampeDimmer.c
  * @brief      Programme utilisé pour la communication avec le contrôleur de la lampe.
  * @author     Base : Marc Juneau
  * @author     Adaptation : Thomas Desrosiers
  * @version    2.0
  * @date       8 Janvier 2021
  *
  * @mainpage	lampeDimmer
  * @author     Base : Marc Juneau
  * @author     Adaptation : Thomas Desrosiers
  *	@section	MainSection1 Description
  *				Programme de gestion de la communication par USB.
  *	@li			Ce programme démontre le fonctionnement minimale d'un lien série USB.
  *	@li			Il utilise la bibliothèque LUFA pour un périphérique CDC
  */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <sources/LUFA/Drivers/USB/USB.h>
#include "adcBasic.h"
#include "Descriptors.h"

/* Macros: */
#define OUTPUT_VALUE(val) (OCR4A = val) //Valeur PWM output R.
#define OUTPUT_INIT() DDRC |= (1 << 7)	//Init output R.
#define SWITCH_INIT() PORTB |= (1 << 1) //Bouton sur PD1.
#define SWITCH() ((PINB & (1 << 1)) == 0)

/* Defines: */
#define TIMER_CNT_CYCLE_ADC 25	//Nombre de cycle comptés en interruption.
#define TIMER_CNT_CYCLE_FADE 50 //Nombre de cycle comptés en interruption.
#define INCREMENT_STEP 1		//Incrément pour le fadding.
#define _MAX_RXDATASIZE_ 16

/* Variables: */
volatile uint16_t msCntAdc = 0;	 //Compteur utilisés pour compter 25 fois un délai de 1ms pour la mesure de l'ADC.
volatile uint8_t msFlagAdc = 0;	 //Flags qui est mis à 1 à chaques 25ms pour la mesure de l'ADC.
volatile uint16_t msCntFade = 0; //Compteur utilisés pour compter 50 fois un délai de 1ms pour le fade de la sortie.
volatile uint8_t msFlagFade = 0; //Flags qui est mis à 1 à chaques 50ms pour le fade de la sortie.
uint16_t valueAdcTbl[2] = {0, 0};
uint16_t valueOut = 0;
uint16_t valueAdc = 0;
int increment = 5;

/* Variables nécessaires à la communication avec l'interface: */
uint8_t rxDataSize;
uint8_t rxDataCnt = 0; //Compteur de donnés reçus.
uint8_t rxData[_MAX_RXDATASIZE_];
uint16_t rxErrorCommCnt = 0;

/* Enum des différents étapes d'une réception: */
enum RX_STATES
{
	WAIT,
	RXSIZE,
	RXCOMMANDE,
	RXDATA,
	VALIDATE
};

/* Enum des différentes commandes utilisées en transmission: */
enum TX_COMMANDES
{
	VAL_ACTU,
	VAL_INIT,
	VAL_POT
};

/* Enum des différentes commandes utilisées en réception: */
enum RX_COMMANDES
{
	GET_VAL_ACTU,
	GET_VAL_INIT,
	GET_VAL_POT,
	SET_SLEEP_MODE,
	SET_VAL
};

enum VEILLE_STATE
{
	VEILLE_NONE,
	VEILLE_OFF,
	VEILLE_ON,
	VEILLE_BREATHING,
	VEILLE_VEILLE
};

/* Déclaration des enums: */
enum RX_STATES rxState;
enum TX_COMMANDES txCommande;
enum RX_COMMANDES rxCommande;
enum VEILLE_STATE veilleState;

/* Prototypes des fonctions locales: */
void hardwareInit(void);
void CDC_Task(void);
uint8_t serialUSBWrite(uint8_t *source, uint8_t size);
uint8_t serialUSBAvailable(void);
uint8_t serialUSBRead(uint8_t *dest, uint8_t size);
uint8_t serialUSBRxData[CDC_TXRX_EPSIZE];

/* Prototypes de fonctions: */
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

/**
 *@brief		Fonction qui traite la valeur de sortie en fonction du mode veille actuel.
 *@param value  Mode veille.
 */
void outputVeille(void);

/**
 * @brief		Fonction qui remplis la structure de donnés avec les paramètres correspondants qui lui sont envoyés en paramètre par la fonction usartRemRxData. Le switch case commence à l'état WAIT qui attend la réception de "<". RXDATA place alors les donnés reçus dans l'union de structure jusqu'à ce que la dernière donnée (ici, la longueur de la trame à été spécifié manuellement à 7 puisque nous n'envoyons pas l'octet qui contient la longueur de la trame. Finalement, VALIDATE s'assure que la trame se termine par ">"
 * @param data  Octet reçu par la fonction usartRemRxData
 */
void parseRxData(uint8_t data);

/**
 * @brief  Fonction d'initialisation du timer #0.
 */
void timer0Init(void);

/**
 * @brief  Fonction d'initialisation du timer #4 utilisé pour le PWM.
 */
void timer4Init(void);

/**
  * @brief   Programme principale
  * @return  Rien
  */
int main(void)
{
	hardwareInit();
	sei();
	for (;;)
	{
		if (serialUSBAvailable()) //Si un caractère est disponible...
		{
			serialUSBRead(serialUSBRxData, CDC_TXRX_EPSIZE); //appel de la fonction parseRxData() avec en paramètre la valeur retournée par usartRemRxData().
			for (uint8_t index = 0; index < (serialUSBRxData[1] + 4); index++)
			{
				parseRxData(serialUSBRxData[index]);
			}
		}
		if (SWITCH()) //Si l'interrupteur du potentiomètre est à la position "ON"...
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
			outputVeille();
			txCommande = VAL_ACTU;
			execTxCommand();
		}
		OUTPUT_VALUE(valueOut);
		USB_USBTask();
	}
}

/**
*@brief  Interruption de la comparaison sur OCR0A (timer0) qui met msFlag à 1 lorsque msCnt atteint TIMER_CNT_CYCLE qui est définie dans le define. msCnt est incrémenté à chaques 1ms.
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
	case GET_VAL_ACTU:
		txCommande = VAL_ACTU;
		execTxCommand();
		break;
	case GET_VAL_INIT:
		txCommande = VAL_INIT;
		execTxCommand();
		break;
	case GET_VAL_POT: //État non utilisé
		txCommande = VAL_POT;
		execTxCommand();
		break;
	case SET_SLEEP_MODE:
		veilleState = rxData[0];
		break;
	case SET_VAL:	  //Réception depuis l'interface de la valeur de la sortie.
		if (SWITCH()) //Si l'interrupteur du potentiomètre est à la position "ON"...
		{
			valueOut = rxData[0];
		}
		break;
	}
}

void execTxCommand(void)
{
	char txData[6];
	switch (txCommande)
	{
	case VAL_ACTU:
		txData[0] = '<';
		txData[1] = 1;
		txData[2] = VAL_POT;
		txData[3] = valueOut;
		txData[4] = '>';
		serialUSBWrite((uint8_t *)txData, 5);
		break;
	case VAL_INIT:
		txData[0] = '<';
		txData[1] = 2;
		txData[2] = VAL_INIT;
		txData[3] = valueOut;
		txData[4] = veilleState;
		txData[5] = '>';
		serialUSBWrite((uint8_t *)txData, 6);
		break;
	case VAL_POT:
		txData[0] = '<';
		txData[1] = 1;
		txData[2] = VAL_POT;
		txData[3] = valueAdc;
		txData[4] = '>';
		serialUSBWrite((uint8_t *)txData, 5);
		break;
	}
}

/**
  * @brief  Initialise le matériel spécifique à cette application.
  */
void hardwareInit(void)
{
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	DDRD |= (1 << 4);
	clock_prescale_set(clock_div_1);

	adcInit();	  //Appel de la fonction d'initialisation du ADC.
	timer0Init(); //Initialisation de timer 0.
	timer4Init(); //Initialisation de timer 4.
	OUTPUT_INIT();
	SWITCH_INIT();

	USB_Init();
}

void outputVeille(void)
{
	switch (veilleState)
	{
	case VEILLE_NONE:

		break;
	case VEILLE_OFF:
		valueOut = 0;
		break;
	case VEILLE_ON:
		valueOut = 255;
		break;
	case VEILLE_BREATHING:
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
	case VEILLE_VEILLE:
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

/************************************************************************/
/*                            Fonctions USB.                            */
/************************************************************************/

/**
  * @brief		   Transmission des données sur le lien série USB.
  * @param source  Adresse de la source (1 ou plusieurs octets).
  * @param size	   Nombre d'octets à transmettre (doit respecter la source).
  * @return		   Nombre d'octets transmis.
  */
uint8_t serialUSBWrite(uint8_t *source, uint8_t size)
{
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return 0;

	Endpoint_SelectEndpoint(CDC_TX_EPADDR);
	Endpoint_Write_Stream_LE(source, size, NULL);
	uint8_t IsFull = (Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE);
	Endpoint_ClearIN();
	if (IsFull)
	{
		Endpoint_WaitUntilReady();
		Endpoint_ClearIN();
	}
	return size;
}
/**
  * @brief   Vérifie s'il y a des données reçues par le lien série USB.
  * @return  Nombre d'octets disponible dans le FIFO.
  */
uint8_t serialUSBAvailable()
{
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return 0;

	Endpoint_SelectEndpoint(CDC_RX_EPADDR);
	return Endpoint_BytesInEndpoint();
}
/**
  * @brief		 Réception des données sur le lien série USB.
  * @param dest	 Adresse de la destination (1 ou plusieurs octets).
  * @param size	 Nombre d'octets à lire du FIGO (doit respecter la destination).
  * @return		 Nombre d'octets lus.
  */
uint8_t serialUSBRead(uint8_t *dest, uint8_t size)
{
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return 0;

	Endpoint_SelectEndpoint(CDC_RX_EPADDR);
	uint16_t rxSize = Endpoint_BytesInEndpoint();
	if (rxSize > size)
		rxSize = size;
	if (rxSize)
	{
		Endpoint_Read_Stream_LE(dest, rxSize, NULL);
		Endpoint_ClearOUT();
	}
	return rxSize;
}

/**********************************************************************************************/
/* Fonctions qui doivent-être définies dans ce programme pour répondre aux demandes de l'hôte */
/*                       Elles sont appelées par les fonctions de LUFA.                       */
/**********************************************************************************************/
/**
 * @brief  Fonction gère l'événement USB_Connect. Ceci indique que le périphérique est énuméré.
 */
void EVENT_USB_Device_Connect(void)
{
	PORTD &= ~(1 << 4);
}

/**
 * @brief  Fonction gère l'événement USB_Disconnect. Ceci indique que le périphérique n'est plus connecté à l'hôte
 */
void EVENT_USB_Device_Disconnect(void)
{
	PORTD |= (1 << 4);
}

// void EVENT_USB_Device_ControlRequest(void
// Structure de la configuration du lien série virtuel. Pour des questions de compatibilité seulement, car la configuration n'affecte pas le lien.
// Le BaudRate à 0 permet d'indiquer à l'hôte qu'il peut envoyer la configuration du lien
static CDC_LineEncoding_t lineEncoding = {
	.BaudRateBPS = 0,
	.CharFormat = CDC_LINEENCODING_OneStopBit,
	.ParityType = CDC_PARITY_None,
	.DataBits = 8};

/**
 * @brief Fonction gère l'événement USB_ConfigurationChanged. 
 * @li    Ceci indique que l'hôte a activé la configuration après l'énumération. 
 * @li    La fonction récupère alors la configuration des Endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;
	/* Setup CDC Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT, CDC_NOTIFICATION_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
	/* Reset line encoding baud rate so that the host knows to send new values */
	lineEncoding.BaudRateBPS = 0;
}

/**
 * @brief  Fonction gère l'événement USB_ControlRequest. 
 * @n      C'est ici que sont gérés les requêtes de contrôle CDC envoyées par l'hôte.
 */
void EVENT_USB_Device_ControlRequest(void)
{
	/* Process CDC specific control requests */
	switch (USB_ControlRequest.bRequest)
	{
	case CDC_REQ_GetLineEncoding:
		if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
		{
			Endpoint_ClearSETUP();
			Endpoint_Write_Control_Stream_LE(&lineEncoding, sizeof(CDC_LineEncoding_t));
			Endpoint_ClearOUT();
		}

		break;
	case CDC_REQ_SetLineEncoding:
		if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
		{
			Endpoint_ClearSETUP();
			Endpoint_Read_Control_Stream_LE(&lineEncoding, sizeof(CDC_LineEncoding_t));
			Endpoint_ClearIN();
		}

		break;
	case CDC_REQ_SetControlLineState:
		if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
		{
			Endpoint_ClearSETUP();
			Endpoint_ClearStatusStage();
		}
		break;
	}
}
