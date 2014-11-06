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
	
	char send[20] = {0xFF, 0xFF, 0x01, 0x02, 0x01};
	
	DDRB = 0xFF;
	
	//servo_response res;
	//if(res == (servo_response)SERVO_ERROR);
	
	while(1)
	{	
		//suart_command_ping(1);
		suart_send_string(send, 5);
    }
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}

ISR(USART1_RX_vect){
	data = UDR0;
}