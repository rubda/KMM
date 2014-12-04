/*
 * SPI.c
 *
 * Created: 2014-11-05 09:20:52
 *  Author: RubenDas
 */ 

#include "SPI.h"

void setup_spi()
{
	SPCR |= (1 << MSTR); // AVR blir master
	SPCR |= (1 << SPE); // Enable SPI
	SPCR |= (SPI_MSB << DORD); // MSB först
	
	SPCR |= (1 << CPOL); // CPOL = 1
	SPCR |= (1 << CPHA); // CPHA = 1
	SPCR |= (1 << SPR0); // SPR0 = 1
	SPCR |= (1 << SPR1); // SPR1 = 1
	
	DDRB |= (1 << SPI_MOSI); // output
	DDRB &= ~(1 << SPI_MISO); // input
	DDRB |= (1 << SPI_SS); // output
	DDRB |= (1 << SPI_SCLK); // output
	
	ss_high();
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


//Skickar totalt 16 bitars data och tar emot 16 bitars data
uint16_t get_spi(uint8_t data)
{
	uint16_t REG = 0;
	uint16_t REG1 = 0;
	uint16_t REG2 = 0;
	
	REG1 = send_spi(data);
	REG1 = REG1 << 8;
	REG2 = send_spi(data);
	REG = REG1 | REG2;
	
	return REG;
}

void ss_low()
{
	PORTB &= ~(1 << SPI_SS);
}	

void ss_high()
{
	PORTB |= (1 << SPI_SS);
}