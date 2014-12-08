#include "hexapod_control.h"

uint8_t speed[2] = {0x00, 0xff};
uint8_t servo_speed[18][2];
uint8_t *leg_list[6];
uint16_t current_pos[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t leg1[] = {1, 3, 5};
uint8_t leg2[] = {2, 4, 6};
uint8_t leg3[] = {13, 15, 17};
uint8_t leg4[] = {14, 16 ,18};
uint8_t leg5[] = {7, 9, 11};
uint8_t leg6[] = {8, 10, 12};
	
uint8_t R_MIDDLE_SERVO_START[] = {0x60, 0x02};	
uint8_t R_OUTER_SERVO_START[] = {0x30, 0x01};
uint8_t L_MIDDLE_SERVO_START[] = {0x9F, 0x01};
uint8_t L_OUTER_SERVO_START[] = {0xCF, 0x02};
uint8_t INNER_SERVO_START_R[] = {0x00, 0x02};
uint8_t INNER_SERVO_START_L[] = {0x00, 0x02};
uint8_t INNER_SERVO_START_M[] = {0x00, 0x02};
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
	int i;
	for(i = 0; i < 18; ++i){
		servo_speed[i][0] = s;
		servo_speed[i][1] = s >> 8;
	}
	speed[0] = s;
	speed[1] = s >> 8;
	
	uint8_t torque_enable[] = {0x01};
	suart_command_write_data(BROADCAST_ID, MOVING_SPEED_L, speed, 2);
	_delay_us(300);
	suart_command_write_data(BROADCAST_ID, 0x18, torque_enable, 1);
}

void set_leg_speed(uint8_t id, uint16_t s){
	set_servo_speed(inner_servo(id),s);
	set_servo_speed(middle_servo(id),s);
	set_servo_speed(outer_servo(id),s);
}

//Set speed of individual servo
void set_servo_speed(uint8_t id, uint16_t s){
	servo_speed[id-1][0] = s;
	servo_speed[id-1][1] = s >> 8;
	
	uint8_t torque_enable[] = {0x01};
	suart_command_write_data(id, MOVING_SPEED_L, servo_speed[id-1], 2);
	suart_command_write_data(id, 0x18, torque_enable, 1);
}

//Put robot into starting position
void robot_start_position(){
	uint8_t torque_enable[] = {0x01};
	suart_command_write_data(BROADCAST_ID, 0x18, torque_enable, 1);
	
	ik(0, 0, 0, 1);
	ik(0, 0, 0, 2);
	ik(0, 0, 0, 3);
	ik(0, 0, 0, 4);
	ik(0, 0, 0, 5);
	ik(0, 0, 0, 6);
	
	SERVO_ACTION;
	
	/*int i;
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
	
	SERVO_ACTION;*/
}

//Move servo using reg write command. Need to run SERVO_ACTION to perform action
uint16_t move_servo_reg(uint8_t id, uint8_t *position){
	suart_command_reg_write(id, GOAL_POSITION_L, position, 2);
	
	uint16_t temp = position[0] + (position[1] << 8);
	uint16_t ret = temp > current_pos[id-1] ? temp-current_pos[id-1] : current_pos[id-1]-temp;
	
	current_pos[id-1] = temp;
	return ret;
}

//Move servo using write data command. Moves servo directly
uint16_t move_servo_dir(uint8_t id, uint8_t *position){
	suart_command_write_data(id, GOAL_POSITION_L, position, 2);
	
	uint16_t temp = position[0] + (position[1] << 8);
	uint16_t ret = temp > current_pos[id-1] ? temp-current_pos[id-1] : current_pos[id-1]-temp;
	
	current_pos[id-1] = temp;
	return ret;
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
	ik(0, 0, 0, leg_id);
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
	
	_delay_ms(10);
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
	
	_delay_ms(10);
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
	}else if(direction == BACKWARD){
		move_leg_backward(1, length_l);
		move_leg_backward(4, length_r);
		move_leg_backward(5, length_l);
	}else if(direction == RIGHT){
		move_leg_forward(1, length_l);
		move_leg_backward(4, length_r);
		move_leg_forward(5, length_l);
	}else if(direction == LEFT){
		move_leg_backward(1, length_l);
		move_leg_forward(4, length_r);
		move_leg_backward(5, length_l);
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

void robot_delay(uint16_t a, uint16_t b){
	robot_delay2(a > b ? a : b);
}

void robot_delay2(uint16_t length){
	double speed_d = speed[0] + (speed[1] << 8);
	
	length = (double)(length*0.29 + 0.5);
	speed_d = (double)(speed_d*0.666 + 0.5);
	
	robot_delay_ms((uint16_t)((length*1000)/speed_d));
}


uint16_t calc_servo_delay(uint8_t id, uint16_t length){
	double speed_d = servo_speed[id-1][0] + (servo_speed[id-1][1] << 8);
	
	length = (double)(length*0.29 + 0.5);
	speed_d = (double)(speed_d*0.666 + 0.5);
	
	return (uint16_t)((length*1000)/speed_d);
}

uint16_t calc_leg_delay(uint8_t leg, uint16_t length){
	return calc_servo_delay(inner_servo(leg), length);
}

void robot_servo_delay(uint8_t id, uint16_t length){
	robot_delay_ms(calc_servo_delay(id, length));
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
		rotate_1(length-0x0019, direction);
	}else{
		rotate_2(length-0x0019, direction);
	}
}

