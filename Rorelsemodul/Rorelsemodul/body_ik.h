/**
 * body_ik.h
 *
 * Created: 2014-12-02 11:25:16
 *  Author: Jonas
 */ 


#ifndef BODY_IK_H_
#define BODY_IK_H_


#define CALC_TIMER 100 //100ms (0.1s) to start with

#include <math.h>
#include "hexapod_control.h"


struct Coordinate{
	uint8_t x;
	uint8_t y;
	uint8_t z;	
};

struct Coordinate initial[3];

void body_init();
void calculate_body_xy();


#endif /* BODY_IK_H_ */