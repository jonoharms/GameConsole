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


byte etch(void);

int main(void)
{

	init_gpio();
	init_spi();
	init_ext_interrupts();
	init_lcd();
	
	
	
	//Game
	sei();
	etch();
	
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
		//if (column)
		select_page(page);
		select_column(column);
		LCD_data_tx(0xFF);
		_delay_ms(100);
	}
}













