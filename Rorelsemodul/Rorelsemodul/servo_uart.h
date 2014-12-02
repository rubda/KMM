#ifndef SERVO_UART_H_
#define SERVO_UART_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

/************************************************************************/
/* PIN definitions                                                      */
/************************************************************************/
#define SUART_RX PIND2
#define SUART_TX PORTD3
#define SUART_MODE PORTD5

/************************************************************************/
/* Change mode                                                          */
/************************************************************************/
#define SUART_TX_ACTIVE PORTD |= (1 << PORTD5)
#define SUART_RX_ACTIVE PORTD &= ~(1 << PORTD5)


struct suart_response
{
	uint8_t id;
	uint8_t error;
	uint8_t parameter_size;
	uint8_t *parameters;
};

typedef struct suart_response servo_response;

void suart_init(long baud);

void suart_send_char(uint8_t data);
char suart_read_char();

int suart_read_string(char *s, int size);
void suart_send_string(char *s, uint8_t size);

servo_response suart_command(uint8_t id, char* command, uint8_t size);

servo_response suart_command_ping(uint8_t id);
servo_response suart_command_read_data(uint8_t id, uint8_t data_addr, uint8_t data_len);
servo_response suart_command_write_data(uint8_t id, uint8_t data_addr, uint8_t *data_list, uint8_t list_len);
servo_response suart_command_reg_write(uint8_t id, uint8_t data_addr, uint8_t *data_list, uint8_t list_len);
servo_response suart_command_action(uint8_t id);
servo_response suart_command_reset(uint8_t id);

#endif