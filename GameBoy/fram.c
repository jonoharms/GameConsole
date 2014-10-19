/*
 * fram.c
 *
 * Created: 11/10/2014 7:18:12 PM
 *  Author: Jonathon
 */ 
#include "lcd.h"
#include "fram.h"
#include "spi.h"

void init_fram(void) {
	HOLD_SET_DIR(OUT);
	FRAM_CHIP_SELECT_DIR(OUT);
	WP_SET_DIR(OUT);
	HOLD_DISABLE;
	WP_DISABLE;
	FRAM_CHIP_DESELECT;
	//HOLD_ENABLE;
	//WP_ENABLE;
}

int8_t write_fram(uint16_t address, byte *buf, byte count) {
	byte msb = ((address>>8) & 0xff);
	byte lsb = address & 0xff;

	FRAM_CHIP_SELECT;
	spi_tx(CMD_WREN);
	FRAM_CHIP_DESELECT;
	
	
	FRAM_CHIP_SELECT;
	spi_tx(CMD_WRITE);
	spi_tx(msb);
	spi_tx(lsb);
	for (int i = 0;i < count;i++) {
		spi_tx(buf[i]);
	}
	FRAM_CHIP_DESELECT;
	return 0;
}

int8_t read_fram(uint16_t address, byte *buf, byte count) {
	byte msb = ((address>>8) & 0xff);
	byte lsb = address & 0xff;
	
	FRAM_CHIP_SELECT;
	spi_tx(CMD_READ);
	spi_tx(msb);
	spi_tx(lsb);
	for (int i=0; i < count; i++) {
		buf[i] = spi_txrx(0x00);
	}
	FRAM_CHIP_DESELECT;
	return 0;
}

byte read_fram_status(void) {
	FRAM_CHIP_SELECT;
	spi_txrx(CMD_RDSR);
	byte out = spi_txrx(0x00);
	FRAM_CHIP_DESELECT;
	return(out);
}

void write_fram_sr() {
	FRAM_CHIP_SELECT;
	spi_tx(CMD_WREN);
	FRAM_CHIP_DESELECT;

	FRAM_CHIP_SELECT;
	spi_tx(CMD_WRSR);
	spi_tx(0b00000000);
	//spi_tx(data);
	FRAM_CHIP_DESELECT;
}
