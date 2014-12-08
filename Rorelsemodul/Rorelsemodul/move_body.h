

/*
 * move_body.h
 *
 * Created: 5/12 2014 16:02:26
 *  Author: Jonas
 */ 


#ifndef MOVE_BODY_H_
#define MOVE_BODY_H_
#include "hexapod_control.h"

void body_move_init();
void move_body(int direction, double length);
void rotate_body(int direction, double length);

extern int current_step;


#endif /* MOVE_BODY_H_ */