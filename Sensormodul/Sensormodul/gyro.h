/*
 * gyro.h
 *
 * Created: 2014-11-05 10:54:43
 *  Author: RubenDas
 */ 

#include <avr/io.h>

#ifndef GYRO_H_
#define GYRO_H_

#define ACTIVATE_ADC 0b10010100
#define START_CONVERSION 0b10010100
#define POLL 0b10000000
#define DEACTIVATE_ADC 0b10010000

void activate_adc();
void start_conversion();
uint16_t get_angular_rate();
double adc_to_angular_rate(uint16_t data);
double rotate_to(int angle);
void deactivate_adc();


#endif /* GYRO_H_ */