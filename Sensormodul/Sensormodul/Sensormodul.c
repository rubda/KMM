/*
 * Sensormodul.c
 *
 * Created: 2014-11-05 09:10:19
 *  Author: RubenDas
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include "SPI.h"
#include "gyro.h"
#include <util/delay.h>
#include "ultraljud.h"
#include "UART.h"
#include "display.h"


int main(void)
{
	uint16_t DISTANCE = 0;
	
	int ANGLE = 0;
	
	uart_message BAJS;
	sei();
	
	//int SENSOR_ID = 0;
	//int ANGLE = 0;
	//int TIME_TO_TURN = 0; //Flagga
	//
	//init_sensors();
	//setup_spi();
	//activateADC();
	
	_delay_ms(30);
	setup_spi();
	activateADC();
	init_display();
	init_sensors();
	uart_init(0x0033);
	
	BAJS = get_message();

	char *name = BAJS.data[0].data;
	uint8_t len = BAJS.data[0].length;
	
	char lol[] = "TEST";
	int i;
	
	while(1)
    {
		
	
	int i;
	
	for (i = 0; i < 4; ++i){
		set_display(0x00, 0x80+i);
		toggle_enable();
	
		set_display(0x02, lol[i]);
		toggle_enable();
	}
	
	cli();
	for (i = 0; i < buffer_size; ++i){
		set_display(0x00, 0x90+i);
		toggle_enable();
	
		set_display(0x02, buffer[i]);
		toggle_enable();
	}
	sei();
	_delay_ms(1000);
	
	clear_display();
	toggle_enable();
		
		
	

		//ANGLE = rotateTo(90);
		//distance_to_display(ANGLE);
		//DISTANCE = get_distance(get_sensor(4));
		//distance_to_display(DISTANCE);
		
		//get_distance(get_sensor(SENSOR_ID));
		//
		////Kolla om styrmodulen har begärt något
	//
		//if (SENSOR_ID = 5) {
			//SENSOR_ID = 0;
		//}else{
			//SENSOR_ID++;
		//}
		//
		////Kolla om styrmodulen har begärt något
		//
		//if (TIME_TO_TURN){
			//rotateTo(ANGLE);
			//while(!getIsRotated());
			//hasRotated(0);
			////Skicka till styrmodulen att den har roterat klart!
		//}
		
	

	}
	
	
	
	
}

