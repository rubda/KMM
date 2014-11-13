/*
 * timer.h
 *
 * Created: 2014-11-06 14:11:23
 *  Author: RubenDas
 */ 


#ifndef TIMER_H_
#define TIMER_H_


#include <avr/io.h>
#include <avr/interrupt.h>

#define ENABLE_TIMER

#define TIMER_PRESCALER_NONE 0
#define TIMER_PRESCALER_1 1
#define TIMER_PRESCALER_8 2
#define TIMER_PRESCALER_64 3
#define TIMER_PRESCALER_256 4
#define TIMER_PRESCALER_1024 5

void timer(uint8_t prescaler, uint16_t ticks, void (*f)());
void timer_stop();
void CHILLAXA(uint8_t prescaler, uint16_t ticks);



#endif /* TIMER_H_ */