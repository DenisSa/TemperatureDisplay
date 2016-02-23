/*
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

#define CS PB3
#define DC PB4
#define CLK PB2
#define MOSI PB0

#define SS1306_OLED_PBM _BV(CLK) | _BV(MOSI) | _BV(DC) | _BV(CS)

#define OUT 1
#define IN 0

void transferByte(int address, int data);
void init();
void oled_clear();
void oled_puts(char *str);
void oled_putc_raw(char c);
void oled_putc(char c);
void oled_move(int row, int col);
void oled_move_raw(int row, int col);
