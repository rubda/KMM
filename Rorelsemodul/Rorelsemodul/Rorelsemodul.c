/*
 * Rorelsemodul.c
 *
 * Created: 11/4/2014 11:12:30 AM
 *  Author: Carl Arvidsson
 */ 

#include "servo_uart.h"
#include <avr/interrupt.h>

volatile unsigned char data;

int main(void)
{	
	suart_init(1000000);
	sei();
	
	//www.wormfood.net/avrbaudcalc.php
	//extremeelectronics.co.in/avr-tutorials/using-the-usart-of-avr-microcontrollers/
	//UBRR = 23, 0x0017
	
	DDRB = 0xFF;
	
	servo_response res;
	uint8_t list1[] = {0x01};
	uint8_t list2[] = {0x01};
		
	//res = suart_command_write_data(12, 0x20, list1, 1);
	//res = suart_command_reset(0xFE);
	suart_command_read_data(12, 0x11, 0x01);
	PORTB = res.error;
	
	while(1)
	{	
		
		
    }
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}