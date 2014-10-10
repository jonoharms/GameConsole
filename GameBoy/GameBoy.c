/*
 * GameBoy.c
 *
 * Created: 6/10/2014 9:00:26 PM
 *  Author: Jonathon
 */ 

#include "gameboy.h" 





//#define WRITE_PORTC_BIT6(x)  (PORTC = (PORTC & ~_BV(PINC6)) | ((x) << PINC6))
#define WRITE_PORTD_BIT4(x)  (PORTD = (PORTD & ~_BV(PIND4)) | ((x) << PIND4))


int main(void)
{

	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.
	BACKLIGHT_SET(OFF);
	BACKLIGHT_DIR(OUT);
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
		}
		else
		{
			BAT_LOW_LED(OFF);
		}
	}
}