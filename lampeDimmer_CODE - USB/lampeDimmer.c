/**
  * @file       demoUSBSerial.c
  * @brief      Programme de d�monstration du lien s�rie USB
  * @author     Marc Juneau (marcjuneau@gmail.com)
  * @version    0.01
  * @date       15 f�vrier 2020
  *
  * @mainpage
  *		Programme de d�monstration du lien s�rie USB
  *	@author Marc Juneau
  *	@section MainSection1 Description
  *
  * Ce programme d�montre le fonctionnement minimale d'un lien s�rie USB.
  * Il utilise la biblioth�que LUFA pour un p�riph�rique CDC
  */ 

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <sources/LUFA/Drivers/USB/USB.h>
#include "adcBasic.h"
#include "usart.h"
#include "Descriptors.h"


#define OUTPUT_VALUE(val)	(OCR4A = val) //Valeur PWM output R.
#define OUTPUT_INIT()		DDRC |= (1<<7) //Init output R.
#define SWITCH_INIT()		PORTB |= (1<<3) //Bouton sur PD1.
#define SWITCH()			((PINB & (1<<3))==0)

#define TIMER_CNT_CYCLE_ADC		25 //Nombre de cycle compt�s en interruption.
#define TIMER_CNT_CYCLE_FADE	50 //Nombre de cycle compt�s en interruption.
#define INCREMENT_STEP			1 //Incr�ment pour le fadding.
#define _MAX_RXDATASIZE_    16


volatile uint16_t msCntAdc = 0; //Compteur utilis�s pour compter 25 fois un d�lai de 1ms pour la mesure de l'ADC.
volatile uint8_t msFlagAdc = 0; //Flags qui est mis � 1 � chaques 25ms pour la mesure de l'ADC.
volatile uint16_t msCntFade = 0; //Compteur utilis�s pour compter 50 fois un d�lai de 1ms pour le fade de la sortie.
volatile uint8_t msFlagFade = 0; //Flags qui est mis � 1 � chaques 50ms pour le fade de la sortie.
uint16_t valueAdc = 0;
uint16_t valueOut = 0;
uint8_t veilleMode = 0;
int increment = 5;
char msg[5];

//variables n�cessaires � la communication avec l'interface
uint8_t rxDataSize;
uint8_t rxDataCnt = 0; //Compteur de donn�s re�us.
uint8_t rxData[_MAX_RXDATASIZE_];
uint16_t rxErrorCommCnt = 0;
uint8_t moteurStopFlag = 0;
uint8_t seqAuto = 0;


//enum des diff�rents param�tres d'une r�ception
enum RX_STATES {
	WAIT,
	RXSIZE,
	RXCOMMANDE,
	RXDATA,
	VALIDATE
};

//enum des diff�rents param�tres d'une transmission
enum TX_COMMANDES {
	VAL_POT
};

//enum des diff�rents commandes de transmission d'une r�ception
enum RX_COMMANDES {
	GET_ETAT,
	SET_VAL,
	SET_SLEEP_MODE
};

//d�claration des enums
enum RX_STATES rxState;
enum TX_COMMANDES txCommande;
enum RX_COMMANDES rxCommande;


// Prototypes des fonctions locales
void hardwareInit(void);
void CDC_Task(void);
uint8_t serialUSBWrite(uint8_t * source, uint8_t size);
uint8_t serialUSBAvailable(void);
uint8_t serialUSBRead(uint8_t * dest, uint8_t size);
uint8_t serialUSBRxData[CDC_TXRX_EPSIZE];

//Prototypes de fonctions.

/**
*@brief Fonction de traitement des donn�es et commandes re�ues.
*/
void execRxCommand(void);

void outputVeille(uint8_t value);

/**
*@brief Fonction qui remplis la structure de donn�s avec les param�tres correspondants qui lui sont envoy�s en param�tre par la fonction usartRemRxData. Le switch case commence � l'�tat WAIT qui attend la r�ception de "<". RXDATA place alors les donn�s re�us dans l'union de structure jusqu'� ce que la derni�re donn�e (ici, la longueur de la trame � �t� sp�cifi� manuellement � 7 puisque nous n'envoyons pas l'octet qui contient la longueur de la trame. Finalement, VALIDATE s'assure que la trame se termine par ">"
*@param  data est l'octet re�u par la fonction usartRemRxData
*/
void parseRxData(uint8_t data);

/**
*@brief  Fonction d'initialisation du Timer #0.
*/
void timer0Init(void);

/**
*@brief  Fonction d'initialisation du timer #4 utilis� pour le PWM.
*/
void timer4Init(void);

