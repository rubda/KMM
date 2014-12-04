#include "hexapod_control.h"

uint8_t speed[2] = {0xFF, 0x00};
uint8_t *leg_list[6];

uint8_t leg1[] = {1, 3, 5};
uint8_t leg2[] = {2, 4, 6};
uint8_t leg3[] = {13, 15, 17};
uint8_t leg4[] = {14, 16 ,18};
uint8_t leg5[] = {7, 9, 11};
uint8_t leg6[] = {8, 10, 12};
	
uint16_t current_position[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1};
	
uint8_t R_MIDDLE_SERVO_START[] = {0x60, 0x02};	
uint8_t R_OUTER_SERVO_START[] = {0x00, 0x01};
uint8_t L_MIDDLE_SERVO_START[] = {0x9F, 0x01};
uint8_t L_OUTER_SERVO_START[] = {0xFF, 0x02};
uint8_t INNER_SERVO_START_R[] = {0x00, 0x02};
uint8_t INNER_SERVO_START_L[] = {0x00, 0x02};
uint8_t INNER_SERVO_START_M[] = {0xFF, 0x01};
uint8_t INNER_SERVO_START[] = {0x00, 0x02};

//Init robot and UART communication
void robot_init(uint16_t speed){
	leg_list[0] = leg1;
	leg_list[1] = leg2;
	leg_list[2] = leg3;
	leg_list[3] = leg4;
	leg_list[4] = leg5;
	leg_list[5] = leg6;
	
	suart_init(1000000);
	_delay_ms(10);
	set_speed(speed);
}

//Get inner servo of leg
uint8_t inner_servo(uint8_t leg_id){
	return leg_list[leg_id - 1][0];
}

//Get middle servo of leg
uint8_t middle_servo(uint8_t leg_id){
	return leg_list[leg_id - 1][1];
}

//Get outer servo of leg
uint8_t outer_servo(uint8_t leg_id){
	return leg_list[leg_id - 1][2];
}

//Get relative speed from current robot speed
uint16_t get_relative_speed(float multiplier){
	return (uint16_t)speed[0]*multiplier + ((uint16_t)(speed[1]*multiplier) << 8);
}

//Set speed for all servos
void set_speed(uint16_t s){
	speed[0] = s;
	speed[1] = s >> 8;
	
	uint8_t torque_enable[] = {0x01};
	uint8_t slope[] = {0xFF};
	suart_command_write_data(BROADCAST_ID, MOVING_SPEED_L, speed, 2);
	_delay_us(300);
	suart_command_write_data(BROADCAST_ID, 0x18, torque_enable, 1);
	_delay_us(300);
	suart_command_write_data(BROADCAST_ID, 0x1C, slope, 1);
	_delay_us(300);
	suart_command_write_data(BROADCAST_ID, 0x1D, slope, 1);
}

//Set speed of individual servo
void set_servo_speed(uint8_t id, uint16_t s){
	uint8_t local_speed[] = {s, s >> 8};
	uint8_t torque_enable[] = {0x01};
	suart_command_write_data(id, MOVING_SPEED_L, local_speed, 2);
	suart_command_write_data(id, 0x18, torque_enable, 1);
}

//Put robot into starting position
void robot_start_position(){
	int i;
	_delay_us(300);
	
	for(i = 1; i < 7; ++i){
		if(i == 2 || i == 5)
			move_servo_reg(inner_servo(i), INNER_SERVO_START_R);
		else if(i == 1 || i == 6)
			move_servo_reg(inner_servo(i), INNER_SERVO_START_L);
		else
			move_servo_reg(inner_servo(i), INNER_SERVO_START_M);
		_delay_ms(1);
	}
	
	SERVO_ACTION;
	_delay_ms(800);
	
	for(i = 1; i < 7; ++i){
		if(i % 2 == 1){
			move_servo_reg(middle_servo(i), L_MIDDLE_SERVO_START);
		}else{
			move_servo_reg(middle_servo(i), R_MIDDLE_SERVO_START);
		}
		
		_delay_ms(1);
	}
	
	SERVO_ACTION;
	_delay_ms(800);
	
	for(i = 1; i < 7; ++i){
		if(i % 2 == 1){
			move_servo_reg(outer_servo(i), L_OUTER_SERVO_START);
		}else{
			move_servo_reg(outer_servo(i), R_OUTER_SERVO_START);
		}
		
		_delay_ms(1);
	}
	
	SERVO_ACTION;
}

