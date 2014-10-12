/*
 * fram.h
 *
 * Created: 11/10/2014 7:18:24 PM
 *  Author: Jonathon
 */ 

#include "gameboy.h"

//FRAM SETUP
#define HOLD_SET_DIR(DIR) SET(DDRD,_BV(PD1),DIR)
#define HOLD_DISABLE SET(PORTD,_BV(PD1),ON)
#define HOLD_ENABLE SET(PORTD,_BV(PD1),OFF)

#define FRAM_CHIP_SELECT_DIR(DIR) SET(DDRD,_BV(PIND0),DIR)
#define FRAM_CHIP_SELECT SET(PORTD,_BV(PIND0),OFF)
#define FRAM_CHIP_DESELECT SET(PORTD,_BV(PIND0),ON)

#define WP_SET_DIR(DIR) SET(DDRD,_BV(PD2),DIR)
#define WP_DISABLE SET(PORTD,_BV(PD2),ON)
#define WP_ENABLE SET(PORTD,_BV(PD2),OFF)

//OPCODES
#define CMD_WREN 0x06
#define CMD_WRDI 0x04
#define CMD_RDSR 0x05
#define CMD_WRSR 0x01
#define CMD_READ 0x03
#define CMD_WRITE 0x02
#define WR_MASK 0x08

//FUNCTIONS
void init_fram(void);
//void write_fram(uint16_t address, byte data);
byte read_fram(uint16_t address);
byte read_fram_status(void);