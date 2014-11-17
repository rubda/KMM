#define F_CPU 16000000UL

/*
 * Rorelsemodul.c
 *
 * Created: 11/4/2014 11:12:30 AM
 *  Author: Carl Arvidsson
 */ 

#include "hexapod_control.h"
#include <avr/interrupt.h>
#include "UART.h"

// BYT TAKTIK www.youtube.com/watch?v=Tsxe8AuSsUc
// Position: 0x01 = 0.29 grader 
// Speed: 0x01 = 0.111 rpm

int main(void)
{	
	sei();
	uart_init(0x0010);
	
	_delay_ms(100);
	uart_send_string("Hejsan");
	
	while(1)
	{
	}	                                                               
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}


