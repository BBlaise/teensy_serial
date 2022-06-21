/*
 * teensy_serial.cpp
 *
 *  Created on: May 31, 2020
 *      Author: Bryan
 */

#include "teensy_serial.h"
#include "Arduino.h"
#include "HardwareSerial.h"
#include "ArxTypeTraits.h"
#include "bitwise.h"

/* ------------------------------------ Transmit Functions - ASCII ------------------------------------------- */
// This function clears any bytes waiting in the UART's receive buffer
void serialFlushInputBuffer(void) {
	  while(Serial.available()) Serial.read();
}

void serialFlushInputBuffer(HardwareSerial& serial) {
	  while(serial.available()) Serial.read();
}

// Print a tab-delimited array of integers to the serial monitor
void serialTransmitAscii(int* array, int n_vals) {
	for(int ii = 0; ii < n_vals; ii++){
		Serial.print(array[ii]);
		if(ii < n_vals-1) Serial.write('\t');
	}

	//Serial.write('\r');
	Serial.write('\n');
}

void serialTransmitAscii(int* array, int n_vals, char delimeter) {
	Serial.println();
	for(int ii = 0; ii < n_vals; ii++){
		Serial.print(array[ii]);
		if(ii < n_vals-1) Serial.write(delimeter);
	}

	//Serial.write('\r');
	Serial.write('\n');
}

void serialTransmitAscii(float* array, int n_vals) {
	for(int ii = 0; ii < n_vals; ii++){
		Serial.print(array[ii], 3);
		if(ii < n_vals-1) Serial.write('\t');
	}

	//Serial.write('\r');
	Serial.write('\n');
}

void serialTransmitAscii(float* array, int n_vals, char delimeter) {
	for(int ii = 0; ii < n_vals; ii++){
		Serial.print(array[ii], 3);
		if(ii < n_vals-1) Serial.write(delimeter);
	}

	//Serial.write('\r');
	Serial.write('\n');
}

void serialTransmitAscii(float* array, int n_vals, int precision) {
	for(int ii = 0; ii < n_vals; ii++){
		Serial.print(array[ii], precision);
		Serial.write('\t');
	}

	//Serial.write('\r');
	Serial.write('\n');
}

void serialTransmitAsciiLabeled(int* array, int n_ints, char labels[][20]) {
	for(int ii = 0; ii < n_ints; ii++){
		Serial.print(labels[ii]);
		Serial.print(": ");
		Serial.print(array[ii]);
		Serial.write('\t');
	}

	//Serial.write('\r');
	Serial.write('\n');
}

void serialTransmitAsciiLabeled(float* array, int n_floats, char labels[][20]) {
	for(int ii = 0; ii < n_floats; ii++){
		Serial.print(labels[ii]);
		Serial.print(": ");
		Serial.print(array[ii], 3);
		Serial.write('\t');
	}

	//Serial.write('\r');
	Serial.write('\n');
}

/* ------------------------------------ Receive Functions - ASCII ------------------------------------------- */
bool serialReadChar(char* rx_buffer, unsigned int& buffer_indx) {
	//TODO use strstr(bigstring, substring) to check if terminator is in buffer
	char newline = '\n'; 				// line feed. send with (CTRL + J) on Putty
	char carriage_return = '\r';		// carriage return. send with ENTER on Putty
	//char terminator[1] = {newline}; unsigned int terminator_len = 1;  // for MATLAB
	//char terminator[1] = {carriage_return}; unsigned int terminator_len = 1;  // for Putty
	char terminator[2] = {carriage_return, newline}; unsigned int terminator_len = 2;
	//char terminator[2] = {newline, carriage_return}; unsigned int terminator_len = 2;
	//char temrinator[2] = {'A', '\n'}; unsigned int terminator_len = 2;
	bool terminator_received = false;

	char next_char = Serial.read();
	if ( buffer_indx == 0 && (next_char == newline || next_char == carriage_return) ) { 
		// Case A: if newline or carriage return arrives at beginning of transmission
		terminator_received = false;
		// Serial.println("A");
	} else if (terminator_len == 1 && next_char == terminator[0]) {
		// Case B: terminator is single char and matches incomming char
		buffer_indx = 0; 
		terminator_received = true;
		// Serial.println("B");
	} else if ( terminator_len == 2 && next_char == terminator[1] && rx_buffer[buffer_indx-1] == terminator[0] ) {
		// Case C: If incomming char and previous char match terminator sequence
		rx_buffer[buffer_indx-1] = 0;			// clear terminator from buffer
		buffer_indx = 0;
		terminator_received = true;
		// Serial.println("C");
	} else {
		// If not receiving last char in terminator
		rx_buffer[buffer_indx] = next_char;
		buffer_indx++;
		terminator_received = false;
		// Serial.println("D");
	}

	return terminator_received;
}

