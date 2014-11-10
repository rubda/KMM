/*
 * Sensormodul.c
 *
 * Created: 2014-11-05 09:10:19
 *  Author: RubenDas
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include "SPI.h"
#include "gyro.h"
#include <util/delay.h>
#include "ultraljud.h"


int main(void)
{
	setup_spi();
	activateADC();
	
	DDRA |= (1 << PORTA5);
	
	uint16_t test;
	int one;
	



	
	while(1)
    {
		test = getAngularRate();
		for(int i=16; i>0; i--){
			one = (test & 0x8000) >> 15;
			
			if (one == 1) {
				PORTA |= (1 <<PORTA5);
			}else{
				PORTA &= ~(1 <<PORTA5);
			}
			test <<= 1;
			_delay_ms(10);
		}
		
		
	}
	
	
	
	
}