//Move servo using reg write command. Need to run SERVO_ACTION to perform action
void move_servo_reg(uint8_t id, uint8_t *position){
	suart_command_reg_write(id, GOAL_POSITION_L, position, 2);
	current_position[id-1] = position[0] + (position[1] << 8);
}

//Move servo using write data command. Moves servo directly
void move_servo_dir(uint8_t id, uint8_t *position){
	suart_command_write_data(id, GOAL_POSITION_L, position, 2);
	current_position[id-1] = position[0] + (position[1] << 8);
}

//Lifts leg to "lift" position. Moves middle and outer servo
void lift_leg(uint8_t leg_id){
	if(leg_id%2 == 1){
		move_servo_reg(middle_servo(leg_id), L_MIDDLE_SERVO_LIFT);
	}else{
		move_servo_reg(middle_servo(leg_id), R_MIDDLE_SERVO_LIFT);
	}
}

//Put down leg. Moves middle and outer servo
void put_down_leg(uint8_t leg_id){
	if(leg_id%2 == 1){
		move_servo_reg(middle_servo(leg_id), L_MIDDLE_SERVO_START);
		move_servo_reg(outer_servo(leg_id), L_OUTER_SERVO_START);
	}else{
		move_servo_reg(middle_servo(leg_id), R_MIDDLE_SERVO_START);
		move_servo_reg(outer_servo(leg_id), R_OUTER_SERVO_START);
	}
}

//Reset individual leg to starting position
void reset_leg(uint8_t leg_id){
	move_servo_dir(inner_servo(leg_id), INNER_SERVO_START);
	_delay_ms(800);
	
	if(leg_id % 2 == 1){
		move_servo_dir(middle_servo(leg_id), L_MIDDLE_SERVO_START);
		_delay_ms(800);
		move_servo_dir(outer_servo(leg_id), L_OUTER_SERVO_START);
	}else{
		move_servo_dir(middle_servo(leg_id), R_MIDDLE_SERVO_START);
		_delay_ms(800);
		move_servo_dir(outer_servo(leg_id), R_OUTER_SERVO_START);
	}
}

// TODO: Kolla plus och minus för benen
void move_leg_forward(uint8_t leg_id, uint16_t length){
	uint8_t start_pos[] = {0xFF, 0x01};
	uint16_t ml = 0x01FF - (length / 2);
	uint16_t mr = 0x01FF + (length / 2);	
	uint16_t bl = 0x01BF - length;
	uint16_t br = 0x024F + length;	
	
	uint8_t middle_l[] = {ml, ml >> 8};
	uint8_t back_l[] = {bl, bl >> 8};
	uint8_t middle_r[] = {mr, mr >> 8};
	uint8_t back_r[] = {br, br >> 8};
	
	if(leg_id == 1){
		move_servo_reg(inner_servo(leg_id), INNER_SERVO_START_L);
	}else if(leg_id == 2){
		move_servo_reg(inner_servo(leg_id), INNER_SERVO_START_R);
	}else if(leg_id == 3){
		move_servo_reg(inner_servo(leg_id), middle_l);
	}else if(leg_id == 4){
		move_servo_reg(inner_servo(leg_id), middle_r);
	}else if(leg_id == 5){
		move_servo_reg(inner_servo(leg_id), back_l);
	}else{
		move_servo_reg(inner_servo(leg_id), back_r);
	}
}

//TODO: Kolla plus och minus för benen
void move_leg_backward(uint8_t leg_id, uint16_t length){
	uint8_t start_pos[] = {0xFF, 0x01};
	uint16_t mr = 0x01FF - (length / 2);
	uint16_t ml = 0x01FF + (length / 2);
	uint16_t fl = 0x024F + length;
	uint16_t fr = 0x01BF - length;
	
	uint8_t middle_r[] = {mr, mr >> 8};
	uint8_t front_r[] = {fr, fr >> 8};
	uint8_t middle_l[] = {ml, ml >> 8};
	uint8_t front_l[] = {fl, fl >> 8};
	
	if(leg_id == 1){
		move_servo_reg(inner_servo(leg_id), front_l);
	}else if(leg_id == 2){
		move_servo_reg(inner_servo(leg_id), front_r);
	}else if(leg_id == 3){
		move_servo_reg(inner_servo(leg_id), middle_l);
	}else if(leg_id == 4){
		move_servo_reg(inner_servo(leg_id), middle_r);
	}else if(leg_id == 5){
		move_servo_reg(inner_servo(leg_id), INNER_SERVO_START_R);
	}else if(leg_id == 6){
		move_servo_reg(inner_servo(leg_id), INNER_SERVO_START_L);
	}
}

