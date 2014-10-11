/*************************************************************************
Title:    game_console Template
Initial Author:   David Jahshan
Extended by : Jonathon Harms 389212
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Main and functions for Game Console basic sample code

*************************************************************************/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


#define byte unsigned char

/*ON OFF*/
#define ON 0xFF
#define OFF 0x00
#define HIGH 0xFF
#define LOW 0x00
#define IN 0x00
#define OUT 0xFF
#define ALL 0xFF
#define TRUE 1
#define FALSE 0
#define FORWARD 0x00
#define BACK 0xFF

// NUUMBERS
#define MAX_PAGES 8
#define MAX_COLUMNS 102

/*SET and GET MACRO*/
#define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
#define GET(PORT,MASK) PORT & MASK





//Devices Direction MACROs
//#define MULTI_LINES_DIR(DIR) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)


#define BACKLIGHT_DIR(DIR) SET(DDRD,_BV(PIND4),DIR)
#define SET_UP_TIMER_REG() SET(TCCR1A,(_BV(WGM10)|_BV(COM1A1)|_BV(COM1B1)),ON)
#define PRESCALER_1() SET(TCCR1A,_BV(CS10),ON)
#define PRESCALER_8() SET(TCCR1B,_BV(CS11),ON)
#define PRESCALER_64() SET(TCCR1B,(_BV(CS11) | _BV(CS10)),ON)
#define BACKLIGHT_BRIGHTNESS(VAL) SET(OCR1B,ALL,VAL)



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

#define CMD_PAGE 0xB0
#define CMD_COL_LSB 0x00 
#define CMD_COL_MSB 0x10

//Devices Outputs
//#define MULTI_LINES(STATE) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)


#define BACKLIGHT_SET(STATE) SET(PORTD,_BV(PIND4),STATE)


//Devices Inputs

