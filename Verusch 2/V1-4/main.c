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
	int i=0;			//Laufvariable
	
	
//Sinnustabelle mit echten SInuswerten
	double werte[32][2] = {	{0 , 0},		
													{0.19635 , 0.19509},
													{0.392699 , 0.382683},
													{0.589049 , 0.55557},
													{0.785398 , 0.707107},
													{0.981748 , 0.83147},
													{1.1781 , 0.92388},
													{1.37445 , 0.980785},
													{1.5708 , 1},
													{1.76715 , 0.980785},
													{1.9635 , 0.92388},
													{2.15984 , 0.83147},
													{2.35619 , 0.707107},
													{2.55254 , 0.55557},
													{2.74889 , 0.382683},
													{2.94524 , 0.19509},
													{3.14159 , -7.65714e-16},
													{3.33794 , -0.19509},
													{3.53429 , -0.382683},
													{3.73064 , -0.55557},
													{3.92699 , -0.707107},
													{4.12334 , -0.83147},
													{4.31969 , -0.92388},
													{4.51604 , -0.980785},
													{4.71239 , -1},
													{4.90874 , -0.980785},
													{5.10509 , -0.92388},
													{5.30144 , -0.83147},
													{5.49779 , -0.707107},
													{5.69414 , -0.55557},
													{5.89049 , -0.382683},
													{6.08684 , -0.19509}	};


/*
//Sinustabelle (Werte an Aufläsung bzw. Wertebereich angepasst
double werte[32][2] = { {0 , 2048},
												{0.19635 , 2408},
												{0.392699 , 2755},
												{0.589049 , 3075},
												{0.785398 , 3356},
												{0.981748 , 3586},
												{1.1781 , 3757},
												{1.37445 , 3862},
												{1.5708 , 3898},
												{1.76715 , 3862},
												{1.9635 , 3757},
												{2.15984 , 3586},
												{2.35619 , 3356},
												{2.55254 , 3075},
												{2.74889 , 2755},
												{2.94524 , 2408},
												{3.14159 , 2047},
												{3.33794 , 1687},
												{3.53429 , 1340},
												{3.73064 , 1020},
												{3.92699 , 739},
												{4.12334 , 509},
												{4.31969 , 338},
												{4.51604 , 233},
												{4.71239 , 198},
												{4.90874 , 233},
												{5.10509 , 338},
												{5.30144 , 509},
												{5.49779 , 739},
												{5.69414 , 1020},
												{5.89049 , 1340},
												{6.08684 , 1687}	};
*/

	//------- Init ----------
	DACInit();		// DAC initialissieren
	USART2Init();
	
												
	//char* string = "Hallo\n\r";
	/*
	while(1){
		//USART2WriteString(string);
		//USART2WriteChar('l');
	}	
*/	
	int d = 0;
	int a = 1;
	


												
	while (1) 
	{
		if(amplitude<=9 && amplitude >= 1) a = amplitude;
		if(delay<=10 && delay >=0) d = delay;
		
		//DACWrite(werte[i++][1]);
		DACWrite(2048+(werte[i++][1]*(1850/a)));
		if(i==32)i=0;
		DelayMs(d);
	}
}
