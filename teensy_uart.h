/*
 * teensy_uart.h
 *
 *  Created on: May 31, 2020
 *      Author: Bryan
 */

#ifndef TEENSY_UART_H_
#define TEENSY_UART_H_

#include "Arduino.h"
#include "stdio.h"
#include "bitwise.h"

void serialPrintArray(int* array, int n_ints);
void serialPrintArray(float* float_array, int n_floats);

void uartWrite(int tx_int);
void uartWrite(float tx_float);

void uartWrite(int* tx_ints, uint8_t tx_bytes[][4], int n_tx);
void uartWrite(int* tx_ints, int n_tx);

void uartWrite(float* tx_floats, uint8_t tx_bytes[][4], int n_tx);
void uartWrite(float* tx_floats, int n_tx);

void uartWriteMultiAscii(uint8_t* tx_msg, int n_bytes);

int uartReadInt(void);
int32_t uartReadInt32(void);
int16_t uartReadInt16(void);
float uartReadFloat(void);

void uartRead(int* rx_ints, uint8_t rx_bytes[][4], int n_rx);
void uartRead(int* rx_ints, int n_rx);

void uartRead(float* rx_ints, uint8_t rx_bytes[][4], int n_rx);
void uartRead(float* rx_ints, int n_rx);

void uartReadMultiAscii(uint8_t* rx_msg, int n_bytes);

#endif /* TEENSY_UART_H_ */