//Moves leg 1, 4, 5 into walking position. Use only if robot is in starting position
void setup_first_step(uint16_t length_l, uint16_t   length_r, int direction){
	lift_leg(1);
	lift_leg(4);
	lift_leg(5);
	SERVO_ACTION;
	_delay_ms(200);
	
	if(direction == FORWARD){
		move_leg_forward(1, length_l);
		move_leg_forward(4, length_r);
		move_leg_forward(5, length_l);
	/*}else if(direction == BACKWARD){
		move_leg_backward(1, length);
		move_leg_backward(4, length);
		move_leg_backward(5, length);
	}else if(direction == RIGHT){
		move_leg_forward(1, length);
		move_leg_backward(4, length);
		move_leg_forward(5, length);
	}else if(direction == LEFT){
		move_leg_backward(1, length);
		move_leg_forward(4, length);
		move_leg_backward(5, length);
	}*/
	}
	SERVO_ACTION;
	_delay_ms(200);
	
	put_down_leg(1);
	put_down_leg(4);
	put_down_leg(5);
	SERVO_ACTION;
	_delay_ms(200);
}

void robot_delay_ms(uint16_t ms){
	while(ms-- > 0){
		_delay_ms(1);
	}
}

void robot_delay(uint16_t length_r, uint16_t length_l){
	double length = length_r > length_l ? length_r : length_l;
	double speed_d = speed[0] + (speed[1] << 8);
	
	length = (double)(length*0.29 + 0.5);
	speed_d = (double)(speed_d*0.666 + 0.5);
	
	robot_delay_ms((uint16_t)((length*1000)/speed_d));
}

void take_step_1(uint16_t length_r, uint16_t length_l, int direction){
	lift_leg(2);
	lift_leg(3);
	lift_leg(6);
	SERVO_ACTION;
	robot_delay(0x003F, 0x003F);
	
	if(direction == FORWARD){
		move_leg_backward(1, length_l);
		move_leg_backward(4, length_r);
		move_leg_backward(5, length_l);
		move_leg_forward(2, length_r);
		move_leg_forward(3, length_l);
		move_leg_forward(6, length_r);
	}else if(direction == BACKWARD){
		move_leg_forward(1, length_l);
		move_leg_forward(4, length_r);
		move_leg_forward(5, length_l);
		move_leg_backward(2, length_r);
		move_leg_backward(3, length_l);
		move_leg_backward(6, length_r);
	}
	
	SERVO_ACTION;
	robot_delay(length_r, length_l);
	
	put_down_leg(2);
	put_down_leg(3);
	put_down_leg(6);
	SERVO_ACTION;
	robot_delay(0x003F, 0x003F);
}

void take_step_2(uint16_t length_r, uint16_t length_l, int direction){
	lift_leg(1);
	lift_leg(4);
	lift_leg(5);
	SERVO_ACTION;
	robot_delay(0x003F, 0x003F);
	
	if(direction == FORWARD){
		move_leg_backward(2, length_r);
		move_leg_backward(3, length_l);
		move_leg_backward(6, length_r);
		move_leg_forward(1, length_l);
		move_leg_forward(4, length_r);
		move_leg_forward(5, length_l);
	}else if(direction == BACKWARD){
		move_leg_forward(2, length_r);
		move_leg_forward(3, length_l);
		move_leg_forward(6, length_r);
		move_leg_backward(1, length_l);
		move_leg_backward(4, length_r);
		move_leg_backward(5, length_l);
	}
	
	SERVO_ACTION;
	robot_delay(length_r, length_l);
	
	put_down_leg(1);
	put_down_leg(4);
	put_down_leg(5);
	SERVO_ACTION;
	robot_delay(0x003F, 0x003F);
}

//Make robot take "one" step. Need to run setup_first_step() first. 
//Loop for walking
uint8_t last_step = 0;
void take_step(uint16_t length_r, uint16_t length_l, int direction){
	if(last_step++ % 2 == 0){
		take_step_1(length_r, length_l, direction);
	}else{
		take_step_2(length_r, length_l, direction);
	}
};

