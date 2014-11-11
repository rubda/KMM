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

	init_sensors();
	

	while(1)
    {

		//get_distance(get_sensor(4));
	
		//DDRB |= (1 << PORTB0);
		//_delay_ms(1000);
		//PORTB ^= (1 << PORTB0);
		get_distance(get_sensor(1));
		//_delay_ms(10);
		
	}
	
	
	
	
}

