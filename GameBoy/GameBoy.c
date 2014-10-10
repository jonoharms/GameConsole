/*
 * GameBoy.c
 *
 * Created: 6/10/2014 9:00:26 PM
 *  Author: Jonathon
 surface
 */ 

#include "gameboy.h" 


int main(void)
{

	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.
	
	BACKLIGHT_SET(OFF);
	BACKLIGHT_DIR(OUT);
	SET_UP_TIMER_REG();
	PRESCALER_64();
	
	
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
	INTERRUPT_DIR(IN); //Set UP_BUTTON I/Os as input.
	INTERRUPT_SIG_PULLUP();

	BACKLIGHT_SET(ON);
	
	while (TRUE)
	{
		if (INTERRUPT_SIG)
		{
			BAT_LOW_LED(ON);
			OCR1B = 255; //beep on
		}
		else
		{
			BAT_LOW_LED(OFF);
			OCR1B = 15; //beep on
		}
	}
}