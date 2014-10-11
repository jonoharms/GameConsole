/*
 * fram.c
 *
 * Created: 11/10/2014 7:18:12 PM
 *  Author: Jonathon
 */ 

#include "fram.h"
#include "spi.h"

void init_fram(void) {
	HOLD_SET_DIR(OUT);
	FRAM_CHIP_SELECT_DIR(OUT);
	WP_SET_DIR(OUT);
	HOLD_DISABLE;
	WP_DISABLE;
	FRAM_CHIP_DESELECT;
}

void write_fram(uint16_t address, byte data) {
	byte msb = ((address>>8) & 0xff);
	byte lsb = address & 0xff;
//	byte temp = ((msb<<3) & WR_MASK);
//	byte msb = (byte) temp;
	FRAM_CHIP_SELECT;
	spi_tx(CMD_WREN);
	FRAM_CHIP_DESELECT;

	FRAM_CHIP_SELECT;
	spi_tx(CMD_WRITE);
	spi_tx(0x00);
	spi_tx(0x07);
	FRAM_CHIP_DESELECT;
}

byte read_fram(uint16_t address) {
	byte out = 0;
	byte msb = ((address>>8) & 0xff);
	byte lsb = address & 0xff;
	//byte temp = ((msb<<3) & WR_MASK);
	FRAM_CHIP_SELECT;
	//spi_tx(WREN);
	spi_tx(CMD_READ);
	spi_tx(0x00);
	out = spi_txrx(0x00);
	FRAM_CHIP_DESELECT;
	return out;
}

byte read_fram_status(void) {
	FRAM_CHIP_SELECT;
	spi_txrx(CMD_RDSR);
	byte out = 0;
	out = spi_txrx(0x00);
	FRAM_CHIP_DESELECT;
	return(out);
}
