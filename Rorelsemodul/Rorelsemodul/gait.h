/*
 * gait.h
 *
 * Created: 7/12 2014 20:45:39
 *  Author: Jonas
 */ 

//    _____
// 1--| ^ |--2
// 3--| | |--4
// 5--|___|--6
//


#ifndef GAIT_H_
#define GAIT_H_

#include <avr/io.h>
#include "hexapod_control.h"

#define GAIT_TRIPOD 1
#define GAIT_RIPPLE 2
#define GAIT_WAVE 3

#define LIFT_HEIGHT 2.0

struct Coordinate {
	double x;
	double y;
	double z;
	double speed;
};

typedef struct Coordinate Coordinate;

struct Gait {
	 Coordinate steps[32];
	 uint8_t length;
	 uint8_t offset[6];
};

typedef struct Gait Gait;

void set_gait(uint8_t g);
void set_tripod_gait();
void set_ripple_gait();
void set_wave_gait();

#endif /* GAIT_H_ */