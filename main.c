/*
 * main.c
 *
 *  Created on: Feb 6, 2016
 *      Author: d
 */

/*
 * Temp Sensor:
 * Data: PB1
 *
 * Display:
 * PB3: C/S This pin is the chip select input. (active LOW).
 * PB4: D/C This is Data/Command control pin. When it is pulled HIGH (i.e. connect to VDD), the data
 at D[7:0] is treated as data. When it is pulled LOW, the data at D[7:0] will be transferred
 to the command register
 When 3-wire serial interface is selected, this pin must be connected to VSS.

 * PB2: CLK When serial interface mode is selected, D0 will be the serial clock input: SCLK;
 * PB0: MOSI D1 will be the serial data input
 *
 */
#define F_CPU 8000000UL
#define SERIAL
#include <avr/io.h>
#include <util/delay.h>
//#include "dht22.h"
#ifdef DISPLAY
#include "ssd1306.h"
#endif
#include "ds18b20.h"

#ifdef SERIAL
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
//#include <stdio.h>

#endif

void ftoa(float n, char *res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char *str, int len);

int main(void) {
	float temp = 0;
	//float humidity = 0;
	int ds_temp = 0;
	int negative = 0;
#ifdef SERIAL

	//softuart_turn_rx_on(); /* redundant - on by default */
	char x[10];
#endif
#ifdef DISPLAY
	int line = 0;
	char x[10];
	char y[10];
	char z[10];
	DDRB |= SS1306_OLED_PBM;
	//_delay_ms(1000);
	init();
	oled_clear();
#endif
	while (1) {
		//if (readTempData_dht22(&temp, &humidity) == 0) {
		if (readTempData_ds18b20(&ds_temp) == 0) {
			if (ds_temp & 0x8000) {
				ds_temp=0x10000-ds_temp;
				negative=1;
			} else {
				negative=0;
			}
			temp = ds_temp / 16.0;
#ifdef SERIAL
			ftoa(temp, x, 2);
			softuart_init();
			sei();
			/*for(i = 0; i < 4; i++){
			 softuart_putchar(x[i]);
			 }*/
			if(negative){
				softuart_puts("-");
			}
			softuart_puts(x);
			softuart_puts("\r\n");
			cli();

#endif
			/*for (int j = 0; j < 7; j++) {
			 oled_clear_fast();
			 oled_move(line, j);

			 oled_puts("TMP: ");
			 for (int i = 0; i < 4; i++) {
			 oled_putc(x[i]);
			 }
			 oled_move(line + 2, j);
			 oled_puts("HUM: ");
			 for (int i = 0; i < 4; i++) {
			 oled_putc(y[i]);
			 }
			 //oled_move(line + 4, j);
			 oled_puts("TMP: ");
			 for (int i = 0; i < 4; i++) {
			 oled_putc(z[i]);
			 }
			 _delay_ms(100);
			 if (j < 6) {
			 oled_clear_fast();
			 }

			 }
			 if (line < 5) {
			 line++;
			 } else {
			 line = 0;
			 }

			 #endif
			 } else {
			 oled_puts("Error");
			 #ifdef DEBUG
			 softuart_puts_P("Something else\r\n");
			 #endif
			 */
		}

		_delay_ms(1000);
	}

	return 1;

}

// reverses a string 'str' of length 'len'
void reverse(char *str, int len) {
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d) {
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
		str[i++] = '0';

	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint) {
	// Extract integer part
	int ipart = (int) n;

	// Extract floating part
	float fpart = n - (float) ipart;

	// convert integer part to string
	int i = intToStr(ipart, res, 0);

	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.';  // add dot

		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter is needed
		// to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);

		intToStr((int) fpart, res + i + 1, afterpoint);
	}
}

