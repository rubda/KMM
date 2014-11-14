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
		
	// BYT TAKTIK www.youtube.com/watch?v=Tsxe8AuSsUc                                                                     */
	
	uint8_t test2[] = {0x70, 0x01}; 
	uint8_t test1[] = {0x8f, 0x02};

	_delay_ms(1000);
	set_speed(0x00FF);
	robot_start_position();
	_delay_ms(2000);
		
	while(1)
	{
		take_step(0x0050);
		_delay_ms(3000);
	}
	
	return 0;
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}

