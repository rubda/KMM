/*
 * ultraljud.c
 *
 * Created: 2014-11-05 19:58:28
 *  Author: RubenDas
 */ 

#include "ultraljud.h"
#include <avr/io.h>
#include "timer.h"

void init_sensors()
{
	
	//Sätt PA0, A2, A3, A4 till ut
	DDRA |= (1 << PORTA4);
	DDRA |= (1 << PORTA3);
	DDRA |= (1 << PORTA2);
	DDRA |= (1 << PORTA0);

	//Sätt PA1 till in	
	DDRA &= ~(1 << PORTA1);
	
	//B0 till ut
	DDRB |= (1 << PORTB0);
	
	sensor_list[0] = (struct soundSensor) {0b00000000, 0};
	sensor_list[1] = (struct soundSensor) {0b00000100, 0};
	sensor_list[2] = (struct soundSensor) {0b00001000, 0};
	sensor_list[3] = (struct soundSensor) {0b00001100, 0};
	sensor_list[4] = (struct soundSensor) {0b00010000, 0};
	sensor_list[5] = (struct soundSensor) {0b00010100, 0};
}

void get_data(struct soundSensor sensor) 
{
	sensor.Distance = get_distance(sensor);
}

struct soundSensor get_sensor(uint8_t id)
{
	return sensor_list[id];
}

uint8_t get_distance(struct soundSensor sensor)
{
	uint8_t DISTANCE;
	uint8_t TIME = 0; 
		
	// Måste se till att rätt bitar ändras först
	PORTA = PORTA & 0b11100011;
	
	// Sen orar vi in ett specifikt ultraljud
	PORTA = PORTA | sensor.id;
	
	PORTA |= (1 << PORTA0);
 	_delay_us(90);
 	PORTA &= ~(1 << PORTA0);

	while (!(PINA & (1 << PINA1)));
	while ((PINA & (1 << PINA1)))
		{	
			_delay_us(1);
			TIME++;
		}
	_delay_ms(20); //för test?
		
	DISTANCE = TIME/58;
	return DISTANCE;	
}