/**
  * @brief Programme principale
  * @return rien
  */
int main(void)
{
	hardwareInit();
	sei();
	for (;;)
	{
		/*
		if(serialUSBAvailable())
		{
			uint8_t rxSize = serialUSBRead(serialUSBRxData,CDC_TXRX_EPSIZE);
			if(rxSize)
			{
				serialUSBWrite(serialUSBRxData,rxSize);	
			}
		}
		*/
		if(serialUSBAvailable())
		{
			uint8_t rxSize = serialUSBRead(serialUSBRxData,CDC_TXRX_EPSIZE);
			if(rxSize)
			{
				parseRxData(*serialUSBRxData);
				//usartSendByte(serialUSBRxData);
				//serialUSBWrite(serialUSBRxData,rxSize);	
//			}
// 		}
// 		if (serialUSBAvailable()) //Si un caract�re est disponible...
// 		{
// 			serialUSBRead(serialUSBRxData,CDC_TXRX_EPSIZE); //appel de la fonction parseRxData() avec en param�tre la valeur retourn�e par usartRemRxData().
// 			parseRxData(serialUSBRxData);
// 		}
		if (SWITCH()) //Si l'interrupteur du potentiom�tre est � la position "ON"...
		{
// 			if (usartRxAvailable()) //Si un caract�re est disponible...
// 				parseRxData(usartRemRxData()); //appel de la fonction parseRxData() avec en param�tre la valeur retourn�e par usartRemRxData().
			if (msFlagAdc)
			{
				msFlagAdc = 0;
				if (valueAdc != adcRead8())
				{
					/*valueAdc = adcRead8();*/
					for (uint8_t i = 0; i < 100; i++) //Une valeur moyenne sur un echantillon de 100 mesures est calcul� afin d'�viter d'�tre entre deux valeurs.
					{
						valueAdc += adcRead8();
						valueOut += adcRead8();
					}
					valueAdc /= 100;
					valueOut /= 100;
					if (valueOut >= 255) //Si valueOut d�passe 255..
						valueOut = 255; //valueOut est limit� � 255.
				}
				sprintf(msg, "%d\n\r", valueOut);
				//usartSendString(msg);
			}
		}
		else  //Si l'interrupteur du potentiom�tre est � la position "OFF"...
			outputVeille(veilleMode);
		OUTPUT_VALUE(valueOut);
		}
	}
	
	USB_USBTask();
	}
}

/**
*@brief Interruption de la comparaison sur OCR0A (timer0) qui met msFlag � 1 lorsque msCnt atteint TIMER_CNT_CYCLE qui est d�finie dans le define. msCnt est incr�ment� � chaques 1ms.
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
		case GET_ETAT: //�tat non utilis�
			txCommande = 0;
			break;
		case SET_VAL: //R�ception depuis l'interface de la valeur de la sortie.
			if (SWITCH()) //Si l'interrupteur du potentiom�tre est � la position "ON"...
				valueOut = rxData[0];
			break;
		case SET_SLEEP_MODE:
				veilleMode = rxData[0];
			break;
	}
	/*
	switch (txCommande)
	{
		char txData[5];
		case VAL_POT:
			txData[0] = '<';
			txData[1] = 1;
			txData[2] = VAL_POT;
			txData[3] = valueSend;
			txData[4] = '>';
			for (int x = 0; x <= 4; x++)
			{
				usartSendByte(txData[x]);
			}
			break;
	}
	*/
}

/**
  * @brief Initialise le mat�riel sp�cifique � cette application
  */
