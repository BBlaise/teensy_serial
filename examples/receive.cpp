#include "Arduino.h"

// Arduino Libraries
#include <TimerOne.h>

// C/C++ Libraries

// Custom Libraries
#include "bitwise.h"
#include "teensy_serial.h"

// Preprocessor Directives
#define DEBUG 0
#if DEBUG == 1
#define toConsole(x) Serial.print(x);
#define toConsoleLn(x) Serial.println(x);
#else
#define toConsole(x)
#define toConsoleLn(x)
#endif

// Function Prototypes
void timer1ISR(void);			// timer loop
void blinkLED(int n_blinks);
void redButtonISR(void);		// triggers start/motor control
const int red_button_pin = 14;			// start button (black)

// Pin Assignments
const int builtin_led_pin = 13;
const int yellow_led_pin = 10;
const int green_led_pin = 11;

// Global Variables
char rx_buffer[20] = {};
unsigned int buffer_indx = 0;
bool terminator_received = false;
const int debounce_delay = 200;
volatile int last_red_press = 0;

float rx_msg = 0.0;		float rx_expected = -1.2;

void setup(){
	pinMode(builtin_led_pin, OUTPUT);				// configure LED pin for debugging
	pinMode(yellow_led_pin, OUTPUT); digitalWrite(yellow_led_pin, LOW);
	pinMode(green_led_pin, OUTPUT); digitalWrite(green_led_pin, LOW);
	pinMode(red_button_pin, INPUT); attachInterrupt(digitalPinToInterrupt(red_button_pin), redButtonISR, FALLING);

	uint32_t baud_rate = 115200;
	Serial.begin(baud_rate);				// open serial port
	//Serial.setTimeout(1500);				
	delay(100);								// wait for serial port to open
	Serial.flush();

	serialFlushInputBuffer();					// clear any bytes that are in the UART buffer

	uint32_t timer1_period = 1000;		// timer period [in milliseconds]
	Timer1.initialize(timer1_period*1000);
	Timer1.attachInterrupt(timer1ISR);		// start program/timer ISR
}

// Do all your calculations/sensor readings in this folder
void timer1ISR(void) {}

void loop() {
	//uint8_t rx_msg = 0;		 uint8_t rx_expected = 254U;
	//uint16_t rx_msg = 0;			uint16_t rx_expected = 65534U;
	//uint32_t rx_msg = 0;		unsigned long rx_expected = 111222333UL;
	//int8_t rx_msg = 0;				int8_t rx_expected = -126;
	// int16_t rx_msg = 0;				int16_t rx_expected = -32766;
	//int32_t rx_msg = 0;			long rx_expected = -111222333;
	// int rx_msg = 0;		int rx_expected = -10;
	// float rx_msg = 0.0;		float rx_expected = -1.2;

	// ---------- Receive Individual Values As Binary -------- //
	// int n_rx_bytes = sizeof(rx_msg);
	// if (Serial.available() >= n_rx_bytes) {
	// 	rx_msg = serialReceiveBinary(rx_msg);
	// 	if (rx_msg == rx_expected) blinkLED(n_rx_bytes);
	// }

	// -------- Receive Individual Values As Ascii -------- //
	if (terminator_received == true) {
		noInterrupts();

		// toConsoleln(rx_buffer);
		rx_msg = serialReceiveAscii(rx_msg, rx_buffer);	
		if (rx_msg == rx_expected) digitalWrite(yellow_led_pin, HIGH);//blinkLED(3);		// might not work if float b/c precision but number is correct
		// if (rx_msg =! (int)0) digitalWrite(yellow_led_pin, HIGH);//blinkLED(3);		// might not work if float b/c precision but number is correct
		// toConsoleLn(rx_msg);					// shows that floats are coming in correctly

		delay(100); 
		digitalWrite(yellow_led_pin, LOW);
		digitalWrite(green_led_pin, LOW);

		terminator_received = false;
		interrupts();
	}
}

void serialEvent() {
	terminator_received = serialReadChar(rx_buffer, buffer_indx);
	digitalWrite(green_led_pin, HIGH);
}

// Blink the LED pin to debug
void blinkLED(int n_blinks) {
	for (int ii = 0; ii < n_blinks; ii++) {
		digitalWrite(builtin_led_pin, HIGH);
		delay(100);
		digitalWrite(builtin_led_pin, LOW);
		delay(50);			
	}
}

void redButtonISR(void) {		          // close sending program and open serial monitor to confirm
	if( (millis() - last_red_press) > debounce_delay) {
		last_red_press = millis();
		Serial.println("Red Button ISR");
		Serial.println(rx_msg, 4);
		digitalWrite(green_led_pin, LOW);
  	}
}