bool serialReadChar(char (*rx_buffer)[20], unsigned int* buffer_indxs) {
	char newline = '\n'; char carriage_return = '\r';
	char delimeter = ','; //char delimeter = '\t'; // char delimeter = ' ';
	//char terminator[1] = {newline}; unsigned int terminator_len = 1;  // for MATLAB
	//char terminator[1] = {carriage_return}; unsigned int terminator_len = 1;  // for Putty
	char terminator[2] = {carriage_return, newline}; unsigned int terminator_len = 2;
	//char terminator[2] = {newline, carriage_return}; unsigned int terminator_len = 2;
	//char temrinator[2] = {'A', '\n'}; unsigned int terminator_len = 2;
	bool terminator_received = false;

	int ii = buffer_indxs[0]; int jj = buffer_indxs[1];

	char next_char = Serial.read();
	if ( (ii == 0 && jj == 0) && (next_char == newline || next_char == carriage_return || next_char == delimeter) ) { 
		terminator_received = false;
	} else if ( (terminator_len == 1 && next_char == terminator[0]) ) {
		buffer_indxs[0] = 0; buffer_indxs[1] = 0;
		terminator_received = true;
	} else if ( terminator_len == 2 && (next_char == terminator[1]) && (rx_buffer[ii][jj-1] == terminator[0])) {
	//( (next_char == newline) && rx_buffer[ii][jj-1] == terminator[0]) {
		rx_buffer[ii][jj-1] = 0;			// clear carriage_return or other elements of terminator in buffer
		buffer_indxs[0] = 0; buffer_indxs[1] = 0;
		terminator_received = true;
	} else if (next_char == delimeter) {
        buffer_indxs[0]++;
        buffer_indxs[1] = 0;
		terminator_received = false;
    } else {
		rx_buffer[ii][jj] = next_char;
		buffer_indxs[1]++;
		terminator_received = false;
	}
	
	return terminator_received;
}
//void serialReceiveAscii(uint8_t* rx_msg, int n_bytes){}

/* ------------------------------------ Transmit Functions - Binary ------------------------------------------- */

// This function transmits a single 32-bit integer via UART
void serialTransmit(int tx_int) {
	uint8_t tx_bytes[4] = {};
	tx_bytes[0] = (tx_int >> 24) & 0xFF;
	tx_bytes[1] = (tx_int >> 16) & 0xFF;
	tx_bytes[2] = (tx_int >> 8) & 0xFF;
	tx_bytes[3] = tx_int & 0xFF;
	for(int jj = 0; jj < 4; jj++){
		Serial.write(tx_bytes[jj]);
	}
}

// This function transmits a single 32-bit float via UART
void serialTransmitBinary(float tx_float) {
	uint8_t tx_bytes[4] = {};
    floatToBytes(tx_float, tx_bytes);
	for(int jj = 0; jj < 4; jj++){
		Serial.write(tx_bytes[jj]);
	}
}

// This function transmits an array of 32-bit integers via UART
/*
void serialTransmitBinary(int* tx_ints, int n_tx){
	uint8_t tx_bytes[n_tx][4] = {};
    for(int ii = 0; ii < n_tx; ii++){
        tx_bytes[ii][0] = (tx_ints[ii] >> 24) & 0xFF;
        tx_bytes[ii][1] = (tx_ints[ii] >> 16) & 0xFF;
        tx_bytes[ii][2] = (tx_ints[ii] >> 8) & 0xFF;
        tx_bytes[ii][3] = tx_ints[ii] & 0xFF;
        for(int jj = 0; jj < 4; jj++){
        	Serial.write(tx_bytes[ii][jj]);
        }
    }
}
*/

void serialTransmitBinary(int* tx_ints, int n_tx) {
	int int_size = sizeof(tx_ints[0]);
	uint8_t tx_bytes[n_tx][int_size] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	intToBytes(tx_ints[ii], tx_bytes[ii]);
    	//int32ToBytes(tx_ints[ii], tx_bytes[ii]);
        for(int jj = 0; jj < int_size; jj++){
        	Serial.write(tx_bytes[ii][jj]);
        }
    }
}

// This function transmits an array of 32-bit integers via UART
void serialTransmitBinary(int32_t* tx_ints, int n_tx){
	int int_size = 4;
	uint8_t tx_bytes[n_tx][int_size] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	int32ToBytes(tx_ints[ii], tx_bytes[ii]);
        for(int jj = 0; jj < int_size; jj++){
        	Serial.write(tx_bytes[ii][jj]);
        }
    }
}

// This function transmits an array of 16-bit integers via UART
void serialTransmitBinary(int16_t* tx_ints, int n_tx){
	int int_size = 2;
	uint8_t tx_bytes[n_tx][int_size] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	int16ToBytes(tx_ints[ii], tx_bytes[ii]);
        for(int jj = 0; jj < int_size; jj++){
        	Serial.write(tx_bytes[ii][jj]);
        }
    }
}

