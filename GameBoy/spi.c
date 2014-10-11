/*
 * spi.c
 *
 * Created: 11/10/2014 3:58:09 PM
 *  Author: Jonathon
 *  Contains code for the spi bus
 */ 


#include "spi.h"

void init_spi(void){
		// SPI Setup
	SCK_SET_DIR(OUT);
	MOSI_SET_DIR(OUT);
	SS_PIN_DIR(OUT);
	SPI_SETUP;
	//SPI_DOUBLE_SPEED;
}


void spi_tx(byte data) {
	/* Start Transmission */
	SPI_WRITE_DATA(data);
	/*WAIT UNTIL FINISHED*/
	while(~SPI_FINISHED) {
		// DO NOTHING
	}
}
