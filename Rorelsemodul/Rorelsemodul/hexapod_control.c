#include "hexapod_control.h"

uint8_t speed[2] = {0xFF, 0x00};
uint8_t *leg_list[6];
uint16_t current_pos[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t leg1[] = {1, 3, 5};
uint8_t leg2[] = {2, 4, 6};
uint8_t leg3[] = {13, 15, 17};
uint8_t leg4[] = {14, 16 ,18};
uint8_t leg5[] = {7, 9, 11};
uint8_t leg6[] = {8, 10, 12};

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
	suart_command_write_data(BROADCAST_ID, MOVING_SPEED_L, speed, 2);
	_delay_us(300);
	suart_command_write_data(BROADCAST_ID, 0x18, torque_enable, 1);
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
	uint8_t torque_enable[] = {0x01};
	suart_command_write_data(BROADCAST_ID, 0x18, torque_enable, 1);
	
	ik(0, 0, 0, 1);
	_delay_ms(1);
	ik(0, 0, 0, 2);
	_delay_ms(1);
	ik(0, 0, 0, 3);
	_delay_ms(1);
	ik(0, 0, 0, 4);
	_delay_ms(1);
	ik(0, 0, 0, 5);
	_delay_ms(1);
	ik(0, 0, 0, 6);
	_delay_ms(1);
	
	SERVO_ACTION;
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

