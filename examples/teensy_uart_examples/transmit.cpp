#include "Arduino.h"

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

float tx[2] = {-1.2, 3.4};		// Array (vector) of float32s to be transmitted every timestep
//int tx[2] = {-1, 10000};		// Array (vector) of ints (int32_t) to be transmitted every timestep
//int32_t tx[2] = {-1, 10000};		// Array (vector) of int32s to be transmitted every timestep
//int16_t tx[2] = {-1, 10000};		// Array (vector) of int16s to be transmitted every timestep
int n_tx = sizeof(tx) / sizeof(tx[0]);

void setup(){
	Serial.begin(115200);
	//Serial.begin(460800);
	delay(100);

	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){}

void timer0ISR(void){
	//Serial.println("timer0 loop");

	// Transmit a single integer
	//uartTransmit(tx_int);

	// Transmit an array of floats
	uartTransmit(tx, n_tx);
	//Serial.write('\n');			// if transmitting to C++ script
}
