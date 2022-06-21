// Teensy/Arduino Libraries
#include <Arduino.h>
#include <ArxTypeTraits.h>

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
void timer0ISR(void);
void blinkLED(int n_blinks);

// Pin Assignments
const int builtin_led_pin = LED_BUILTIN;

// Objects
IntervalTimer timer0;

void setup() {
	Serial.begin(115200);
	//Serial.begin(460800);
	while(!Serial);
	//delay(100);

	pinMode(builtin_led_pin, OUTPUT);
	digitalWrite(builtin_led_pin, LOW);

	uint32_t timer0_period = 1500;		// timer period [ms]
	timer0.begin(timer0ISR, timer0_period*1000);  // start program/timer ISR
}

void loop() {}

void timer0ISR(void) {
	//Serial.println("timer0 loop");

	// Transmit a single value
	// static int tx_msg = -10;
	// static float tx_msg = -1.2;
	//static char tx_msg = 'a';

	// Transmit an array of values
	static float tx_msg[2] = {-1.2, 3.4};			// Array (vector) of float32s to be transmitted every timestep
	// static int tx_msg[2] = {-1, 10000};			// Array (vector) of ints (int32_t) to be transmitted every timestep
	//static int32_t tx_msg[2] = {-1, 10000};		// Array (vector) of int32s to be transmitted every timestep
	//static int16_t tx_msg[2] = {-1, 10000};		// Array (vector) of int16s to be transmitted every timestep
	int n_tx = sizeof(tx_msg) / sizeof(tx_msg[0]);

	if (Serial) {
		// Transmit a Single Value
		// serialTransmitBinary(tx_msg);
		// serialTransmitAscii(tx_msg);
		// tx_msg--;

		// Transmit an Array of Values
		// serialTransmitBinary(tx_msg, n_tx);
		serialTransmitAscii(tx_msg, n_tx);
		// serialTransmitAscii(tx_msg, n_tx, ',');
		tx_msg[0]--; tx_msg[1]++;

		blinkLED(2);
	}
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

