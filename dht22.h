//#define DEBUG

#define TDATA PB1
#define TPIN PINB
#define TDDR DDRB
#define DHT_TIMEOUT 200

uint8_t readTempData_dht22(float *temperature, float *humidity);
