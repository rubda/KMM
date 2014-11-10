//#define F_CPU 16000000UL

/*
 * Rorelsemodul.c
 *
 * Created: 11/4/2014 11:12:30 AM
 *  Author: Carl Arvidsson
 */ 

#include "servo_uart.h"
#include <avr/interrupt.h>
//#include <util/delay.h>

//volatile unsigned char data;

int main(void)
{	
	suart_init(1000000);
	sei();
	
	//www.wormfood.net/avrbaudcalc.php
	//extremeelectronics.co.in/avr-tutorials/using-the-usart-of-avr-microcontrollers/
	//UBRR = 23, 0x0017
	
	DDRB = 0xFF;
	
	while(1)
	{	
    }
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}