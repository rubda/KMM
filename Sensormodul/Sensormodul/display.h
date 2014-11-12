/*
 * display.h
 *
 * Created: 11/12/2014 11:33:47 AM
 *  Author: jakbi869
 */ 


#include <avr/io.h>

#ifndef DISPLAY_H_
#define DISPLAY_H_

void init_displayOutput();

void functionSet();
void displayOnOffControl();
void displayClear();
void entryModeSet();



#endif /* DISPLAY_H_ */