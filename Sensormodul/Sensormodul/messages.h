/*
 * messages.h
 *
 * Created: 11/18/2014 11:07:59 AM
 *  Author: jakbi869
 */ 


#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "UART.h"
#include "ultraljud.h"
#include "gyro.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



	
int get_cmd(uart_message *message);
int get_validation(uart_message *message);
void message_handler(uart_message *message_in);
//char* int_to_string(uint16_t digit);
//int string_to_int(char *string);

#endif /* MESSAGES_H_ */