
#ifndef MOVE_BODY_H_
#define MOVE_BODY_H_
#include "hexapod_control.h"

void body_move_init();
void move_body(int direction, double length);
void rotate_body(int direction, double length);
uint8_t init_body(int direction, double length);
void turn_body(int direction, double length);

#endif /* MOVE_BODY_H_ */