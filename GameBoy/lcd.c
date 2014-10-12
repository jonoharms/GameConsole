/*
 * lcd.c
 *
 * Created: 11/10/2014 4:34:52 PM
 *  Author: Jonathon
 */

#include "spi.h"
#include "lcd.h"

const extern uint8_t PROGMEM font[];

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
	BACKLIGHT_BRIGHTNESS(50);
	
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

/* Everything Below is adapted from
from https://github.com/adafruit/ST7565-LCD */
  
void write_buffer(byte buff[][MAX_PAGES]) {

  byte c, p;
  
  for(p = 0; p < MAX_PAGES; p++) {

    select_page(p);
	select_column(0x00);    
    for(c = 0; c < MAX_COLUMNS; c++) {
	  select_column(c);
      LCD_data_tx(buff[c][p]);
    }
  }
}

void setpixel(byte buff[][MAX_PAGES], byte x, byte y) {
	if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
	return;

	// x is which column
	byte page = y/8;
	byte pixel = y%8;
	pixel = (_BV(pixel) | (buff[x][page])); 
	
	select_page(page);
	select_column(x);
	buff[x][page] |= pixel;
	LCD_data_tx(pixel);
}

void clearpixel(byte buff[][MAX_PAGES], byte x, byte y) {
	if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
	return;

	// x is which column
	byte page = y/8;
	byte pixel = y%8;
	pixel = ((~_BV(pixel) & 0xff) & (buff[x][page]));
	
	select_page(page);
	select_column(x);
	buff[x][page] &= pixel;
	LCD_data_tx(pixel);
}

void drawline(byte buff[][MAX_PAGES],byte x0, byte y0, byte x1, byte y1) {

	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	uint8_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int8_t err = dx / 2;
	int8_t ystep;

	if (y0 < y1) {
		ystep = 1;
		} else {
	ystep = -1;}

	for (; x0<x1; x0++) {
		if (steep) {
			setpixel(buff, y0, x0);
			} else {
			setpixel(buff, x0, y0);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void drawchar(byte buff[][MAX_PAGES], byte x, byte line, byte c) {
	if((c>MAX_ASCII) || (c<MIN_ASCII)) 
		return;

	
	for (uint8_t i=0; i<FONT_WIDTH; i++ ) {
		uint16_t temp = FONT_WIDTH*(c-MIN_ASCII);
		byte ascii = pgm_read_byte(font+temp+i);
		buff[x][line] = ascii;
		select_page(line);
		select_column(x);
		LCD_data_tx(ascii);
		x++;
	}
}

void drawstring(byte buff[][MAX_PAGES], byte x, byte line, byte *c) {
	while (c[0] != 0) {
		drawchar(buff, x, line, *c);
		c++;
		x += FONT_WIDTH+FONT_SPACE; // 6 pixels wide
		if (x + FONT_WIDTH+FONT_SPACE >= LCDWIDTH) {
			x = 0;    // ran out of this line
			line++;
		}
		if (line >= MAX_PAGES)
		return;        // ran out of space :(
	}

}

void clearbuffer(byte buff[][MAX_PAGES]) {
	for (byte i= 0; i<MAX_PAGES; i++) {
		for(byte j = 0; j<MAX_COLUMNS; j++) {
			buff[j][i] = 0x00;
		}
	}
}

//THE FOLLOWING ARE FROM http://geoffg.net/Downloads/GLCD_Driver/glcd_library_1_0.h
// Purpose:       Draw a rectangle on a graphic LCD
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                fill  - YES or NO
// Dependencies:  drawline()
void glcd_rect(byte buff[][MAX_PAGES], byte x1, byte y1, byte x2, byte y2, byte fill)
{
	if(fill)
	{
		byte y, ymax;                          // Find the y min and max
		if(y1 < y2)
		{
			y = y1;
			ymax = y2;
		}
		else
		{
			y = y2;
			ymax = y1;
		}

		for(; y<=ymax; ++y)                    // Draw lines to fill the rectangle
		drawline(buff,x1, y, x2, y);
	}
	else
	{
		drawline(buff,x1, y1, x2, y1);      // Draw the 4 sides
		drawline(buff,x1, y2, x2, y2);
		drawline(buff,x1, y1, x1, y2);
		drawline(buff,x2, y1, x2, y2);
	}
}

// Purpose:       Draw a bar (wide line) on a graphic LCD
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                width  - The number of pixels wide
void glcd_bar(byte buff[][MAX_PAGES], byte x1, byte y1, byte x2, byte y2, byte width)
{
	int16_t x, y, addx, addy, j;
	int16_t P, dx, dy, c1, c2;
	byte i;
	dx = abs((x2 - x1));
	dy = abs((y2 - y1));
	x = x1;
	y = y1;
	c1 = -dx*x1 - dy*y1;
	c2 = -dx*x2 - dy*y2;

	if(x1 > x2)
	{
		addx = -1;
		c1 = -dx*x2 - dy*y2;
		c2 = -dx*x1 - dy*y1;
	}
	else
	addx = 1;
	if(y1 > y2)
	{
		addy = -1;
		c1 = -dx*x2 - dy*y2;
		c2 = -dx*x1 - dy*y1;
	}
	else
	addy = 1;

	if(dx >= dy)
	{
		P = 2*dy - dx;

		for(i=0; i<=dx; ++i)
		{
			for(j=-(width/2); j<width/2+width%2; ++j)
			{
				if(dx*x+dy*(y+j)+c1 >= 0 && dx*x+dy*(y+j)+c2 <=0)
				setpixel(buff,x, y+j);
			}
			if(P < 0)
			{
				P += 2*dy;
				x += addx;
			}
			else
			{
				P += 2*dy - 2*dx;
				x += addx;
				y += addy;
			}
		}
	}
	else
	{
		P = 2*dx - dy;

		for(i=0; i<=dy; ++i)
		{
			if(P < 0)
			{
				P += 2*dx;
				y += addy;
			}
			else
			{
				P += 2*dx - 2*dy;
				x += addx;
				y += addy;
			}
			for(j=-(width/2); j<width/2+width%2; ++j)
			{
				if(dx*x+dy*(y+j)+c1 >= 0 && dx*x+dy*(y+j)+c2 <=0)
				setpixel(buff,x+j, y);
			}
		}
	}
}



// Purpose:       Draw a circle on a graphic LCD
// Inputs:        (x,y) - the center of the circle
//                radius - the radius of the circle
//                fill - YES or NO
//                colour - ON or OFF
void glcd_circle(byte buff[][MAX_PAGES], byte x, byte y, byte radius, byte fill)
{
	int16_t a, b, P;
	a = 0;
	b = radius;
	P = 1 - radius;

	do
	{
		if(fill)
		{
			drawline(buff,x-a, y+b, x+a, y+b);
			drawline(buff,x-a, y-b, x+a, y-b);
			drawline(buff,x-b, y+a, x+b, y+a);
			drawline(buff,x-b, y-a, x+b, y-a);
		}
		else
		{
			setpixel(buff,a+x, b+y);
			setpixel(buff,b+x, a+y);
			setpixel(buff,x-a, b+y);
			setpixel(buff,x-b, a+y);
			setpixel(buff,b+x, y-a);
			setpixel(buff,a+x, y-b);
			setpixel(buff,x-a, y-b);
			setpixel(buff,x-b, y-a);
		}

		if(P < 0)
		P+= 3 + 2*a++;
		else
		P+= 5 + 2*(a++ - b--);
	} while(a <= b);
}