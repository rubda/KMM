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

	uart_init(1200);
	//uart_send_char('A');
	while(1)
    {

		uart_send_char('P');
		_delay_ms(1000);
	}
	
	
	
	
}

