/*
 * GameBoy.c
 *
 * Created: 6/10/2014 9:00:26 PM
 *  Author: Jonathon
 surface
 */ 

#include "gameboy.h" 
byte LCD_data_tx(byte tx_byte);
byte LCD_initialise(void);
byte LCD_command_tx(byte tx_byte);
byte select_page (byte page);
byte select_column (byte column);

int main(void)
{

	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.
	
	BACKLIGHT_SET(OFF);
	BACKLIGHT_DIR(OUT);
	SET_UP_TIMER_REG();
	PRESCALER_8();
	
	
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
	
	
	LCD_CHIP_SELECT_DIR(OUT);
	SCK_SET_DIR(OUT);
	MOSI_SET_DIR(OUT);
	LCD_DATA_SET_DIR(OUT);
	LCD_RST_SET_DIR(OUT);
	LCD_RST_SET_HIGH;
	LCD_initialise();
//	select_page(0); 
//	select_column();
//	LCD_data_tx(0xFF); 
	BACKLIGHT_BRIGHTNESS(127);
	
	for(int page=0;page<8; page++) {
		for(int column = 0; column<102; column++)	{
			select_page(page);
			select_column(column);
			LCD_data_tx(OFF);
			//_delay_ms(20);
		}
	}
	 
//	SCK_SET_HIGH;
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

byte select_page (byte page) { 
	byte page_cmd_address; 
	page_cmd_address =(CMD_PAGE | page); 
	LCD_command_tx(page_cmd_address ); 
	return(TRUE);
}

byte select_column (byte column) { 
	byte page_cmd_address_MSB; 
	byte page_cmd_address_LSB; 
	page_cmd_address_LSB =(CMD_COL_LSB | (column&0x0F)); 
	page_cmd_address_MSB =(CMD_COL_MSB | (column >> 4)); 
	LCD_command_tx(page_cmd_address_LSB); 
	LCD_command_tx(page_cmd_address_MSB); 
	return(TRUE); 
}



byte LCD_data_tx(byte tx_byte) //Sends  a data byte 
{ 
	byte tx_processed; 
	byte tx_mask = 0x80; 
	LCD_CHIP_SELECT; 
	LCD_DATA_SET_HIGH; 
	while (tx_mask != 0x00) { 
		tx_processed = tx_byte & tx_mask; 
		SCK_SET_HIGH; 
		if(tx_processed) 
			MOSI_SET_HIGH; 
		else 
			MOSI_SET_LOW; 
		SCK_SET_LOW; 
		tx_mask>>=1; 
	} 
	
	SCK_SET_HIGH; 
	LCD_CHIP_DESELECT; 
	return(TRUE); 
	
}

byte LCD_command_tx(byte tx_byte) //Sends  a data byte
{
	byte tx_processed;
	byte tx_mask = 0x80;
	LCD_CHIP_SELECT;
	LCD_DATA_SET_LOW;
	while (tx_mask != 0x00) {
		tx_processed = tx_byte & tx_mask;
		SCK_SET_HIGH;
		if(tx_processed)
		MOSI_SET_HIGH;
		else
		MOSI_SET_LOW;
		SCK_SET_LOW;
		tx_mask>>=1;
	}
	
	SCK_SET_HIGH;
	LCD_CHIP_DESELECT;
	return(TRUE);
	
}

byte LCD_initialise(void) { 
	LCD_command_tx(0xE2);//reset
	LCD_command_tx(0x40);//Display start line 0 
	LCD_command_tx(0xA1);//SEG reverse 
	LCD_command_tx(0xC0);//Normal COM0~COM63 
	//LCD_command_tx(0xA4);//Disable -> Set All Pixel to ON 
	LCD_command_tx(0xA6);//Display inverse off 
	_delay_ms(120); 
	LCD_command_tx(0xA2);//Set LCD Bias Ratio A2/A3 
	LCD_command_tx(0x2F);//Set Power Control 28...2F 
	LCD_command_tx(0x27);//Set VLCD Resistor Ratio 20...27 
	LCD_command_tx(0x81);//Set Electronic Volume 
	LCD_command_tx(0x10);//Set Electronic Volume 00...3F 
	LCD_command_tx(0xFA);//Set Adv. Program Control 
	LCD_command_tx(0x90);//Set Adv. Program Control x00100yz yzcolumn wrap x Temp Comp 
	LCD_command_tx(0xAF);//Display on 
	return(TRUE); 
}

