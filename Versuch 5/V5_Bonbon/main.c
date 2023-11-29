/*----------------------------------------------------------------------------
	MCT-Praktikumsversuch V5 Nucleo
	Bonbonautomat
	Mechatronik mit Bibliotheken
	
	04/2022
 *----------------------------------------------------------------------------*/
#include "def.h"

#include <stdio.h>

#include "LED.h"
#include "ADC.h"
#include "TIM3_PWM.h"
#include "DELAY.h"

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!! Vor dem Start müssen in "def.h" alle defines sinnvoll gesetzt werden !!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int main(void)
{
	
	// Initialisierungen
	TIM3InitPWM();
	ADCInit(1);
	LEDInit();

	
	
	TIM3Servo(SERVO_MIN);
	
	while (1)
	{
		
		//Füllstand auslesen
			if(ADCRead()<=LICHTGRENZE){
				//LED rot
				TIM3RGB(255,0,0);
			}else{
				//LED grün
				TIM3RGB(0,255,0);
			}
		
		//Taster auslesen
		if(getTaster()){
			
			//LED blau setzen
			TIM3RGB(0,0,255);
			
			//Servo an Auswurfposition bewegen
			for(int i = SERVO_MIN; i<=SERVO_MAX; i+=((SERVO_MAX-SERVO_MIN)/100)){
				TIM3Servo(i);
				DelayMs(10);
			}
			
			//Servo an Startposition bewegen
			for(int i = SERVO_MAX; i>=SERVO_MIN; i-=((SERVO_MAX-SERVO_MIN)/100)){
				TIM3Servo(i);
				DelayMs(10);
			}
				
		}
	}

}
