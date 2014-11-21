/*
 * display.c
 *
 * Created: 11/12/2014 11:33:04 AM
 *  Author: jakbi869
 */ 
#include "display.h"
#include <util/delay.h>
//#include "UART.h"



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
    _delay_ms(1000);
    
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
    
    // Entry mode
    set_display(0x00, 0x06);
    toggle_enable();

	

	//BOSSE();

	
}

void clear_display()
{
	set_display(0x00, 0x01);
	toggle_enable();
}

void toggle_enable()
{
	PORTB |= (1 << DISP_E);
	//wait(20);
	_delay_ms(10);
	PORTB &= ~(1 << DISP_E);
	//wait(20);
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
	
	_delay_ms(10);
	
	PORTC |= (((instr >> 7) & 1) << DISP_DB7);
	PORTC |= (((instr >> 6) & 1) << DISP_DB6);
	PORTD |= (((instr >> 5) & 1) << DISP_DB5);
	PORTD |= (((instr >> 4) & 1) << DISP_DB4);
	PORTD |= (((instr >> 3) & 1) << DISP_DB3);
	PORTD |= (((instr >> 2) & 1) << DISP_DB2);
	PORTD |= (((instr >> 1) & 1) << DISP_DB1);
	PORTD |= (((instr >> 0) & 1) << DISP_DB0);
	
	_delay_ms(10);
	
/*	_delay_us(80);
	//wait(80);
	PORTB &= ~(1 << DISP_E);
	//wait(250);
	_delay_us(250);*/
}

void BOSSE()
{
	int i;
	char name[] = "BOSSE";
	
	for (i = 0; i < 5; ++i){
		set_display(0x00, 0x85+i);
		toggle_enable();
	
		set_display(0x02, name[i]);
		toggle_enable();
		}
}

void write_string(char* str, int pos){
	int i;
	for (i = 0; str[i] != '\0' && i <= 32; ++i){
		set_display(0x00, 0x00+pos+i);
		toggle_enable();
	
		set_display(0x02, str[i]);
		toggle_enable();
	}	
}
	
void write_to_display(uint16_t value, int pos)
{
	uint16_t symbol = int_to_char(value);
	
	set_display(0x00, 0x95+pos);
	toggle_enable();
	
	set_display(0x02, symbol);
	toggle_enable();
}

void write_to_display2(uint16_t value, int pos)
{
	uint16_t symbol = int_to_char(value);
	
	set_display(0x00, 0x85+pos);
	toggle_enable();
	
	set_display(0x02, symbol);
	toggle_enable();
}

void dist_to_display(int id)
{
	uint16_t  DISTANCE = get_sensor(id)->Distance;
	char* dist[1];

	itoa(DISTANCE, dist[0], 10);

	switch(id){
		case 0:
			write_string("D1: ", 0x50); //D1: 000 D2: 000
			write_string(dist[0], 0x84);
			break;
		case 1:
			write_string("D2: ", 0x88);
			write_string(dist[0], 0x92);
			break;
		case 2:
			write_string("D3: ", 0x50);
			write_string(dist[0], 0x54);
			break;
		case 3:
			write_string("D4: ", 0x59);
			write_string(dist[0], 0x5D);
			break;
		case 4:
			write_string("D5: ", 0x60);
			write_string(dist[0], 0x64);
			break;
		case 5:
			write_string("D6: ", 0x69);
			write_string(dist[0], 0x6D);
			break;
	}
	
	
}

void distance_to_display(int id)
{
	char name[] = "DIST";
	char space[] = " ";
	char col[] = ":";
	uint16_t  DISTANCE = get_sensor(id)->Distance;
	
	uint16_t FORTH = DISTANCE / 1000;
	uint16_t THIRD = DISTANCE / 100 - (DISTANCE / 1000)*10;
	uint16_t SECOND = (DISTANCE / 10) - (DISTANCE / 100)*10;
	uint16_t FIRST = (DISTANCE / 1) - (DISTANCE / 10)*10;
	
	int i;
	for (i = 0; i < 4; ++i){
		set_display(0x00, 0x90+i);
		toggle_enable();
	
		set_display(0x02, name[i]);
		toggle_enable();
		}
	
	//Ett mellanslag
	set_display(0x00, 0x94);
	toggle_enable();
	
	set_display(0x02, space[0]);
	toggle_enable();	
	
	//Id
	write_to_display(id, 0);
	
	//Ett kolon
	set_display(0x00, 0x96);
	toggle_enable();
	
	set_display(0x02, col[0]);
	toggle_enable();
	
	//Ett mellanslag
	set_display(0x00, 0x97);
	toggle_enable();
	
	set_display(0x02, space[0]);
	toggle_enable();		
		
	write_to_display(FORTH, 3);
	write_to_display(THIRD, 4);
	write_to_display(SECOND, 5);
	write_to_display(FIRST, 6);
}

uint16_t int_to_char(uint16_t digit)
{
	uint16_t data = '0' + digit;
	return data; 
}



void data_to_display(int DISTANCE)
{
	char name[] = "TEST";
	
	uint16_t FORTH = DISTANCE / 1000;
	uint16_t THIRD = DISTANCE / 100 - (DISTANCE / 1000)*10;
	uint16_t SECOND = (DISTANCE / 10) - (DISTANCE / 100)*10;
	uint16_t FIRST = (DISTANCE / 1) - (DISTANCE / 10)*10;
	
	int i;
	for (i = 0; i < 4; ++i){
		set_display(0x00, 0x80+i);
		toggle_enable();
	
		set_display(0x02, name[i]);
		toggle_enable();
		}
	
	write_to_display2(FORTH, 3);
	write_to_display2(THIRD, 4);
	write_to_display2(SECOND, 5);
	write_to_display2(FIRST, 6);
}