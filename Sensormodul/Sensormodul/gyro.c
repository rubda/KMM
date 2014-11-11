/*
 * gyro.c
 *
 * Created: 2014-11-05 10:53:50
 *  Author: RubenDas
 */ 

#define F_CPU 8000000UL

#include "gyro.h"
#include "SPI.h"


#include <util/delay.h>

void activateADC()
{
	uint16_t REG = 0;
	
	//Steg 1 sätt ADC till aktiv mode
	ss_low();
	send_spi(ACTIVATE_ADC);
	REG = get_spi(0xFF);
	ss_high();
	
	if (REG & (1 << 16)) activateADC();
	REG = 0;
	_delay_us(150);
}

void startConversion(){
	uint16_t REG = 0;
	
	//Steg 2 conversion
	ss_low();
	send_spi(START_CONVERSION);
	REG = get_spi(0xFF);
	ss_high();
	if (REG & (1 << 16)) startConversion();
	REG = 0;
}


uint16_t getAngularRate()
{
	uint16_t REG = 0;
	uint16_t DATA = 0;
	
	//Steg 3 poll
	ss_low();
	send_spi(POLL);
	_delay_us(150);
	
	if (REG & (1 << 16)) getAngularRate();
	REG = get_spi(0xFF);
	ss_high();
	
	REG = REG >> 1;
	DATA = REG & 0x07FF; 
	
	return DATA;		
}	

int adcToAngularRate(uint16_t data)
	{
		int vOutAngularRate = (data * 25/12)+400; //Uttryckt i millivolts
		
		return (vOutAngularRate - 2500)/6.67; // Uttryckt i grader/sec (beroende på vilken gyro modell vi har)
	}
	
bool isRotationDone(uint16_t angle)
{
	uint16_t rate;
	int achievedAngle = 0;
		
	if (angle > 0){
		while(angle > achievedAngle)
		{		
			startConversion();
			rate = getAngularRate();
			achievedAngle += adcToAngularRate(rate);
			//_delay_ms(1000); behövs nog inte
		}
		return true;
	}else{
		while(achievedAngle > angle)
		{	
			startConversion();
			rate = getAngularRate();
			achievedAngle += adcToAngularRate(rate);
		}
		return true;
	}
		
}
