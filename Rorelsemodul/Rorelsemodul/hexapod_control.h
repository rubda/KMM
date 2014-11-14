#include "servo_uart.h"
#include <util/delay.h>

#define BROADCAST_ID 0xFE

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
#define R_MIDDLE_SERVO_START (uint8_t[2]){0xC0, 0x02}
#define R_OUTER_SERVO_START (uint8_t[2]){0xD3, 0x00}
#define L_MIDDLE_SERVO_START (uint8_t[2]){0x3F, 0x01}
#define L_OUTER_SERVO_START (uint8_t[2]){0x2C, 0x03}
#define INNER_SERVO_START (uint8_t[2]){0xFF, 0x01}

/************************************************************************/
/* Servo lift values                                                    */
/************************************************************************/
#define L_MIDDLE_SERVO_LIFT (uint8_t[2]){0xFF, 0x01}
#define R_MIDDLE_SERVO_LIFT (uint8_t[2]){0x20, 0x03}
#define L_OUTER_SERVO_LIFT (uint8_t[2]){0xFF, 0x02}
#define R_OUTER_SERVO_LIFT (uint8_t[2]){0x00, 0x01}
	
/************************************************************************/
/* Robot leg definitions                                                */
/************************************************************************/
#define LEG1 (uint8_t[3]){1, 3, 5}
#define LEG2 (uint8_t[3]){2, 4, 6}
#define LEG3 (uint8_t[3]){13, 15, 17}
#define LEG4 (uint8_t[3]){14, 16 ,18}
#define LEG5 (uint8_t[3]){7, 9, 11}
#define LEG6 (uint8_t[3]){8, 10, 12}

#define SERVO_ACTION suart_command_action(BROADCAST_ID)

void robot_start_position();
void set_speed(uint16_t s);
void set_servo_speed(uint8_t id, uint16_t s);
void move_servo_dir(uint8_t id, uint8_t *position);
void move_servo_reg(uint8_t id, uint8_t *position);
void lift_leg(uint8_t leg_id);
void put_down_leg(uint8_t leg_id);
void stretch_leg(uint8_t leg_id);
void reset_leg(uint8_t leg_id);
void move_front_leg(uint8_t leg_id);
void move_back_leg(uint8_t leg_id);
uint16_t get_relative_speed(float multiplier);
void move_middle_leg(uint8_t leg_id);
void take_step(uint16_t length);
void robot_init();
void setup_first_step();