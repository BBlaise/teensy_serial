#include "Arduino.h"

// C/C++ Libraries
#include <stdint.h>

// Custom Libraries
#include "bitwise.h"
#include "teensy_uart.h"

#include "odrive_controller.h"

// Function Prototypes
void timer0ISR(void);
void blinkLED(void);
void commandOnReceive(char rx);
void commandOnReceive(char* rx);

// Pin Assignments
const int led_pin = 13;

// Objects
IntervalTimer timer0;
uint32_t timer0_period = 1000;		// timer period [ms]

// Global Variables
char rx_char;
char rx_chars[2] = {};

void setup(){
	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, LOW);

	Serial.begin(115200);
	delay(100);
	Serial.flush();

	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){}

void serialEvent(){
	odriveTerminalCommand();
}

void commandOnReceive(char rx){
	switch(rx) {
	   case 'a': Serial.println('a'); break; case 'b': Serial.println('b'); break;
	}
}

void commandOnReceive(char* rx){
	switch(rx[0]) {
	   case 'a':
		   Serial.println('a'); break;
	   case 'b': Serial.println('b'); break;
	}
}

void timer0ISR(void){
	//Serial.println(rx_char);
}

void blinkLED(){
	digitalWrite(led_pin, HIGH);
	delay(250);
	digitalWrite(led_pin, LOW);
	delay(100);
}
