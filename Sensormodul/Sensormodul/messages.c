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

char* int_to_string(uint16_t digit)
{
	char dist[20];

	sprintf(dist, "%u", digit);
	return dist; 
}


int string_to_int(char *string)
{
	int val;
	char str[20];
	
	strcpy(str, string);
	val = atoi(str);
	
	return val;
}
	
int get_cmd(uart_message *message){
	
	char *cmd = (*message).data[0].data;
	
	write_string(cmd);
	if(strcmp(cmd,"rotate") == 0){
		//data_to_display(1336);
		return 1;
	}else if(strcmp(cmd,"distance") == 0){
		//data_to_display(1337);
		return 2;
	}else if(strcmp(cmd,"accept") == 0){
		//data_to_display(1338);
		return 3;
	}else{
		//data_to_display(1339);
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
	
	int ANGLE = 0;
	uint8_t c;
	
	switch (get_cmd(message_in)){
		case 1:	//rotate
			send_message("accept", true, 1);
			ANGLE = string_to_int((*message_in).data[1].data);
			rotate_to(ANGLE);
			rot[0] = int_to_string(ANGLE);
			send_message("rotate",rot, 1);
			break;
		case 2:	//distance
			c = string_to_int((*message_in).data[1].data);
			if(c == 0){
				get_sensors_distance(SENSOR_DATA);
				send_message("accept", true, 1);
				send_message("distance", (const char**)SENSOR_DATA, 6);
			}else if(c > 0 && c <= 6){
				get_sensors_distance(SENSOR_DATA);
				send_message("accept", true, 1);
				send_message("distance",(const char**)SENSOR_DATA[c-1], 1);
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
					break;
			}
			break;
		default:
			//data_to_display(get_cmd(message_in));
			break;
	}

}