#include "UART.h"

char buffer[256];
uint8_t buffer_size = 0;

int uart_read_string(char *s, int size);

void uart_init(long baud)
{
	DDRD = 0x20;

	UBRR1H = ((F_CPU / 16 + baud / 2) / baud - 1) >> 8;
	UBRR1L = ((F_CPU / 16 + baud / 2) / baud - 1);

	UCSR1C = (3 << UCSZ10);
	UCSR1B |= ((1 << RXEN1) | (1 << TXEN1));
}

void check_rx() {
	if(UCSR1A & (1 << RXC1)){
		//TIME TO READ
		buffer_size = uart_read_string(buffer, 255);
	}
	timer0(PRESCALER, TICKS, check_rx);
}

void uart_start(){
	timer0(PRESCALER, TICKS, check_rx);
}


char uart_read_char()
{
	while(!(UCSR1A & (1 << RXC1)));
	
	return UDR1;
}

void uart_send_char(uint8_t data)
{
	while(!(UCSR1A & (1 << UDRE1)));
	
	UDR1 = data;
}

int uart_read_string(char *s, int size)
{
	uint8_t i = 0;
	char c;
	
	while(i < size - 1){
		c = uart_read_char();
		if(c == '\0')
			break;
		s[i] = c;
		i++;
	}
	
	s[i] = 0;
	
	return i + 1;
}

void uart_send_string(char *s, uint8_t size)
{
	uint8_t i = 0;
	
	uint8_t checksum = 0;
	
	for(i = 0; i < size; ++i){
		if(i >= 2) checksum += s[i];
		uart_send_char(s[i]);
	}
	uart_send_char(~checksum);
	while(!(UCSR1A & (1 << TXC1)));
}

uint8_t got_message(){
	return buffer_size;
}

attribute create_attribute(char *string, int start, int end){
	char str[end-start];
	int i;
	for(i = start; i < end; ++i){
		str[i-start] = string[i];
	}
	
	attribute attr;
	attr.data = str;
	attr.length = end-start-1;
	
	return attr;
}

// DISABLE INTERRUPT?!
uart_message get_message(){
	uart_message message;
	message.length = 0;
	if(got_message() && buffer[0] == '#'){
		//#COMMAND:ATTR1:ATTRN;	
		int i;
		int start = 1;
		for(i = start; i < buffer_size && buffer[i-1] != ';'; ++i){
			if(buffer[i] == ':' || buffer[i] == ';'){
				message.data[message.length++] = create_attribute(buffer, start, i);
				start = i+1;
			}
		}
		buffer_size = 0;
		return message;
	}
	return message;
}