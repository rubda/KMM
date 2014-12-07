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
	
	sensor_list[3] = (struct soundSensor) {0b00000000, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[2] = (struct soundSensor) {0b00000100, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[1] = (struct soundSensor) {0b00001000, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[0] = (struct soundSensor) {0b00001100, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[4] = (struct soundSensor) {0b00010000, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[5] = (struct soundSensor) {0b00010100, 0, 0, {0, 0, 0, 0, 0}};
		
	refresh_sensors();
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
	_delay_ms(30); 
			
	DISTANCE = TIME/40; //G�r om det till ett avst�nd uttryckt i cm
	
	//L�gger till den nya distansen f�r en specifik plats i listan
	int NR = sensor->nr;
	sensor->Dists[NR] = DISTANCE;	
	
	if (NR == 4) {
		sensor->nr = 0;
	}else{
		sensor->nr++;
	};
	
	//Kopierar arrayen
	int VALUES[5];
	for (int i = 0; i < 5; i++) {
		   VALUES[i] = sensor->Dists[i];
	}
	
	//Sorterar distanserna och tar ut mittenv�rdet 
	qsort(VALUES, 5, sizeof(uint16_t), cmpfunc);
	
	sensor->medDist = VALUES[2];
}

// En j�mf�relsefunktion som kr�vs f�r att quicksorten ska funka
uint16_t cmpfunc (const void * a, const void * b)
{
	return (*(uint16_t*)a - *(uint16_t*)b);
}


//Uppdaterar alla sensorer fem g�nger var, p� en g�ng
void refresh_sensors()
{
	int order[] = {0, 2, 4, 1, 3, 5};
	int i, j;
	
	for(j = 0; j < 5; ++j){
		for(i = 0; i < 6; ++i){
			get_distance(get_sensor(order[i]));
		}
	}
}