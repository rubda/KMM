#define F_CPU 16000000UL

#define BV(bit)               (1 << bit)
#define set_bit(byte, bit)    (byte |= BV(bit))  // old sbi()
#define clear_bit(byte, bit)  (byte &= ~BV(bit)) // old cbi()
#define toggle_bit(byte, bit) (byte ^= BV(bit))

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

struct attribute 
{
	char * data;
	uint8_t length;
};

typedef struct attribute attribute;

struct uart_message
{
	attribute data[10];
	uint8_t length;
};
typedef struct uart_message uart_message;

void uart_init(uint16_t settings);

uint8_t got_message();
uart_message get_message();
void send_message(char name[], const char *attributes[], uint8_t attr_length);
void uart_send_string(const char *s);
void uart_send_char(uint8_t data);