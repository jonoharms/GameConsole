/*
 * buttons.c
 *
 * Created: 11/10/2014 4:05:33 PM
 *  Author: Jonathon
 */ 

#include "buttons.h"

void init_buttons(void) {
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
}