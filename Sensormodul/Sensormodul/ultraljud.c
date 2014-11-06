/*
 * ultraljud.c
 *
 * Created: 2014-11-05 19:58:28
 *  Author: RubenDas
 */ 

#include "ultraljud.h"
#include <avr/io.h>
#include "timer.h"

void initSensors()
{
	
	//S�tt PA0, A2, A3, A4 till ut
	DDRA |= (1 << PORTA4);
	DDRA |= (1 << PORTA3);
	DDRA |= (1 << PORTA2);
	DDRA |= (1 << PORTA0);

	//S�tt PA1 till in	
	DDRA &= ~(1 << PORTA1);
	DDRB |= (1 << PORTB0);
	
	sensor_list[0] = (struct soundSensor) {0b00000000, 0};
	sensor_list[1] = (struct soundSensor) {0b11100111, 0};
	sensor_list[2] = (struct soundSensor) {0b11101011, 0};
	sensor_list[3] = (struct soundSensor) {0b11101111, 0};
	sensor_list[4] = (struct soundSensor) {0b11110011, 0};
	sensor_list[5] = (struct soundSensor) {0b11110111, 0};
}

void getData(struct soundSensor sensor) 
{
	sensor.Distance = getDistance(sensor);
}

uint8_t getDistance(struct soundSensor sensor)
{
	uint8_t DISTANCE;
	uint8_t TIME = 0; 
		
	PORTA = sensor.id & PORTA;
	
	PORTA |= (1 << PORTA0);
	_delay_us(90);
	PORTA &= ~(1 << PORTA0);

	while (!(PINA & (1 << PINA1)));
	while ((PINA & (1 << PINA1)))
		{	
			_delay_us(1);
			TIME++;
		}
	_delay_ms(20);
		
	DISTANCE = (TIME/100)/58;
	return DISTANCE;
		
}