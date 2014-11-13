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
	
	//int SENSOR_ID = 0;
	//int ANGLE = 0;
	//int TIME_TO_TURN = 0; //Flagga
	//
	//init_sensors();
	//setup_spi();
	//activateADC();
	
	_delay_ms(50);
	init_display();
	while(1)
    {
		
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

