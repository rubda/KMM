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
#include <stdlib.h>
#include <string.h>

// BYT TAKTIK www.youtube.com/watch?v=Tsxe8AuSsUc
// Position: 0x01 = 0.29 grader 
// Speed: 0x01 = 0.111 rpm

void do_nothing(uint16_t t, uint16_t c, int d){
	return;
}

void (*func)(uint16_t, uint16_t, int) = do_nothing;
int direction;
uint16_t step_len = 0x0050;

uint8_t is_command(uart_message* mess, char* c){
	return strcmp(mess->data[0].data, c) == 0;
}

uint8_t get_direction(uart_message* mess){
	switch (mess->data[1].data[0])
	{
		case 'r':
			return RIGHT;
		case 'l':
			return LEFT;
		case 'f':
			return FORWARD;
		case 'b':
			return BACKWARD;
		default:
			return -1;
	}
}

void handle_message(uart_message *mess){
	if(is_command(mess, "rotate")){
		func = rotate;
	}else if(is_command(mess, "walk")){
		func = take_step;
	}else if(is_command(mess, "stop")){
		func = do_nothing;
	}else{
		send_message("accept", false, 1);
		func = do_nothing;
		return;
	}
	direction = get_direction(mess);
	if(direction != -1){
		send_message("accept", true, 1);
	}else{
		send_message("accept", false, 1);
		func = do_nothing;
	}
}

int main(void)
{	
	_delay_ms(500);
	robot_init(0x0080);
	_delay_ms(1000);
	robot_start_position();
	uart_init(0x0067);
	_delay_ms(1000);
	sei();
	
	uart_message mess;
	while(1)
	{
		if(got_message()){
			get_message(&mess);
			handle_message(&mess);
		}
		func(step_len, step_len, direction);
	}	                                                               
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}


