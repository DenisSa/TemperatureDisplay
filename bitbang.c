#include <avr/io.h>

void setpin(int pn, int val) {
	//0 - low
	//1 - high
	if (val == 0) {
		PORTB &= ~(1 << pn);
	} else if (val == 1) {
		PORTB |= (1 << pn);
	}
}

void setdirection(int pn, int val) {
	// 0 - in
	// 1 - out
	if (val) {
		DDRB |= 1 << pn;
	} else {
		DDRB &= ~(1 << pn);
	}
}
