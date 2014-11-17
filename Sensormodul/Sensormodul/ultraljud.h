/*
 * ultraljud.h
 *
 * Created: 2014-11-05 19:58:40
 *  Author: RubenDas
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#ifndef ULTRALJUD_H_
#define ULTRALJUD_H_

struct soundSensor
{
	uint8_t id;
	uint16_t Distance;
};

struct soundSensor sensor_list[6];

void init_sensors();
struct soundSensor get_sensor(uint8_t id);
uint16_t get_distance(struct soundSensor);
uint16_t get_data(uint8_t id);



	



#endif /* ULTRALJUD_H_ */