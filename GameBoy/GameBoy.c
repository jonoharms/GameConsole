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


int main(void)
{

	init_gpio();
	init_spi();
	init_ext_interrupts();
	init_lcd();
	glcd_rect(buffer, 30, 30, 40 , 40, TRUE);
	_delay_ms(3000);
	glcd_bar(buffer, 40, 40, 0, 0, 2);
	_delay_ms(3000);
	glcd_bar(buffer, 0, 0, 30, 10, 5);
	_delay_ms(3000);
	glcd_circle(buffer,50,50,10,FALSE);
	_delay_ms(3000);
	//init_fram();
	
		/*
	byte data = 0x07;
	data = 0x08;
	uint16_t add = 10;
	write_fram(add,data);
	byte rec = 0;
	//rec = read_fram_status();
	rec = read_fram(add);

	//Game

	
	for(int i = 0; i<rec; i++){
		BAT_LOW_LED(ON);
		_delay_ms((100));
		BAT_LOW_LED(OFF);
		_delay_ms((100));
	}
	*/
	//sei();
	//etch();
	uint8_t string1[] = {0x41,0x42,0x43,0x44,0x45,0x00};
	uint8_t string2[] = {'A','B','C','D','E',0x00};
	byte string3[127];
	for (int i = 0; i<127; i++) {
		string3[i] = i+0x20;
	}
	string3[127-0x20] = 0;
	clearbuffer(buffer);
	drawchar(buffer,30,2,0x41);
	_delay_ms(3000);
	drawstring(buffer,30,3,string1);
	drawstring(buffer,30,4,string2);
	drawstring(buffer,0,0,string3);
//	_delay_ms(3000);
	/*
	drawline(buffer,0, 0, 45, 45);
	_delay_ms(200);
	drawline(buffer,45, 45, 0, 45);
	_delay_ms(200);
	drawline(buffer,0, 45, 100, 60);
	_delay_ms(200);
	for (int i=0; i<LCDWIDTH ; i++) {
		for (int j = 0; j<LCDHEIGHT; j++) {
			setpixel(buffer,i,j);
			_delay_ms(1);
		}
	}
		for (int i=0; i<LCDWIDTH ; i+=5) {
			for (int j = 0; j<LCDHEIGHT; j+=3) {
				clearpixel(buffer,i,j);
				_delay_ms(1);
			}
		}
	//write_buffer(buffer);
	
	*/
	while(TRUE){
		
		
	}
}

ISR(INT1_vect){
	BAT_LOW_LED(~BAT_LOW_LED_VAL);
}


byte etch(void){
	byte page = 0;
	byte column = 0;
	while (TRUE)
	{
		if(UP_BUTTON) page--;
		if(DOWN_BUTTON) page ++;
		if(LEFT_BUTTON)column--;
		if(RIGHT_BUTTON)column++;
		if (page>7) page = 0;
		if (column>101) column = 0;
		select_page(page);
		select_column(column);
		LCD_data_tx(0xFF);
		_delay_ms(100);
	}
}













