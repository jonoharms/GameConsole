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

struct game_t {
	int16_t x;
	int16_t y;
	byte brightness;
	byte width;
};

uint16_t count = 0;
uint16_t count2 = 0;
byte buffer[BUFFER_SIZE];
struct game_t game;

uint16_t game_address = 0x0400;
uint16_t buffer_address = 0x0000;

int main(void)
{
	
	init_gpio();
	init_spi();
	init_ext_interrupts();
	init_lcd();
	init_fram();
	clearbuffer(buffer);

				
	char wel[]="WELCOME";
	drawstring(buffer, 0, 0, (byte *) wel); 
	
	char a_load[]="PRESS A TO LOAD OLD GAME";
	drawstring(buffer, 0, 1, (byte *) a_load);
	
	char b_load[]="PRESS B TO START NEW GAME";
	drawstring(buffer, 0, 3, (byte *) b_load);
	

	while(1) {
		if (A_BUTTON) {
			clearbuffer(buffer);
			byte b[sizeof(game)];
			read_fram(game_address,b,sizeof(game));
			memcpy(&game,b,sizeof(game));
			BACKLIGHT_BRIGHTNESS(game.brightness);
			read_fram(buffer_address, buffer, BUFFER_SIZE);
			write_buffer(buffer);
			break;
		} else if (B_BUTTON) {
			game.x = LCDWIDTH/2;
			game.y = LCDHEIGHT/2;
			game.brightness = 50;
			game.width = 1;
			clearbuffer(buffer);
			write_buffer(buffer);
			BACKLIGHT_BRIGHTNESS(game.brightness);
			break;
		}
		_delay_ms(30);
	}
	
	_delay_ms(1000);
	ADCSRA |= 1<<ADSC;
	sei();	
	while(TRUE){

	}
}

ISR(INT1_vect){
	etch();
}

ISR(TIMER0_OVF_vect) {
	count2++;
	
	if (count2>10) {
		byte b[sizeof(game)];
		memcpy(b,&game,sizeof(game));
		write_fram(game_address, b ,sizeof(game));
		write_fram(buffer_address, buffer, BUFFER_SIZE);
		count2 = 0;
	}
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
			clearbuffer(buffer);
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
			setpixel(buffer,(byte)game.x,(byte)game.y);
		} else {
			glcd_rect(buffer, (byte)(game.x-game.width/2) , (byte)(game.y-game.width/2), (byte)(game.x+game.width/2) , (byte)(game.y+game.width/2) , TRUE);
		}
		_delay_ms(20);
	}
	return (TRUE);
}













