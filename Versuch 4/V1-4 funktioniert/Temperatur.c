/*----------------------------------------------------------------------------
  Funktionen für Temperatursensor	MCP 9808
	
	06/2020
 *----------------------------------------------------------------------------*/

#include "I2C.h"

#define TEMP_ADR    0x18  // Slave Adresse Temperaturfühler
#define TEMP_RES    0x08  // Resolution Register
#define TEMP_TR     0x05  // Temperature Register

int TempRead()
// liest Temperatur aus und gibt sie in 1/16 Grad zurück
{
	uint8_t buffer[2];
	*buffer = TEMP_TR;
	I2CWrite(buffer, 1, TEMP_ADR);			//RegisterPointer auf TR
	
	I2CRead(buffer, 2, TEMP_ADR);
	
	//Konvertierung
	int Lowerbyte=buffer[1];
	int Upperbyte=buffer[0];
	
	Upperbyte &= 0x1F;
	if((Upperbyte&0x10)==0x10){
		Upperbyte &= 0x0f;
		return (-1*(Upperbyte * 256 + Lowerbyte));
	}else{
		return (Upperbyte * 256 + Lowerbyte);
	}
}

void TempSetRes(int resolution)
// Auflösung des Temperatursensors setzen (Modi 0...3 erlaubt)
{ 
	uint8_t buffer[2];
	buffer[0] = TEMP_RES;
	buffer[1] = resolution;
	
	I2CWrite(buffer, 2, TEMP_ADR);	
}
