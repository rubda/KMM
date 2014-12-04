#include <avr/io.h>

/*
 * SPI.h
 *
 * Created: 2014-11-05 09:21:21
 *  Author: RubenDas
 */ 


#ifndef SPI_H_
#define SPI_H_

// Definierar alla pins
#define SPI_SS PORTB4
#define SPI_MOSI PORTB5
#define SPI_MISO PINB6
#define SPI_SCLK PORTB7

// Data directions
#define SPI_LSB 1 // Skickar minst signifikant bit först (bör nog aldrig göras)
#define SPI_MSB 0 // Skickar mest signifikant bit först

void setup_spi();
void disable_spi();
uint8_t send_spi(uint8_t out);
uint16_t get_spi(uint8_t data);
void ss_low();
void ss_high();

#endif /* SPI_H_ */