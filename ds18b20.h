#define TDATA PB1
#define TPIN PINB
#define TDDR DDRB
#define DHT_TIMEOUT 200

uint8_t readTempData_ds18b20(float *temperature);
uint8_t resetSensor();
void OWBlock(unsigned char *data, int data_len);
int OWTouchByte(int data);
int OWReadByte(void);
void OWWriteByte(int data);
uint8_t readBit();
void writeBit(uint8_t bit);
int readScratchpad(int bytes);
