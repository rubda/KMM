/*
 * Rorelsemodul.c
 *
 * Created: 11/4/2014 11:12:30 AM
 *  Author: Carl Arvidsson
 */ 

#include "servo_uart.h"

int main(void)
{	
	suart_init(1000000);
	
	//www.wormfood.net/avrbaudcalc.php
	//extremeelectronics.co.in/avr-tutorials/using-the-usart-of-avr-microcontrollers/
	//UBRR = 23, 0x0017
	char send[20] = {0xFF, 0xFF, 0x01, 0x02, 0x01};
	//char recv[20];
	//int i = 0;
	
	DDRB = 0xFF;
	
	servo_response res;
	//if(res == (servo_response)SERVO_ERROR);
	res = suart_command_ping(1);
	
	while(1)
<<<<<<< HEAD
	{
		PORTD &= ~(1 << PORTD5);
		PORTD |= (1 << PORTD5);
		suart_send_string(send, 5);
		//res = suart_command_ping(1);
		//PORTB = res.error;*/
=======
	{	
		
		PORTB = res.error;
>>>>>>> 8780d3e853488ce3e828b6cb6a7f5960643bd646
    }
}