#include <avr/io.h>

/*
 * SPI.h
 *
 * Created: 2014-11-05 09:21:21
 *  Author: RubenDas
 */ 


#ifndef SPI_H_
#define SPI_H_

// Alla modes beroende på klocka och polaritet 
#define SPI_MODE_0 0b00 /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=0 */
#define SPI_MODE_1 0b01 /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=1 */
#define SPI_MODE_2 0b10 /* Sample (Rising) Setup (Falling) CPOL=1, CPHA=0 */
#define SPI_MODE_3 0b11 /* Sample (Rising) Setup (Falling) CPOL=1, CPHA=1 */

// Definierar alla pins
#define SPI_SS_PIN PORTB4
#define SPI_MOSI_PIN PORTB5
#define SPI_MISO_PIN PINB6
#define SPI_SCLK_PIN PORTB7

// Data directions
#define SPI_LSB 1 // Skickar minst signifikant bit först (bör nog aldrig göras)
#define SPI_MSB 0 // Skickar mest signifikant bit först

void setup_spi();
void disable_spi();
uint8_t send_spi(uint8_t out);
uint8_t get_spi(uint8_t data);
void ss_low();
void ss_high();

#endif /* SPI_H_ */