/*
 * display.c
 *
 * Created: 11/12/2014 11:33:04 AM
 *  Author: jakbi869
 */ 
#include "display.h"
#include <util/delay.h>


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
    
    // Detta borde funka
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
	
	BOSSE();

	write_params();
	
}

//Clear display
void clear_display()
{
	set_display(0x00, 0x01);
	toggle_enable();
}

//Sätter enable hög för en stund
void toggle_enable()
{
	PORTB |= (1 << DISP_E);
	_delay_us (250);
	PORTB &= ~(1 << DISP_E);
	_delay_us(250);
}

//Sätter displayen till ett visst läge
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
	
	_delay_us(250);
	
	PORTC |= (((instr >> 7) & 1) << DISP_DB7);
	PORTC |= (((instr >> 6) & 1) << DISP_DB6);
	PORTD |= (((instr >> 5) & 1) << DISP_DB5);
	PORTD |= (((instr >> 4) & 1) << DISP_DB4);
	PORTD |= (((instr >> 3) & 1) << DISP_DB3);
	PORTD |= (((instr >> 2) & 1) << DISP_DB2);
	PORTD |= (((instr >> 1) & 1) << DISP_DB1);
	PORTD |= (((instr >> 0) & 1) << DISP_DB0);
	
	_delay_us(250);

}

//Skriver ut BOSSE på displayen
void BOSSE()
{
	write_string("BOSSE", 0x85);	
}

//Skriver ut en sträng på displayen med önskad position
void write_string(char* str, int pos){
	int i;
	for (i = 0; str[i] != '\0' && i <= 32; ++i){
		set_display(0x00, pos+i);
		toggle_enable();
	
		set_display(0x02, str[i]);
		toggle_enable();
	}	
}
//Skriver ut helttal till displayen	
void write_to_display(uint16_t value, int pos)
{
	uint16_t symbol = int_to_char(value);
	
	set_display(0x00, pos);
	toggle_enable();
	
	set_display(0x02, symbol);
	toggle_enable();
}


//Skriver ut parametrar på display 
void write_params()
{
	write_string("D1: ", 0xC0); //D1: 000
	write_string("D2: ", 0xC9);
	write_string("D3: ", 0x90);
	write_string("D4: ", 0x99);
	write_string("D5: ", 0xD0);
	write_string("D6: ", 0xD9);
}

//Uppdaterar distanser på displayer
void dist_to_display(int id)
{
	switch(id){
		case 0:
			distance_to_display(id,0xC4);
			break;
		case 1:
			distance_to_display(id,0xCD);
			break;
		case 2:
			distance_to_display(id,0x94);
			break;
		case 3:
			distance_to_display(id,0x9D);
			break;
		case 4:
			distance_to_display(id,0xD4);
			break;
		case 5:
			distance_to_display(id,0xDD);
			break;
	}
	
}

//Bryter upp ett värde (går att använda inbyggda funktioner)
void distance_to_display(int id, int pos)
{
	uint16_t  DISTANCE = get_sensor(id)->medDist;
	
	uint16_t THIRD = DISTANCE / 100;
	uint16_t SECOND = (DISTANCE / 10) - (DISTANCE / 100)*10;
	uint16_t FIRST = (DISTANCE / 1) - (DISTANCE / 10)*10;

	write_to_display(THIRD, pos);
	write_to_display(SECOND,pos+1);
	write_to_display(FIRST, pos+2);
}

uint16_t int_to_char(uint16_t digit)
{
	uint16_t data = '0' + digit;
	return data; 
}

