/*----------------------------------------------------------------------------
	MCT-Praktikumsversuche V1...4 Nucleo mit LL-Library
		
	08/2022
 *----------------------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
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
#include "I2C.h"
#include "LCD.h"
#include "Temperatur.h"
#include "LCDInit.h"
#include "ESP_USART3.h"

int a=1;
int d=0;


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
	if(USART2CmdFlag==1){
	//Befehlsunterscheidung
	switch(USART2InputBuffer[0]){
		case 'd':
			// DAC - Sinusausgabe
			sscanf(USART2InputBuffer,"d%da%d", &d, &a);
			break;
		case 'm':
			// Schrittmotor Position anfahren
			sscanf(USART2InputBuffer,"m%d", &ZielPos);
			break;
		case 's':
			{
			// Servo
			int winkel = 0;
			sscanf(USART2InputBuffer,"s%d", &winkel);
			TIM3Servo(winkel);
			break;
			}
		case 'r':
			{
			// RGB-LED setzen
			int r,g,b = 0;
			sscanf(USART2InputBuffer,"r%dg%db%d", &r, &g ,&b);
			TIM3RGB(r,g,b);
			break;
			}
		case 't':
			// Temperatursensor
			switch (USART2InputBuffer[1]){
				case 'r':	// Auslesen
					{
					char buffer[16];
					sprintf(buffer, "%f", TempRead()/16.0);
					USART2WriteString(buffer);
					break;
					}
				case 's':	// Auflösung setzen
					{
					int mode=0; 
					sscanf(USART2InputBuffer,"ts%d", &mode);
					if(mode >= 0 && mode <= 3)TempSetRes(mode);
					break;
					}
			}
			break;
		case 'l':
			// LCD
			switch (USART2InputBuffer[1]){
				case 'c':	// Kommandos
					{
					char cmd=0x00;
					sscanf(USART2InputBuffer,"lc0x%hhx", &cmd);
					LCDSendCmd(cmd);
					break;
					}
				case 'd':	// Daten
					{
					char data=0x00;
					sscanf(USART2InputBuffer,"ld0x%hhx", &data);
					LCDSendData(data);
					break;
					}
			}
			break;		
	}
}
	USART2CmdFlag=0;
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)
{ 	
	//------- Inits ----------
	DACInit();		
	TIM3InitPWM();
	StepperInit();
	USART2Init();
	I2CInit(0x08);
	//LCDInit();
	
	Start_ESP_Wifi(5,123456789);
	
	//Grad-Zeichen erstellen
	//LCDDefineDegree();
	
	
	
	//Sinustabelle
	int i = 0;
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
	
													
													
	while(1){
		// DAC Sinusausgabe
		/*
		if(a<=9 && a >= 1 && d<=10 && d >=0){
			//DACWrite(werte[i++][1]);
			DACWrite(2048+(werte[i++][1]*(1850/a)));
			if(i==32)i=0;
			DelayMs(d);
		}
		*/
		ExecCmd();
		
		
	}
}
