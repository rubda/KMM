#define F_CPU 16000000UL 

#include "hexapod_control.h"
#include <avr/interrupt.h>
#include "UART.h"
#include <stdlib.h>
#include <string.h>
#include "move_body.h"

int direction;
double step_len = 3;
int led_mode = 0;

void do_nothing(int d, double a){
	init_body(direction, step_len);
	return;
}

void (*func)(int, double) = do_nothing;

uint8_t is_command(uart_message* mess, char* c){
	return strcmp(mess->data[0].data, c) == 0;
}

uint8_t get_direction(uart_message* mess){
	switch (mess->data[1].data[0]){
		case 'r':
			return 0;
		case 'l':
			return 180;
		case 'f':
			return 9;
		case 'b':
			return 180;
		default:
			return -1;
	}
}

void handle_message(uart_message *mess){
	char *attr[] = {mess->data[0].data};
	
	if(is_command(mess, "speed")){
		uint16_t speed = atoi(mess->data[1].data);
		if(speed > 0 && speed < 1024){
			set_speed(speed);
		}else{
			send_message("denied", attr, 1);
			return;
		}
	}else if(is_command(mess, "rotate")){
		func = rotate_body;
		direction = get_direction(mess);
	}else if(is_command(mess, "walk")){
		func = move_body;
		if(get_direction(mess) == 9)
			direction = atoi(mess->data[2].data);
		else
			direction = 180;
	}else if(is_command(mess, "stop")){
		func = do_nothing;
	}else if(is_command(mess, "init")){
		robot_start_position();
		uint16_t speed = atoi(mess->data[1].data);
		if(speed > 0 && speed < 1024){
			set_speed(speed);
		}
		func = do_nothing;
	}else if(is_command(mess, "turn")){
		func = turn_body;
		direction = get_direction(mess);
	}else if(is_command(mess, "denied")){
		func = do_nothing;
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
	robot_init(0x00C8);
	_delay_ms(1000);
	uart_init(0x0067);
	_delay_ms(1000);
	
	body_move_init();
	
	EICRA |= (1 << ISC21);
	EIMSK |= (1 << INT2);
	
	sei();
	
	uart_message mess;
	int i, j;
	
	while(1){
		
		if(got_message()){
			get_message(&mess);
			handle_message(&mess);
		}
		
		func(direction, step_len);
	}	                                                               
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}

ISR(INT2_vect){
	cli();
	suart_command_write_data(BROADCAST_ID, 0x019, (uint8_t[1]){(led_mode++ % 2)}, 1);
	uart_send_string("#mode:change;");
	sei();
}



