// Do not remove the include below
#include "teensy_uart_devel.h"

// C/C++ Libraries
#include <stdint.h>

// Custom Libraries
#include "bitwise.h"
#include "teensy_uart.h"

// Function Prototypes
void timer0ISR(void);
void blinkLED(void);

// Objects
IntervalTimer timer0;
uint32_t timer0_period = 1000;		// timer period [ms]

// Global Variables
float rx_float = 0;			// individual integer recieved via UART
float rx_floats[2] = {};	// should be {1.2, 2.3}
int n_rx_floats = sizeof(rx_floats) / sizeof(rx_floats[0]);
int n_rx_bytes = n_rx_floats * sizeof(rx_floats[0]);

int led_pin = 13;

void setup(){
	pinMode(led_pin, OUTPUT);

	Serial.begin(115200);
	delay(100);

	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){
	if(rx_floats[0] == 1.2 && rx_floats[1] == 2.3){
		blinkLED();
		rx_floats[0] = 0; rx_floats[1] = 0;
	}
}

void timer0ISR(void){
	//Serial.println("timer0 loop");

	//Serial.println(Serial.available());

	// Receive a single float
/*	if(Serial.available() >= sizeof(rx_float){
		rx_float = uartReadInt();
		//Serial.println(rx_float);
	}*/

	// Receive an array of integers
	if(Serial.available() >= n_rx_bytes){
		uartReceive(rx_floats, n_rx_floats);
		//serialPrintArray(rx_floats, n_rx_floats);		// if using putty or serial monitor

		//blinkLED();
		//if(rx_floats[0] == 1.2 && rx_floats[1] == 2.3){blinkLED();}
	}
}

void blinkLED(){
	digitalWrite(led_pin, HIGH);
	delay(250);
	digitalWrite(led_pin, LOW);
	delay(100);
}