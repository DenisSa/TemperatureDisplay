#include <avr/io.h>
#include <util/delay.h>
#include "ds18b20.h"
#include "bitbang.h"

uint8_t readTempData_ds18b20(float *temperature) {
	if(resetSensor() != 0){
		return 1;
	}
	OWWriteByte(0xCC); //skip rom

	OWWriteByte(0x44); //convert T
	_delay_ms(800);
	if(resetSensor() != 0){
			return 1;
	}
	OWWriteByte(0xCC); //skip rom
	OWWriteByte(0xBE); //read scratchpad
	*temperature = readScratchpad(2);
	return 0;
}

uint8_t resetSensor() {
	uint8_t result;
	//_delay_ms(2000);
	setdirection(TDATA, 1); //PB1 output
	setpin(TDATA, 1); //PB1 high VDD init
	_delay_ms(100);
	setpin(TDATA, 0);
	_delay_us(490);
	setdirection(TDATA, 0);
	_delay_us(70);
	result = (TPIN & (1 << TDATA));
	_delay_us(410);
	return result;
}

void writeBit(uint8_t bit) {
	setdirection(TDATA, 1);
	if (bit) {
		setpin(TDATA, 0);
		_delay_us(6);
		setpin(TDATA, 1);
		_delay_us(64);
	} else {
		setpin(TDATA, 0);
		_delay_us(60);
		setpin(TDATA, 1);
		_delay_us(10);
	}
}

uint8_t readBit() {
	uint8_t result;
	setdirection(TDATA, 1);
	setpin(TDATA,0);
	_delay_us(6);
	setpin(TDATA,1);
	_delay_us(9);
	setdirection(TDATA, 0);
	result = TPIN & (1 << TDATA);
	_delay_us(55);
	return result;
}

void OWWriteByte(int data)
{
        int loop;

        // Loop to write each bit in the byte, LS-bit first
        for (loop = 0; loop < 8; loop++)
        {
                writeBit(data & 0x01);

                // shift the data byte for the next bit
                data >>= 1;
        }
}

//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
int OWReadByte(void)
{
        int loop, result=0;

        for (loop = 0; loop < 8; loop++)
        {
                // shift the result to get it ready for the next bit
                result >>= 1;

                // if result is one, then set MS bit
                if (readBit())
                        result |= 0x80;
        }
        return result;
}

//-----------------------------------------------------------------------------
// Write a 1-Wire data byte and return the sampled result.
//
int OWTouchByte(int data)
{
        int loop, result=0;

        for (loop = 0; loop < 8; loop++)
        {
                // shift the result to get it ready for the next bit
                result >>= 1;

                // If sending a '1' then read a bit else write a '0'
                if (data & 0x01)
                {
                        if (readBit())
                                result |= 0x80;
                }
                else
                        writeBit(0);

                // shift the data byte for the next bit
                data >>= 1;
        }
        return result;
}

//-----------------------------------------------------------------------------
// Write a block 1-Wire data bytes and return the sampled result in the same
// buffer.
//
void OWBlock(unsigned char *data, int data_len)
{
        int loop;

        for (loop = 0; loop < data_len; loop++)
        {
                data[loop] = OWTouchByte(data[loop]);
        }
}

int readScratchpad(int bytes){
	//int byte[2];
	int result = 0;
	int intermediate = 0;
	for(int i = 0; i < bytes; i++){
		intermediate = OWReadByte();
		//byte[i] = intermediate;
		result |= intermediate << (8*i);
	}
	/*
	resetSensor();
	_delay_ms(2000);
	OWWriteByte(byte[0]);
	OWWriteByte(byte[1]);
	*/
	return result;
}

