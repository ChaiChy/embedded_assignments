#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_usart.h>
#include <stdio.h>
#include <string.h>
#include "USART2.h"
#include "main.h"

//Globale Variablen fuer USART3
char InputBufferESP[100];
int CmdFlagESP=0;
int BufferPosESP=0;
int ESPFertig=0;

//===========================================================================
// USART3 zur Kommunikation mit ESP
//===========================================================================
void InitUSART3(void)
// USART3 initialisieren
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);		// GPIOC Takt einschalten

	// PC10 initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_10,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_10,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_10,LL_GPIO_AF_7);

	// PC11 initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_11,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_11,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_11,LL_GPIO_AF_7);
	
	// USART3 initialisieren
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);		// USART3 Takt einschalten
	LL_USART_SetBaudRate(USART3,16000000,LL_USART_OVERSAMPLING_16,9600);
	LL_USART_SetDataWidth(USART3,LL_USART_DATAWIDTH_8B);
	LL_USART_SetStopBitsLength(USART3,LL_USART_STOPBITS_1);
	LL_USART_SetParity(USART3,LL_USART_PARITY_NONE);
  LL_USART_SetTransferDirection(USART3,LL_USART_DIRECTION_TX_RX);
	LL_USART_SetHWFlowCtrl(USART3,LL_USART_HWCONTROL_NONE);
	
	LL_USART_Enable(USART3);																// USART3 einschalten:
	LL_USART_EnableIT_RXNE(USART3);													// USART3 RXNE-Interrupt einschalten:
	NVIC_EnableIRQ(USART3_IRQn);														// NVIC aktivieren
}

void WriteCharUSART3(char c)
// schreibt Zeichen c an USART3
{
	while(!LL_USART_IsActiveFlag_TXE(USART3))								// Auf TXE Bit warten:
		;
	LL_USART_TransmitData8(USART3,c);												// Daten ins Data-Register schreiben
}

void WriteStringUSART3(char *str)
// schreibt STring an USART3
{
	while (*str != 0)
	{
		WriteCharUSART3(*(str++));
	}
}

// Interruptroutine für USART3
// optimiert für ESP
void USART3_IRQHandler (void)
{	
	char InputChar;																		// Eingelesenes Zeichen
	
	InputChar = LL_USART_ReceiveData8(USART3);				// Zeichen holen
	InputBufferESP[BufferPosESP] = InputChar;					// und in Puffer speichern
	USART2WriteChar(InputChar);															// Echo an Terminal
	
	//Sobald der ESP laeuft muss seine Ausgabe in den anderen inputBuffer
	//kopiert werden, damit das restliche Programm auch funktioniert
	if(ESPFertig){
		USART2InputBuffer[BufferPosESP]=InputChar;
	}
	
	BufferPosESP++;																	// Pufferzeiger erhöhen

	if (InputChar == '\n')													// wenn Zeilenenderückmeldung von ESP
	{
		InputBufferESP[BufferPosESP] = 0; 			  	  // String abschließen	
		CmdFlagESP=1;																	// Kommandoflag setzen
		if(ESPFertig){
			USART2InputBuffer[BufferPosESP]=0;								// Kommandoflag setzen und String abschließen
			USART2CmdFlag=1;																	// zur Kommunikation mit anderem Programm über USART2-Puffer

		}
		BufferPosESP=0;																// Pufferzeiger zurücksetzen
	}
}

void Start_ESP_Wifi( int nr,int pwd)
/*
Bsp nr = 23, pwd = 12345678
Es wird der String "W:23P:12345678." an USART3 an den ESP8266 ausgegeben
Auf dieses Kommando startet der ESP ein WiFi mit dem Namen ESP_23 und dem Passwort 12345678
(Passwort mindestens 8 Stellen)
ESP macht dazu einige Ausgaben.
Ab dann wird _ESP_fertig gesetzt. Alle Nachrichten vom ESP werden nun in den
inputbuffer von USART2 (von Putty) umkopiert
*/
{ 
	char cmd[50];
	ESPFertig=0;	

	InitUSART3();		// USART3 fuer ESP-Verbindung 

 	sprintf(cmd,"Start ESP_%d P:%d\n\r",nr,pwd); 
	USART2WriteString(cmd);

	DelayMs(100); // Warten, bis ESP8266 startet
	sprintf(cmd,"W:%dP:%d.",nr,pwd);
	WriteStringUSART3(cmd);

	// warten, bis der ESP Server gestartet ist
	DelayMs(1000);

	// Flag zurücksetzen
	CmdFlagESP=0;
	ESPFertig=1;
	
	return;
}
