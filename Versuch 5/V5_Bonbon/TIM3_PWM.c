/*----------------------------------------------------------------------------
  Funktionen für PWM (Servo und RGB-LED)
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_tim.h>

#include "def.h"




void TIM3InitPWM(void)
// Initialisierung des Timer 3 für PWM (Servo + RGB-LED)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);			// GPIOA Takt einschalten
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);			// GPIOC Takt einschalten
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);		// TIM3 Takt einschalten --------------------------------

	
	//PC12 (schwarzer Taster) initialisieren
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_12, LL_GPIO_MODE_INPUT);
	
	// PA6 (Servo) initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_6,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_6,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_6,LL_GPIO_AF_2);

	// PA7 (LED rot) initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_7,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_7,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_7,LL_GPIO_AF_2);

	// PC8 (LED grün) initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_8,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_8,LL_GPIO_AF_2);

	// PC9 (LED blau) initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_9,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_9,LL_GPIO_AF_2);

	// Timer initialisieren
	//Prescale: clk = fCK_PSC / (PSC[15:0] + 1) --> PSC = 16-1 = 15
	LL_TIM_SetPrescaler(TIM3, 15);
	LL_TIM_SetAutoReload(TIM3, 20000);
	LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);
	
	
	
	//LL_TIM_CC_EnablePreload 	//Notwendig?
	
	// Kanal 1 (Servo)
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);	//Channel einschalten
	LL_TIM_OC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);		//Polarität setzen
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);		//Modus setzen
	LL_TIM_OC_SetCompareCH1(TIM3, 1000);		//Compare -Wert setzen

	// Kanal 2...4 (RGB-LED)
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH2);		//Channel einschalten
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	
	LL_TIM_OC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_OCPOLARITY_LOW);		//Polarität setzen
	LL_TIM_OC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCPOLARITY_LOW);
	LL_TIM_OC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH4, LL_TIM_OCPOLARITY_LOW);
	
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);		//Modus setzen
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
	
	LL_TIM_OC_SetCompareCH2(TIM3, 15000);		//Compare -Wert setzen
	LL_TIM_OC_SetCompareCH3(TIM3, 15000);
	LL_TIM_OC_SetCompareCH4(TIM3, 15000);
	
	// Timer starten
	LL_TIM_EnableCounter(TIM3);
	
}

int getTaster(void){
	//low-activ -> 0 ist gedrückt
//	return !(LL_GPIO_IsInputPinSet(GPIOC, 0x0800));
	
	int zustandButton;
	zustandButton = LL_GPIO_ReadInputPort(GPIOC) & 0x1000;
	if(zustandButton==0){
		return 1;
	}else{
		return 0;
	}
}

void TIM3Servo (int pos)
// Servo auf Position fahren (0...100)
{
  if (pos <SERVO_MIN || pos >SERVO_MAX)														// Abbruch, weil ausserhalb des Wertebereichs
		return;
	
	LL_TIM_OC_SetCompareCH1(TIM3, pos);
	
}

void TIM3RGB(int red, int green, int blue)
// RGB-LED-Helligkeit setzen (0...255)
{
	if ((red >=0) && (red <=255))
		; LL_TIM_OC_SetCompareCH2(TIM3, (20000*red/255));
	if ((green >=0) && (green <=255))
		; LL_TIM_OC_SetCompareCH3(TIM3, (20000*green/255));
	if ((blue >=0) && (blue <=255))
		; LL_TIM_OC_SetCompareCH4(TIM3, (20000*blue/255));
}
