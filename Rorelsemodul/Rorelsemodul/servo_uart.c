#include "servo_uart.h"

uint8_t speed[2] = {0xCF, 0x00};
uint8_t r_middle_servo_start[2] = {0xC0, 0x02};
uint8_t r_outer_servo_start[2] = {0xD3, 0x00};
uint8_t l_middle_servo_start[2] = {0x3f, 0x01};
uint8_t l_outer_servo_start[2] = {0x2C, 0x03};
uint8_t inner_servo_start[2] ={0xFF, 0x01};
	
uint8_t l_middle_servo_lift[2] = {0xFF, 0x00};
uint8_t r_middle_servo_lift[2] = {0x20, 0x03};
	
uint8_t leg1[3] = {1, 3, 5};
uint8_t leg2[3] = {2, 4, 6};
uint8_t leg3[3] = {13, 15, 17};
uint8_t leg4[3] = {14, 16, 18};
uint8_t leg5[3] = {7, 9, 11};
uint8_t leg6[3] = {8, 10, 12};

uint8_t *leg_list[6];

void suart_init(long baud){
	DDRD = 0x20;
	//UBRR1H = ((F_CPU / 16 + baud / 2) / baud - 1) >> 8;
	//UBRR1L = ((F_CPU / 16 + baud / 2) / baud - 1);
	UBRR1 = 0x0000;

	UCSR1C = (3 << UCSZ10);
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << TXCIE1);
	
	leg_list[0] = leg1;
	leg_list[1] = leg2;
	leg_list[2] = leg3;
	leg_list[3] = leg4;
	leg_list[4] = leg5;
	leg_list[5] = leg6;	
}

char suart_read_char(){
	while(!(UCSR1A & (1 << RXC1)));
	while(!(UCSR1A & (1 << UDRE1)));
	return UDR1;
}

