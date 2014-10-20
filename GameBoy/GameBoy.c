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
int16_t x = 51;
int16_t y = 32;
uint16_t count = 0;
byte brightness = 50;
byte width = 1;

int main(void)
{

	init_gpio();
	init_spi();
	init_ext_interrupts();
	init_lcd();
	init_fram();
	BACKLIGHT_BRIGHTNESS(brightness);
	char buf[]="WELCOME STRANGER";
	
	uint16_t address = 0x0001;

	write_fram(address,(byte *)buf,14);
	drawstring(buffer, 0, 0, (byte *) buf); 

    read_fram(address,(byte *)buf,14);
	drawstring(buffer, 0, 2, (byte *)  buf); 
	
	_delay_ms(1000);
	clearbuffer(buffer);
	set_all_lcd_pages(OFF);
	
	ADCSRA |= 1<<ADSC;
	
	sei();
	while(TRUE){
		
		
	}
}

ISR(INT1_vect){
	etch();
}

ISR(TIMER0_OVF_vect){
	// DO NOTHING EXCEPT CLEAR THE FLAG
}

ISR(ADC_vect)
{
	uint16_t val;
	byte lsb = ADCL;
	byte msb = ADCH;
	val = (msb << 8) | lsb ;
	char s[15];
	float voltage = ((float)val)*2.56/1024.0;
	count++;
	if ((voltage < 1.1 ) && (count>30) ) {
		BAT_LOW_LED(ON);
		sprintf(s, "LowBatt: %.2fV", voltage);
		s[14] = 0x00;
		count = 0;
		drawstring(buffer, 0,7,(byte*) s);
	}
}


byte etch(void){

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
			x = 51;
			y = 32;
			width = 1;
			clearbuffer(buffer);
			set_all_lcd_pages(OFF);
		}
		if(B_BUTTON) {
			if (brightness == 0) {
				brightness = 255;
			} else if (brightness < 25) {
				brightness = 0;
			} else {
				brightness -= 25;
			}
			BACKLIGHT_BRIGHTNESS(brightness);
			_delay_ms(200);
		}
		if(C_BUTTON) {
			if (width > 8) {
				width = 1;
			} else {
				width +=2;
			}
			_delay_ms(200);
		}
		if((y-width/2)<0) {
			y = LCDHEIGHT-1-width/2;
		}
		if ((y+width/2)>=LCDHEIGHT) {
			y = width/2;
		}
		if ((x-width/2)<0) {
			x = LCDWIDTH-1-width/2;
		}
		if ((x+width/2)>=LCDWIDTH) {
			x = width/2;
		}
		//
		if (width < 2) {
			setpixel(buffer,(byte)x,(byte)y);
		} else {
			glcd_rect(buffer,  x-width/2 , y-width/2, x+width/2 , y+width/2 , TRUE);
		}
		_delay_ms(20);
	}
	return (TRUE);
}













