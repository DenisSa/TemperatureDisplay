#define TDATA PB2
#define TPIN PINB
#define TDDR DDRB

uint8_t readTempData_ds18b20(float *temperature);
uint8_t resetSensor();
void OWBlock(unsigned char *data, int data_len);
int OWTouchByte(int data);
int OWReadByte(void);
void OWWriteByte(int data);
uint8_t readBit();
void writeBit(uint8_t bit);
int readScratchpad(int bytes);
