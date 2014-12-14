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


//Strängar som skickas
const char *rotate[] = {"rotate"};
const char *distance[] = {"distance"};
char *returnMessage[1];


//Tolkar det kommando som har mottagits 
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

//Tolkar den validationen som har mottagits
int get_validation(uart_message *message){
	
	char *valid = (*message).data[1].data;
	
	if(strcmp(valid,"false") == 0){
		return 0;
	}else if(strcmp(valid,"true") == 0){
		return 1;
	}
	return 0;
}

//Hanterar de meddelanden som fås från styrenheten
//Det som sker till mesta del är konverteringar mellan tal till strängar eller tvärtom
//och förstås användandet av funktioner som var definierade i gyro.c och ultraljud.c
void message_handler(uart_message *message_in){

	int ANGLE = 0;
	double CALC_ANGLE = 0;
	uint8_t c;

	switch (get_cmd(message_in)){
		case 1:	//rotate
			{
				char *rot[1];
				rot[0] = malloc(4);
				send_message("accept", rotate, 1);
				ANGLE = (int) strtol((*message_in).data[1].data, (char **)NULL, 10);
				CALC_ANGLE = rotate_to(ANGLE);
				snprintf(rot[0], 4, "%d", (int) CALC_ANGLE);
				send_message("rotate", rot, 1);
				free(rot[0]);
			}
			break;
		case 2:	//distance
			c = (int) strtol((*message_in).data[1].data, (char **)NULL, 10);
			if(c == 0){
				send_message("accept", distance, 1);
				char* attr[6];
				int i; for(i = 0; i < 6; ++i){
					attr[i] = malloc(4);
					snprintf(attr[i], 4, "%u", get_sensor(i)->medDist);
				}
				send_message("distance", attr, 6);
				for(i = 0; i < 6; ++i){
					free(attr[i]);	
				}
			}else if(c > 0 && c <= 6){
				send_message("accept", distance, 1);
				char* attr[2];
				attr[0] = malloc(2);
				attr[1] = malloc(4);
				snprintf(attr[0], 2, "%u", c);
				snprintf(attr[1], 4, "%u", get_sensor(c-1)->medDist);
				send_message("distance", attr, 2);			
				free(attr[0]);
				free(attr[1]);
			}else if(c == 7){
				send_message("accept", distance, 1);
				refresh_sensors();
				char* attr[6];
				int i; for(i = 0; i < 6; ++i){
					attr[i] = malloc(4);
					snprintf(attr[i], 4, "%u", get_sensor(i)->medDist);
				}
				send_message("distance", attr, 6);
				for(i = 0; i < 6; ++i){
					free(attr[i]);
				}
			}else if(c == 8){
				send_message("accept", distance, 1);
				median_of_dists(3);
				char* attr[6];
				int i; for(i = 0; i < 6; ++i){
				attr[i] = malloc(4);
				snprintf(attr[i], 4, "%u", get_sensor(i)->medDist);
				}
				send_message("distance", attr, 6);
				for(i = 0; i < 6; ++i){
					free(attr[i]);
				}
			}else{
				send_message("denied", distance, 1);
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
			returnMessage[0] = (*message_in).data[0].data;
			send_message("denied", returnMessage, 1);
			break;
	}
}