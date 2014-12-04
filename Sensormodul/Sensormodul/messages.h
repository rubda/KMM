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
void dist_to_display(int id);

#endif /* MESSAGES_H_ */