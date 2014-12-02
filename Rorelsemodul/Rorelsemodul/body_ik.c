/*
 * body_ik.c
 *
 * Created: 2014-12-02 11:25:32
 *  Author: Jonas
 */ 


#include "body_ik.h"

uint8_t vect_speed = 5; // cm/s
double vect_direction = 0; //

void body_init(){
	initial[0].x = 10;
	initial[0].y = 20;
	initial[0].z = 0;
	
	initial[1].x = 0;
	initial[1].y = 20;
	initial[1].z = 0;
	
	initial[2].x = -10;
	initial[2].y = 20;
	initial[2].z = 0;
}

uint16_t body_time = 0;
void calculate_body_xy(){
	double change_body_x = vect_speed*cos(vect_direction);
	double change_body_y = vect_speed*sin(vect_direction);
	
	double t = body_time++;
	
	//ik(double x, double y, double z, int leg);
	int i, init;
	for(i = 1, init = 0; i <= 1; ++i){
		double mult = i % 2 == 0 ? sin(2*M_PI*t/(100000/CALC_TIMER)) : cos(2*M_PI*t/(100000/CALC_TIMER)); //FEL MED MODULO
		double change_leg_x = mult*change_body_x/2;
		double change_leg_y = mult*change_body_y/2;
		
		change_leg_y = i % 2 == 0 ? change_leg_y : -change_leg_y;
		//TODO: Calc Z
		
		ik(initial[init].x + change_leg_x, 
			initial[init].y + change_leg_y, 
			initial[init].z, i);
		
		if(i % 2 == 0){ init++; }
	}
	
	SERVO_ACTION;
}