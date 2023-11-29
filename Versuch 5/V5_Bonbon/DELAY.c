#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>

void DelayMs(int ms)
// wartet ms Millisekunden unter Verwendung von Timer6
{
	if (ms==0)																							// wenn Wartezeit=0, dann sofort zurueck
		return;
	
	// Timer initialisieren
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);			// Tim6 Takt einschalten
		
	LL_TIM_SetPrescaler(TIM6,SystemCoreClock/1000-1);				// Prescaler f?r 1kHz = 1ms
	LL_TIM_SetAutoReload(TIM6,ms);													// Wartezeit in Auto-Reload-Register
	LL_TIM_SetCounter(TIM6,0);															// Z?hler auf 0 zur?cksetzen
	LL_TIM_EnableCounter(TIM6);															// Z?hler starten
		
	while (LL_TIM_GetCounter(TIM6)<ms)
		;
	
	LL_TIM_DisableCounter(TIM6);
}