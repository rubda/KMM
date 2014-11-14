#include "hexapod_control.h"

uint8_t speed[2] = {0xFF, 0x00};
uint8_t *leg_list[6];

void robot_init(){
	leg_list[0] = LEG1;
	leg_list[1] = LEG2;
	leg_list[2] = LEG3;
	leg_list[3] = LEG4;
	leg_list[4] = LEG5;
	leg_list[5] = LEG6;
	
	suart_init(1000000);
	set_speed();
	_delay_us(200);
	robot_start_position();
}

uint8_t inner_servo(uint8_t leg_id){
	return leg_list[leg_id - 1][0];
}

uint8_t middle_servo(uint8_t leg_id){
	return leg_list[leg_id - 1][1];
}

uint8_t outer_servo(uint8_t leg_id){
	return leg_list[leg_id - 1][2];
}

uint16_t get_relative_speed(float multiplier){
	return (uint16_t)speed[0]*multiplier + ((uint16_t)(speed[1]*multiplier) << 8);
}

void set_speed(uint16_t s){
	speed[0] = s;
	speed[1] = s >> 8;
	
	uint8_t torque_enable[] = {0x01};
	suart_command_write_data(BROADCAST_ID, MOVING_SPEED_L, speed, 2);
	suart_command_write_data(BROADCAST_ID, 0x18, torque_enable, 1);
}

void set_servo_speed(uint8_t id, uint16_t s){
	uint8_t local_speed[] = {s, s >> 8};
	uint8_t torque_enable[] = {0x01};
	suart_command_write_data(id, MOVING_SPEED_L, local_speed, 2);
	suart_command_write_data(id, 0x18, torque_enable, 1);
}

void robot_start_position(){
	int i;
	_delay_us(300);
	
	for(i = 1; i < 7; ++i){
		move_servo_reg(inner_servo(i), INNER_SERVO_START);
		_delay_us(200);
	}
	
	SERVO_ACTION;
	_delay_ms(800);
	
	for(i = 1; i < 7; ++i){
		if(i % 2 == 1){
			move_servo_reg(middle_servo(i), L_MIDDLE_SERVO_START);
		}else{
			move_servo_reg(middle_servo(i), R_MIDDLE_SERVO_START);
		}
		
		_delay_us(200);
	}
	
	SERVO_ACTION;
	_delay_ms(800);
	
	for(i = 1; i < 7; ++i){
		if(i % 2 == 1){
			move_servo_reg(outer_servo(i), L_OUTER_SERVO_START);
		}else{
			move_servo_reg(outer_servo(i), R_OUTER_SERVO_START);
		}
		
		_delay_us(200);
	}
	
	SERVO_ACTION;
}

void move_servo_reg(uint8_t id, uint8_t *position){
	suart_command_reg_write(id, GOAL_POSITION_L, position, 2);
}

void move_servo_dir(uint8_t id, uint8_t *position){
	suart_command_write_data(id, GOAL_POSITION_L, position, 2);
}

void lift_leg(uint8_t leg_id){
	uint8_t test1[] = {0xFF, 0x02};
	
	if(leg_id%2 == 1){
		move_servo_reg(middle_servo(leg_id), L_MIDDLE_SERVO_LIFT);
		move_servo_reg(outer_servo(leg_id), L_OUTER_SERVO_LIFT);
	}else{
		move_servo_reg(middle_servo(leg_id), R_MIDDLE_SERVO_LIFT);
		move_servo_reg(outer_servo(leg_id), R_OUTER_SERVO_LIFT);
	}
}

void put_down_leg(uint8_t leg_id){
	if(leg_id%2 == 1){
		move_servo_reg(middle_servo(leg_id), L_MIDDLE_SERVO_START);
		move_servo_reg(outer_servo(leg_id), L_OUTER_SERVO_START);
	}else{
		move_servo_reg(middle_servo(leg_id), R_MIDDLE_SERVO_START);
		move_servo_reg(outer_servo(leg_id), R_OUTER_SERVO_START);
	}
}

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

void prepare_step_forward(int8_t leg_id, uint16_t length){
	prepare_step_backward(leg_id, length);
}

