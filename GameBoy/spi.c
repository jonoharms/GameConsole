/*
 * spi.c
 *
 * Created: 11/10/2014 3:58:09 PM
 *  Author: Jonathon
 *  Contains code for the spi bus
 */ 


#include "spi.h"
#include "fram.h"
#include "lcd.h"

void init_spi(void){
	// SPI Setup
	SCK_SET_DIR(OUT);
	MOSI_SET_DIR(OUT);
	MISO_SET_DIR(IN);
	SS_PIN_DIR(OUT);
	SPI_SETUP;
	SPI_MODE_3;
	SPI_DOUBLE_SPEED;
	LCD_CHIP_DESELECT;
	FRAM_CHIP_DESELECT;
}


void spi_tx(byte data) {
	/* Start Transmission */
	SPI_WRITE_DATA(data);
	/*WAIT UNTIL FINISHED*/
	while(!(SPSR & _BV(SPIF))) {
		// DO NOTHING
	}
}

byte spi_txrx(byte data) {
	SPI_WRITE_DATA(data);
	while(!(SPSR & _BV(SPIF))) {
		// DO NOTHING
	}
	return SPDR;
}
