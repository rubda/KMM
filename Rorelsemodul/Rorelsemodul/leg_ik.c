/*
 * leg_ik.c
 *
 * Created: 2014-12-02 16:08:15
 *  Author: Jonas
 */ 

#include "leg_ik.h"

#define INNER_LENGTH 5.5
#define MIDDLE_LENGTH 6.0
#define OUTER_LENGTH 13.5

int rad_to_degree(double rad){
	return (int)(rad*57.295779513 + 0.5);
}

int calc_angle1(int x, int y, int z){
	double G = sqrt(pow(x, 2) + pow(z, 2));
	double H = sqrt(pow(y, 2) + pow(G, 2));
	double b = acos((pow(MIDDLE_LENGTH, 2) + pow(H, 2) - pow(OUTER_LENGTH, 2))/(2*MIDDLE_LENGTH*H));
	
	return rad_to_degree((atan(G/y))+b-90);
}

int calc_angle2(int x, int y, int z){
	double G = sqrt(pow(x, 2) + pow(z, 2));
	double H = sqrt(pow(y, 2) + pow(G, 2));
	double b = acos((pow(MIDDLE_LENGTH, 2) + pow(H, 2) - pow(OUTER_LENGTH, 2))/(2*MIDDLE_LENGTH*H));
	double h2 = asin(H*(sin(b))/OUTER_LENGTH);
	
	return rad_to_degree(h2) - 180;
}

int calc_angle3(int x, int y, int z){
	return rad_to_degree(atan2(x, z));
}

uint16_t move_servo_degree(uint8_t servo, int deg){
	uint16_t to_servo;
	
	deg = 150 + deg;
	to_servo = (uint16_t)(deg/0.29296875);
	
	if(to_servo < 0){
		to_servo = 0;
	}else if(to_servo > 1023){
		to_servo = 1023;
	}
	
	if(servo % 2 == 0){
		to_servo = 0x3ff - to_servo;
	}
	
	move_servo_reg(servo, (uint8_t[2]){to_servo, to_servo >> 8});
	
	return to_servo;
}

void move_leg_ik(int x, int y, int z){
	int angle1 = calc_angle1(x, y, z);
	int angle2 = calc_angle2(x, y, z);
	int angle3 = calc_angle3(x, y, z);
}