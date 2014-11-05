/*
 * SPI.h
 *
 * Created: 2014-11-05 09:21:21
 *  Author: RubenDas
 */ 


#ifndef SPI_H_
#define SPI_H_

//Alla modes beroende på klocka och polaritet 
#define SPI_MODE_0 0b00; /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=0 */
#define SPI_MODE_1 0b01; /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=1 */
#define SPI_mode_2 0b10; /* Sample (Rising) Setup (Falling) CPOL=1, CPHA=0 */
#define SPI_MODE_3 0b11; /* Sample (Rising) Setup (Falling) CPOL=1, CPHA=1 */

//Definierar alla pins
#define SPI_SS_PIN PORTB4;
#define SPI_MOSI_PIN PORTB5;
#define SPI_MISO_PIN PORTB6;
#define SPI_SCLK_PIN PORTB7;


void setup_spi();





#endif /* SPI_H_ */