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
int rx_int = 0;			// individual integer recieved via UART
int rx_ints[2] = {};	// should be {1, 10}
int n_rx_ints = sizeof(rx_ints) / sizeof(rx_ints[0]);
int n_rx_bytes = n_rx_ints * sizeof(rx_ints[0]);

int led_pin = 13;

void setup(){
	pinMode(led_pin, OUTPUT);

	Serial.begin(115200);
	delay(100);

	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){}

void timer0ISR(void){
	//Serial.println("timer0 loop");

	Serial.println(Serial.available());

	// Receive a single integer
/*	if(Serial.available() >= sizeof(rx_int){
		rx_int = uartReadInt();
		//Serial.println(rx_int);
	}*/

	// Receive an array of integers
	if(Serial.available() >= n_rx_bytes){
		uartRead(rx_ints, n_rx_ints);
		//serialPrintArray(rx_ints, n_rx_ints);		// if using putty or serial monitor

		if(rx_ints[0] == 1 && rx_ints[1] == 10){
			blinkLED();
		}
	}
}

void blinkLED(){
	digitalWrite(led_pin, HIGH);
	delay(250);
	digitalWrite(led_pin, LOW);
	delay(100);
}
