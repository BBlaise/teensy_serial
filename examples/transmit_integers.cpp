// Do not remove the include below
#include "teensy_uart_devel.h"

// C/C++ Libraries
#include <stdint.h>

// Custom Libraries
#include "bitwise.h"
#include "teensy_uart.h"

// Function Prototypes
void timer0ISR(void);

// Objects
IntervalTimer timer0;
uint32_t timer0_period = 1000;		// timer period [ms]

int tx_int = 1633837924;			// = abcd in ASCII. Single integer to be transmitted via UART
//int tx_ints[2] = {1633837924, 1701209960};		// abcdefgh in ASCII. Array to be transmitted
int tx_ints[2] = {3, 4};		// Array to be transmitted
int n_tx_ints = sizeof(tx_ints) / sizeof(tx_ints[0]);

void setup(){
	Serial.begin(115200);
	delay(100);

	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){}

void timer0ISR(void){
	//Serial.println("timer0 loop");

	// Transmit a single integer
	//uartWrite(tx_int);

	// Transmit an array of integers
	uartTransmit(tx_ints, n_tx_ints);

	//Serial.println();			// if outputting to a terminal
}
