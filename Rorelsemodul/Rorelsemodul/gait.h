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
#define GAIT_TRIPLE 4

#define LIFT_HEIGHT 3.0

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

//Set type of gait
void set_gait(uint8_t g);
void set_tripod_gait();
void set_triple_gait();
void set_ripple_gait(); //Not real ripple gait, just more stable tripod gait
void set_wave_gait();


//Move robot in @direction(-360,360) with each step @length cm long
void gait_move(int direction, double length);

//Moves all legs to init position.
void gait_stop(int direction, double length);

//Same as gait_move but with rotation(-180,180)
void gait_rotate(int direction, int rotation, double length);

#endif /* GAIT_H_ */