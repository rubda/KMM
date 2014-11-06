/*
 * ultraljud.h
 *
 * Created: 2014-11-05 19:58:40
 *  Author: RubenDas
 */ 


#include <avr/io.h>

#ifndef ULTRALJUD_H_
#define ULTRALJUD_H_

struct soundSensor
{
	uint8_t id;
	uint8_t Distance;
};

void initSensors();
void getData(struct soundSensor);
uint8_t getDistance(struct soundSensor);



	



#endif /* ULTRALJUD_H_ */