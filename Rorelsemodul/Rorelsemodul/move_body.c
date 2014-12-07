/*
 * move_body.c
 *
 * Created: 5/12 2014 15:09:57
 *  Author: Jonas
 */ 

/*
uint16_t move_servo_degree(uint16_t servo, double deg, int time){
	uint16_t to_servo;
	
	deg = 150 + deg;
	to_servo = (uint16_t)(deg/0.29296875);
	
	if(to_servo < 0){
		to_servo = 0;
		}else if(to_servo > 1023){
		to_servo = 1023;
	}
	
	if(servo % 2 == 1){
		to_servo = 0x3ff - to_servo;
	}
	if(current_position[servo-1] != -1){
		set_servo_speed(servo, calc_servo_speed(abs(to_servo-current_position[servo-1]), time));
	}
	_delay_ms(1);
	
	move_servo_reg(servo, (uint8_t[2]){to_servo, to_servo >> 8});
	
	return to_servo;
}
*/
#include "move_body.h"

double degree_to_rad(double deg){
	return (deg/57.295779513);
}

#define LIFT_HEIGHT 2.0

double step_multi[10][3];

void set_init_xyz(int id, double x, double y, double z){
	step_multi[id][0] = x;
	step_multi[id][1] = y;
	step_multi[id][2] = z;
}
	
void body_move_init(){
	set_init_xyz(0,		 0,		 0,		0);
	set_init_xyz(1,		 0.5,	 0.5,	0);
	set_init_xyz(2,		 1,		 1,		0);
	set_init_xyz(3,		 1,		 1,		0.5);
	set_init_xyz(4,		 0.5,	 0.5,	0.8);
	set_init_xyz(5,		 0,		 0,		1);
	set_init_xyz(6,		-0.5,	-0.5,	0.8);
	set_init_xyz(7,		-1,		-1,		0.5);
	set_init_xyz(8,		-1,		-1,		0);
	set_init_xyz(9,		-0.5,	-0.5,	0);
}

uint16_t get_max_2(uint16_t a, uint16_t b){
	return b > a ? b : a;
}

int current_step = 0;
void move_body(int direction, double length){
	double move_x = -length*cos(degree_to_rad(direction))/2.0;
	double move_y = length*sin(degree_to_rad(direction))/2.0;
	
	int i,a;
	int y_multi = 1;
	uint16_t max = 0;
	for(i = 1; i <= 6; ++i){
		a = current_step%10;
		if(i == 1 || i == 4 || i == 5){
			a = (current_step + 5)%10;
		}
		y_multi = (i % 2 == 0 ? 1 : -1);
		max = get_max_2(ik(step_multi[a][0]*move_x, y_multi*step_multi[a][1]*move_y, step_multi[a][2]*LIFT_HEIGHT, i), max);
	}
	
	//CALCULATE SPEED
	//CALCULATE WAIT
	
	current_step++;
	SERVO_ACTION;
	robot_delay2(max);
}

void rotate_body(int direction, double length){
	double move_x = -length*cos(degree_to_rad(direction))/2.0;
	double move_y = length*sin(degree_to_rad(direction))/2.0;
	
	double move_x1 = -length*cos(degree_to_rad(180 - direction))/2.0;
	double move_y1 = length*sin(degree_to_rad(180 - direction))/2.0;
	
	int i,a;
	int y_multi = 1;
	uint16_t max = 0;
	for(i = 1; i <= 6; ++i){
		a = current_step%10;
		if(i == 1 || i == 4 || i == 5){
			a = (current_step + 5)%10;
		}
		y_multi = (i % 2 == 0 ? 1 : -1);
		
		if(i % 2 == 1){
			max = get_max_2(ik(step_multi[a][0]*move_x, y_multi*step_multi[a][1]*move_y, step_multi[a][2]*LIFT_HEIGHT, i), max);
		}else if(i % 2 == 0){
			max = get_max_2(ik(step_multi[a][0]*move_x1, y_multi*step_multi[a][1]*move_y1, step_multi[a][2]*LIFT_HEIGHT, i), max);
		}
	}
	
	current_step++;
	SERVO_ACTION;
	robot_delay2(max);
}