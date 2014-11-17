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
	while(1)
    {
		
		
		distance_to_display(4);
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

