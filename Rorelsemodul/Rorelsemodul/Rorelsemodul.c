#define F_CPU 16000000UL

/*
 * Rorelsemodul.c
 *
 * Created: 11/4/2014 11:12:30 AM
 *  Author: Carl Arvidsson
 */ 

#include "servo_uart.h"
#include <avr/interrupt.h>

int main(void)
{	
	sei();
	suart_init(1000000);
		
	uint8_t test1[] = {0xFF, 0x00};
		
	_delay_ms(1000);
	set_servo_speed();
	robot_start_position();
	//_delay_ms(2000);
	//reset_leg(4);
	_delay_ms(1000);
	
	
	while(1)
	{
		
	}
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}

