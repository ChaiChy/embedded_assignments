/*----------------------------------------------------------------------------
  Funktionen für Schrittmotor mit LL-Library
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_utils.h>
#include <stm32f4xx_ll_cortex.h>
#include "stepper.h"
#include "USART2.h"

int StepDir = 0;  						// Richtung des Schrittmotors
int AktPos=0;									// aktuelle Position des Schrittmotors
int ZielPos=0;								// Zielposition des Schrittmotors
int StepMode=StepMode_NONE;		// Modus des Schrittmotors
int Ticks=0;									// Zähler, der bei jedem Systick-Aufruf erhöht wird

void SysTickInit (int freq)
// initialisiert SysTick für freq Hz und startet ihn
{
	LL_InitTick(SystemCoreClock,freq);			// SysTick initialisieren und starten
	LL_SYSTICK_EnableIT();					// Interrupt aktivieren
}

void StepperInit(void)
// initialisiert Schrittmotorausgänge GPIOB0...3 und Referenzeingang GPIOB4
{
	// ... (Ausgänge konfigurieren)
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	//GPIO0
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_0,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_0,LL_GPIO_MODE_OUTPUT);
	//GPIO1
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_1,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_1,LL_GPIO_MODE_OUTPUT);
	//GPIO2
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_2,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
	//GPIO3
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_3,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_3,LL_GPIO_MODE_OUTPUT);
	//GPIO4
	LL_GPIO_SetPinSpeed(GPIOB,LL_GPIO_PIN_4,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_4,LL_GPIO_MODE_OUTPUT);	
	

	// ... (SysTick initialisieren, ca. 300 Hz)
	
	SysTickInit(300);
}

void StepOut(int StepPattern)
// gibt das Schrittmotormuster StepPattern an Motor aus
{	
	// ... (Ausgabe an die unteren vier Bit von Port B    0x.......0110
	LL_GPIO_WriteOutputPort(GPIOB, StepPattern);
	
	
}

/*----------------------------------------------------------------------------
  Interrupt-Handler SysTick
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void)
{
	
	int Richtung = ZielPos-AktPos;
	if(Richtung>0){
		StepDir='+';
	}else if(Richtung<0){
		StepDir='-';
	}else{
		return;
	}
	
	
	int HalbschrittDEZ[8] = {3, 2, 6, 4, 12, 8, 9, 1};			//1 Schritt

	switch (StepDir){
				
				case '+':
					StepOut(HalbschrittDEZ[Ticks++]);
					AktPos++;
					break;
				
				case '-':
					StepOut(HalbschrittDEZ[Ticks--]);
					AktPos--;
					break;
				
				/*case '0':
					//stehen bleiben
					StepOut(HalbschrittDEZ[Ticks]);
					break;*/
	}
	if(Ticks<0)Ticks=7;
	if(Ticks >7)Ticks=0;
	
	
	
	
}
