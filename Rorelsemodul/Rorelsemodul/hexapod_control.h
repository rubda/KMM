#include "servo_uart.h"
#include <util/delay.h>
#include <math.h>

#define BROADCAST_ID 0xFE //Broadcast ID for servos

/************************************************************************/
/* Robot direction constants                                             */
/************************************************************************/
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
/* Servo lift values                                                    */
/************************************************************************/
#define L_MIDDLE_SERVO_LIFT (uint8_t[2]){0x00, 0x01}
#define R_MIDDLE_SERVO_LIFT (uint8_t[2]){0x20, 0x03}
#define L_OUTER_SERVO_LIFT (uint8_t[2]){0xFF, 0x02}
#define R_OUTER_SERVO_LIFT (uint8_t[2]){0x00, 0x01}
	
/************************************************************************/
/* Robot leg definitions                                                */
/************************************************************************/
extern uint8_t leg1[3];
extern uint8_t leg2[3];
extern uint8_t leg3[3];
extern uint8_t leg4[3];
extern uint8_t leg5[3];
extern uint8_t leg6[3];

// Broadcast action command to servos
#define SERVO_ACTION suart_command_action(BROADCAST_ID)

//Initialize UART communication with servos
//Set speed of robot
//Put robot into starting position
void robot_init(uint16_t speed);

//Put robot into starting position
void robot_start_position();

void robot_delay_ms(uint16_t ms);
void robot_delay2(uint16_t length);
uint16_t calc_servo_delay(uint8_t id, uint16_t length);

//Set speed of all servos
void set_speed(uint16_t s);

//Set speed of individual servo
void set_servo_speed(uint8_t id, uint16_t s);

//	Move individual servo
//	Uses "write data" command
uint16_t move_servo_dir(uint8_t id, uint8_t *position);

//	Move individual servo
//	Uses "reg write" command
//	Must send "action" command after 
uint16_t move_servo_reg(uint8_t id, uint8_t *position);

//	Put leg into "lift" position
void lift_leg(uint8_t leg_id);

//	Put down leg from "lift" position
void put_down_leg(uint8_t leg_id);

//	Reset leg to defined starting position
void reset_leg(uint8_t leg_id);

//	Moves leg into walking position
void setup_first_step(uint16_t length_l, uint16_t length_r, int direction);

//	Take a step
//	Set direction to FORWARD or BACKWARD
void take_step(uint16_t length_l, uint16_t length_r, int direction);

//	Rotate one step
//	Set direction to RIGHT or LEFT
void rotate(uint16_t length, uint16_t bla, int direction);

//	Turn while walking forward
//	Set direction to RIGHT or LEFT
void turn(uint16_t length_r, uint16_t length_l, int direction);

//	Move leg to "forward" position
void move_leg_forward(uint8_t leg_id, uint16_t length);

//	Move leg to "backward" position
void move_leg_backward(uint8_t leg_id, uint16_t length);

//	Inverse kinematics for leg
uint16_t ik(double x, double y, double z, int leg);