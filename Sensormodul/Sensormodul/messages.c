/*
 * messages.c
 *
 * Created: 11/18/2014 11:07:47 AM
 *  Author: jakbi869
 */ 
#include "messages.h"
#include "display.h"
#include "gyro.h"
#include <stdlib.h>
int bajs = 0;

const char *true[] = {"true"};
const char *false[] = {"false"};



/*char* int_to_string(uint16_t digit)
{
	char dist[20];

	//sprintf(dist, "%u", digit);
	itoa(digit, dist, 10);
	return dist; 
}


int string_to_int(char *string)
{
	int val;
	char str[20];
	
	strcpy(str, string);
	val = atoi(str);
	
	return val;
}*/
	
int get_cmd(uart_message *message){
	
	char *cmd = (*message).data[0].data;
	
	if(strcmp(cmd,"rotate") == 0){
		return 1;
	}else if(strcmp(cmd,"distance") == 0){
		//write_string("GC", 0xD6);
		return 2;
	}else if(strcmp(cmd,"accept") == 0){
		return 3;
	}else{
		return 0;
	}
	
}

int get_validation(uart_message *message){
	
	char *valid = (*message).data[1].data ;
	
	if(strcmp(valid,"false") == 0){
		return 0;
		}else if(strcmp(valid,"true") == 0){
		return 1;
	}
	return 0;
}

void message_handler(uart_message *message_in){

	//write_string("BC", 0x80);
	
	int ANGLE = 0;
	int CALC_ANGLE = 0;
	uint8_t c;
	//write_string("TT", 0x86);
	switch (get_cmd(message_in)){
		case 1:	//rotate
			{
				char *rot[1];
				rot[0] = malloc(4);
				send_message("accept", true, 1);
				//ANGLE = atoi((*message_in).data[1].data);
				ANGLE = (int) strtol((*message_in).data[1].data, (char **)NULL, 10);
				CALC_ANGLE = rotate_to(ANGLE);
				snprintf(rot[0], 4, "%d", CALC_ANGLE);
				send_message("rotate", rot, 1);
				free(rot[0]);
			}
			break;
		case 2:	//distance
			//write_string("C2", 0x83);
			//c = atoi((*message_in).data[1].data);
			c = (int) strtol((*message_in).data[1].data, (char **)NULL, 10);
			if(c == 0){
				send_message("accept", true, 1);
				char* attr[6];
				int i; for(i = 0; i < 6; ++i){
					attr[i] = malloc(4);
					snprintf(attr[i], 4, "%u", get_sensor(i)->Distance);
				}
				send_message("distance", attr, 6);
				for(i = 0; i < 6; ++i){
					free(attr[i]);	
				}
			}else if(c > 0 && c <= 6){
				//write_string("IF", 0xC0);
				send_message("accept", true, 1);
				char* attr[2];
				//write_string("BA", 0xC3);
				attr[0] = malloc(2);
				attr[1] = malloc(4);
				//write_string("AA", 0x90);
				snprintf(attr[0], 2, "%u", c);
				snprintf(attr[1], 4, "%u", get_sensor(c-1)->Distance);
				
				//write_string("BS", 0x93);
				
				send_message("distance", attr, 2);
				
				//write_string("BF", 0xD0);
				
				free(attr[0]);
				free(attr[1]);
				
				//write_string("AF", 0xD3);
				
			}else{
				send_message("accept", false, 1);
			}
			break;
		case 3: //accept
			switch (get_validation(message_in)){
				case 0: //false
					break;
				case 1: //true
					break;
				default:
					send_message("accept", false, 1);
					break;
			}
			break;
		default:
			send_message("accept", false, 1);
			break;
	}
}