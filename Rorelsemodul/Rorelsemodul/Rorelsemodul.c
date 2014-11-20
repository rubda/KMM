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

void *(func)(uint16_t *, int *);
int direction;
uint16_t step_len;

int main(void)
{	
	sei();
	robot_init(0x0080);
	uart_init(0x0067);
	
	uart_message mess;
	char *attr[] = {"12"};

	while(1)
	{
		if(got_message()){
			get_message(&mess);
			if(strcmp(mess.data[0].data, "stop") == 0){
				//uart_send_char('0' + mess.length);
				uart_send_string(mess.data[1].data);
				uart_send_char(';');
				if(strcmp(mess.data[1].data, "r") == 0)
					rotate(0x0050, RIGHT);	
			}
		}
	}	                                                               
}

ISR(USART1_TX_vect){
	SUART_RX_ACTIVE;
}


