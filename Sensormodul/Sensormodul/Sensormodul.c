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

	init_sensors();
	init_display();
	setup_spi();
	ss_high();
	activate_adc();
	uart_init(0x0067);
	sei();		
	
	while(1)
    {
		get_distance(get_sensor(SENSOR_ID));
		dist_to_display(SENSOR_ID);
		
		if (got_message()){
			get_message(&message_in);
			message_handler(&message_in);
		}

		if (SENSOR_ID == 5){
			SENSOR_ID = 0;
		}else{
			SENSOR_ID++;
		}
	}
}	
	
	