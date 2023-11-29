/*----------------------------------------------------------------------------
	MCT-Praktikumsversuche V1...4 Nucleo mit LL-Library
		
	08/2022
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>
#include "main.h"
#include "LED.h"
#include "ADC.h"
#include "DAC.h"
#include "USART2.h"
#include "Stepper.h"
#include "TIM3_PWM.h"

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
	//------- Init ----------
	//DACInit();		// DAC initialissieren
	USART2Init();
	//TIM3InitPWM();
	StepperInit();
	
	while(1){
		
	}
}
