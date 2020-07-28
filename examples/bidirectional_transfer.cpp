// Do not remove the include below
#include "teensy_uart_devel.h"

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
float tx[2] = {5.6, -7.8};
int n_tx = sizeof(tx) / sizeof(tx[0]);
float rx[2] = {};	// should be {1.2, 2.3}
int n_rx = sizeof(rx) / sizeof(rx[0]);
int n_rx_bytes = sizeof(rx);
//int n_rx_bytes = n_rx * sizeof(rx[0]);

void setup(){
	pinMode(led_pin, OUTPUT);

	Serial.begin(115200);
	delay(100);

	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){
	if(rx[0] == 1.2 && rx[1] == -3.4){
		blinkLED();
		rx[0] = 0; rx[1] = 0;
	}
}

void timer0ISR(void){
	//Serial.println("timer0_ISR");

	// Transmit Data to MATLAB
	uartTransmit(tx, n_tx);

	// Receive Data from MATLAB (if available)
	if(Serial.available() >= n_rx_bytes){
		uartReceive(rx, n_rx);
		//serialPrintArray(rx_floats, n_rx_floats);		// if using putty or serial monitor
	}
}

void blinkLED(){
	digitalWrite(led_pin, HIGH);
	delay(250);
	digitalWrite(led_pin, LOW);
	delay(100);
}
