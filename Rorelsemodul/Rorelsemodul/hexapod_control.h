#include "servo_uart.h"
#include <util/delay.h>

#define BROADCAST_ID 0xFE //Broadcast ID for servos

#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4

/************************************************************************/
/* Servo address definitions                                            */
/************************************************************************/
#define GOAL_POSITION_L 0x1E 
#define GOAL_POSITION_H 0x1F
#define MOVING_SPEED_L 0x20
#define MOVING_SPEED_H 0x21
#define PRESENT_POSITION_L 0x24
#define PRESENT_POSITION_H 0x25
#define PRESENT_SPEED_L 0x26
#define PRESENT_SPEED_H 0x27
#define PRESENT_LOAD_L 0x28
#define PRESENT_LOAD_H 0x29
#define ID_ADDR 0x03

/************************************************************************/
/* Servo starting values                                                */
/************************************************************************/


/************************************************************************/
/* Servo lift values                                                    */
/************************************************************************/
//#define L_MIDDLE_SERVO_LIFT (uint8_t[2]){0xDF, 0x00}
//#define R_MIDDLE_SERVO_LIFT (uint8_t[2]){0x20, 0x03}
//#define L_OUTER_SERVO_LIFT (uint8_t[2]){0xFF, 0x02}
//#define R_OUTER_SERVO_LIFT (uint8_t[2]){0x00, 0x01}
	
/************************************************************************/
/* Robot leg definitions                                                */
/************************************************************************/
extern uint8_t leg1[3];
extern uint8_t leg2[3];
extern uint8_t leg3[3];
extern uint8_t leg4[3];
extern uint8_t leg5[3];
extern uint8_t leg6[3];


#define SERVO_ACTION suart_command_action(BROADCAST_ID)

/* 
	Initialize UART communication with servos
	Set speed of robot
	Put robot into starting position
*/
void robot_init();

/*
	Put robot into starting position
*/
void robot_start_position();

/*
	Set speed of all servos
*/
void set_speed(uint16_t s);

/*
	Set speed of individual servo
*/
void set_servo_speed(uint8_t id, uint16_t s);

//	Move individual servo
//	Uses "write data" command
void move_servo_dir(uint8_t id, uint8_t *position);

//	Move individual servo
//	Uses "reg write" command
//	Must send "action" command after 
void move_servo_reg(uint8_t id, uint8_t *position);

//	Put leg into "lift" position
void lift_leg(uint8_t leg_id);

//	Put down leg from "lift" position
void put_down_leg(uint8_t leg_id);


//	Reset leg to defined starting position
void reset_leg(uint8_t leg_id);

//	Moves leg into walking position
//	
void setup_first_step(uint16_t length, int direction);
void take_step(uint16_t length);

void move_leg_forward(uint8_t leg_id, uint16_t length);
void move_leg_backward(uint8_t leg_id, uint16_t length);

/*void stretch_leg(uint8_t leg_id);
void move_front_leg(uint8_t leg_id, uint16_t length);
void move_back_leg(uint8_t leg_id);
uint16_t get_relative_speed(float multiplier);
void move_middle_leg(uint8_t leg_id);*/