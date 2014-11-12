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
#include "UART.h"


int main(void)
{

	DDRB |= (1 << PORTB0);
	PORTB &= ~(1 << PORTB0);
	setup_spi();
	activateADC();
	
	int foo = 90;
	
	
	while(1)
    {
		foo = -foo;
		rotate(foo);
		while(!getIsRotated());
		PORTB |= (1 << PORTB0);
		hasRotated(0);
		_delay_ms(5000);
		PORTB &= ~(1 << PORTB0);
		
	}
	
	
	
	
}

