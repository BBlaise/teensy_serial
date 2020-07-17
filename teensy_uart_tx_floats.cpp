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

float tx_floats[2] = {1.2, 3.4};		// Array to be transmitted
int n_tx_floats = sizeof(tx_floats) / sizeof(tx_floats[0]);

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

	// Transmit an array of floats
	uartWrite(tx_floats, n_tx_floats);

	//Serial.println();			// if outputting to a terminal
}
