#include "Arduino.h"

// C/C++ Libraries
#include <stdint.h>

// Custom Libraries
#include "bitwise.h"
#include "teensy_uart.h"

// Function Prototypes
void timer0ISR(void);
void blinkLED(void);

// Pin Assignments
const int led_pin = 13;

// Objects
IntervalTimer timer0;
uint32_t timer0_period = 1000;		// timer period [ms]

// Global Variables
float rx_float = 0;			// individual integer received via UART
float rx[2] = {};			// Array (vector) of floats to be received via UART
float correct_rx[2] = {-1.2, 3.4};		// values known to be sent by other device
//int rx[2] = {};			// Array (vector) of int32_ts to be received via UART
//int correct_rx[2] = {-1, 10000};		// values known to be sent by other device
//int16_t rx[2] = {};			// Array (vector) of int16_ts to be received via UART
//int16_t correct_rx[2] = {-1, 10000};		// values known to be sent by other device
int n_rx = sizeof(rx) / sizeof(rx[0]);
int n_rx_bytes = sizeof(rx);

void setup(){
	pinMode(led_pin, OUTPUT);

	Serial.begin(115200);
	delay(100);

	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){}

void timer0ISR(void){
	//Serial.println("timer0 loop");

	// Receive a single float
/*	if(Serial.available() >= sizeof(rx_float){
		rx_float = uartReceiveInt();
	}*/

	// Receive an array of integers
	if(Serial.available() >= n_rx_bytes){
	//if(Serial.available() >= n_rx_bytes){			// if trailing newline is sent (C++)
		uartReceive(rx, n_rx);
		Serial.read();				// read newline character (i.e. if using C++)
		//serialPrintArray(rx_floats, n_rx_floats);		// if using putty or serial monitor

		if(rx[0] == correct_rx[0] && rx[1] == correct_rx[1]){
			blinkLED();
			rx[0] = 0; rx[1] = 0;
		}
	}
}

void blinkLED(){
	digitalWrite(led_pin, HIGH);
	delay(250);
	digitalWrite(led_pin, LOW);
	delay(100);
}
