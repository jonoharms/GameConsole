/*
 * interrupt.h
 *
 * Created: 11/10/2014 4:19:06 PM
 *  Author: Jonathon
 */ 
#include "gameboy.h"

//INTERRUPT SETUP
#define INTERRUPT_DIR(DIR) SET(DDRD,_BV(PIND3),DIR)
#define INTERRUPT GET(PIND,_BV(PIND3))
#define INTERRUPT_PULLUP_ENABLE SET(PORTD, _BV(PIND3),ON)
#define INTERRUPT_ENABLE SET(GICR, _BV(INT1),ON)
#define INTERRUPT_DISABLE SET(GICR, _BV(INT1),OFF)
#define INTERRUPT_SET_LOGIC_TRIGGER (SET(MCUCR,_BV(ISC10),ON))
#define INTERRUPT_SET_FALLING_TRIGGER (SET(MCUCR,_BV(ISC11),ON))
#define INTERRUPT_SET_RISING_TRIGGER (SET(MCUCR,(_BV(ISC11)|_BV(ISC10)),ON))

//INTERRUPT FUNCS
void init_ext_interrupts(void);