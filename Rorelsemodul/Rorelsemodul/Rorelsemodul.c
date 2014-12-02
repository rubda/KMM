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
#include "body_ik.h"
#include "atmega-timers.h"

// BYT TAKTIK www.youtube.com/watch?v=Tsxe8AuSsUc
// Position: 0x01 = 0.29 grader 
// Speed: 0x01 = 0.111 rpm

void do_nothing(uint16_t t, uint16_t c, int d){
	return;
}

void (*func)(uint16_t, uint16_t, int) = do_nothing;
int direction;
uint16_t step_len = 0x0018;

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
		case '0':
			return 5;
		default:
			return -1;
	}
}

void handle_message(uart_message *mess){
	char *attr[] = {mess->data[0].data};
	
	if(is_command(mess, "rotate")){
		func = rotate;
	}else if(is_command(mess, "walk")){
		func = take_step;
	}else if(is_command(mess, "stop")){
		func = do_nothing;
	}else if(is_command(mess, "init")){
		robot_start_position();
		func = do_nothing;
	}else{
		func = do_nothing;
		send_message("denied", attr, 1);
		return;
	}
	direction = get_direction(mess);
	if(direction != -1){
		send_message("denied", attr, 1);
	}else{
		func = do_nothing;
		send_message("accept", attr, 1);
	}
}

void move_robot(int x, int y, int z){
	int8_t leg1[] = {11, 20, 0};
	int8_t leg2[] = {11, 20, 0};
	int8_t leg3[] = {0, 20, 0};
	int8_t leg4[] = {0, 20, 0};
	int8_t leg5[] = {-11, 20, 0};
	int8_t leg6[] = {-11, 20, 0};
		
	int8_t *legs[] = {leg1, leg2, leg3, leg4, leg5, leg6};
		
	int i;
	
	for(i = 1; i < 7; ++i){
		if(i == 1 || i == 4 || i == 5)
			ik(legs[i - 1][0] + x, legs[i - 1][1] + y, z, i);
		_delay_ms(400);
	}
}

int main(void)
{	
	_delay_ms(500);
	robot_init(0x0060);
	_delay_ms(1000);
	uart_init(0x0067);
	_delay_ms(1000);
	sei();
	
	body_init();
	robot_start_position();
	_delay_ms(1000);
	
	//timer1(TIMER1_PRESCALER_64, (unsigned int)(CALC_TIMER/1000.0)/(1.0/(F_CPU/64.0)), calculate_body_xy);
	
	//move_robot(0,-5,0);
	//SERVO_ACTION;
	
	for(;;);
	
	uart_message mess;
	
	int i = 0;
	
	while(1)
	{
		if(got_message()){
			get_message(&mess);
			handle_message(&mess);
			break;
		}
		func(step_len, step_len, direction);
	}	                                                               
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}


