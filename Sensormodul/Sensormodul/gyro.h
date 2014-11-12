/*
 * gyro.h
 *
 * Created: 2014-11-05 10:54:43
 *  Author: RubenDas
 */ 

#include <avr/io.h>

#ifndef GYRO_H_
#define GYRO_H_

//Tveksamt om detta ska vara kvar
#define ACTIVATE_ADC 0b10010100
#define START_CONVERSION 0b10010100
#define POLL 0b10000000
#define DEACTIVATE_ADC 0b10010000

void activateADC();
void startConversion();
uint16_t getAngularRate();
int adcToAngularRate(uint16_t data);
void rotateTo(int angle);
int getIsRotated();



#endif /* GYRO_H_ */