/*
 * teensy_uart.h
 *
 *  Created on: May 31, 2020
 *      Author: Bryan
 */

#ifndef TEENSY_UART_H_
#define TEENSY_UART_H_

#include "Arduino.h"
#include "bitwise.h"

// Print to Console Functions
void serialPrintArray(int* array, int n_ints);
void serialPrintArray(float* float_array, int n_floats);

/* ---------------- Transmit Functions - Binary ------------------------ */
// Transmit Individual Numbers
void uartTransmit(float tx_float);
void uartTransmit(int tx_int);
void uartTransmit(int32_t tx_int);
void uartTransmit(int16_t tx_int);
void uartTransmit(int8_t tx_int);
void uartTransmit(uint32_t tx_int);
void uartTransmit(uint16_t tx_int);
void uartTransmit(uint8_t tx_int);

// Transmit An Array/Vector of Numbers
void uartTransmit(float* tx_floats, int n_tx);
void uartTransmit(int* tx_ints, int n_tx);
void uartTransmit(int32_t* tx_ints, int n_tx);
void uartTransmit(int16_t* tx_ints, int n_tx);
void uartTransmit(uint16_t* tx_ints, int n_tx);

/* ---------------- Receive Functions - Binary ------------------------ */
// Receive Individual Numbers
int uartReceiveInt(void);
int32_t uartReceiveInt32(void);
int16_t uartReceiveInt16(void);
float uartReceiveFloat(void);

// Receive An Array/Vector of Numbers
void uartReceive(int* rx_ints, int n_rx);
void uartReceive(int32_t* rx_ints, int n_rx);
void uartReceive(int16_t* rx_ints, int n_rx);
void uartReceive(float* rx_ints, int n_rx);

/* ---------------- Transmit Functions - ASCII ------------------------ */
// Transmit An Array/Vector of Numbers Coded as ASCII
void uartTransmitAscii(uint8_t* tx, int n_tx);

/* ---------------- Receive Functions - ASCII ------------------------ */
// Receive An Array/Vector of Numbers Coded as ASCII
void uartReceiveAscii(uint8_t* rx, int n_rx);

#endif /* TEENSY_UART_H_ */