void hardwareInit(void)
{
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	DDRD |= (1<<4);
	clock_prescale_set(clock_div_1);
	
	adcInit(); //Appel de la fonction d'initialisation du ADC.
	//timer0Init(); //Initialisation de timer 0.
	timer4Init(); //Initialisation de timer 4.
	//usartInit(1000000, 16000000); //Initialisation du USART � 1Mbps.
	
	OUTPUT_INIT();
	SWITCH_INIT();
	
	USB_Init();
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
			if (valueOut <= 1) //Lorsque oc4aValue � atteint son minimum.
			{
				increment = INCREMENT_STEP;
			}
			if (valueOut >= 255) //Lorsque oc4aValue � atteint son maximum.
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
* @brief S�pare les donn�es re�ues sur le port s�rie.
* @param data la donn�e � traiter
*/
void parseRxData(uint8_t data)
{
	//switch case des diff�rents param�tres de la trame de r�ception
	switch (rxState)
	{
		//confirmation que la trame d�bute par '<'
		default :
			if(data == '<')
			{
				rxState = RXSIZE;
				rxDataCnt = 0;
			}
			break;
		//////////////////////////////////////////////////////////////////////////
		case RXSIZE:
			rxDataSize = data;
			if(rxDataSize >= _MAX_RXDATASIZE_)
				rxState = WAIT;
			else
				rxState = RXCOMMANDE;
			break;
		//////////////////////////////////////////////////////////////////////////
		//Traitement de la commande.
		case RXCOMMANDE:
			rxCommande = data;
			if(rxDataSize)
				rxState = RXDATA;
			else
				rxState = VALIDATE;
			break;
		//////////////////////////////////////////////////////////////////////////
		//Traitement de la donn�e.
		case RXDATA:
			rxData[rxDataCnt++] = data;
			if(rxDataCnt == rxDataSize)
				rxState = VALIDATE;
			break;
		//////////////////////////////////////////////////////////////////////////
		//confirmation que la trame se termine par '>'
		case VALIDATE :
			rxState = WAIT;
			if(data == '>')
				execRxCommand();//si oui la fonction execRxCommand() est appel�e
			else
				rxErrorCommCnt++;// sinon le nombre d'erreur augmente
			break;
	}
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

//Fonctions USB.

/**
  * @brief Transmission des donn�es sur le lien s�rie USB
  * @param source	adresse de la source (1 ou plusieurs octets)
  * @param size		nombre d'octets � transmettre (doit respecter la source)
  * @return le nombre d'octets transmis
  */
uint8_t serialUSBWrite(uint8_t * source, uint8_t size)
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
  * @brief V�rifie s'il y a des donn�es re�ues par le lien s�rie USB
  * @return le nombre d'octets disponible dans le FIFO
  */
uint8_t serialUSBAvailable()
{
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return 0;

	Endpoint_SelectEndpoint(CDC_RX_EPADDR);
	return Endpoint_BytesInEndpoint();
}
/**
  * @brief R�ception des donn�es sur le lien s�rie USB
  * @param dest	adresse de la destination (1 ou plusieurs octets)
  * @param size	nombre d'octets � lire du FIGO (doit respecter la destination)
  * @return le nombre d'octets lus
  */
uint8_t serialUSBRead(uint8_t * dest, uint8_t size)
{
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return 0;

	Endpoint_SelectEndpoint(CDC_RX_EPADDR);
	uint16_t rxSize = Endpoint_BytesInEndpoint();
	if(rxSize > size)
	rxSize = size;
	if (rxSize)
	{
		Endpoint_Read_Stream_LE(dest,rxSize,NULL);
		Endpoint_ClearOUT();
	}
	return rxSize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions qui doivent-�tre d�finies dans ce programme pour r�pondre aux demandes de l'h�te
// Elles sont appel�es par les fonctions de LUFA.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Fonction g�re l'�v�nement USB_Connect. Ceci indique que le p�riph�rique est �num�r�.
 */
void EVENT_USB_Device_Connect(void)
{
	PORTD &= ~(1<<4);
}

/**
 * @brief Fonction g�re l'�v�nement USB_Disconnect. Ceci indique que le p�riph�rique n'est plus connect� � l'h�te
 */
void EVENT_USB_Device_Disconnect(void)
{
	PORTD |= (1<<4);
}

// void EVENT_USB_Device_ControlRequest(void
// Structure de la configuration du lien s�rie virtuel. Pour des questions de compatibilit� seulement, car la configuration n'affecte pas le lien.
// Le BaudRate � 0 permet d'indiquer � l'h�te qu'il peut envoyer la configuration du lien
static CDC_LineEncoding_t lineEncoding = { 
	.BaudRateBPS = 0,
	.CharFormat  = CDC_LINEENCODING_OneStopBit,
	.ParityType  = CDC_PARITY_None,
	.DataBits    = 8 };


/**
 * @brief Fonction g�re l'�v�nement USB_ConfigurationChanged. 
 *        Ceci indique que l'h�te a activ� la configuration apr�s l'�num�ration. 
 *        La fonction r�cup�re alors la configuration des Endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;
	/* Setup CDC Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT, CDC_NOTIFICATION_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK,  CDC_TXRX_EPSIZE, 1);
	/* Reset line encoding baud rate so that the host knows to send new values */
	lineEncoding.BaudRateBPS = 0;
}

/**
 * @brief Fonction g�re l'�v�nement USB_ControlRequest. 
 *        C'est ici que sont g�r�s les requ�tes de contr�le CDC envoy�es par l'h�te.
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

