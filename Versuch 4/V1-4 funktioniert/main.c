/*----------------------------------------------------------------------------
	MCT-Praktikumsversuche V1...4 Nucleo mit LL-Library
		
	08/2022
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>
#include "main.h"
#include "LED.h"
#include "ADC.h"
#include "DAC.h"
#include "USART2.h"
#include "Stepper.h"
#include "TIM3_PWM.h"
#include "I2C.h"
#include "LCD.h"
#include "Temperatur.h"
#include "LCDInit.h"

void DelayMs(int ms)
// wartet ms Millisekunden unter Verwendung von Timer6
{
	if (ms==0)																							// wenn Wartezeit=0, dann sofort zurueck
		return;
	
	// Timer initialisieren
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);			// Tim6 Takt einschalten
		
	LL_TIM_SetPrescaler(TIM6,SystemCoreClock/1000-1);				// Prescaler für 1kHz = 1ms
	LL_TIM_SetAutoReload(TIM6,ms);													// Wartezeit in Auto-Reload-Register
	LL_TIM_SetCounter(TIM6,0);															// Zähler auf 0 zurücksetzen
	LL_TIM_EnableCounter(TIM6);															// Zähler starten
		
	while (LL_TIM_GetCounter(TIM6)<ms)
		;
	
	LL_TIM_DisableCounter(TIM6);
}

void ExecCmd(void){
	
	char VollschrittHEX[4] = {'9', '3', '6', 'C'};
	char VollschrittBin[4][4] = { {1, 0, 0, 1},
	
																{0, 0, 1 ,1},
																{0, 1, 1, 0},
																{1, 1, 0, 0} };
	int HalbschrittDEZ[8] = {3, 2, 6, 4, 12, 8, 9, 1};			//brauchen wir
	
	char HalbschrittBin[8][4] = { {0, 0, 1, 1},
																{0, 0, 1, 0},
																{0, 1, 1, 0},
																{0, 1, 0, 0},
																{1, 1, 0, 0},
																{1, 0, 0, 0},
																{1, 0, 0, 1},
																{0, 0, 0, 1} };
	
	
	
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)
	

{ 	
	
	/*
	//------- Inits ----------
	//DACInit();		
	//TIM3InitPWM();
	StepperInit();
	USART2Init();
	I2CInit(0x08);
	LCDInit();
	
	//Grad-Zeichen erstellen
	LCDDefineDegree();
	
	//Res auf 0,25, sieht besser aus
	TempSetRes(1);
	
	float temp = 0.0;
	
	while(1){
		
		if(TicksGlobal%300==0){	//bei 300HZ, Ausführung 1x pro Sekunde
			
			temp = TempRead()/16.0;
			
			LCDClear();
			
			char buffer[16];
			sprintf(buffer, "%c    %.2f%c    %c",0x02,  temp, 0x01, 0x02);
			LCDSendString(buffer);
			
			//20 Grad - 30 Grad -> 200 Schritte
			ZielPos = 200 - 200*(30.0 - temp)/10;
			
			
		}
		
		
		
		
		
		
	}
	
	//while(1){}
	
	*/
}
