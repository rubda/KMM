#include "UART.h"

char buffer[256];
uint8_t buffer_size = 0;

int uart_read_string(char *s, int size);

void uart_init(uint16_t settings) // 0x0340
{
	// Initialize serial
	//UBRR0 = ((F_CPU / 16 + baud / 2) / baud - 1);         /* (16 MHz / 16 / 38400) - 1 */
	UBRR0 = settings;
	set_bit(UCSR0B, RXEN0);                               /* enable RX */
	set_bit(UCSR0B, TXEN0);                               /* enable TX */

	set_bit(UCSR0B, RXCIE0);
	UCSR0C = (3 << UCSZ00);
}

void check_rx() {
	if (bit_is_set(UCSR0A, RXC0)){
		buffer_size = uart_read_string(buffer, 255);
	}
}

ISR(USART0_RX_vect){
	cli();
	check_rx();
	sei();
}

char uart_read_char()
{
	while(!bit_is_set(UCSR0A, RXC0));
	
	return UDR0;
}

void uart_send_char(uint8_t data)
{
	while(!bit_is_set(UCSR0A, UDRE0));
	UDR0 = data;
}

int uart_read_string(char *s, int size)
{
	uint8_t i = 0;
	char c;
	
	while(i < size - 1){
		c = uart_read_char();
		if(c == '\0')
			break;
		else if(c == '#'){
			i = 0;
		}
		s[i++] = c;
	}
	
	s[i] = 0;
	
	return i + 1;
}

void uart_send_string(const char *s){
	int i;
	for(i = 0; s[i] != '\0'; ++i){
		uart_send_char(s[i]);
	}
}
/*
void uart_send_string(char *s, uint8_t size)
{
	uint8_t i = 0;
	
	//uint8_t checksum = 0;
	
	for(i = 0; i < size; ++i){
		//if(i >= 2) checksum += s[i];
		uart_send_char(s[i]);
	}
	//uart_send_char(~checksum);
	//while(!bit_is_set(UCSR0A, TXC0));
}*/

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

void send_message(char name[], const char *attr[], uint8_t attr_length) {
	//EXAMPLE COMMAND
	//const char * attr[] = {"DATA1", "DATA2"};	
	//send_message("ROTATE", attr, 2)
	
	int i;
	uart_send_string(name);
	for(i = 0; i < attr_length; ++i){
		uart_send_char(':');
		uart_send_string(attr[i]);
	}
	uart_send_char(';');
}