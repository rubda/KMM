/*
 * ultraljud.h
 *
 * Created: 2014-11-05 19:58:40
 *  Author: RubenDas
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#ifndef ULTRALJUD_H_
#define ULTRALJUD_H_

#include "UART.h"
#include "messages.h"

struct soundSensor
{
	uint8_t id;
	uint8_t Distance;
};

struct soundSensor sensor_list[6];

void init_sensors();
struct soundSensor* get_sensor(uint8_t id);
void get_distance(struct soundSensor*);



	



#endif /* ULTRALJUD_H_ */