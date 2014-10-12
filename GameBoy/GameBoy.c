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
byte delay = 20;

int main(void)
{

	init_gpio();
	init_spi();
	init_ext_interrupts();
	init_lcd();
	init_fram();
	
	byte data = 0x07;
	byte s[30];
	uint16_t address = 0x0001;
	
	write_fram(address,data,buffer);
//	write_fram_sr();
	byte wc = SPSR;
	draw_byte(buffer, 0, 0, data); 
	byte out = read_fram_status();
	draw_byte(buffer, 0, 1, out); 
	byte out2 = read_fram(address);
	sprintf(s,"%03d",out2);
	draw_byte(buffer, 0, 2, out2); 
	//sei();
	while(TRUE){
		
		
	}
}

ISR(INT1_vect){
	BAT_LOW_LED(~BAT_LOW_LED_VAL);
	//etch();
}


byte etch(void){
	byte del[10];
	sprintf(del,"%03d",delay);
	drawstring(buffer,0,0,del);
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
		if(B_BUTTON) {
			delay += 5;
			_delay_ms(200);
		}
		if(C_BUTTON) {
			delay -= 5;
			_delay_ms(200);
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
	return (TRUE);
}