void rotate_1(uint16_t length, int direction){
	lift_leg(2);
	lift_leg(3);
	lift_leg(6);
	SERVO_ACTION;
	robot_delay(0x003F, 0x003F);
	
	if(direction == RIGHT){
		move_leg_backward(1, length);
		move_leg_forward(4, length);
		move_leg_backward(5, length);
		move_leg_backward(2, length);
		move_leg_forward(3, length);
		move_leg_backward(6, length);
	}else if(direction == LEFT){
		move_leg_forward(1, length);
		move_leg_backward(4, length);
		move_leg_forward(5, length);
		move_leg_forward(2, length);
		move_leg_backward(3, length);
		move_leg_forward(6, length);
	}
	
	SERVO_ACTION;
	robot_delay(length, length);
	
	put_down_leg(2);
	put_down_leg(3);
	put_down_leg(6);
	SERVO_ACTION;
	robot_delay(0x003F, 0x003F);
}

void rotate_2(uint16_t length, int direction){
	lift_leg(1);
	lift_leg(4);
	lift_leg(5);
	SERVO_ACTION;
	robot_delay(0x003F, 0x003F);
	
	if(direction == RIGHT){
		move_leg_forward(2, length);
		move_leg_backward(3, length);
		move_leg_forward(6, length);
		move_leg_forward(1, length);
		move_leg_backward(4, length);
		move_leg_forward(5, length);
	}else if(direction == LEFT){
		move_leg_backward(2, length);
		move_leg_forward(3, length);
		move_leg_backward(6, length);
		move_leg_backward(1, length);
		move_leg_forward(4, length);
		move_leg_backward(5, length);
	}
	SERVO_ACTION;
	robot_delay(length, length);
	
	put_down_leg(1);
	put_down_leg(4);
	put_down_leg(5);
	SERVO_ACTION;
	robot_delay(0x003F, 0x003F);
}

void rotate(uint16_t length, uint16_t bla, int direction){
	if(last_step++ % 2 == 0){
		rotate_1(length, direction);
	}else{
		rotate_2(length, direction);
	}
}

#define INNER_LENGTH 5.5
#define MIDDLE_LENGTH 6.0
#define OUTER_LENGTH 13.5

double rad_to_degree(double rad){
	return rad*57.295779513;
}

double degree_to_rad(double deg){
	return (deg/57.295779513);
}

double calc_angle1(double x, double y, double z){
	double G = sqrt(pow(x, 2) + pow(z, 2));
	double H = sqrt(pow(y, 2) + pow(G, 2));
	double b = acos((pow(MIDDLE_LENGTH, 2) + pow(H, 2) - pow(OUTER_LENGTH, 2))/(2*MIDDLE_LENGTH*H));
	
	return rad_to_degree((atan2(G, y))+b)-90;
}

double calc_angle2(double x, double y, double z){
	double G = sqrt(pow(x, 2) + pow(z, 2));
	double H = sqrt(pow(y, 2) + pow(G, 2));
	double b = acos((pow(MIDDLE_LENGTH, 2) + pow(H, 2) - pow(OUTER_LENGTH, 2))/(2*MIDDLE_LENGTH*H));
	double h2 = asin(H*(sin(b))/OUTER_LENGTH);
		
	return rad_to_degree(h2) - 130;
}

double calc_angle3(double x, double y, double z){
	return rad_to_degree(atan2(z, x));
}

uint16_t calc_servo_speed(unsigned length, unsigned ms){
	double speed = (length/0.29296875)/(ms/1000.0);
	speed *= 0.166666666667;
	return (uint16_t)(speed+0.5);
}

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

double angle3_offset(uint8_t leg, double y){
	if(leg == 3 || leg == 4){
		return y;
	}
	return y - 45;
}

void move_leg_ik(uint8_t leg, double x, double y, double z, int time){
	int angle3 = angle3_offset(leg, calc_angle3(x, y, z));
	
	//int x_proj = x;// / cos(degree_to_rad(angle3));
	
	int angle1 = calc_angle1(x, y, z);
	int angle2 = calc_angle2(x, y, z);
	
	move_servo_degree(inner_servo(leg), angle3, time);
	move_servo_degree(middle_servo(leg), angle1, time);
	move_servo_degree(outer_servo(leg), angle2, time);
	
	SERVO_ACTION;
}