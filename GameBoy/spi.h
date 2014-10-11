/*
 * spi.h
 *
 * Created: 11/10/2014 3:58:36 PM
 *  Author: Jonathon
 */ 
#include "gameboy.h"

//SPI SETUP
#define SPI_SETUP SET(SPCR,(_BV(SPE)|_BV(MSTR)|_BV(CPOL)|_BV(CPHA) ), ON)
#define SPI_DOUBLE_SPEED SET(SPSR,_BV(SPI2X),ON)
#define SPI_FINISHED GET(SPSR,_BV(SPIF))
#define SPI_WRITE_DATA(DATA) SET(SPDR,ALL,DATA)
#define SS_PIN_DIR(DIR) SET(DDRB, _BV(PB4), DIR)

#define SCK_SET_DIR(DIR) SET(DDRB,_BV(PB7),DIR)
#define SCK_SET_HIGH SET(PORTB,_BV(PB7),ON)
#define SCK_SET_LOW SET(PORTB,_BV(PB7),OFF)

#define MOSI_SET_DIR(DIR) SET(DDRB,_BV(PB5),DIR)
#define MOSI_SET_HIGH SET(PORTB,_BV(PB5),ON)
#define MOSI_SET_LOW SET(PORTB,_BV(PB5),OFF)

#define MISO_SET_DIR(DIR) SET(DDRB,_BV(PB6),DIR)
#define MISO_SET_HIGH SET(PORTB,_BV(PB6),ON)
#define MISO_SET_LOW SET(PORTB,_BV(PB6),OFF)

void SPI_MasterInit(void);
void SPI_MasterTransmit(byte data);
