// Do not remove the include below
#include "Arduino.h"

// C/C++ Libraries
#include <stdint.h>

// Custom Libraries
#include "bitwise.h"
#include "teensy_uart.h"

// Function Prototypes
void timer0ISR(void);			// ISR for UART communication with MATLAB
void blinkLED(void);

// Objects
IntervalTimer timer0;
uint32_t timer0_period = 1000;		// timer period [ms]

// Pin Assignments
int led_pin = 13;

// Global Variables
float tx[2] = {-1.2, 3.4};		// Array (vector) of float32s to be transmitted every timestep
int n_tx = sizeof(tx) / sizeof(tx[0]);

float rx[2] = {};			// Array (vector) of floats to be received via UART
int n_rx = sizeof(rx) / sizeof(rx[0]);
int n_rx_bytes = sizeof(rx);
float correct_rx[2] = {-1.2, 3.4};		// values known to be sent by other device

void setup(){
	pinMode(led_pin, OUTPUT);

	Serial.begin(115200);
	delay(100);

	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){
	// Receive Data from MATLAB (if available)
	if(Serial.available() >= n_rx_bytes){
	//if(Serial.available() >= n_rx_bytes + 1){		// if trailing newline is being sent (i.e. C++)
		uartReceive(rx, n_rx);		// receive vector of numbers
		Serial.read();				// read trailing newling (i.e. C++)

		uartTransmit(tx, n_tx);		// transmit vector of numbers
	}
}

void timer0ISR(void){
	//Serial.println("timer0_ISR");

	// Do Something
}

void blinkLED(){
	digitalWrite(led_pin, HIGH);
	delay(250);
	digitalWrite(led_pin, LOW);
	delay(100);
}
