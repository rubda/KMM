/*
 * gyro.c
 *
 * Created: 2014-11-05 10:53:50
 *  Author: RubenDas
 */ 

#define F_CPU 8000000UL

#include "gyro.h"
#include "SPI.h"


#include <util/delay.h>
int IS_ROTATED = 0; 



void activate_adc()
{
	uint16_t REG = 0;
	
	//Steg 1 s�tt ADC till aktiv mode
	ss_low();
	send_spi(ACTIVATE_ADC);
	REG = get_spi(0xFF);
	ss_high();
	
	if (REG & (1 << 15)) activate_adc();
	REG = 0;
	_delay_us(200);
}

void deactivate_adc()
{
	uint16_t REG = 0;
	
	//Avaktivera adc omvandlingen
	ss_low();
	send_spi(DEACTIVATE_ADC);
	REG = get_spi(0xFF);
	ss_high();
	
	if (REG & (1 << 15)) deactivate_adc();
	REG = 0;
}

void start_conversion()
{
	uint16_t REG = 0;
	
	//Steg 2 conversion
	ss_low();
	send_spi(START_CONVERSION);
	REG = get_spi(0xFF);
	ss_high();
	if (REG & (1 << 15)) start_conversion();
	REG = 0;
}


uint16_t get_angular_rate()
{
	uint16_t REG = 0;
	uint16_t DATA = 0;
	
	//Steg 3 poll
	ss_low();
	send_spi(POLL);
	_delay_us(200);
	
	//if (REG & (1 << 16)) get_angular_rate();
	REG = get_spi(0xFF);
	ss_high();
	
	REG = REG >> 1;
	DATA = REG & 0x07FF; 
	
	return DATA;		
}	

int adc_to_angular_rate(uint16_t data)
	{
		int OFFSET = 2500; //2500
		
		int vOutAngularRate = (data * 25/12)+400; //Uttryckt i millivolts
		
		return (vOutAngularRate - OFFSET)/6.67; // Uttryckt i grader/sec (beroende p� vilken gyro modell vi har)
	}
	
	
int rotate_to(int angle)
{
	activate_adc();
	uint16_t rate;
	int ACHIEVED_ANGLE = 0;
	int OFFSET = 0;
	int LEFT_ANGLE = (angle - OFFSET)/2;
	int RIGHT_ANGLE = (angle + OFFSET)/2;
		
	if (angle > 0){
		while(LEFT_ANGLE > ACHIEVED_ANGLE)
		{		
			start_conversion();
			rate = get_angular_rate();
			ACHIEVED_ANGLE += adc_to_angular_rate(rate)/100;
			_delay_ms(10);
		}
		has_rotated(1);
	}else if (angle < 0){
		while(ACHIEVED_ANGLE > RIGHT_ANGLE)
		{
			start_conversion();
			rate = get_angular_rate();
			ACHIEVED_ANGLE += adc_to_angular_rate(rate)/100;
			_delay_ms(10);
		}
		has_rotated(1);
	}
	deactivate_adc();	
	return ACHIEVED_ANGLE*2;
}

void has_rotated(int bit)
{
	IS_ROTATED = bit;
}

int get_is_rotated()
{
	return IS_ROTATED;
}
