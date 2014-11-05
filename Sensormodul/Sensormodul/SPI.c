/*
 * SPI.c
 *
 * Created: 2014-11-05 09:20:52
 *  Author: RubenDas
 */ 

#include "SPI.h"'

void setup_spi()
{
	DDRB |= (1 << SPI_MOSI_PIN) // output
	DDRB &= ~(1 << SPI_MISO_PIN) // input
	DDRB |= (1 << SPI_SS_PIN) // output
	DDRB |= (1 << SPI_SS_PIN) //output
}

void disable_spi()
{
	SPCR = 0x00;
}

uint8_t send_spi(uint8_t out)
{
	SPDR = out;
	while (!(SPSR & (1<<SPIF))); //Loopar sålänge vi inte har skickat klart
	return SPDR;
}

uint8_t get_from_spi(uint8_t data)
{
	SPDR = data;
	return SPDR;
}

	
