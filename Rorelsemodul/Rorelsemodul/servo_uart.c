#include "servo_uart.h"

void suart_init(long baud)
{
	DDRD = 0x20;
<<<<<<< HEAD
	UBRR1 = UART_SETTINGS;
=======
	UBRR1H = ((F_CPU / 16 + baud / 2) / baud - 1) >> 8;
	UBRR1L = ((F_CPU / 16 + baud / 2) / baud - 1);
>>>>>>> 8780d3e853488ce3e828b6cb6a7f5960643bd646
	
	UCSR1C = (3 << UCSZ10);
	UCSR1B |= ((1 << RXEN1) | (1 << TXEN1));
}

char suart_read_char()
{
	while(!(UCSR1A & (1 << RXC1)));
	
	return UDR1;
}

void suart_send_char(uint8_t data)
{
	while(!(UCSR1A & (1 << UDRE1)));
	
	UDR1 = data;
	
}

int suart_read_string(char *s, int size)
{
	uint8_t i = 0;
	SUART_RX_ACTIVE;
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

void suart_send_string(char *s, uint8_t size)
{
	uint8_t i = 0;
	SUART_TX_ACTIVE;
	
	uint8_t checksum = 0;
	
	for(i = 0; i < size; ++i){
		if(i >= 2) checksum += s[i];
		suart_send_char(s[i]);
	}
	suart_send_char(~checksum);
	while(!(UCSR1A & (1 << TXC1)));
}

servo_response suart_command(uint8_t id, char* command, uint8_t size){
	servo_response response;
	
	suart_send_string(command, size);
	char buffer[16];
	uint8_t bytes_read = suart_read_string(buffer, 16);
	
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
	response.parameters = parameter_list;
	
	return response;
}

servo_response suart_command_ping(uint8_t id){
	char command[] = {0xFF, 0xFF, 0xFE, 0x02, 0x01};
	return suart_command(id, command, 5);
}