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
#define BAT_LOW_LED_DIR(DIR) SET(DDRC,_BV(PINC6),DIR)

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

#define SCK_SET_DIR(DIR) SET(DDRB,_BV(PB7),DIR)
#define SCK_SET_HIGH SET(PORTB,_BV(PB7),ON)
#define SCK_SET_LOW SET(PORTB,_BV(PB7),OFF)

#define MOSI_SET_DIR(DIR) SET(DDRB,_BV(PB5),DIR)
#define MOSI_SET_HIGH SET(PORTB,_BV(PB5),ON)
#define MOSI_SET_LOW SET(PORTB,_BV(PB5),OFF)

#define LCD_DATA_SET_DIR(DIR) SET(DDRB,_BV(PB3),DIR)
#define LCD_DATA_SET_HIGH SET(PORTB,_BV(PB3),ON)
#define LCD_DATA_SET_LOW SET(PORTB,_BV(PB3),OFF)

#define LCD_RST_SET_DIR(DIR) SET(DDRB,_BV(PB2),DIR)
#define LCD_RST_SET_HIGH SET(PORTB,_BV(PB2),ON)
#define LCD_RST_SET_LOW SET(PORTB,_BV(PB2),OFF)

#define CMD_PAGE 0xB0
#define CMD_COL_LSB 0x00 
#define CMD_COL_MSB 0x10


#define PRESCALER_ONE() 
#define UP_BUTTON_DIR(DIR) SET(DDRA,_BV(PINA5),DIR)
#define DOWN_BUTTON_DIR(DIR) SET(DDRA,_BV(PINA6),DIR)
#define LEFT_BUTTON_DIR(DIR) SET(DDRA,_BV(PINA7),DIR)
#define RIGHT_BUTTON_DIR(DIR) SET(DDRA,_BV(PINA4),DIR)
#define A_BUTTON_DIR(DIR) SET(DDRA,_BV(PINA2),DIR)
#define B_BUTTON_DIR(DIR) SET(DDRA,_BV(PINA1),DIR)
#define C_BUTTON_DIR(DIR) SET(DDRA,_BV(PINA0),DIR)
#define INTERRUPT_DIR(DIR) SET(DDRD,_BV(PIND3),DIR)

//Devices Outputs
//#define MULTI_LINES(STATE) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)

#define BAT_LOW_LED(STATE) SET(PORTC,_BV(PINC6),STATE)
#define BACKLIGHT_SET(STATE) SET(PORTD,_BV(PIND4),STATE)


//Devices Inputs
#define UP_BUTTON ~GET(PINA,_BV(PINA5))
#define UP_BUTTON_PULLUP() SET(PORTA, _BV(PINA5),ON)
#define DOWN_BUTTON ~GET(PINA,_BV(PINA6))
#define DOWN_BUTTON_PULLUP() SET(PORTA, _BV(PINA6),ON)
#define LEFT_BUTTON ~GET(PINA,_BV(PINA7))
#define LEFT_BUTTON_PULLUP() SET(PORTA, _BV(PINA7),ON)
#define RIGHT_BUTTON ~GET(PINA,_BV(PINA4))
#define RIGHT_BUTTON_PULLUP() SET(PORTA, _BV(PINA4),ON)
#define A_BUTTON ~GET(PINA,_BV(PINA2))
#define A_BUTTON_PULLUP() SET(PORTA, _BV(PINA2),ON)
#define B_BUTTON ~GET(PINA,_BV(PINA1))
#define B_BUTTON_PULLUP() SET(PORTA, _BV(PINA1),ON)
#define C_BUTTON ~GET(PINA,_BV(PINA0))
#define C_BUTTON_PULLUP() SET(PORTA, _BV(PINA0),ON)
#define INTERRUPT_SIG GET(PIND,_BV(PIND3))
#define INTERRUPT_SIG_PULLUP() SET(PORTD, _BV(PIND3),ON)