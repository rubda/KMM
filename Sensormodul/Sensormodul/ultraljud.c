/*
 * ultraljud.c
 *
 * Created: 2014-11-05 19:58:28
 *  Author: RubenDas
 */ 

#include "ultraljud.h"
#include <avr/io.h>

void initSensors()
{
	struct soundSensor sensor0 = {0b11100011, 0};
	struct soundSensor sensor1 = {0b11100111, 0};
	struct soundSensor sensor2 = {0b11101011, 0};
	struct soundSensor sensor3 = {0b11101111, 0};
	struct soundSensor sensor4 = {0b11110011, 0};
	struct soundSensor sensor5 = {0b11110111, 0};
}

void getData(struct soundSensor sensor) 
{
	sensor.Distance = getDistance(sensor);
}

uint8_t getDistance(struct soundSensor sensor)
{
	uint8_t SPEED_OF_SOUND;
	uint8_t DISTANCE;
	uint8_t TIME;
	
	PORTA = sensor.id & PORTA;
	
	//Sätt räknaren till noll
	while (!(PORTA & (1 << PORTA1)))
		{
			//Räkna upp	
		}
		
	DISTANCE = SPEED_OF_SOUND*TIME;
	return DISTANCE;
		
}