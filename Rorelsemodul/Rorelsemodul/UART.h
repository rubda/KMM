#define F_CPU 16000000UL

#define BV(bit)               (1 << bit)
#define set_bit(byte, bit)    (byte |= BV(bit))  // old sbi()
#define clear_bit(byte, bit)  (byte &= ~BV(bit)) // old cbi()
#define toggle_bit(byte, bit) (byte ^= BV(bit))

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

extern const char *false[];
extern const char *true[];

struct attribute 
{
	char data[20];
	uint8_t length;
};

typedef struct attribute attribute;

struct uart_message
{
	attribute data[10];
	uint8_t length;
};
typedef struct uart_message uart_message;

//	Init UART between modules
void uart_init(uint16_t settings);

//	Returns true if a message is in the buffer
uint8_t got_message();

//	Retrieve message from buffer
void get_message(uart_message *);

//	Send message 
void send_message(char name[], const char *attributes[], uint8_t attr_length);

//	Send string 
void uart_send_string(const char *s);

//	Send char
void uart_send_char(uint8_t data);