/*----------------------------------------------------------------------------
  Funktionen für USART mit LL-Library
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_usart.h>

#include <string.h>
#include <stdlib.h>

#include <stdio.h>

#include "TIM3_PWM.h"
#include "Stepper.h"
#include "Temperatur.h"
#include "LCD.h"

char USART2InputBuffer[64];											// Eingabepuffer für Befehle
int USART2CmdFlag=0;														// Flag für anliegendes Kommando
int mode=0; 
char cmd=0x00;
char data=0x00;

void USART2Init(void)
// Initialisierung der USART2-Schnittstelle
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);		// GPIOA Takt einschalten
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	
	
	// PA2 initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_2,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_2,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_2,LL_GPIO_AF_7);

	// PA3 initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_3,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_3,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_3,LL_GPIO_AF_7);

	
	// USART2 initialisieren
	
	LL_USART_InitTypeDef USART_InitStruct = {0};
	
	USART_InitStruct.BaudRate = 9600;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;			
	
	LL_USART_Init(USART2, &USART_InitStruct);
	
	//LL_USART_ConfigAsyncMode(USART2);
	LL_USART_Enable(USART2);
	
	NVIC_EnableIRQ(USART2_IRQn);														// NVIC aktivieren
  
	LL_USART_EnableIT_RXNE(USART2);
}

void USART2WriteChar(char c)
// schreibt Zeichen c an USART2
{
	
	while(!LL_USART_IsActiveFlag_TXE(USART2)){
		}
	
	LL_USART_TransmitData8(USART2, c);

}

void USART2WriteString(char *str)
// schreibt String str an USART2
{

	for(int i = 0; i <= strlen(str); i++){
		
		USART2WriteChar(str[i]);
	
	}
		

}

/*----------------------------------------------------------------------------
  Interrupt-Handler USART2
 *----------------------------------------------------------------------------*/
void USART2_IRQHandler (void)
{
		
		char message = LL_USART_ReceiveData8(USART2);		//Putty Echo
		USART2WriteChar(message);
		
		if(message=='\r'){
			USART2InputBuffer[USART2CmdFlag++]='\0';
			
			if(USART2InputBuffer[0]=='t'){
				if(USART2InputBuffer[1]=='s'){
					
					sscanf(USART2InputBuffer,"ts%d", &mode);
					if(mode >= 0 && mode <= 3)TempSetRes(mode);
					
					
				}else if (USART2InputBuffer[1]=='r'){
					char buffer[16];
					sprintf(buffer, "%f", TempRead()/16.0);
					USART2WriteString(buffer);
				}
			}else if (USART2InputBuffer[0]=='l'){
				if(USART2InputBuffer[1]=='c'){
					
					sscanf(USART2InputBuffer,"lc0x%hhx", &cmd);
					LCDSendCmd(cmd);
					
					
				}else if (USART2InputBuffer[1]=='d'){
					
					sscanf(USART2InputBuffer,"ld0x%hhx", &data);
					LCDSendData(data);
					
				}
			}
			
		
		
			USART2CmdFlag=0;
			
		}else{
			USART2InputBuffer[USART2CmdFlag++]=message;
		}
		
		
		
	

}
