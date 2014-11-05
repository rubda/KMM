/*
 * gyro.c
 *
 * Created: 2014-11-05 10:53:50
 *  Author: RubenDas
 */ 

#include "gyro.h"
#include "SPI.h"
//#include <util/delay.h>

int getAngularRate(uint8_t adcValue)
{
	
	
	//Steg 1 sätt ADC till aktiv mode
	ss_low();
	send_spi(ACTIVATE_ADC);
	ss_high();
	/* Kolla om den femtonde biten är 0 */
	//_delay_us(120);

	//Steg 2 conversion
	send_spi(START_CONVERSION);
	/* Kolla om den femtonde biten är 0 */
	
	//Steg 3 poll
	send_spi(POLL);
	/* Kolla om den femtonde biten är 0 */
	//_delay_us(120);	
	
	
}	
	