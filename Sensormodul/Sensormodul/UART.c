#include "UART.h"

char buffer[MAX_BUFFER];
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
		buffer_size = uart_read_string(buffer, MAX_BUFFER);
	}
}

ISR(USART0_RX_vect){
	check_rx();
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
		c = uart_read_char(); //#walk:f;
		
		if(c == '#'){
			i = 0;
		}
		s[i++] = c;
		
		if(c == ';')
			break;
	}
	
	s[i] = '\0';
	
	return i + 1;
}

void uart_send_string(const char *s){
	int i;
	for(i = 0; s[i] != '\0'; ++i){
		uart_send_char(s[i]);
	}
}

uint8_t got_message(){
	return buffer_size;
}

void create_attribute(char *string, int start, int end, attribute* attr){
	int i;
	
	for(i = start; i < end; ++i){
		attr->data[i-start] = string[i];
	}
	
	attr->data[end-start] = '\0';
	attr->length = end - start;
}

     
// DISABLE INTERRUPT?!
void get_message(uart_message * message){
	cli();
	message->length = 0;
	if(got_message() && buffer[0] == '#'){
		//#COMMAND:ATTR1:ATTRN;	
		int i;
		int start = 1;
		for(i = start; i < buffer_size; ++i){
			if(buffer[i] == ':' || buffer[i] == ';'){
				create_attribute(buffer, start, i, &message->data[message->length++]);
				
				start = i + 1;
			}else if(buffer[i-1] == ';'){
				break;
			}
		}
		buffer_size = 0;
		sei();
	}
	sei();
}

void send_message(char name[], const char *attr[], uint8_t attr_length) {
	//EXAMPLE COMMAND
	//const char * attr[] = {"DATA1", "DATA2"};	
	//send_message("ROTATE", attr, 2)
	
	int i;
	uart_send_char('#');
	uart_send_string(name);
	for(i = 0; i < attr_length; ++i){
		uart_send_char(':');
		uart_send_string(attr[i]);
	}
	uart_send_char(';');
}