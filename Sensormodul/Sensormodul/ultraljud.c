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
	
	sensor_list[3] = (struct soundSensor) {0b00000000, 0, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[2] = (struct soundSensor) {0b00000100, 0, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[1] = (struct soundSensor) {0b00001000, 0, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[0] = (struct soundSensor) {0b00001100, 0, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[4] = (struct soundSensor) {0b00010000, 0, 0, 0, {0, 0, 0, 0, 0}};
	sensor_list[5] = (struct soundSensor) {0b00010100, 0, 0, 0, {0, 0, 0, 0, 0}};
		
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
	sensor->Dists[sensor->nr] = DISTANCE;	
	sensor->nr = (sensor->nr + 1) % 5;
	sensor->Dist = DISTANCE;

	//Kopierar arrayen
	int VALUES[] = {sensor->Dists[0], sensor->Dists[1], sensor->Dists[2], sensor->Dists[3], sensor->Dists[4]};
	//int weakVALUES[] = {sensor->Dists[0], sensor->Dists[1], sensor->Dists[2]};
	
	//Sorterar distanserna och tar ut mittenvärdet 	
	insertion_sort(VALUES, 5);
	sensor->medDist = VALUES[2];
	
	//Kass "optimering"
/*	if (sensor->nr == 3){ 
		insertion_sort(weakVALUES, 3);
		sensor->weakDist = weakVALUES[1];
		}*/		
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

//Uppdaterar varje sensor endast tre gånger
void refresh_less()
{
	int order[] = {0, 2, 4, 1, 3, 5};
	int i, j;
	
	for(j = 0; j < 6; ++j){
		get_sensor(j)->nr = 0;
	}
	
	for(j = 0; j < 3; ++j){
		for(i = 0; i < 6; ++i){
			get_distance(get_sensor(order[i]));
		}
	}
}

//Insertion sort
void insertion_sort(int *a, const size_t n) {
	size_t i, j;
	int value;
	for (i = 1; i < n; i++) {
		value = a[i];
		for (j = i; j > 0 && value < a[j - 1]; j--) {
			a[j] = a[j - 1];
		}
		a[j] = value;
	}
}


void median_of_dists(int n)
{
	int order[] = {0, 2, 4, 1, 3, 5};
	int values[6][n];
	int i, j;

	for(j = 0; j < n; ++j){
		for(i = 0; i < 6; ++i){
			get_distance(get_sensor(order[i]));
			values[order[i]][j]= get_sensor(order[i])->Dist;
		}
		insertion_sort(values[order[i]], n);
		get_sensor(order[i])->medDist = values[order[i]][n/2];
	}
}
