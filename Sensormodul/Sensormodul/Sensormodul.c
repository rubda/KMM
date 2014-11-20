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
	int TEST = 0;

	_delay_ms(30);
	init_sensors();
	init_display();
	setup_spi();
	activate_adc();
	uart_init(0x0033);
	sei();
	
	uart_message message_in;


	while(1)
    {
		
	
		get_distance(get_sensor(SENSOR_ID));
		
		distance_to_display(SENSOR_ID);
		//_delay_ms(1000);
		
		//Kolla om styrmodulen har begärt något
		
		
		if (got_message()){
		clear_display();
			
		//TEST = got_message();
		
		//data_to_display(TEST);

		message_in = get_message();
	
		//int i;
		//for (i = 0; i < message_in.data[0].length+1; ++i){
			//set_display(0x00, 0x90+i);
			//toggle_enable();
	//
			//set_display(0x02, message_in.data[0].data[i]);
			//toggle_enable();
			//}
			//
		message_handler(&message_in);
		
		_delay_ms(2000);
			
		}
	
		if (SENSOR_ID == 5){
			SENSOR_ID = 0;
		}else{
			SENSOR_ID++;
		}
	
		

	}
	
	
	
	
}

