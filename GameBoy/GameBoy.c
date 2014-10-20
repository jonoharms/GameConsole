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

typedef struct game_t {
	byte buffer[MAX_COLUMNS][MAX_PAGES];
	int16_t x;
	int16_t y;
	byte brightness;
	byte width;
} game_t;

game_t game;
uint16_t count;

int main(void)
{
	clearbuffer(game.buffer);
	game.x = LCDWIDTH/2;
	game.y = LCDHEIGHT/2;
	game.brightness = 50;
	game.width = 1;
	BACKLIGHT_BRIGHTNESS(game.brightness);
	
	byte b[sizeof(game)];
	memcpy(b, &game, sizeof(game));
//	write_fram(0x0000, b, sizeof(game));
	
	_delay_ms(1000);

//	read_fram(0x0000, b, sizeof(game));
	memcpy(&game, b, sizeof(game));
	
					
	init_gpio();
	init_spi();
	init_ext_interrupts();
	init_lcd();
	init_fram();
	
	
	char buf[]="WELCOME STRANGER";
	
	uint16_t address = 0x0001;

	write_fram(address,(byte *)buf,14);
	drawstring(game.buffer, 0, 0, (byte *) buf); 

    read_fram(address,(byte *)buf,14);
	drawstring(game.buffer, 0, 2, (byte *)  buf); 
	
	_delay_ms(1000);
	clearbuffer(game.buffer);
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
		drawstring(game.buffer, 0,7,(byte*) s);
	}
}


byte etch(void){

	while(INTERRUPT) {
		if(UP_BUTTON) {
			game.y--;
		}
		if(DOWN_BUTTON) {
			game.y++;
		}
		if(LEFT_BUTTON) {
			game.x--;
		}
		if(RIGHT_BUTTON) {
			game.x++;
		}
		if(A_BUTTON) {
			game.x = 51;
			game.y = 32;
			game.width = 1;
			clearbuffer(game.buffer);
			set_all_lcd_pages(OFF);
		}
		if(B_BUTTON) {
			if (game.brightness == 0) {
				game.brightness = 255;
			} else if (game.brightness < 25) {
				game.brightness = 0;
			} else {
				game.brightness -= 25;
			}
			BACKLIGHT_BRIGHTNESS(game.brightness);

			_delay_ms(200);
		}
		if(C_BUTTON) {
			if (game.width > 8) {
				game.width = 1;
			} else {
				game.width +=2;
			}
			_delay_ms(200);
		}
		if((game.y-game.width/2)<0) {
			game.y = LCDHEIGHT-1-game.width/2;
		}
		if ((game.y+game.width/2)>=LCDHEIGHT) {
			game.y = game.width/2;
		}
		if ((game.x-game.width/2)<0) {
			game.x = LCDWIDTH-1-game.width/2;
		}
		if ((game.x+game.width/2)>=LCDWIDTH) {
			game.x = game.width/2;
		}
		//
		if (game.width < 2) {
			setpixel(game.buffer,(byte)game.x,(byte)game.y);
		} else {
			glcd_rect(game.buffer, game.x-game.width/2 , game.y-game.width/2, game.x+game.width/2 , game.y+game.width/2 , TRUE);
		}
		_delay_ms(20);
	}
	return (TRUE);
}













