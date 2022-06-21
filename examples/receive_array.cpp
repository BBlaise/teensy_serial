#include "Arduino.h"

// Arduino Libraries

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
void timer1ISR(void);
void blinkLED(int led_pin, int n_blinks);
void redButtonISR(void);		// triggers start/motor control

// Pin Assignments
const int builtin_led_pin = 13;
const int yellow_led_pin = 10;
const int green_led_pin = 11;
const int red_button_pin = 14;			// start button (black)

// Global Variables
char rx_buffers[2][20] = {};
unsigned int buffer_indxs[2] = {0, 0};
bool terminator_received = false;
const int debounce_delay = 200;
volatile int last_red_press = 0;

// int32_t rx_msg[2] = {};				int32_t rx_expected[2] = {1, -111222333};
int rx_msg[2] = {};					int rx_expected[2] = {-1, 10000};
// float rx_msg[2] = {};				float rx_expected[2] = {-1.2, 3.4};

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
}


void loop() {
	// uint8_t rx_msg[2] = {};				uint8_t rx_expected[2] = {1, 254};
	// uint16_t rx_msg[2] = {};			uint16_t rx_expected[2] = {1, 65534};
	// uint32_t rx_msg[2] = {};			uint32_t rx_expected[2] = {1, 111222333};
	// int8_t rx_msg[2] = {};				int8_t rx_expected[2] = {-1, 126};
	// int16_t rx_msg[2] = {};				int16_t rx_expected[2] = {-1, -32766};
	// int32_t rx_msg[2] = {};				int32_t rx_expected[2] = {1, -111222333};
	// int rx_msg[2] = {};					int rx_expected[2] = {-1, 10000};
	// float rx_msg[2] = {};				float rx_expected[2] = {-1.2, 3.4};
	unsigned int n_rx = sizeof(rx_msg) / sizeof(rx_msg[0]);

	// -------- Receive An Array of Values As Binary --------- //
	//unsigned int n_rx_bytes = n_rx * sizeof(rx_msg[0]);				// receiving binary
	//if (Serial.available() >= n_rx_bytes) serialReceiveBinary(rx_msg, n_rx);		// if total amount of bytes expected -> read buffer data and convert from bytes to ints or floats

	// -------- Receive An Array of Values As Ascii -------- //
	if (terminator_received == true) {
		noInterrupts();

		//toConsole(rx_buffers[0]); toConsole('\t'); toConsoleLn(rx_buffers[1]);
		serialReceiveAscii(rx_msg, n_rx, rx_buffers);
		//toConsole(rx_msg[0]); toConsole(','); toConsoleLn(rx_msg[1]);

		// if(rx_msg[0] == rx_expected[0] && rx_msg[1] == rx_expected[1]) blinkLED(4);	// check to make sure values are as expected
		// else if(rx_msg[0] == rx_expected[0] && rx_msg[1] != rx_expected[1]) blinkLED(1);
		// else if(rx_msg[0] != rx_expected[0] && rx_msg[1] == rx_expected[1]) blinkLED(2);

		if(rx_msg[0] == rx_expected[0]) digitalWrite(yellow_led_pin, HIGH);	// check to make sure values are as expected
		if(rx_msg[1] == rx_expected[1]) digitalWrite(green_led_pin, HIGH);
		delay(100);
		digitalWrite(yellow_led_pin, LOW);
		digitalWrite(green_led_pin, LOW);

		terminator_received = false;
		interrupts();
	}
}

void serialEvent() {
	noInterrupts();
	terminator_received = serialReadChar(rx_buffers, buffer_indxs);
	interrupts();
}

// Blink the LED pin to debug
void blinkLED(int led_pin_, int n_blinks) {
	for (int ii = 0; ii < n_blinks; ii++) {
		digitalWrite(led_pin_, HIGH);
		delay(100);
		digitalWrite(led_pin_, LOW);
		delay(50);			
	}
}

void redButtonISR(void) {		          // close sending program and open serial monitor to confirm
	if( (millis() - last_red_press) > debounce_delay) {
		last_red_press = millis();
		Serial.println("Red Button ISR");
		Serial.println(rx_msg[0]); Serial.println(rx_msg[1]);
		digitalWrite(green_led_pin, LOW);
  	}
}

