/*----------------------------------------------------------------------------
	MCT-Praktikumsversuch V1.1 Nucleo
	mit Lauflicht/Warteschleife in C
	
	06/2020
 *----------------------------------------------------------------------------*/

#define __IO volatile							// Datentyp fl�chtig
#define __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define __IOM    volatile            /*! Defines 'read / write' structure member permissions */
	
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region                                */
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)	// Basisadresse Peripherie
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)	// RCC-Basisadresse
#define RCC                 	((RCC_TypeDef *) RCC_BASE)		// RCC-Register
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address */
#define SysTick             ((SysTick_TypeDef   *)     SysTick_BASE  )   /*!< SysTick configuration struct */

typedef unsigned int uint32_t;		// Datentyp 32bit Integer

extern uint32_t SystemCoreClock;	// Systemtakt

typedef struct
// Struktur f�r RCC-Register
{
  __IO uint32_t CR;
  __IO uint32_t PLLCFGR;
  __IO uint32_t CFGR;
  __IO uint32_t CIR;
  __IO uint32_t AHB1RSTR;
  __IO uint32_t AHB2RSTR;
  __IO uint32_t AHB3RSTR;
  uint32_t      RESERVED0;
  __IO uint32_t APB1RSTR;
  __IO uint32_t APB2RSTR;
  uint32_t      RESERVED1[2];
  __IO uint32_t AHB1ENR;
  __IO uint32_t AHB2ENR;
  __IO uint32_t AHB3ENR;
  uint32_t      RESERVED2;
  __IO uint32_t APB1ENR;
  __IO uint32_t APB2ENR;
  uint32_t      RESERVED3[2];
  __IO uint32_t AHB1LPENR;
  __IO uint32_t AHB2LPENR;
  __IO uint32_t AHB3LPENR;
  uint32_t      RESERVED4;
  __IO uint32_t APB1LPENR;
  __IO uint32_t APB2LPENR;
  uint32_t      RESERVED5[2];
  __IO uint32_t BDCR;
  __IO uint32_t CSR;
  uint32_t      RESERVED6[2];
  __IO uint32_t SSCGR;
  __IO uint32_t PLLI2SCFGR;
  __IO uint32_t PLLSAICFGR;
  __IO uint32_t DCKCFGR;
  __IO uint32_t CKGATENR;
  __IO uint32_t DCKCFGR2;
} RCC_TypeDef;

typedef struct
// Struktur f�r GPIO-Register
{
  __IO uint32_t MODER;    // Modus
  __IO uint32_t OTYPER;   // Ausgabetyp
  __IO uint32_t OSPEEDR;  // Geschwindigkeit
  __IO uint32_t PUPDR;    // Pull up/down
  __IO uint32_t IDR;      // Eingangsdaten
  __IO uint32_t ODR;      // Ausgangsdaten
  __IO uint32_t BSRR;     // Regsiter, um einzelne Bits zu setzen/r�cksetzen
  __IO uint32_t LCKR;     // Sperrung
  __IO uint32_t AFR[2];   // Alternative Funktionen
} GPIO_TypeDef;

typedef struct
// Struktur f�r SysTick-Register
{
  __IOM uint32_t CTRL;		// Kontrollregister
  __IOM uint32_t LOAD;		// Register Startwert
  __IOM uint32_t VAL;			// aktueller Wert
  __IM  uint32_t CALIB;		// Kalibration
} SysTick_TypeDef;

/*
void wait (int wait) 
// Wartefunktion, wartet entsprechend des wait-Werts
{
  int  d1, d2;

	for (d1 = 0; d1< wait; d1++)
		for (d2 = 0; d2 < 1800; d2++);
}*/

void SysTick_Handler(void){
	
	
	int output = GPIOB->ODR;
	
	if (output >= 0x80 || output == 0x00){
		GPIOB->ODR = 0x01;
	}else{
		GPIOB->ODR = output*2;
	}
	
}

void SysTickTime(int ms){
	
	SysTick->CTRL |= 0x04;
	SysTick->LOAD = 16000 * ms;
	
}


void SysTickEnable(void){
	
	SysTick->CTRL |= 0x03;
	
}
	
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)
{
	//int output,
	int input=0;												// Ein-/Ausgabewerte
	int time=0;																		// Laufvariable
	
	// GPIO initialisieren
  RCC->AHB1ENR |= 0x06;											// Takt f�r GPIOB und C einschalten
	GPIOB->MODER = 0x5555;										// PB0...7 als Ausgang
	GPIOC->MODER &= 0xFF00;							// ...	(PC0...7 als Eingang)
	
	
	//SysTick initialissieren
	
	SysTickEnable();
	SysTickTime(1000);
	
  while (1) 
	{
		
		input = GPIOC->IDR & 0x3;						// ... (Schalter einlesen)
		time = 1000-(input * 250);
		SysTickTime(time);
		
	}		
}
