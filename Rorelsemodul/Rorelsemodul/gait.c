/*
 * gait.c
 *
 * Created: 7/12 2014 20:45:51
 *  Author: Jonas
 */ 

#include "gait.h"

Gait gait;

double degree_to_rad_2(double deg){
	return (deg/57.295779513);
}

uint16_t get_max_3(uint16_t a, uint16_t b){
	return b > a ? b : a;
}

void set_gait(uint8_t g){
	switch (g)
	{
		case GAIT_RIPPLE:
			set_ripple_gait();
		break;
		case GAIT_WAVE:
			set_wave_gait();
		break;
		case GAIT_TRIPOD:
		default:
			set_tripod_gait();
		break;
	}
}

void add_gait_step(uint8_t s, double x, double z, double speed){
	gait.steps[s].x = x;
	gait.steps[s].y = x;
	gait.steps[s].z = z;
	gait.steps[s].speed = speed;
}

void add_gait_simple_step(uint8_t s, double x, double z){
	add_gait_step(s, x, z, 1);
}

void set_leg_offset(uint8_t id, uint8_t offset){
	gait.offset[id-1] = offset;
}

void set_tripod_gait(){
	add_gait_simple_step(0,		 0,		 0);
	add_gait_simple_step(1,		 0.5,	 0);
	add_gait_simple_step(2,		 1,		 0);
	add_gait_simple_step(3,		 1,		 0.5);
	add_gait_simple_step(4,		 0.5,	 0.8);
	add_gait_simple_step(5,		 0,		 1);
	add_gait_simple_step(6,		-0.5,	 0.8);
	add_gait_simple_step(7,		-1,		 0.5);
	add_gait_simple_step(8,		-1,		 0);
	add_gait_simple_step(9,		-0.5,	 0);
	
	gait.length = 10;
	
	set_leg_offset(1, 0);
	set_leg_offset(2, 5);
	set_leg_offset(3, 5);
	set_leg_offset(4, 0);
	set_leg_offset(5, 0);
	set_leg_offset(6, 5);
}

void set_ripple_gait(){
	add_gait_simple_step(0, 0, 0);
	add_gait_simple_step(1, 0.25, 0);
	add_gait_simple_step(2, 0.5, 0);
	add_gait_simple_step(3, 0.75, 0);
	add_gait_simple_step(4, 1, 0);
	add_gait_simple_step(5, 1, 0.5);
	add_gait_simple_step(6, 0.75, 0.8);
	add_gait_simple_step(7, 0.5, 1);
	add_gait_simple_step(8, 0.25, 1);
	add_gait_simple_step(9, 0, 1);
	add_gait_simple_step(10, -0.25, 1);
	add_gait_simple_step(11, -0.5, 1);
	add_gait_simple_step(12, -0.75, 0.8);
	add_gait_simple_step(13, -1, 0.5);
	add_gait_simple_step(14, -1, 0);
	add_gait_simple_step(15, -0.75, 0);
	add_gait_simple_step(16, -0.5, 0);
	add_gait_simple_step(17, -0.25, 0);
	
	gait.length = 18;
	
	set_leg_offset(1, 0);
	set_leg_offset(2, 9);
	set_leg_offset(3, 12);
	set_leg_offset(4, 3);
	set_leg_offset(5, 6);
	set_leg_offset(6, 15);
}

void set_wave_gait(){
	add_gait_step(0,		 0,			0,		1.0/6.0);
	add_gait_step(1,		 1.0/7,		0,		1.0/6.0);
	add_gait_step(2,		 2.0/7,		0,		1.0/6.0);
	add_gait_step(3,		 3.0/7,		0,		1.0/6.0);
	add_gait_step(4,		 4.0/7,		0,		1.0/6.0);
	add_gait_step(5,		 5.0/7,		0,		1.0/6.0);
	add_gait_step(6,		 6.0/7,		0,		1.0/6.0);
	add_gait_step(7,		 1,			0,		1.0/6.0);
	add_gait_step(8,		 3.5/7,		0.8,	1);
	add_gait_step(9,		 0,			1,		1);
	add_gait_step(10,		-3.5/7,		0.8,	1);
	add_gait_step(11,		-1,			0,		1.0/6.0);
	add_gait_step(12,		-6.0/7,		0,		1.0/6.0);
	add_gait_step(13,		-5.0/7,		0,		1.0/6.0);
	add_gait_step(14,		-4.0/7,		0,		1.0/6.0);
	add_gait_step(15,		-3.0/7,		0,		1.0/6.0);
	add_gait_step(16,		-2.0/7,		0,		1.0/6.0);
	add_gait_step(17,		-1.0/7,		0,		1.0/6.0);
	
	gait.length = 18;
	
	set_leg_offset(1, 0);
	set_leg_offset(2, 3);
	set_leg_offset(3, 6);
	set_leg_offset(4, 9);
	set_leg_offset(5, 12);
	set_leg_offset(6, 15);
}

int gait_step = 0;
uint8_t servo_done[6];

void gait_move(int direction, double length){
	double move_x = -length*cos(degree_to_rad_2(direction))/2.0;
	double move_y = length*sin(degree_to_rad_2(direction))/2.0;
	
	int i,a;
	int y_multi = 1;
	uint16_t max = 0;
	for(i = 1; i <= 6; ++i){
		servo_done[i] = 0; //Not done
		
		a = (gait_step+gait.offset[i-1])%gait.length;
		y_multi = (i % 2 == 0 ? 1 : -1);
		set_servo_speed(i, get_relative_speed(gait.steps[a].speed));
		max = get_max_3(ik(gait.steps[a].x*move_x, y_multi*gait.steps[a].y*move_y, gait.steps[a].z*LIFT_HEIGHT, i), max);
	}
	
	gait_step++;
	SERVO_ACTION;
	robot_delay_ms(max);
}

void gait_stop(int direction, double length){
	double move_x = -length*cos(degree_to_rad_2(direction))/2.0;
	double move_y = length*sin(degree_to_rad_2(direction))/2.0;
	
	int i,a;
	int y_multi = 1;
	uint16_t max = 0;
	for(i = 1; i <= 6; ++i){
		if(servo_done[i] > 0) continue;
		
		a = (gait_step+gait.offset[i-1])%gait.length;
		
		if(a == 0){
			servo_done[i] = 1;
		}
		
		y_multi = (i % 2 == 0 ? 1 : -1);
		set_servo_speed(i, get_relative_speed(gait.steps[a].speed));
		max = get_max_3(ik(gait.steps[a].x*move_x, y_multi*gait.steps[a].y*move_y, gait.steps[a].z*LIFT_HEIGHT, i), max);
	}
	
	gait_step++;
	SERVO_ACTION;
	robot_delay_ms(max);
}
