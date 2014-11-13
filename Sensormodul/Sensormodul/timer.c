/*
 * timer.c
 *
 * Created: 2014-11-06 14:11:10
 *  Author: RubenDas
 */ 

#include "timer.h"

void (*_t_func)();

void timer(uint8_t prescaler, uint16_t ticks, void (*f)()) {
	TIMSK1 &= ~(_BV(OCIE1A));
	_t_func = f;
	OCR1A = ticks;
	TCCR1A = 0;
	TCCR1B = prescaler | _BV(WGM12);
	TCNT1 = 0;
	TIMSK1 |= _BV(OCIE1A);
}

void timer_stop() {
	TCCR1B = 0;
}

#ifdef ENABLE_TIMER
ISR(TIMER1_COMPA_vect) {
	_t_func();
}
#endif

void CHILLAXA(uint8_t prescaler, uint16_t ticks) {
	if (ticks == 0)
	return;
	TIMSK1 &= ~(_BV(OCIE1A));
	OCR1A = ticks;
	TCCR1A = 0;
	TCCR1B = prescaler | _BV(WGM12);
	TCNT1 = 0;
	TIFR1 |= _BV(OCF1A);
	while(!(TIFR1 & _BV(OCF1A)));
	TCCR1B = 0;
}

