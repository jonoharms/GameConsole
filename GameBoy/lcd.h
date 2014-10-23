/*
 * lcd.h
 *
 * Created: 11/10/2014 4:35:02 PM
 *  Author: Jonathon
 */ 

#include "gameboy.h"


// NUMBERS
#define MAX_PAGES 8
#define MAX_COLUMNS 102
#define BUFFER_SIZE MAX_PAGES*MAX_COLUMNS
#define LCDWIDTH MAX_COLUMNS
#define LCDHEIGHT MAX_PAGES*8
#define CMD_PAGE 0xB0
#define CMD_COL_LSB 0x00
#define CMD_COL_MSB 0x10
#define MAX_ASCII 0x7D
#define MIN_ASCII 0x20
#define FONT_WIDTH 0x05
#define FONT_HEIGHT 0x07
#define FONT_SPACE 0x01

// BACKLIGHT SETUP
#define BACKLIGHT_DIR(DIR) SET(DDRD,_BV(PIND4),DIR)
#define BACKLIGHT_BRIGHTNESS(VAL) SET(OCR1B,ALL,VAL)
#define BACKLIGHT_SET(STATE) SET(PORTD,_BV(PIND4),STATE)

// PWM SETUP
#define SET_UP_TIMER_REG() SET(TCCR1A,(_BV(WGM10)|_BV(COM1A1)|_BV(COM1B1)),ON)
#define PRESCALER_1() SET(TCCR1B,_BV(CS10),ON)
#define PRESCALER_8() SET(TCCR1B,_BV(CS11),ON)
#define PRESCALER_64() SET(TCCR1B,(_BV(CS12) | _BV(CS10)),ON)

//LCD SETUP
#define LCD_CHIP_SELECT_DIR(DIR) SET(DDRB,_BV(PINB1),DIR)
#define LCD_CHIP_SELECT SET(PORTB,_BV(PINB1),OFF)
#define LCD_CHIP_DESELECT SET(PORTB,_BV(PINB1),ON)

#define LCD_DATA_SET_DIR(DIR) SET(DDRB,_BV(PB3),DIR)
#define LCD_DATA_SET_HIGH SET(PORTB,_BV(PB3),ON)
#define LCD_DATA_SET_LOW SET(PORTB,_BV(PB3),OFF)

#define LCD_RST_SET_DIR(DIR) SET(DDRB,_BV(PB2),DIR)
#define LCD_RST_SET_HIGH SET(PORTB,_BV(PB2),ON)
#define LCD_RST_SET_LOW SET(PORTB,_BV(PB2),OFF)

// FUNCTIONS
byte LCD_data_tx(byte tx_byte);
byte LCD_command_tx(byte tx_byte);
byte select_page (byte page);
byte select_column (byte column);
byte init_lcd(void);
byte set_all_lcd_pages(byte val);
void write_buffer(byte buff[][MAX_PAGES]);
void setpixel(byte buff[][MAX_PAGES], byte x, byte y);
void clearpixel(byte buff[][MAX_PAGES], byte x, byte y);
void drawline(byte buff[][MAX_PAGES],byte x0, byte y0, byte x1, byte y1);
void drawchar(byte buff[][MAX_PAGES], byte x, byte line, byte c);
void drawstring(byte buff[][MAX_PAGES], byte x, byte line, byte *c);
void clearbuffer(byte buff[][MAX_PAGES]);
void glcd_rect(byte buff[][MAX_PAGES], byte x1, byte y1, byte x2, byte y2, byte fill);
