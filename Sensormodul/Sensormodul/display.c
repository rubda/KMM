/*
 * display.c
 *
 * Created: 11/12/2014 11:33:04 AM
 *  Author: jakbi869
 */ 
#include "display.h"
#include <util/delay.h>
int wait_time = 40;

void wait(int n){
	
	volatile int a = 0;
	while(a<n){a++;}
}

void init_display()
{
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
    
    PORTB &= ~(1 << DISP_E);
    _delay_ms(20);
    
    // Detta borde funka, men fråga mig inte varför
    set_display(0x00, 0x30);
    toggle_enable();
    
    set_display(0x00, 0x30);
    toggle_enable();
    
    set_display(0x00, 0x30);
    toggle_enable();
    
    // Function set
    set_display(0x00, 0x38);
    toggle_enable();
    
    // Display on
    set_display(0x00, 0x0F);
    toggle_enable();
    
    // Clear display
    clear_display();
    toggle_enable();
    //wait(2000);
    
    // Entry mode
    set_display(0x00, 0x06);
    //_delay_us(1000);
    toggle_enable();
    //wait(2000);

}

void clear_display()
{
	set_display(0x00, 0x01);
	//toggle_enable();
}

void toggle_enable()
{
	PORTB |= (1 << DISP_E);
	//wait(40);
	_delay_ms(10);
	PORTB &= ~(1 << DISP_E);
	//wait(40);
	_delay_ms(10);
}


void set_display(int mode, int instr)
{	
	PORTB &= ~(1 << DISP_RS);
	PORTB &= ~(1 << DISP_RW);
	PORTC &= ~(1 << DISP_DB7);
	PORTC &= ~(1 << DISP_DB6);
	PORTD &= ~(1 << DISP_DB5);
	PORTD &= ~(1 << DISP_DB4);
	PORTD &= ~(1 << DISP_DB3);
	PORTD &= ~(1 << DISP_DB2);
	PORTD &= ~(1 << DISP_DB1);
	PORTD &= ~(1 << DISP_DB0);
	
	PORTB |= (((mode >> 1) & 1) << DISP_RS);
	PORTB |= (((mode >> 0) & 1) << DISP_RW);
	/*
	_delay_us(40);
	//wait(40);
	PORTB |= (1 << DISP_E);
	//wait(150);
	_delay_us(150);
	*/
	PORTC |= (((instr >> 7) & 1) << DISP_DB7);
	PORTC |= (((instr >> 6) & 1) << DISP_DB6);
	PORTD |= (((instr >> 5) & 1) << DISP_DB5);
	PORTD |= (((instr >> 4) & 1) << DISP_DB4);
	PORTD |= (((instr >> 3) & 1) << DISP_DB3);
	PORTD |= (((instr >> 2) & 1) << DISP_DB2);
	PORTD |= (((instr >> 1) & 1) << DISP_DB1);
	PORTD |= (((instr >> 0) & 1) << DISP_DB0);
	
/*	_delay_us(80);
	//wait(80);
	PORTB &= ~(1 << DISP_E);
	//wait(250);
	_delay_us(250);*/
}
	
void write_to_display(value)
{
	unsigned char symbol = int_to_char(value);
	
	set_display(0x00, 0x86);
	toggle_enable();
	
	set_display(0x02, symbol);
	toggle_enable();
}

unsigned char int_to_char(int digit)
{
	unsigned char data = '0' + digit;
	return data; 
}
