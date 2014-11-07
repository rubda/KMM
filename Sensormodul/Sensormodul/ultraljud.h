/*
 * ultraljud.h
 *
 * Created: 2014-11-05 19:58:40
 *  Author: RubenDas
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

#ifndef ULTRALJUD_H_
#define ULTRALJUD_H_

struct soundSensor
{
	uint8_t id;
	uint8_t Distance;
};

struct soundSensor sensor_list[6];

void init_sensors();
void get_data(struct soundSensor);
struct soundSensor get_sensor(uint8_t id);
uint8_t get_distance(struct soundSensor);



	



#endif /* ULTRALJUD_H_ */