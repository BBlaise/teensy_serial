#include "Arduino.h"
#include "ArxTypeTraits.h"

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
void timer0ISR(void);

// Objects
IntervalTimer timer0;

// Pin Assignments
const int builtin_led_pin = 13;
const int yellow_led_pin = 10;
const int green_led_pin = 11;

// Global Variables
const char* delimeter = ",";
// const char* delimeter = "\t";
const char* terminator = "\r\n";		// write extra newline if transmitting to a C++ script
// const char* terminator = "\r\n\n";		// write extra newline if transmitting to a C++ script

// const size_t tx_len = 2; static long tx_msg[tx_len] = {1, -111222333};
const size_t tx_len = 2; static int tx_msg[tx_len] = {-1, 10000};		// Array of int values (int16_t) be transmitted
// const size_t tx_len = 2; static float tx_msg[tx_len] = {-1.2, 3.4};		// Array of float values to be transmitted
// const size_t tx_len = 3; static float tx_msg[tx_len] = {-1.2, 3.4, -5.6};		// Array of float values to be transmitted
// size_t tx_len = sizeof(tx_msg) / sizeof(tx_msg[0]); // number of elements to be transmitted
bool transmit_now = false;

const size_t rx_len = 2;
// int32_t rx_msg[2] = {};				int32_t rx_expected[2] = {1, -111222333};
int rx_msg[2] = {};					int rx_expected[2] = {-1, 10000};
// float rx_msg[2] = {};				float rx_expected[2] = {-1.2, 3.4};
char rx_buffers[rx_len][20] = {};
unsigned int buffer_indxs[2] = {0, 0};
bool terminator_received = false;

void setup() {
	pinMode(builtin_led_pin, OUTPUT);				// configure LED pin for debugging
	pinMode(yellow_led_pin, OUTPUT); digitalWrite(yellow_led_pin, LOW);
	pinMode(green_led_pin, OUTPUT); digitalWrite(green_led_pin, LOW);
	
	uint32_t baud_rate = 115200;
	Serial.begin(baud_rate);					// open serial port
	delay(100);								// wait for serial port to open
	Serial.flush();
	serialFlushInputBuffer();		// clears any bytes waiting in the UART's receive buffer

	uint32_t timer0_period = 1500;		// timer period [ms]
	timer0.priority(130);
	timer0.begin(timer0ISR, timer0_period*1000); //running = true;
}

void loop() {
	// if (transmit_now == true) {
	// 	noInterrupts();
	// 	//serialTransmitBinary(tx_msg, tx_len);
	// 	//serialTransmitAscii(tx_msg, tx_len);
	// 	// serialTransmitAscii(tx_msg, tx_len, delimeter, terminator);
	// 	toConsole(tx_msg[0]); toConsole(delimeter); toConsoleLn(tx_msg[1]);
	// 	tx_msg[0]--; tx_msg[1]++;

	// 	transmit_now = false;
	// 	// interrupts
	// }

	if (terminator_received == true) {
		noInterrupts();

		//toConsole(rx_buffers[0]); toConsole('\t'); toConsoleLn(rx_buffers[1]);
		serialReceiveAscii(rx_msg, rx_len, rx_buffers);
		//toConsole(rx_msg[0]); toConsole(','); toConsoleLn(rx_msg[1]);

		// Blink to indicate correct message was received
		// if(rx_msg[0] == rx_expected[0] && rx_msg[1] == rx_expected[1]) blinkLED(4);	// check to make sure values are as expected
		// else if(rx_msg[0] == rx_expected[0] && rx_msg[1] != rx_expected[1]) blinkLED(1);
		// else if(rx_msg[0] != rx_expected[0] && rx_msg[1] == rx_expected[1]) blinkLED(2);

		// If using external LEDs, blink to indicate correct message was received
		if(rx_msg[0] == rx_expected[0]) digitalWrite(yellow_led_pin, HIGH);	// check to make sure values are as expected
		if(rx_msg[1] == rx_expected[1]) digitalWrite(green_led_pin, HIGH);
		delay(100);
		digitalWrite(yellow_led_pin, LOW);
		digitalWrite(green_led_pin, LOW);

		terminator_received = false;
		interrupts();
	}
}

void timer0ISR(void) {
	// transmit_now = true;
	//serialTransmitBinary(tx_msg, tx_len);
	//serialTransmitAscii(tx_msg, tx_len);
	serialTransmitAscii(tx_msg, tx_len, delimeter, terminator);
	toConsole(tx_msg[0]); toConsole(delimeter); toConsoleLn(tx_msg[1]);
	tx_msg[0]--; tx_msg[1]++;
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