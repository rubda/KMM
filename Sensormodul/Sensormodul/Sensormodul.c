/*
 * Sensormodul.c
 *
 * Created: 2014-11-05 09:10:19
 *  Author: RubenDas
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include "gyro.h"
#include <util/delay.h>
#include "ultraljud.h"
//#include "UART.h"
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
	activate_adc();
	uart_init(0x0033);
	sei();
	
	while(1)
    {
		clear_display();
		
		//BOSSE();
		get_distance(get_sensor(SENSOR_ID));
		dist_to_display(0);
		_delay_ms(1000);
		
		if (got_message()){
			get_message(&message_in);
			message_handler(&message_in);
		}
	
		//_delay_ms(1000);
	
		if (SENSOR_ID == 5){
			SENSOR_ID = 0;
		}else{
			SENSOR_ID++;
		}
	}
}	
	
	
	


		//send_message("distance", (const char**)SENSOR_DATA, 6);
		//const char* t[1];
		//t[1] = SENSOR_DATA[2];
		//send_message("distance",t, 1);
		//write_string(SENSOR_DATA[2]);