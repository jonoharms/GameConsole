/*
 * interupt.c
 *
 * Created: 11/10/2014 4:18:45 PM
 *  Author: Jonathon
 */ 

#include "interrupt.h"

void init_ext_interrupts(void) {
	//INTERRUPTS
	INTERRUPT_DIR(IN); //Set UP_BUTTON I/Os as input.
	INTERRUPT_PULLUP_ENABLE;
	INTERRUPT_ENABLE;
	INTERRUPT_SET_LOGIC_TRIGGER;
	//INTERRUPT_SET_RISING_TRIGGER;
	//INTERRUPT_SET_FALLING_TRIGGER;
	
}
