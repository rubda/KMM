#define F_CPU 7372800UL

#define PRESCALER 1 //Beroende på vad som passar för att timea RX
#define TICKS 1		//Beroende på vad som passar för att timea RX

#include <avr/io.h>
#include <util/delay.h>
#include "atmega-timers.h"

/************************************************************************/
/* Baudrate: 19200                                                      */
/************************************************************************/
#define UART_SETTINGS 0x0001

#define UART_RX PORTD2
#define UART_TX PORTD3

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

void uart_init(long baud);

uint8_t got_message();
uart_message get_message();
//uart_message create_message(char name[], char attributes[][]);
void send_message(uart_message m);
