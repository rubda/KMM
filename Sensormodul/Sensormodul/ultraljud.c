/*
 * ultraljud.c
 *
 * Created: 2014-11-05 19:58:28
 *  Author: RubenDas
 */ 

#include "ultraljud.h"
#include <avr/io.h>

//Initierar alla avståndssensorer 
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
	
	sensor_list[3] = (struct soundSensor) {0b00000000, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[2] = (struct soundSensor) {0b00000100, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[1] = (struct soundSensor) {0b00001000, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[0] = (struct soundSensor) {0b00001100, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[4] = (struct soundSensor) {0b00010000, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[5] = (struct soundSensor) {0b00010100, 0, 0, {0, 0, 0, 0, 0}};
		
	refresh_sensors();
}

//Returnerar en viss sensor struct från vår lista av sensorer
struct soundSensor *get_sensor(uint8_t id)
{
	return &sensor_list[id];
}

void get_distance(struct soundSensor* sensor)
{
	uint16_t DISTANCE;
	uint16_t TIME = 0;
	
	// Måste se till att rätt bitar ändras först
	PORTA = PORTA & 0b11100011;
	
	// Sen orar vi in ett specifikt ultraljud
	PORTA = PORTA | sensor->id;
	
	// Startar en mätning från ett ultraljud
	PORTA |= (1 << PORTA0);
 	_delay_us(15);
 	PORTA &= ~(1 << PORTA0);

	// Mäter tiden det tar för att få respons
	while (!(PINA & (1 << PINA1)) && TIME++ < 36000) _delay_us(1);
	TIME = 0;
	while ((PINA & (1 << PINA1)) && TIME < 36000)
		{	
			_delay_us(1);
			TIME++;
		}
	_delay_ms(30); 
			
	DISTANCE = TIME/40; //Gör om det till ett avstånd uttryckt i cm
	
	//Lägger till den nya distansen för en specifik plats i listan
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
	
	//Sorterar distanserna och tar ut mittenvärdet 
	qsort(VALUES, 5, sizeof(uint16_t), cmpfunc);
	
	sensor->medDist = VALUES[2];
}

// En jämförelsefunktion som krävs för att quicksorten ska funka
uint16_t cmpfunc (const void * a, const void * b)
{
	return (*(uint16_t*)a - *(uint16_t*)b);
}


//Uppdaterar alla sensorer fem gånger var, på en gång
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