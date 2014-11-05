/*
 * gyro.c
 *
 * Created: 2014-11-05 10:53:50
 *  Author: RubenDas
 */ 

#include "gyro.h"
#include "SPI.h"

int getAngularRate(uint8_t adcValue)
{
	SPCR |= (1 << MSTR);
	
}	
	