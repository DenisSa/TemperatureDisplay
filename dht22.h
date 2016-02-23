//#define DEBUG

#define TDATA PB1
#define TPIN PINB
#define TDDR DDRB
#define DHT_TIMEOUT 200

uint8_t readTempData(float *temperature, float *humidity);
void setpin(int pn, int val);
