/*
 * Sensormodul.c
 *
 * Created: 2014-11-05 09:10:19
 *  Author: RubenDas
 */ 

#define F_CPU 16000000UL
 
#include <avr/io.h>
#include "gyro.h"
#include <util/delay.h>
#include "ultraljud.h"
#include "display.h"
#include "SPI.h"
#include "messages.h"


int main(void)
{	
	int SENSOR_ID = 0;
	uart_message message_in;
	
	//Används för att begära avståndssensorer i den här ordningen
	int order[] = {0, 2, 4, 1, 3, 5};

	//Allt som behövs initieras 
	init_sensors();
	init_display();
	setup_spi();
	uart_init(0x0067);
	sei();		
	
	while(1)
    {
		//Begär ett avstånd från en specifik avståndssensor och lägger ut det på displayen
		get_distance(get_sensor(order[SENSOR_ID]));
		dist_to_display(order[SENSOR_ID]);
		
		//Kollar om vi har fått ett message för att sedan behandla det
		if (got_message()){
			get_message(&message_in);
			message_handler(&message_in);
		}
		
		//Iterera över våra sensor id:n
		SENSOR_ID = (SENSOR_ID+1) % 6;
	}
}	
	
	