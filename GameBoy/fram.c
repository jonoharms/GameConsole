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

void write_fram(uint16_t address, byte data,byte buffer[][MAX_PAGES]) {
//	byte msb = ((address>>8) & 0xff);
	byte lsb = address & 0x00ff;
//	byte temp = ((msb<<3) & WR_MASK);
//	byte msb = (byte) temp;
	FRAM_CHIP_SELECT;
	spi_tx(CMD_WREN);
	FRAM_CHIP_DESELECT;
	_delay_ms(200);
	
	
	byte out = read_fram_status();
	draw_byte(buffer, 0, 5, out);
	
	
	FRAM_CHIP_SELECT;
	spi_tx(CMD_WRITE);
	spi_tx(lsb);
	spi_tx(data);
	spi_tx(data);
	FRAM_CHIP_DESELECT;
	
	out = read_fram_status();
	draw_byte(buffer, 0, 6, out);
	
}

byte read_fram(uint16_t address) {
//	byte out = 0;
	//byte msb = ((address>>8) & 0xff);
	byte lsb = address & 0x00ff;
	//byte temp = ((msb<<3) & WR_MASK);
	FRAM_CHIP_SELECT;
	spi_tx(CMD_READ);
	spi_tx(lsb);
	byte out = spi_txrx(0x00);
	FRAM_CHIP_DESELECT;
	return out;
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
