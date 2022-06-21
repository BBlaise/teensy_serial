/*
 * teensy_serial.h
 *
 *  Created on: May 31, 2020
 *      Author: Bryan
 */

#ifndef TEENSY_SERIAL_H_
#define TEENSY_SERIAL_H_

#include "Arduino.h"
#include "HardwareSerial.h"
#include "ArxTypeTraits.h"
#include "bitwise.h"

void serialFlushInputBuffer(void);		// clears any bytes waiting in the UART's receive buffer
void serialFlushInputBuffer(HardwareSerial& serial);

////---------- Binary Functions ------- ////
template <typename T> void serialTransmitBinary(T& tx);
template <typename T> void serialTransmitBinary(T* tx_array, int n_tx);
template <typename T> T serialReceiveBinary(T& rx);
template <typename T> void serialReceiveBinary(T* rx_array, unsigned int n_tx);

// ----------------- Ascii Functions -------------- //
template <typename T> void serialTransmitAscii(T& tx_val, const char* terminator = "\r\n");
template <typename T> void serialTransmitAscii(T* tx_array, unsigned int n_tx, const char* delimeter = ",", const char* terminator = "\r\n");
// void serialTransmitAscii(uint8_t* tx_bytes, unsigned int n_tx, const char* delimeter = ",", const char* terminator = "\r\n");
// void serialTransmitAscii(uint16_t* tx_ints, unsigned int n_tx, const char* delimeter = ",", const char* terminator = "\r\n");
// void serialTransmitAscii(int* tx_ints, unsigned int n_tx, const char* delimeter = ",", const char* terminator = "\r\n");
// void serialTransmitAscii(float* tx_floats, unsigned int n_tx, const char* delimeter = ",", const char* terminator = "\r\n");
//void serialTransmitAsciiLabeled(int* int_array, int n_ints, char labels[][20]);
//void serialTransmitAsciiLabeled(float* float_array, int n_floats, char labels[][20]);
bool serialReadChar(char* rx_buffer, unsigned int& buffer_indx);
bool serialReadChar(char (*rx_buffer)[20], unsigned int* buffer_indx);
template <typename T> T serialReceiveAscii(T& rx, char (&rx_buffer)[20]);
template <typename T> void serialReceiveAscii(T* rx, unsigned int n_rx, char (*rx_buffer)[20]);

//// ---------------- String Functions ------------------------ */
void serialTransmitString(String str);
void serialTransmitString(HardwareSerial& serial, String str);
void serialTransmitString(usb_serial_class& serial, String str);
String serialReceiveString(void);
String serialReceiveString(unsigned long timeout);
String serialReceiveString(HardwareSerial& serial);
String serialReceiveString(usb_serial_class& serial);
String serialReceiveString(HardwareSerial& serial, unsigned long timeout);
String serialReceiveString(usb_serial_class& serial, unsigned long timeout);

// ------------Templated Functions Definitions ------------ //
template <typename T> void serialTransmitBinary(T* tx_array, int n_tx) {
	int n_bytes = sizeof(T);
	uint8_t tx_bytes[n_tx][n_bytes] = {};

	for (int ii = 0; ii < n_tx; ii++) {
		if (std::is_same<T, float>::value) floatToBytes(tx_array[ii], tx_bytes[ii]);
		else intToBytes(tx_array[ii], tx_bytes[ii]);
		for(int jj = 0; jj < n_bytes; jj++) Serial.write(tx_bytes[ii][jj]);
	}
}

// --- Receive Functions --- // 
// Receives a single binary-encoded numerical value via serial
template <typename T> T serialReceiveBinary(T& rx) {
	int n_bytes = sizeof(T);
	uint8_t rx_bytes[n_bytes] = {};
	for (int ii = 0; ii < n_bytes; ii++) rx_bytes[ii] = Serial.read();

	if (std::is_same<T, float>::value) {
		rx = bytesToFloat(rx_bytes);
	} else {
		rx = bytesToInt(rx_bytes, rx);
		//rx = bytesToInt(rx_bytes);			// doesn't work since it expects 4 bytes even if 2 byte int is needed
	}

	return rx;
}

// Receives an array of binary-encoded numerical values via serial
template <typename T> void serialReceiveBinary(T* rx_array, unsigned int n_tx) {
	int n_bytes = sizeof(T);
	uint8_t rx_bytes[n_bytes] = {};

	for (int ii = 0; ii < n_tx; ii++) {
		for (int jj = 0; jj < n_bytes; jj++) rx_bytes[jj] = Serial.read();

		if (std::is_same<T, float>::value) {
			rx_array[ii] = bytesToFloat(rx_bytes);
			// T rx = bytesToFloat(rx_bytes);
			// rx_array[ii] = rx;
		} else {
			rx_array[ii] = bytesToInt(rx_bytes, rx_array[ii]);
			// T rx = bytesToInt(rx_bytes, rx);
			// rx_array[ii] = rx;
		}
	}

}

// -------- Ascii Functions ----------- //
// Transmits an individaul ascii-encoded numerical value with an option to specify the terminator
template <typename T> void serialTransmitAscii(T& tx_val, const char* terminator = "\r\n") {
	Serial.print(tx_val);
	for (int ii = 0; ii < strlen(terminator); ii++) Serial.write(terminator[ii]);
}

// Transmits an array of numerical values as ascii with options to specify the delimeter and terminator
template <typename T> void serialTransmitAscii(T* tx_array, unsigned int n_tx, const char* delimeter = ",", const char* terminator = "\r\n") {
	for (int ii = 0; ii < n_tx; ii++) {
		Serial.print(tx_array[ii]);
		if(ii != n_tx-1) Serial.write(delimeter);
	}

	for (int ii = 0; ii < strlen(terminator); ii++) Serial.write(terminator[ii]);
}

template <typename T> T serialReceiveAscii(T& rx, char (&rx_buffer)[20]) {
	if (std::is_same<T, float>::value) rx = (float)atof(rx_buffer);
	else if (sizeof(rx) == 4) rx = atol(rx_buffer);
	else rx = atoi(rx_buffer);

	memset(rx_buffer, 0, sizeof(rx_buffer));      // Clear the current buffer
	return rx;
}

template <typename T> void serialReceiveAscii(T* rx, unsigned int n_rx, char (*rx_buffer)[20]) {
	for(int ii = 0; ii < n_rx; ii++) {
		if (std::is_same<T, float>::value) rx[ii] = atof(rx_buffer[ii]);
		else if (sizeof(rx[ii]) == 4) rx[ii] = atol(rx_buffer[ii]);
		else rx[ii] = atoi(rx_buffer[ii]);

		memset(rx_buffer[ii], 0, sizeof(rx_buffer[ii]));      // Clear the current buffer
	}
}


#endif /* TEENSY_UART_H_ */
