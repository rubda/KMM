/*
 * display.c
 *
 * Created: 11/12/2014 11:33:04 AM
 *  Author: jakbi869
 */ 
#include "display.h"
#include <util/delay.h>



void init_displayOutput(){
		DDRB |= (1 << PORTB0);
		DDRB |= (1 << PORTB1);
		DDRB |= (1 << PORTB2);
		DDRD |= (1 << PORTD2);
		DDRD |= (1 << PORTD3);
		DDRD |= (1 << PORTD4);
		DDRD |= (1 << PORTD5);
		DDRD |= (1 << PORTD6);
		DDRD |= (1 << PORTD7);
		DDRC |= (1 << PORTC0);
		DDRC |= (1 << PORTC1);
}


void functionSet(){
	PORTB &= ~(1 << PORTB0);	//rs
	PORTB &= ~(1 << PORTB1);	//r/w
	PORTC &= ~(1 << PORTC1);	//db7
	PORTC &= ~(1 << PORTC0);	//db6
	PORTD |= (1 << PORTD7);		//db5
	PORTD |= (1 << PORTD6);		//db4
	PORTD |= (1 << PORTD5);		//db3 N=1
	PORTD &= ~(1 << PORTD4);	//db2 F=0
	PORTD &= ~(1 << PORTD3);	//db1
	PORTD &= ~(1 << PORTD2);	//db0	
	
	_delay_us(39);
}

void displayOnOffControl(){
	PORTB &= ~(1 << PORTB0);	//rs
	PORTB &= ~(1 << PORTB1);	//r/w
	PORTC &= ~(1 << PORTC1);	//db7
	PORTC &= ~(1 << PORTC0);	//db6
	PORTD &= ~(1 << PORTD7);	//db5
	PORTD &= ~(1 << PORTD6);	//db4
	PORTD |= (1 << PORTD5);		//db3
	PORTD |= (1 << PORTD4);		//db2
	PORTD |= (1 << PORTD3);		//db1
	PORTD |= (1 << PORTD2);		//db0
}

void displayClear(){
	PORTB &= ~(1 << PORTB0);	//rs
	PORTB &= ~(1 << PORTB1);	//r/w
	PORTC &= ~(1 << PORTC1);	//db7
	PORTC &= ~(1 << PORTC0);	//db6
	PORTD &= ~(1 << PORTD7);	//db5
	PORTD &= ~(1 << PORTD6);	//db4
	PORTD &= ~(1 << PORTD5);	//db3 
	PORTD &= ~(1 << PORTD4);	//db2 
	PORTD &= ~(1 << PORTD3);	//db1
	PORTD |= (1 << PORTD2);		//db0
}

void entryModeSet(){
	PORTB &= ~(1 << PORTB0);	//rs
	PORTB &= ~(1 << PORTB1);	//r/w
	PORTC &= ~(1 << PORTC1);	//db7
	PORTC &= ~(1 << PORTC0);	//db6
	PORTD &= ~(1 << PORTD7);	//db5
	PORTD &= ~(1 << PORTD6);	//db4
	PORTD &= ~(1 << PORTD5);	//db3
	PORTD |= (1 << PORTD4);		//db2
	PORTD |= (1 << PORTD3);		//db1
	PORTD |= (1 << PORTD2);		//db0
}