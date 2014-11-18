/*
 * messages.c
 *
 * Created: 11/18/2014 11:07:47 AM
 *  Author: jakbi869
 */ 
#include "messages.h"

const char *true[] = {"true"};
const char *false[] = {"false"};
const char *rot[] = {"0"};
char *SENSOR_DATA[] = {"000", "000", "000", "000", "000", "000"};
char *distance_data[1];


	
int get_cmd(uart_message message){
	
	char *cmd = message.data[0].data;
	
	if(strcmp(cmd,"rotate")==0){
		return 1;
		}else if(strcmp(cmd,"distance")==0){
		return 2;
		}else if(strcmp(cmd,"accept")==0){
		return 3;
	}
	return 0;
}

int get_validation(uart_message message){
	
	char *valid = message.data[1].data ;
	
	if(strcmp(valid,"false") == 0){
		return 0;
		}else if(strcmp(valid,"true") == 0){
		return 1;
	}
	return 0;
}

void message_handler(uart_message message_in){
	
	int ANGLE = 0;
	
	switch (get_cmd(message_in)){
		case 1:	//rotate
			send_message("accept", true, 1);
			ANGLE = string_to_int(message_in.data[1].data);
			rotate_to(ANGLE);
			rot[0] = int_to_string(ANGLE);
			send_message("rotate",rot, 1);
			break;
		case 2:	//distance
			switch (string_to_int(message_in.data[1].data)){
				 get_sensors_distance(SENSOR_DATA);
				case 0:
					send_message("accept", true, 1);
					send_message("distance", (const char**)SENSOR_DATA, 6);
					break;
				case 1:
					send_message("accept", true, 1);
					distance_data[0] = SENSOR_DATA[0];
					send_message("distance",(const char**)distance_data, 1);
					break;
				case 2:
					send_message("accept", true, 1);
					distance_data[0] = SENSOR_DATA[1];
					send_message("distance", (const char**)distance_data, 1);
					break;
				case 3:
					send_message("accept", true, 1);
					distance_data[0] = SENSOR_DATA[2];
					send_message("distance", (const char**)distance_data, 1);
					break;
				case 4:
					send_message("accept", true, 1);
					distance_data[0] = SENSOR_DATA[3];
					send_message("distance", (const char**)distance_data, 1);
					break;
				case 5:
					send_message("accept", true, 1);
					distance_data[0] = SENSOR_DATA[4];
					send_message("distance", (const char**)distance_data, 1);
					break;
				case 6:
					send_message("accept", true, 1);
					distance_data[0] = SENSOR_DATA[5];
					send_message("distance", (const char**)distance_data, 1);
					break;
				default:
					break;
			}
			break;
		case 3: //accept
			switch (get_validation(message_in)){
				case 0: //false
					break;
				case 1: //true
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

}