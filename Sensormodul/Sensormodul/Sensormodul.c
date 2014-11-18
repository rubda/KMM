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
	
	_delay_ms(30);
	init_sensors();
	init_display();
	setup_spi();
	activate_adc();
	uart_init(0x0033);
	
	uart_message message_in;
	//message_in = get_message();


	while(1)
    {
		
	
	//int i;
	//
	//for (i = 0; i < 4; ++i){
		//set_display(0x00, 0x80+i);
		//toggle_enable();
	//
		//set_display(0x02, lol[i]);
		//toggle_enable();
	//}
	//
	//cli();
	//for (i = 0; i < buffer_size; ++i){
		//set_display(0x00, 0x90+i);
		//toggle_enable();
	//
		//set_display(0x02, buffer[i]);
		//toggle_enable();
	//}
	//sei();
	//_delay_ms(1000);
	//
	//clear_display();
	//toggle_enable();
	//
	//

	
	

		get_distance(get_sensor(SENSOR_ID));
		
		//Kolla om styrmodulen har begärt något
		message_in = get_message();
		message_handler(message_in);
	
		if (SENSOR_ID == 5){
			SENSOR_ID = 0;
		}else{
			SENSOR_ID++;
		}
	
		

	}
	
	
	
	
}

