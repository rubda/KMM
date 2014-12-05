#define F_CPU 16000000UL 

#include "hexapod_control.h"
#include <avr/interrupt.h>
#include "UART.h"
#include <stdlib.h>
#include <string.h>

int direction;
uint16_t step_len = 0x0020;

void do_nothing(uint16_t t, uint16_t c, int d){
	return;
}

void (*func)(uint16_t, uint16_t, int) = do_nothing;

uint8_t is_command(uart_message* mess, char* c){
	return strcmp(mess->data[0].data, c) == 0;
}

uint8_t get_direction(uart_message* mess){
	switch (mess->data[1].data[0]){
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
	char *attr[] = {mess->data[0].data};
	
	if(is_command(mess, "rotate")){
		func = rotate;
		direction = get_direction(mess);
	}else if(is_command(mess, "walk")){
		func = take_step;
		direction = get_direction(mess);
	}else if(is_command(mess, "stop")){
		func = do_nothing;
		direction = 5;
	}else if(is_command(mess, "init")){
		robot_start_position();
		func = do_nothing;
		direction = 5;
	}else if(is_command(mess, "turn")){
		func = turn;
		direction = get_direction(mess);
	}else if(is_command(mess, "denied")){
		func = do_nothing;
		direction = 5;
		return;
	}else{
		func = do_nothing;
		send_message("denied", attr, 1);
		return;
	}
	
	if(direction != -1){
		send_message("accept", attr, 1);
	}else{
		func = do_nothing;
		send_message("denied", attr, 1);
	}
}

int main(void)
{	
	_delay_ms(500);
	robot_init(0x00B0);
	_delay_ms(1000);
	uart_init(0x0067);
	_delay_ms(1000);
	
	sei();
	
	uart_message mess;
	
	ik(0, 0, 0, 1);
	ik(0, 0, 0, 2);
	ik(0, 0, 0, 3);
	ik(0, 0, 0, 4);
	ik(0, 0, 0, 5);
	ik(0, 0, 0, 6);
	SERVO_ACTION;
	
	while(1){
		
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



