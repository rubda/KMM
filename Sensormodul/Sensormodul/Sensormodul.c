/*
 * Sensormodul.c
 *
 * Created: 2014-11-05 09:10:19
 *  Author: RubenDas
 */ 

#define F_CPU 7372800UL

#include <avr/io.h>
#include "SPI.h"
#include "gyro.h"
#include <util/delay.h>
#include "ultraljud.h"


int main(void)
{
	setup_spi();
	activateADC();
	getAngularRate();
	while(1)
    {
        //ss_low();
		//get_spi(data);
		//ss_high();
		
		
	
		
		
    }
}

