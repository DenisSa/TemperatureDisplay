//#define DEBUG

#include "bitbang.h"
#include "ssd1306.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "c64_lower.h"

#ifdef DEBUG
#include <avr/interrupt.h>
#include "softuart.h"
#endif

void transferByte(int address, int data) {
#ifdef DEBUG
	softuart_puts_P("\r\nStart\r\n");
#endif
	if(data > 0){
		setpin(DC, 1);
	}
	else {
		setpin(DC, 0);
	}
	setpin(CS, 0);
	for (int i = 7; i >= 0; i--) {
		setpin(CLK, 0);
		setpin(MOSI, (address >> i) & 0x1);
		setpin(CLK, 1);
#ifdef DEBUG
		if ((address >> i) & 0x1) {
			softuart_putchar('1');
		} else {
			softuart_putchar('0');
		}
#endif
	}
	setpin(CLK, 0);
	setpin(CS, 1);
#ifdef DEBUG
	softuart_puts_P("\r\nEnd\r\n");
#endif
}

void init() {
#ifdef DEBUG
	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */

	sei();
	softuart_puts_P("\r\nInit\r\n");
#endif
	//setdirection(MOSI, OUT);
	//setdirection(CS, OUT);
	//setdirection(DC, OUT);
	//setdirection(CLK, OUT);

	transferByte(0xAE, 0x00);

	transferByte(0xD5, 0x00);
	transferByte(0x80, 0x00);

	transferByte(0xA8, 0x00);
	transferByte(0x3F, 0x00);

	transferByte(0xD3, 0x00);
	transferByte(0x00, 0x00);
	transferByte(0x00, 0x00);

	transferByte(0x8D, 0x00);
	transferByte(0x14, 0x00);

	transferByte(0x20, 0x00);
	transferByte(0x00, 0x00);

	transferByte(0xA1, 0x00);

	transferByte(0xA5, 0x00);

	transferByte(0xC8, 0x00);
	transferByte(0xDA, 0x00);
	transferByte(0x12, 0x00);

	transferByte(0x81, 0x00);
	transferByte(0x0F, 0x00);

	transferByte(0xD9, 0x00);
	transferByte(0x11, 0x00);

	transferByte(0xDB, 0x00);
	transferByte(0x20, 0x00);

	transferByte(0xA6, 0x00);
	transferByte(0xA4, 0x00);
	transferByte(0xAF, 0x00);
}

void oled_home() {
	transferByte(0x21, 0x00); // column range
	transferByte(0x00, 0x00); //  set start to 0
	transferByte(0x7F, 0x00); //  set end to 0x7F
	transferByte(0x22, 0x00); // row range
	transferByte(0x00, 0x00); //  set start to 0
	transferByte(0x07, 0x00); //  set end to 0x07
}

void oled_puts(char *str) {
	while(*str != 0) {
		oled_putc(*str++);
	}
}

void oled_putc(char c) {
	// remap from petscii to ascii, shifts drawing characters into the lower 32 ascii cells
	if(c > 'A' && c < 'Z') { }               // upper-case ascii range
	else if(c > 'a' && c < 'z') { c -= 96; } // lower-case ascii range
	else if(c > 31 && c < 64) { }            // numbers and symbols
	else if(c < 32) { c += 96; }             // low ascii
	oled_putc_raw(c);
}

void oled_putc_raw(char c) {
	for(uint16_t i = c << 3; i < (c << 3) + 8; i++) {
		transferByte(pgm_read_byte(font + i), 1);
	}
}

void oled_clear() {
	oled_home();
	//setpin(DC,1);
	for(uint16_t i = 889; i > 0; i--) {
		transferByte(0x00, 1);
	}
}

void oled_move(int row, int col) {
	if(col > 15) { col = 15; }
	if(row > 7) { row = 7; }

	oled_move_raw(row, col << 3);
}

void oled_move_raw(int row, int col) {
	if(col > 127) { col = 127; }
	if(row > 7) { row = 7; }

	transferByte(0x21,0);     // set column
	transferByte(col,0);      // start = col
	transferByte(0x7F,0);     // end = col max
	transferByte(0x22,0);     // set row
	transferByte(row,0);      // start = row
	transferByte(0x07,0);     // end = row max
}


