/*
 * body_ik.c
 *
 * Created: 2014-12-02 11:25:32
 *  Author: Jonas
 */ 


#include "body_ik.h"

uint8_t vect_speed = 4; // cm/s
double vect_direction = M_PI/4; //

int body_time = 0;
int step_multi[] = {0, 1, 0, -1};
	
void calculate_body_xy(){
	double change_body_x = vect_speed*cos(vect_direction);
	double change_body_y = vect_speed*sin(vect_direction);
	
	int t = body_time++ % 4;
	
	//ik(double x, double y, double z, int leg);
	int i;
	for(i = 2; i <= 2; ++i){
		double mult = step_multi[t%4];
		
		double change_leg_z = (i == 1 || i == 4 || i == 5) == 0 ? mult*change_body_x/2 : -mult*change_body_x/2;
		double change_leg_x = mult*change_body_y/2;
		
		//double change_leg_y = i % 2 == 0 ? change_leg_y : -change_leg_y;
		//TODO: Calc Z
		double change_leg_y = 0; 
		
		move_leg_ik(i, 4 + change_leg_x, 11 - change_leg_y, change_leg_z, CALC_TIMER);
	}
}