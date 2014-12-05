#include "servo_uart.h"

void suart_init(long baud){
	DDRD = 0x20;
	//UBRR1H = ((F_CPU / 16 + baud / 2) / baud - 1) >> 8;
	//UBRR1L = ((F_CPU / 16 + baud / 2) / baud - 1);
	UBRR1 = 0x0000;

	UCSR1C = (3 << UCSZ10);
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << TXCIE1);
}

char suart_read_char(){
	while(!(UCSR1A & (1 << RXC1)));
	//while(!(UCSR1A & (1 << UDRE1)));
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
	int len;
	
	while(i < 5){
		c = suart_read_char();
		s[i++] = c;
	}
	
	return 5;
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
	char buffer[16];
	char error[16] = "#ERROR:";

	suart_send_string(command, size);
	/*_delay_us(10);
	uint8_t bytes_read = suart_read_string(buffer, 16);
	
	error[7] = buffer[4];
	error[8] = ';';
	
	uint8_t led[] = {0x01};
	
	if(buffer[4] == 0)
		suart_command_write_data(id, 0x19, led, 1);
	
	switch(buffer[4]){
		case 0:
			return;
		default:
			uart_send_string(error);
	}
	
	response.id = id;
	response.error = buffer[4];
	response.parameter_size = buffer[3]-2;
	
	uint8_t parameter_list[response.parameter_size];
	int i;
	for(i = 0; i < response.parameter_size; ++i){
		parameter_list[i] = buffer[5+i];
	}
	response.parameters = parameter_list;
	*/
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

