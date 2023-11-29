/*----------------------------------------------------------------------------
	MCT-Praktikumsversuch V1.1 Nucleo
	mit Lauflicht/Warteschleife in C
	
	06/2020
 *----------------------------------------------------------------------------*/

#define __IO volatile							// Datentyp flüchtig
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
// Struktur für RCC-Register
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
// Struktur für GPIO-Register
{
  __IO uint32_t MODER;    // Modus
  __IO uint32_t OTYPER;   // Ausgabetyp
  __IO uint32_t OSPEEDR;  // Geschwindigkeit
  __IO uint32_t PUPDR;    // Pull up/down
  __IO uint32_t IDR;      // Eingangsdaten
  __IO uint32_t ODR;      // Ausgangsdaten
  __IO uint32_t BSRR;     // Regsiter, um einzelne Bits zu setzen/rücksetzen
  __IO uint32_t LCKR;     // Sperrung
  __IO uint32_t AFR[2];   // Alternative Funktionen
} GPIO_TypeDef;

typedef struct
// Struktur für SysTick-Register
{
  __IOM uint32_t CTRL;		// Kontrollregister
  __IOM uint32_t LOAD;		// Register Startwert
  __IOM uint32_t VAL;			// aktueller Wert
  __IM  uint32_t CALIB;		// Kalibration
} SysTick_TypeDef;

void wait (int wait) 
// Wartefunktion, wartet entsprechend des wait-Werts
{
  int  d1, d2;

	for (d1 = 0; d1< wait; d1++)
		for (d2 = 0; d2 < 1800; d2++);
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)
{
	int output,input=0;												// Ein-/Ausgabewerte
	int i;																		// Laufvariable
	
	// GPIO initialisieren
  RCC->AHB1ENR |= 0x06;											// Takt für GPIOB und C einschalten
	GPIOB->MODER = 0x5555;										// PB0...7 als Ausgang
	GPIOC->MODER &= 0xFF00;							// ...	(PC0...7 als Eingang)
	
  while (1) 
	{
    output = 0x01;  												// Start mit LED 0
		for (i=0;i<=7;i++)											//Schalter einlesen, ausmaskieren und Geschwindigkeit ändern
		{
			GPIOB->ODR = output;									// ausgeben
			output = output <<1;  								// schieben

			input = GPIOC->IDR & 0x3;						// ... (Schalter einlesen)
			
			wait(100*(input+1));									// (1-4) * 100 ms warten
		}
	}		
}