void suart_send_char(uint8_t data){	
	SUART_TX_ACTIVE;
	while(!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}

int suart_read_string(char *s, int size){
	
	uint8_t i = 0;
	char c;
	
	while(i < size - 1){
		c = suart_read_char();
		if(c == '\0')
			break;
		s[i] = c;
		i++;
	}
	
	s[i] = 0;
	
	return i + 1;
}

void suart_send_string(char *s, uint8_t size){
	uint8_t i = 0;
	
	char checksum = 0;
	
	for(i = 0; i < size; ++i){
		if(i >= 2) checksum += s[i];
		suart_send_char(s[i]);
	}
	
	suart_send_char(~checksum);
}

servo_response suart_command(uint8_t id, char* command, uint8_t size){
	servo_response response;

	suart_send_string(command, size);
	//_delay_us(10);
	//uint8_t bytes_read = suart_read_string(buffer, 16);
	/*
	if(bytes_read > 5){
		PORTB = 0x99;
	}
	
	response.id = id;
	response.error = buffer[4];
	response.parameter_size = buffer[3]-2;
	
	uint8_t parameter_list[response.parameter_size];
	int i;
	for(i = 0; i < response.parameter_size; ++i){
		parameter_list[i] = buffer[5+i];
	}
	response.parameters = parameter_list;*/
	
	return response;
}

servo_response suart_command_ping(uint8_t id){
	char command[] = {0xFF, 0xFF, id, 0x02, 0x01};
	return suart_command(id, command, 5);
}

servo_response suart_command_read_data(uint8_t id, uint8_t data_addr, uint8_t data_len){
	char command[] = {0xFF, 0xFF, id, 0x04, 0x02, data_addr, data_len};
	return suart_command(id, command, 7);
}

servo_response suart_command_write_data(uint8_t id, uint8_t data_addr, uint8_t *data_list, uint8_t list_len){
	char command[6+list_len];
	command[0] = 0xFF;
	command[1] = 0xFF;
	command[2] = id;
	command[3] = 3+list_len;
	command[4] = 0x03;
	command[5] = data_addr;
	int i;
	
	for(i = 0; i < list_len; ++i){
		command[i+6] = data_list[i];
	}
	
	return suart_command(id, command, 6+list_len);
}

servo_response suart_command_reg_write(uint8_t id, uint8_t data_addr, uint8_t *data_list, uint8_t list_len){
	char command[6+list_len];
	command[0] = 0xFF;
	command[1] = 0xFF;
	command[2] = id;
	command[3] = 3+list_len;
	command[4] = 0x04;
	command[5] = data_addr;
	int i;
	
	for(i = 0; i < list_len; ++i){
		command[6+i] = data_list[i];
	}
	
	return suart_command(id, command, 6+list_len);
}

servo_response suart_command_action(uint8_t id){
	char command[] =  {0xFF, 0xFF, id, 0x02, 0x05};
	return suart_command(id, command, 5);
}

servo_response suart_command_reset(uint8_t id){
	char command[] = {0xFF, 0xFF, id, 0x02, 0x06};
	return suart_command(id, command, 5);
}

void robot_start_position(){
	int i;
	_delay_us(300);
	/*for(i = 1; i < 7; ++i){
		reset_leg(i);
	}*/
	
	for(i = 0; i < 6; ++i){
		move_servo_reg(leg_list[i][0], inner_servo_start);
		_delay_us(400);
	}
	
	suart_command_action(BROADCAST_ID);
	_delay_ms(800);
	
	for(i = 1; i < 7; ++i){
		if(i % 2 == 1)
			move_servo_reg(leg_list[i-1][1], l_middle_servo_start);
		else
			move_servo_reg(leg_list[i-1][1], r_middle_servo_start);
		_delay_us(400);
	}
	
	suart_command_action(BROADCAST_ID);
	_delay_ms(800);
	
	for(i = 1; i < 7; ++i){
		if(i % 2 == 1)
			move_servo_reg(leg_list[i-1][2], l_outer_servo_start);
		else
			move_servo_reg(leg_list[i-1][2], r_outer_servo_start);
		_delay_us(400);
	}
	
	suart_command_action(BROADCAST_ID);
}

void set_servo_speed(){
	uint8_t speed[] = {0x70, 0x00};
	uint8_t torque_enable[] = {0x01};
	suart_command_write_data(BROADCAST_ID, MOVING_SPEED_L, speed, 2);
	suart_command_write_data(BROADCAST_ID, 0x18, torque_enable, 1);
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
		move_servo_reg(leg_list[leg_id-1][1], l_middle_servo_lift);
		move_servo_reg(leg_list[leg_id-1][2], test1);
	}else{
		move_servo_reg(leg_list[leg_id-1][1], r_middle_servo_lift);
	}
}

void put_down_leg(uint8_t leg_id){
	if(leg_id%2 == 1){
		move_servo_reg(leg_list[leg_id-1][1], l_middle_servo_start);
	}else{
		move_servo_reg(leg_list[leg_id-1][1], r_middle_servo_start);
	}
}

void stretch_leg(uint8_t leg_id){
	uint8_t outer_stretched_r[] = {0x65, 0x01};
	uint8_t middle_stretched_r[] = {0x70, 0x02};
	uint8_t outer_stretched_l[] = {0xE0, 0x01};
	uint8_t middle_stretched_l[] = {0xEF, 0x01};
		
	if(leg_id%2 == 1){
		move_servo_reg(leg_list[leg_id-1][1], middle_stretched_l);
		move_servo_reg(leg_list[leg_id-1][2], outer_stretched_l);
	}else{
		move_servo_reg(leg_list[leg_id-1][1], middle_stretched_r);
		move_servo_reg(leg_list[leg_id-1][2], outer_stretched_r);
	}
}

void reset_leg(uint8_t leg_id){
	move_servo_dir(leg_list[leg_id-1][0], inner_servo_start);
	_delay_ms(800);
	
	if(leg_id % 2 == 1){
		move_servo_dir(leg_list[leg_id-1][1], l_middle_servo_start);
		_delay_ms(800);
		move_servo_dir(leg_list[leg_id-1][2], l_outer_servo_start);
	}else{
		
		move_servo_dir(leg_list[leg_id-1][1], r_middle_servo_start);
		_delay_ms(800);
		move_servo_dir(leg_list[leg_id-1][2], r_outer_servo_start);
	}
}
