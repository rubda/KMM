#define F_CPU 16000000UL

/*
 * Rorelsemodul.c
 *
 * Created: 11/4/2014 11:12:30 AM
 *  Author: Carl Arvidsson
 */ 

#include "servo_uart.h"
#include <avr/interrupt.h>
#include "UART.h"
//#include <util/delay.h>

//volatile unsigned char data;

int main(void)
{	
	sei();
	uart_init(1200);
	
	//www.wormfood.net/avrbaudcalc.php
	//extremeelectronics.co.in/avr-tutorials/using-the-usart-of-avr-microcontrollers/
	//UBRR = 23, 0x0017
	
	DDRB = 0xFF;
	
	while(1)
	{
		uart_send_char('0');	
		_delay_us(10);
    }
}

ISR(USART0_RX_vect){
	//while (!bit_is_set(UCSR0A, RXC0));
	//toggle_bit(PORTB, PORTB1);
	//if(UDR0 == '0')
	//	set_bit(PORTB, PORTB0);
	//else
	//	clear_bit(PORTB, PORTB0);
	PORTB = UDR0;
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}