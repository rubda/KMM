/*
 * SPI.c
 *
 * Created: 2014-11-05 09:20:52
 *  Author: RubenDas
 */ 

#include "SPI.h"'

void setup_spi()
{
	SPCR |= (1 << MSTR); // AVR blir master
	SPCR |= (1 << SPE); // Enable SPI?
	SPCR |= (SPI_MSB << DORD); // MSB först
	
	DDRB |= (1 << SPI_MOSI_PIN) // output
	DDRB &= ~(1 << SPI_MISO_PIN) // input
	DDRB |= (1 << SPI_SS_PIN) // output
	DDRB |= (1 << SPI_SS_PIN) // output
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

void ss_low()
{
	PORTB &= ~(1 << SPI_SS_PIN);
}	

void ss_high()
{
	PORTB |= (1 << SPI_SS_PIN);
}