// This function transmits an array of 32-bit floats via UART
void serialTransmitBinary(float* tx_floats, int n_tx){
	 uint8_t tx_bytes[n_tx][4] = {};
    for(int ii = 0; ii < n_tx; ii++){
        floatToBytes(tx_floats[ii], tx_bytes[ii]);
        for(int jj = 0; jj < 4; jj++){
        	Serial.write(tx_bytes[ii][jj]);
        }
    }
}

/* ------------------------------------ Receive Functions - Binary ------------------------------------------- */

// This function receives bytes and returns an integer
int serialReceiveBinaryInt(void){
	int n_bytes = sizeof(int);
	uint8_t rx_bytes[n_bytes] = {};
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = Serial.read();
	}

	return bytesToInt(rx_bytes);
}

// This function receives bytes and returns a 32-bit integer
int32_t serialReceiveBinaryInt32(void){
	int n_bytes = 4;
	uint8_t rx_bytes[n_bytes] = {};
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = Serial.read();
	}

	int32_t rx_int = bytesToInt32(rx_bytes);
	return rx_int;
}

// This function receives bytes and returns a 16-bit integer
int16_t serialReceiveBinaryInt16(void){
	int n_bytes = 2;
	uint8_t rx_bytes[n_bytes] = {};
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = Serial.read();
	}

	int16_t rx_int = bytesToInt16(rx_bytes);
	return rx_int;
}

// This function receives 4 bytes and returns a 32-bit float
float serialReceiveBinaryFloat(void){
	uint8_t rx_bytes[4] = {};
	for(int ii = 0; ii < 4; ii++){
		rx_bytes[ii] = Serial.read();
	}

	float rx_float = bytesToFloat(rx_bytes);
	return rx_float;
}

// This function receives an array of 32-bit integers via UART
void serialReceiveBinary(int* rx_ints, int n_tx){
	uint8_t rx_bytes[n_tx][4] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	for(int jj = 0; jj < 4; jj++){
    		rx_bytes[ii][jj] = Serial.read();
    	}

    	rx_ints[ii] = bytesToInt32(rx_bytes[ii]);
    }
}

// This function receives an array of 32-bit integers via UART
void serialReceiveBinary(int32_t* rx_ints, int n_tx){
	uint8_t rx_bytes[n_tx][4] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	for(int jj = 0; jj < 4; jj++){
    		rx_bytes[ii][jj] = Serial.read();
    	}

    	rx_ints[ii] = bytesToInt32(rx_bytes[ii]);
    }
}

// This function receives an array of 32-bit integers via UART
void serialReceiveBinary(int16_t* rx_ints, int n_tx){
	uint8_t rx_bytes[n_tx][4] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	for(int jj = 0; jj < 2; jj++){
    		rx_bytes[ii][jj] = Serial.read();
    	}

    	rx_ints[ii] = bytesToInt16(rx_bytes[ii]);
    }
}

// This function receives an array of 32-bit floats via UART
void serialReceiveBinary(float* rx_ints, int n_tx){
	uint8_t rx_bytes[n_tx][4] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	for(int jj = 0; jj < 4; jj++){
    		rx_bytes[ii][jj] = Serial.read();
    	}

    	rx_ints[ii] = bytesToFloat(rx_bytes[ii]);
    }
}


/* ------------------------------------ Transmit Functions - Strings ------------------------------------------- */
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

void serialTransmitString(String str){
	Serial << str;
}

void serialTransmitString(HardwareSerial& serial, String str){
	serial << str;
}

void serialTransmitString(usb_serial_class& serial, String str){
	serial << str;
}


/* ------------------------------------ Receive Functions - Strings ------------------------------------------- */
String serialReceiveString(void){
    String str = "";
    for (;;) {
        while (!Serial1.available()) {}
        char c = Serial1.read();
        if (c == '\n') break;
        str += c;
    }
    return str;
}

String serialReceiveString(unsigned long timeout){
    String str = "";
    unsigned long timeout_start = millis();
    for (;;) {
        while (!Serial.available()) {
            if (millis() - timeout_start >= timeout) return str;
        }
        char c = Serial.read();
        if(c == '\n') break;
        str += c;
    }
    return str;
}

String serialReceiveString(HardwareSerial& serial){
    String str = "";
    for (;;) {
        while (!serial.available()) {}
        char c = serial.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}

String serialReceiveString(HardwareSerial& serial, unsigned long timeout){
    String str = "";
    unsigned long timeout_start = millis();
    for (;;) {
        while (!serial.available()) {
            if (millis() - timeout_start >= timeout) {
                return str;
            }
        }
        char c = serial.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}
