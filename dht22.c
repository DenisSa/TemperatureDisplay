#include <avr/io.h>
#include <util/delay.h>
#include "dht22.h"
#include "bitbang.h"

#ifdef DEBUG
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <stdio.h>
#endif

uint8_t readTempData(float *temperature, float *humidity);
void setpin(int pn, int val);

uint8_t readTempData(float *temperature, float *humidity) {

	uint8_t i, j = 0;
	uint8_t bits[5];
	//TDDR |= 1 << TDATA;
	_delay_ms(2000);
	setdirection(TDATA,1); //PB1 output
	setpin(TDATA, 1); //PB1 high VDD init
	_delay_ms(100);

	setpin(TDATA, 0); //PB1 low //Tbe
	_delay_us(500);

	setpin(TDATA, 1); //PB1 high //Tgo
	//_delay_us(1);
	setdirection(TDATA,0); //PB1 input
	_delay_us(40); //sensor pulls low for 80us

	if ((TPIN & (1 << TDATA))) { //sensor pulls low
#ifdef DEBUG
		softuart_puts_P("Timeout0\r\n");
#endif
		return -1;
	}
	_delay_us(80);
	if (!(TPIN & (1 << TDATA))) {		//sensor pulls high for next 80us
#ifdef DEBUG
		softuart_puts_P("Timeout1\r\n");
#endif
		return -1;
	}

	//data transmission begins
	_delay_us(80);

	//read the data
	uint16_t timeoutcounter = 0;
	for (j = 0; j < 5; j++) { //read 5 byte
		uint8_t result = 0;
		for (i = 0; i < 8; i++) { //read every bit
			timeoutcounter = 0;
			while (!(TPIN & (1 << TDATA))) { //wait for an high input (non blocking)
				timeoutcounter++;
				if (timeoutcounter > DHT_TIMEOUT) {
#ifdef DEBUG
					softuart_puts_P("Timeout2\r\n");
#endif
					return -1; //timeout
				}
			}
			_delay_us(30);
			if (TPIN & (1 << TDATA)) //if input is high after 30 us, get result
				result |= (1 << (7 - i));
			timeoutcounter = 0;
			while (TPIN & (1 << TDATA)) { //wait until input get low (non blocking)
				timeoutcounter++;
				//_delay_us(1);
				if (timeoutcounter > DHT_TIMEOUT) {
#ifdef DEBUG
					softuart_puts_P("Timeout3\r\n");
#endif
					return -1; //timeout
				}
			}
			//softuart_puts_P("hit\r\n");

		}
		bits[j] = result;
	}

	setdirection(TDATA,1);
	setpin(TDATA, 0);
	_delay_ms(100);

	if ((uint8_t) (bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
		//return temperature and humidity
		uint16_t rawhumidity = bits[0] << 8 | bits[1];
		uint16_t rawtemperature = bits[2] << 8 | bits[3];
		if (rawtemperature & 0x8000) {
			*temperature = (float) ((rawtemperature & 0x7FFF) / 10.0) * -1.0;
		} else {
			*temperature = (float) (rawtemperature) / 10.0;
		}
		*humidity = (float) (rawhumidity) / 10.0;
		return 0;
	}

	return -1;
}
