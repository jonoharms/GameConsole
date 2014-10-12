/*
 * GameBoy.c
 *
 * Created: 6/10/2014 9:00:26 PM
 *  Author: Jonathon Harms 389212
 */ 

#include "gameboy.h" 
#include "spi.h"
#include "gpio.h"
#include "interrupt.h"
#include "lcd.h"
#include "fram.h"

byte etch(void);

byte buffer[MAX_COLUMNS][MAX_PAGES];
int16_t x = 0;
int16_t y = 0;

int main(void)
{

	init_gpio();
	init_spi();
	init_ext_interrupts();
	init_lcd();

	sei();
	while(TRUE){
		
		
	}
}

ISR(INT1_vect){
	BAT_LOW_LED(~BAT_LOW_LED_VAL);
	etch();
}


byte etch(void){
	byte delay = 20;
	while(INTERRUPT) {
		if(UP_BUTTON) {
			y--;
		}
		if(DOWN_BUTTON) {
			y++;
		}
		if(LEFT_BUTTON) {
			x--;
		}
		if(RIGHT_BUTTON) {
			x++;
		}
		if(A_BUTTON) {
			clearbuffer(buffer);
			set_all_lcd_pages(OFF);
		}
		if(y<0) {
			y = LCDHEIGHT;
		}
		if (y>LCDHEIGHT) {
			y = 0;
		}
		if (x<0) {
			x = LCDWIDTH;
		}
		if (x>LCDWIDTH) {
			x = 0;
		}
		
		
		
		setpixel(buffer,x,y);
		_delay_ms(delay);
	}
}













