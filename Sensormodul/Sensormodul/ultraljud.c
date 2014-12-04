/*
 * ultraljud.c
 *
 * Created: 2014-11-05 19:58:28
 *  Author: RubenDas
 */ 

#include "ultraljud.h"
#include <avr/io.h>

//Initierar alla avst�ndssensorer 
void init_sensors()
{
	
	//S�tt PA0, A2, A3, A4 till ut
	DDRA |= (1 << PORTA4);
	DDRA |= (1 << PORTA3);
	DDRA |= (1 << PORTA2);
	DDRA |= (1 << PORTA0);

	//S�tt PA1 till in	
	DDRA &= ~(1 << PORTA1);
	
	//B0 till ut
	DDRB |= (1 << PORTB0);
	
	sensor_list[3] = (struct soundSensor) {0b00000000, 1, 0, 0, 0, 0, 0, 0};
	sensor_list[2] = (struct soundSensor) {0b00000100, 1, 0, 0, 0, 0, 0, 0};
	sensor_list[1] = (struct soundSensor) {0b00001000, 1, 0, 0, 0, 0, 0, 0};
	sensor_list[0] = (struct soundSensor) {0b00001100, 1, 0, 0, 0, 0, 0, 0};
	sensor_list[4] = (struct soundSensor) {0b00010000, 1, 0, 0, 0, 0, 0, 0};
	sensor_list[5] = (struct soundSensor) {0b00010100, 1, 0, 0, 0, 0, 0, 0};
}

//Returnerar en viss sensor struct fr�n v�r lista av sensorer
struct soundSensor *get_sensor(uint8_t id)
{
	return &sensor_list[id];
}

void get_distance(struct soundSensor* sensor)
{
	uint16_t DISTANCE;
	uint16_t TIME = 0;
	
	// M�ste se till att r�tt bitar �ndras f�rst
	PORTA = PORTA & 0b11100011;
	
	// Sen orar vi in ett specifikt ultraljud
	PORTA = PORTA | sensor->id;
	
	// Startar en m�tning fr�n ett ultraljud
	PORTA |= (1 << PORTA0);
 	_delay_us(15);
 	PORTA &= ~(1 << PORTA0);

	// M�ter tiden det tar f�r att f� respons
	while (!(PINA & (1 << PINA1)) && TIME++ < 36000) _delay_us(1);
	TIME = 0;
	while ((PINA & (1 << PINA1)) && TIME < 36000)
		{	
			_delay_us(1);
			TIME++;
		}
	_delay_ms(5); 
			
	DISTANCE = TIME/40; //G�r om det till ett avst�nd uttryckt i cm
	
	//R�tt s� sj�lvf�rklarande 
	switch(sensor->nr)
	{
		case 1:
			sensor->Dist1 = DISTANCE;
			sensor->nr++;
			break;
		case 2:
			sensor->Dist2 = DISTANCE;
			sensor->nr++;
			break;
		case 3:
			sensor->Dist3 = DISTANCE;
			sensor->nr++;
			break;
		case 4:
			sensor->Dist4 = DISTANCE;
			sensor->nr++;
			break;
		case 5:
			sensor->Dist5 = DISTANCE;
			sensor->nr = 1;
			break;
		default:
			break;
	}
	
	uint16_t VALUES[] = {sensor->Dist1, sensor->Dist2, sensor->Dist3, sensor->Dist4, sensor->Dist5};
	
	//Sorterar distanserna och tar ut mittenv�rdet 
	qsort(VALUES, 5, sizeof(uint16_t), cmpfunc);
	
	sensor->medDist = VALUES[2];
}

// En j�mf�relsefunktion som kr�vs f�r att quicksorten ska funka
uint16_t cmpfunc (const void * a, const void * b)
{
	return ( *(uint16_t*)a - *(uint16_t*)b );
}

