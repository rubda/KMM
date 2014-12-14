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
	
	//Anv�nds f�r att beg�ra avst�ndssensorer i den h�r ordningen
	int order[] = {0, 2, 4, 1, 3, 5};

	//Allt som beh�vs initieras 
	init_sensors();
	init_display();
	setup_spi();
	uart_init(0x0067);
	sei();		
	
	while(1)
    {
		//Beg�r ett avst�nd fr�n en specifik avst�ndssensor och l�gger ut det p� displayen
		get_distance(get_sensor(order[SENSOR_ID]));
		dist_to_display(order[SENSOR_ID]);
		
		//Kollar om vi har f�tt ett message f�r att sedan behandla det
		if (got_message()){
			get_message(&message_in);
			message_handler(&message_in);
		}
		
		//Iterera �ver v�ra sensor id:n
		SENSOR_ID = (SENSOR_ID+1) % 6;
	}
}	
	
	