void turn(uint16_t length_r, uint16_t length_l, int direction){
	if(direction == RIGHT){
		take_step(length_r, length_l, FORWARD);
		rotate(length_r+0x0019, 0x0000, RIGHT);
	}else if(direction == LEFT){
		take_step(length_r, length_l, FORWARD);
		rotate(length_r+0x0019, 0x0000, LEFT);
	}
}

double calc_gamma(double x, double y){
	double gamma = atan2(x, y);
	
	return gamma;
}

uint16_t gamma_to_hex(double g, int leg){
	if(leg == 5)
		return 0x0168 - (long) (g * 57.2957795 / 0.29);
	else if(leg == 2)
		return 0x0168 + (long) (g * 57.2957795 / 0.29);
	else if(leg == 1)
		return 0x029F - (long) (g * 57.2957795 / 0.29);
	else if(leg == 6)
		return 0x029F + (long) (g * 57.2957795 / 0.29); 
	else if(leg == 4)
		return 0x0200 + (long) (g * 57.2957795 / 0.29);
	else
		return 0x0200 - (long) (g * 57.2957795 / 0.29);
}	

uint16_t calc_alpha(double x, double y, double z, int leg){
	double L = sqrt(pow(y-5.0, 2) + pow(z, 2));
	
	double a1 = acos(z/L);
	
	double a2_t = pow(13.0, 2) - pow(6.5, 2) - pow(L, 2);
	double a2_n = -2.0 * 6.5 * L;
	double arg = a2_t/a2_n;
	
	double a2 = acos(arg);
	
	double alpha = a1 + a2;
	
	if(leg % 2 == 1)
		return 0x0330 - (int)(alpha * 57.2957795 / 0.29); 
	else
		return 0x00CF + (int)(alpha * 57.2957795 / 0.29); 
}

uint16_t calc_beta(double y, double z, double z_offs, int leg){
	double beta;
	double a, b, l, l1;
	
	l1 = y - 5.0;
	l = sqrt(pow(l1, 2.0) + pow(z_offs, 2.0));
	
	a = pow(l, 2) - pow(13.0, 2.0) - pow(6.5, 2.0);
	b = -2 * 13.0 * 6.5;
	
	beta = acos(a / b);
	
	if(leg % 2 == 1)
		return 0x0330 - (long) ((beta * 57.2957795 - 45 )/ 0.29);
	else
		return 0x00CF + (long) ((beta * 57.2957795 - 45 )/ 0.29);

}


uint16_t get_max(uint16_t a, uint16_t b, uint16_t c){
	uint16_t max = a;
	max = b > max ? b : max;
	max = c > max ? c : max;
	return max;
}

uint16_t ik(double x, double y, double z, int leg){
	double l1, l2;
	uint16_t alpha, beta, gamma_hex;
	double alpha1, alpha2;
	double z_offs = 9 - z;
	double x_offs, y_offs;
	double y_proj;
	double gamma;
	
	if(leg == 1 || leg == 2){
		x_offs = 6 + x;
		y_offs = 10 + y;
	}else if(leg == 5 || leg == 6){
		x_offs = -6 + x;
		y_offs = 10 + y;
	}else{
		x_offs = x;
		y_offs = 10 + y;
	}
	
	gamma = calc_gamma(x_offs, y_offs); gamma_hex = gamma_to_hex(gamma, leg);
	y_proj = y_offs / cos(gamma);
	beta = calc_beta(y_proj, z, z_offs, leg);
	alpha = calc_alpha(x, y_proj, z_offs, leg);

	uint16_t mov1 = move_servo_reg(inner_servo(leg), (uint8_t[2]){gamma_hex, gamma_hex >> 8});
	uint16_t mov2 = move_servo_reg(outer_servo(leg), (uint8_t[2]){beta, beta >> 8});
	uint16_t mov3 = move_servo_reg(middle_servo(leg), (uint8_t[2]){alpha, alpha >> 8});
	
	return get_max(mov1, mov2, mov3);
}

