#include <avr/io.h>

/*
 * gyro.h
 *
 * Created: 2014-11-05 10:54:43
 *  Author: RubenDas
 */ 

#ifndef GYRO_H_
#define GYRO_H_

//Tveksamt om detta ska vara kvar
#define ACTIVATE_ADC 0b10010100
#define START_CONVERSION 0b10010100
#define POLL 0b10000000
#define DEACTIVATE_ADC 0b10010000

int getAngularRate(uint8_t adcValue);

#endif /* GYRO_H_ */