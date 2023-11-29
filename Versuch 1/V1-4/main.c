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

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)
{ 	
	
	//------- Init ----------
	LEDInit();									// LEDs initialisieren
	ADCInit(0);// ...										// ADC Kanal 0 initialisieren

	while (1) 
	{

		// ... ADC einlesen
		int wert = ADCRead();
		
		// ... an LEDs 4...7 anzeigen	
		if(wert >= 3277){
			LEDSet(7);
			LEDSet(6);
			LEDSet(5);
			LEDSet(4);
		}else if(wert >= 2458){
			LEDSet(7);
			LEDSet(6);
			LEDSet(5);
			LEDReset(4);
		}else if(wert >= 1638){
			LEDSet(7);
			LEDSet(6);
			LEDReset(5);
			LEDReset(4);
		}else if (wert >= 819){
			LEDSet(7);
			LEDReset(6);
			LEDReset(5);
			LEDReset(4);
		}else if (wert < 819){
			LEDReset(7);
			LEDReset(6);
			LEDReset(5);
			LEDReset(4);
		}
		

	}
}