void prepare_step_backward(int8_t leg_id, uint16_t length){
	uint16_t pos = 0x01FF;
	if(leg_id == 3 || leg_id == 4){
		//length = length >> 2;
	}
	
	if(leg_id % 2 == 0){
		pos += length;
	}else{
		pos -= length;
	}
	
	uint8_t pos2[] = {pos, pos >> 8};
	//move_servo_reg(inner_servo(leg_id), pos2);
}

uint8_t* int_to_list(uint16_t i){
	uint8_t pos[2] = {i, i >> 8};
	return pos;
}

// TODO: Kolla plus och minus för benen
void move_leg_forward(uint8_t leg_id, uint16_t length){
	uint16_t start_pos = 0x01FF;
	
	if(leg_id == 1 || leg_id == 2){
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos));
	}else if(leg_id == 3){
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos - (length / 2)));
	}else if(leg_id == 4){
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos + (length / 2)));
	}else if(leg_id == 5){
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos - length));
	}else{
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos + length));
	}
}

//TODO: Kolla plus och minus för benen
void move_leg_backward(uint8_t leg_id, uint16_t length){
	uint16_t start_pos = 0x01FF;
	
	if(leg_id == 1){
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos - length));
	}else if(leg_id == 2){
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos + length));
	}else if(leg_id == 3){
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos - (length / 2)));
	}else if(leg_id == 4){
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos + (length / 2)));
	}else{
		move_servo_reg(inner_servo(leg_id), int_to_list(start_pos));
	}
}

void setup_first_step(uint16_t length){
	lift_leg(1);
	lift_leg(4);
	lift_leg(5);
	SERVO_ACTION;
	_delay_ms(1000);
	
	move_leg_forward(1, length);
	move_leg_forward(4, length);
	move_leg_forward(5, length);
	SERVO_ACTION;
	_delay_ms(1000);
	
	put_down_leg(1);
	put_down_leg(4);
	put_down_leg(5);
	SERVO_ACTION;
	_delay_ms(1000);
}

void take_step(uint16_t length){
	/*lift_leg(1);
	lift_leg(4);
	lift_leg(5);
	SERVO_ACTION;
	_delay_ms(1000);
	
	prepare_step_forward(1, length);
	prepare_step_forward(4, length);
	prepare_step_forward(5, length);
	prepare_step_backward(2, length);
	prepare_step_backward(3, length);
	prepare_step_backward(6, length);
	SERVO_ACTION;
	_delay_ms(1000);
	
	put_down_leg(1);
	put_down_leg(4);
	put_down_leg(5);
	SERVO_ACTION;
	_delay_ms(1000);
	
	lift_leg(2);
	lift_leg(3);
	lift_leg(6);
	SERVO_ACTION;
	_delay_ms(1000);
	
	prepare_step_forward(2, length);
	prepare_step_forward(3, length);
	prepare_step_forward(6, length);
	prepare_step_backward(1, length);
	prepare_step_backward(4, length);
	prepare_step_backward(5, length);
	SERVO_ACTION;
	_delay_ms(1000);
	
	put_down_leg(2);
	put_down_leg(3);
	put_down_leg(6);
	SERVO_ACTION;
	_delay_ms(1000);*/
	
	lift_leg(2);
	lift_leg(3);
	lift_leg(6);
	SERVO_ACTION;
	_delay_ms(1000);
	
	move_leg_backward(1, length);
	move_leg_backward(4, length);
	move_leg_backward(5, length);
	move_leg_forward(2, length);
	move_leg_forward(3, length);
	move_leg_forward(6, length);
	SERVO_ACTION;
	_delay_ms(1000);
	
	put_down_leg(2);
	put_down_leg(3);
	put_down_leg(6);
	SERVO_ACTION;
	_delay_ms(1000);
	
	lift_leg(1);
	lift_leg(4);
	lift_leg(5);
	SERVO_ACTION;
	_delay_ms(1000);
	
	move_leg_backward(2, length);
	move_leg_backward(3, length);
	move_leg_backward(6, length);
	move_leg_forward(1, length);
	move_leg_forward(4, length);
	move_leg_forward(5, length);
	SERVO_ACTION;
	_delay_ms(1000);
	
	put_down_leg(1);
	put_down_leg(4);
	put_down_leg(5);
	SERVO_ACTION;
	_delay_ms(1000);
	
};