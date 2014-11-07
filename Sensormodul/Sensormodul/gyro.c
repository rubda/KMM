/*
 * gyro.c
 *
 * Created: 2014-11-05 10:53:50
 *  Author: RubenDas
 */ 

#define F_CPU 7372800UL

#include "gyro.h"
#include "SPI.h"


#include <util/delay.h>

int get_angularrate(uint8_t adcValue)

	uint16_t outData;
	uint8_t	tmpData;

{
	//Steg 1 sätt ADC till aktiv mode
	ss_low();
	send_spi(ACTIVATE_ADC);
	outData = send_spi(0xFF);
	outData << 8; 
	tmpData = send_spi(0xFF);
	
	ss_high();
	/* Kolla om den femtonde biten är 0 */
	_delay_us(120);

	//Steg 2 conversion
	send_spi(START_CONVERSION);
	/* Kolla om den femtonde biten är 0 */
	
	
	//Steg 3 poll
	send_spi(POLL);
	/* Kolla om den femtonde biten är 0 */
	_delay_us(120);		
}	
	