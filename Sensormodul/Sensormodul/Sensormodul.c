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
	uint8_t data;
	setup_spi();
	
	while(1)
    {
        //ss_low();
		send_spi(POLL);
		_delay_ms(1000);
		//get_spi(data);
		//ss_high();
    }
}

