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
	uint8_t id; //Det id som krävs för att aktivera ett visst ultraljud
	uint8_t nr; //Ett nr för att iterera mellan dist1 till dist5
	uint16_t medDist; //Medianen av fem andra distanser som syns nedan

	uint16_t Dist1;
	uint16_t Dist2;
	uint16_t Dist3;
	uint16_t Dist4;
	uint16_t Dist5;
};

struct soundSensor sensor_list[6];

void init_sensors();
struct soundSensor* get_sensor(uint8_t id);
void get_distance(struct soundSensor*);
uint16_t cmpfunc (const void * a, const void * b);



	



#endif /* ULTRALJUD_H_ */