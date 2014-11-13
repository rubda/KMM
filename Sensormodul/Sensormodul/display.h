/*
 * display.h
 *
 * Created: 11/12/2014 11:33:47 AM
 *  Author: jakbi869
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>

#define DISP_RS PORTB0
#define DISP_RW PORTB1
#define DISP_E PORTB2
#define DISP_DB0 PORTD2
#define DISP_DB1 PORTD3
#define DISP_DB2 PORTD4
#define DISP_DB3 PORTD5
#define DISP_DB4 PORTD6
#define DISP_DB5 PORTD7
#define DISP_DB6 PORTC0
#define DISP_DB7 PORTC1


#ifndef DISPLAY_H_
#define DISPLAY_H_

void wait(int n);
void init_display();

void displayClear();




#endif /* DISPLAY_H_ */