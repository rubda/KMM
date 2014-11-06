#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

/************************************************************************/
/* Baudrate: 19200                                                      */
/************************************************************************/
#define UART_SETTINGS 0x0000

#define SUART_RX PORTD2
#define SUART_TX PORTD3
#define SUART_MODE PORTD5

#define SUART_TX_ACTIVE PORTD |= (1 << PORTD5)
#define SUART_RX_ACTIVE PORTD &= ~(1 << PORTD5)

#define BROADCAST_ID 0xFE
#define GOAL_POSITION_L 0x1E
#define GOAL_POSITION_H 0x1F
#define MOVING_SPEED_L 0x20
#define MOVING_SPEED_H 0x21
#define PRESENT_POSITION_L 0x24
#define PRESENT_POSITION_H 0x25
#define PRESENT_SPEED_L 0x26
#define PRESENT_SPEED_H 0x27
#define PRESENT_LOAD_L 0x28
#define PRESENT_LOAD_H 0x29
#define ID_ADDR 0x03

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