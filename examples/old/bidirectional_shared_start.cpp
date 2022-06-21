#include "Arduino.h"

// C/C++ Libraries
#include <stdint.h>

// Custom Libraries
#include "bitwise.h"
#include "teensy_uart.h"

// Function Prototypes
void timer0ISR(void);
void button0ISR(void);
void button1ISR(void);

// Pin Assignments
const int button0_pin = 24;
const int button1_pin = 25;

// Objects
IntervalTimer timer0;
uint32_t timer0_period = 1000;		// timer period [ms]

// Global Variables
bool running = false;
int debounce_delay = 25;
int last_press_time = 0;

// Data to Send Via MATLAB
float tx[3] = {(float)running, -1.2, 3.4};		// Array (vector) of float32s to be transmitted every timestep
int n_tx = sizeof(tx) / sizeof(tx[0]);
float rx[2] = {};			// Array (vector) of floats to be received via UART
int n_rx = sizeof(rx) / sizeof(rx[0]);
int n_rx_bytes = sizeof(rx);
float correct_rx[2] = {-1.2, 3.4};		// values known to be sent by other device

void setup(){
	// Configure UART
	Serial.begin(115200);
	delay(100);

	// Configure Button Inputs/Interrupts
	pinMode(button0_pin, INPUT);
	attachInterrupt(digitalPinToInterrupt(button0_pin), button0ISR, RISING);
	pinMode(button1_pin, INPUT);
	attachInterrupt(digitalPinToInterrupt(button1_pin), button1ISR, RISING);

	//timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop(){}

void timer0ISR(void){
	//Serial.println("timer0 loop");
	tx[0] = (float)running;
	if(running == true){
	// Transmit an array of floats
		uartTransmit(tx, n_tx);
		if(Serial.available() >= n_rx_bytes){			// non blocking receive
			uartReceive(rx, n_rx);		// receive vector of numbers
		}
	}
}

// ISR to Start Program On Button Press
void button0ISR(void){
	if( (millis() - last_press_time) > debounce_delay){
		//Serial.println("button0");
		last_press_time = millis();
		if(running == false){
			uartFlushInputBuffer();				// flush any data on rx line
			running = true;
			tx[0] = (float)running;
			uartTransmit(tx, n_tx);
			timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
		}
	}
}

// ISR to For Emergency Stop On Button Press
void button1ISR(void){
	if( (millis() - last_press_time) > debounce_delay){
		//Serial.println("button1");
		last_press_time = millis();
		if(running == true){
			timer0.end();								// end program/timer ISR
			running = false;
			tx[0] = (float)running;
			uartTransmit(tx, n_tx);
		}
		uartFlushInputBuffer();				// flush any data on rx line
	}
}
