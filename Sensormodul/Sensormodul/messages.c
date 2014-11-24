/*
 * messages.c
 *
 * Created: 11/18/2014 11:07:47 AM
 *  Author: jakbi869
 */ 
#include "messages.h"
#include "display.h"
#include <stdlib.h>

const char *true[] = {"true"};
const char *false[] = {"false"};
char *rot[] = {"0"};

char *distance_data[1];

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
	
	int ANGLE = 0;
	int CALC_ANGLE = 0;
	char test[20];
	uint8_t c;
	
	switch (get_cmd(message_in)){
		case 1:	//rotate
			send_message("accept", true, 1);
			ANGLE = atoi((*message_in).data[1].data);
			CALC_ANGLE = rotate_to(ANGLE);
			itoa(CALC_ANGLE, rot[0], 10);
			send_message("rotate", rot, 1);
			break;
		case 2:	//distance
			c = atoi((*message_in).data[1].data);
			if(c == 0){
				send_message("accept", true, 1);
				char* attr[6];
				int i; for(i = 0; i < 6; ++i){
					itoa(get_sensor(i)->Distance, attr[i], 10);
				}
				send_message("distance", attr, 6);
			}else if(c > 0 && c <= 6){
				send_message("accept", true, 1);
				char* attr[2];
				itoa(c, attr[0], 10);
				itoa(get_sensor(c-1)->Distance, attr[1], 10);
				send_message("distance", attr, 2);
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