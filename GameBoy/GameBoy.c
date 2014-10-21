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


uint16_t count = 0;
uint16_t count2 = 0;
byte buffer[MAX_COLUMNS][MAX_PAGES];
int16_t x;
int16_t y;
byte brightness = 50;
byte width = 1;
uint16_t x_address = 0x0000;
uint16_t y_address = 0x0002;
uint16_t buffer_address = 0x0100;

int main(void)
{
	clearbuffer(buffer);
	x = LCDWIDTH/2;
	y = LCDHEIGHT/2;
	brightness = 50;
	width = 1;
	BACKLIGHT_BRIGHTNESS(brightness);
						
	init_gpio();
	init_spi();
	init_ext_interrupts();
	init_lcd();
	init_fram();
	
	char buf[]="WELCOME";
	drawstring(buffer, 0, 0, (byte *) buf); 
	_delay_ms(1000);
	
	byte x_byte[2];
	byte y_byte[2];
	read_fram(x_address,x_byte,2);
	read_fram(y_address,y_byte,2);
	
	for(byte i=0; i<MAX_COLUMNS; i++) {
		read_fram(buffer_address+i*MAX_COLUMNS, buffer[i], MAX_PAGES);
	}	
	memcpy(&x,x_byte,2);
	memcpy(&y,y_byte,2);
	//y = pos[1];
	set_all_lcd_pages(OFF);
	write_buffer(buffer);
	
	ADCSRA |= 1<<ADSC;
	sei();	
	while(TRUE){

	}
}

ISR(INT1_vect){
	etch();
}

ISR(TIMER0_OVF_vect) {
	cli();
	count2++;
	byte x_byte[2];
	byte y_byte[2];
	memcpy(x_byte,&x,2);
	memcpy(y_byte,&y,2);

	if (count2>10) {
		for(byte i=0; i<MAX_COLUMNS; i++) {
			write_fram(buffer_address+i*MAX_COLUMNS, buffer[i], MAX_PAGES);
		}
		write_fram(x_address, x_byte , 2);
		write_fram(y_address, y_byte , 2);
		count2 = 0;
	}
	sei();
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
			glcd_rect(buffer, x-width/2 , y-width/2, x+width/2 , y+width/2 , TRUE);
		}
		_delay_ms(20);
	}
	return (TRUE);
}













