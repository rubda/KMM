#include <avr/io.h>

/************************************************************************/
/* Baudrate: 19200                                                      */
/************************************************************************/
#define UART_SETTINGS 0x0017

#define SUART_RX PORTD2
#define SUART_TX PORTD3
#define SUART_MODE PORTD4
#define SUART_RX_ACTIVE PORTD |= (1 << PORTD4)
#define SUART_TX_ACTIVE PORTD &= ~(1 << PORTD4)

#define SERVO_ERROR 0

struct suart_response
{
	uint8_t id;
	uint8_t error;
	uint8_t parameter_size;
	uint8_t *parameters;
};

typedef struct suart_response servo_response;

void suart_init();

void suart_send_char(uint8_t data);
char suart_read_char();

int suart_read_string(char *s, int size);
void suart_send_string(char *s, uint8_t size);

servo_response suart_command(uint8_t id, char* command, uint8_t size);
servo_response suart_command_ping(uint8_t id);