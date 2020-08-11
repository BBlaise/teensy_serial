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

void uartFlushInputBuffer(void);		// clears any bytes waiting in the UART's receive buffer

// Print to Console Functions
void serialPrintArray(int* array, int n_ints);
void serialPrintArray(float* float_array, int n_floats);

/* ---------------- Transmit Functions - Binary ------------------------ */
// Transmit Individual Numbers
void uartTransmit(float tx);
void uartTransmit(int tx);
void uartTransmit(int32_t tx);
void uartTransmit(int16_t tx);
void uartTransmit(int8_t tx);
void uartTransmit(uint32_t tx);
void uartTransmit(uint16_t tx);

// Transmit An Array/Vector of Numbers
void uartTransmit(float* tx, int n_tx);
void uartTransmit(int* tx, int n_tx);
void uartTransmit(int32_t* tx, int n_tx);
void uartTransmit(int16_t* tx, int n_tx);
void uartTransmit(int8_t* tx, int n_tx);
void uartTransmit(uint32_t* tx, int n_tx);
void uartTransmit(uint16_t* tx, int n_tx);
void uartTransmit(uint8_t* tx, int n_tx);

/* ---------------- Receive Functions - Binary ------------------------ */
// Receive Individual Values
float uartReceiveFloat(void);
int uartReceiveInt(void);
int32_t uartReceiveInt32(void);
int16_t uartReceiveInt16(void);
int8_t uartReceiveInt8(void);
uint32_t uartReceiveUint32_t(void);
uint16_t uartReceiveUint16_t(void);

// Receive An Array/Vector of Values
void uartReceive(float* rx, int n_rx);
void uartReceive(int* rx, int n_rx);
void uartReceive(int32_t* rx, int n_rx);
void uartReceive(int16_t* rx, int n_rx);
void uartReceive(int8_t* rx, int n_rx);
void uartReceive(uint32_t* rx, int n_rx);
void uartReceive(uint16_t* rx, int n_rx);
void uartReceive(uint8_t* rx, int n_rx);

/* ---------------- Transmit Functions - ASCII ------------------------ */
// Transmit An Array/Vector of Numbers Coded as ASCII
void uartTransmitAscii(float* tx, int n_tx);

/* ---------------- Receive Functions - ASCII ------------------------ */
// Receive An Array/Vector of Numbers Coded as ASCII
void uartReceiveAscii(float* rx, int n_rx);

#endif /* TEENSY_UART_H_ */
