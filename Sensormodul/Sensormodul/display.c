/*
 * display.c
 *
 * Created: 11/12/2014 11:33:04 AM
 *  Author: jakbi869
 */ 
#include "display.h"
#include <util/delay.h>

volatile int display_tester_loop=0;
int wait_time = 20;

void wait(int n){
	
	volatile int a = 0;
	while(a<n){a++;}
}

void init_display(){
		DDRB |= (1 << DISP_RS);
		DDRB |= (1 << DISP_RW);
		DDRB |= (1 << DISP_E);
		DDRC |= (1 << DISP_DB7);
		DDRC |= (1 << DISP_DB6);
		DDRD |= (1 << DISP_DB5);
		DDRD |= (1 << DISP_DB4);
		DDRD |= (1 << DISP_DB3);
		DDRD |= (1 << DISP_DB2);
		DDRD |= (1 << DISP_DB1);
		DDRD |= (1 << DISP_DB0);
		
		//PORTB &= ~(1 << DISP_E);
		wait(wait_time);
		//function set
		PORTB &= ~(1 << DISP_RS);	
		PORTB &= ~(1 << DISP_RW);	
		PORTC &= ~(1 << DISP_DB7);	
		PORTC &= ~(1 << DISP_DB6);	
		PORTD |= (1 << DISP_DB5);		
		PORTD |= (1 << DISP_DB4);		
		PORTD |= (1 << DISP_DB3);		
		PORTD &= ~(1 << DISP_DB2);	
		PORTD &= ~(1 << DISP_DB1);	
		PORTD &= ~(1 << DISP_DB0);	
		
		PORTB |= (1 << DISP_E);
		wait(wait_time);
		PORTB &= ~(1 << DISP_E);
		wait(wait_time);
	
		//display on
		PORTB &= ~(1 << DISP_RS);	
		PORTB &= ~(1 << DISP_RW);
		PORTC &= ~(1 << DISP_DB7);	
		PORTC &= ~(1 << DISP_DB6);	
		PORTD &= ~(1 << DISP_DB5);
		PORTD &= ~(1 << DISP_DB4);	
		PORTD |= (1 << DISP_DB3);	
		PORTD |= (1 << DISP_DB2);		
		PORTD |= (1 << DISP_DB1);		
		PORTD |= (1 << DISP_DB0);	
		
		PORTB |= (1 << DISP_E);
		wait(wait_time);
		PORTB &= ~(1 << DISP_E);
		wait(wait_time);
		
		displayClear();
		
		//entry mode set
		PORTB &= ~(1 << DISP_RS);	
		PORTB &= ~(1 << DISP_RW);
		PORTC &= ~(1 << DISP_DB7);
		PORTC &= ~(1 << DISP_DB6);
		PORTD &= ~(1 << DISP_DB5);	
		PORTD &= ~(1 << DISP_DB4);	
		PORTD &= ~(1 << DISP_DB3);	
		PORTD |= (1 << DISP_DB2);		
		PORTD |= (1 << DISP_DB1);		
		PORTD |= (1 << DISP_DB0);		
		
		PORTB |= (1 << DISP_E);
		wait(wait_time);
		PORTB &= ~(1 << DISP_E);
		wait(wait_time+1000);
}

void displayClear(){
		PORTB &= ~(1 << DISP_RS);	
		PORTB &= ~(1 << DISP_RW);	
		PORTC &= ~(1 << DISP_DB7);	
		PORTC &= ~(1 << DISP_DB6);	
		PORTD &= ~(1 << DISP_DB5);	
		PORTD &= ~(1 << DISP_DB4);	
		PORTD &= ~(1 << DISP_DB3);	
		PORTD &= ~(1 << DISP_DB2);	
		PORTD &= ~(1 << DISP_DB1);	
		PORTD |= (1 << DISP_DB1);	
		
		PORTB |= (1 << DISP_E);
		wait(wait_time);
		PORTB &= ~(1 << DISP_E);
		wait(wait_time);
}

