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

void activate_adc();
void start_conversion();
uint16_t get_angular_rate();
int adc_to_angular_rate(uint16_t data);
int rotate_to(int angle);
int get_is_rotated();
void deactivate_adc();
void has_rotated(int bit);


#endif /* GYRO_H_ */