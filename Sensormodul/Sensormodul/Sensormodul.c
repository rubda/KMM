/*
 * Sensormodul.c
 *
 * Created: 2014-11-05 09:10:19
 *  Author: RubenDas
 */ 

#define F_CPU 7372800UL

#include <avr/io.h>
#include "SPI.h"
#include "gyro.h"
#include <util/delay.h>


int main(void)
{
<<<<<<< HEAD

    while(1)
    {	
		
		ss_high();
	
=======
    
	setup_spi();
	
	while(1)
    {
        ss_low();
>>>>>>> df85c5717b1029083b513e6142062a42380832ee
    }
}

