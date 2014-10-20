/*
 * buttons.c
 *
 * Created: 11/10/2014 4:05:33 PM
 *  Author: Jonathon
 */ 

#include "gpio.h"

void init_gpio(void) {
	UP_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	UP_BUTTON_PULLUP();
	DOWN_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	DOWN_BUTTON_PULLUP();
	LEFT_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	LEFT_BUTTON_PULLUP();
	RIGHT_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	RIGHT_BUTTON_PULLUP();
	A_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	A_BUTTON_PULLUP();
	B_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	B_BUTTON_PULLUP();
	C_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	C_BUTTON_PULLUP();
	VBATT_DIR(IN);
	
	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.
	
	SET(TCCR0,( _BV(CS02) | _BV(CS00) ), ON);		// SET 1024 Prescaler
	SET(TIMSK, _BV(TOIE0), ON);					// ENABLE INTERRUPT OVERFLOW	
	
	SET(ADCSRA, _BV(ADEN), ON);					// ENABLE ADC
	SET(ADCSRA, _BV(ADATE), ON);				// ENABLE Auto Trigger
	SET(SFIOR, _BV(ADTS2), ON);					// TRIGGER ON TIMER0 OVERFLOW
	SET(ADCSRA, _BV(ADIE), ON);					// ENABLE ADC INTERRUPT
	SET(ADCSRA, (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0)), ON);					// SET PRESCALER /128 (62.5 kHz)

	SET(ADMUX,( _BV(REFS1) | _BV(REFS0) ),ON);  // ENABLE INTERNAL VREF WITH CAP
	//SET(ADMUX,( _BV(ADLAR) ),ON);				// ENABLE LEFT JUSTIFY
	SET(ADMUX, ( _BV(MUX1) | _BV(MUX0) ),ON);	// SELECT PA3 as INPUT
}