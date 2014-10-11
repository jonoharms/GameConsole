/*
 * lcd.c
 *
 * Created: 11/10/2014 4:34:52 PM
 *  Author: Jonathon
 */

#include "spi.h"
#include "lcd.h"


byte LCD_data_tx(byte tx_byte) //Sends  a data byte
{
	LCD_CHIP_SELECT;
	LCD_DATA_SET_HIGH;
	spi_tx(tx_byte);
	LCD_CHIP_DESELECT;
	return(TRUE);
}

byte LCD_command_tx(byte tx_byte) //Sends  a data byte as command
{
	LCD_CHIP_SELECT;
	LCD_DATA_SET_LOW;
	spi_tx(tx_byte);
	LCD_CHIP_DESELECT;
	return(TRUE);
}

byte select_page (byte page) { //selects page to be written to 
	byte page_cmd_address;
	if(page > MAX_PAGES) {
		page = MAX_PAGES-1;
	}
	page_cmd_address =(CMD_PAGE | page);
	LCD_command_tx(page_cmd_address );
	return(TRUE);
}

byte select_column (byte column) { //selects column to be written to
	byte page_cmd_address_MSB;
	byte page_cmd_address_LSB;
	if(column > MAX_COLUMNS) {
		column = MAX_COLUMNS-1;
	}
	page_cmd_address_LSB =(CMD_COL_LSB | (column & 0x0F));
	page_cmd_address_MSB =(CMD_COL_MSB | (column >> 4));
	LCD_command_tx(page_cmd_address_LSB);
	LCD_command_tx(page_cmd_address_MSB);
	return(TRUE);
}

byte init_lcd(void) { // initialize lcd, must be run prior to using lcd
	// BACKLIGHT
	BACKLIGHT_SET(OFF);
	BACKLIGHT_DIR(OUT);
	SET_UP_TIMER_REG();
	PRESCALER_8();
	BACKLIGHT_BRIGHTNESS(127);
	
	//LCD
	LCD_CHIP_SELECT_DIR(OUT);
	LCD_DATA_SET_DIR(OUT);
	LCD_RST_SET_DIR(OUT);
	LCD_RST_SET_HIGH;
	_delay_ms(100);
	LCD_command_tx(0xE2);//reset
	LCD_command_tx(0x40);//Display start line 0
	LCD_command_tx(0xA1);//SEG reverse
	LCD_command_tx(0xC0);//Normal COM0~COM63
	LCD_command_tx(0xA4);//Disable -> Set All Pixel to ON
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
	set_all_lcd_pages(OFF);
	return(TRUE);
}

byte set_all_lcd_pages(byte val) {  //sets all pages to val
	for(int page=0;page<MAX_PAGES; page++) {
		for(int column = 0; column<MAX_COLUMNS; column++)	{
			select_page(page);
			select_column(column);
			LCD_data_tx(val);
		}
	}
	return(TRUE);
}

