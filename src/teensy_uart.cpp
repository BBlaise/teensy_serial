/*
 * teensy_uart.cpp
 *
 *  Created on: May 31, 2020
 *      Author: Bryan
 */

#include "teensy_uart.h"
#include "Arduino.h"
#include "bitwise.h"

// Print a tab-delimited array of integers to the serial monitor
void serialPrintArray(int* array, int n_vals){
	for(int ii = 0; ii < n_vals; ii++){
		Serial.print(array[ii]);
		Serial.write('\t');
	}

	Serial.write('\r');
	Serial.write('\n');
}

void serialPrintArray(float* array, int n_vals){
	for(int ii = 0; ii < n_vals; ii++){
		Serial.print(array[ii]);
		Serial.write('\t');
	}

	Serial.write('\r');
	Serial.write('\n');
}

/*------------------------------------ Transmit Functions -------------------------------------------*/
// This function transmits a single 32-bit integer via UART
void uartTransmit(int tx_int){
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
void uartTransmit(float tx_float){
	uint8_t tx_bytes[4] = {};
    floatToBytes(tx_float, tx_bytes);
	for(int jj = 0; jj < 4; jj++){
		Serial.write(tx_bytes[jj]);
	}
}


// This function transmits an array of 32-bit integers via UART
/*
void uartTransmit(int* tx_ints, int n_tx){
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

void uartTransmit(int* tx_ints, int n_tx){
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
void uartTransmit(int32_t* tx_ints, int n_tx){
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
void uartTransmit(int16_t* tx_ints, int n_tx){
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
void uartTransmit(float* tx_floats, int n_tx){
	 uint8_t tx_bytes[n_tx][4] = {};
    for(int ii = 0; ii < n_tx; ii++){
        floatToBytes(tx_floats[ii], tx_bytes[ii]);
        for(int jj = 0; jj < 4; jj++){
        	Serial.write(tx_bytes[ii][jj]);
        }
    }
}

void uartTransmitAscii(uint8_t* tx_msg, int n_bytes){
	for(int ii = 0; ii < n_bytes; ii++){
		Serial.print(tx_msg[ii]);
		Serial.write('\t');
	}
	Serial.write('\n');
	//Serial.println();
}

// This function receives bytes and returns an integer
int uartReceiveInt(void){
	int n_bytes = sizeof(int);
	uint8_t rx_bytes[n_bytes] = {};
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = Serial.read();
	}

	return bytesToInt(rx_bytes);
}

// This function receives bytes and returns a 32-bit integer
int32_t uartReceiveInt32(void){
	int n_bytes = 4;
	uint8_t rx_bytes[n_bytes] = {};
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = Serial.read();
	}

	int32_t rx_int = bytesToInt32(rx_bytes);
	return rx_int;
}

// This function receives bytes and returns a 16-bit integer
int16_t uartReceiveInt16(void){
	int n_bytes = 2;
	uint8_t rx_bytes[n_bytes] = {};
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = Serial.read();
	}

	int16_t rx_int = bytesToInt16(rx_bytes);
	return rx_int;
}

// This function receives 4 bytes and returns a 32-bit float
float uartReceiveFloat(void){
	uint8_t rx_bytes[4] = {};
	for(int ii = 0; ii < 4; ii++){
		rx_bytes[ii] = Serial.read();
	}

	float rx_float = bytesToFloat(rx_bytes);
	return rx_float;
}

// This function receives an array of 32-bit integers via UART
void uartReceive(int* rx_ints, int n_tx){
	uint8_t rx_bytes[n_tx][4] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	for(int jj = 0; jj < 4; jj++){
    		rx_bytes[ii][jj] = Serial.read();
    	}

    	rx_ints[ii] = bytesToInt32(rx_bytes[ii]);
    }
}

// This function receives an array of 32-bit integers via UART
void uartReceive(int32_t* rx_ints, int n_tx){
	uint8_t rx_bytes[n_tx][4] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	for(int jj = 0; jj < 4; jj++){
    		rx_bytes[ii][jj] = Serial.read();
    	}

    	rx_ints[ii] = bytesToInt32(rx_bytes[ii]);
    }
}

// This function receives an array of 32-bit integers via UART
void uartReceive(int16_t* rx_ints, int n_tx){
	uint8_t rx_bytes[n_tx][4] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	for(int jj = 0; jj < 2; jj++){
    		rx_bytes[ii][jj] = Serial.read();
    	}

    	rx_ints[ii] = bytesToInt16(rx_bytes[ii]);
    }
}

// This function receives an array of 32-bit floats via UART
void uartReceive(float* rx_ints, int n_tx){
	uint8_t rx_bytes[n_tx][4] = {};

    for(int ii = 0; ii < n_tx; ii++){
    	for(int jj = 0; jj < 4; jj++){
    		rx_bytes[ii][jj] = Serial.read();
    	}

    	rx_ints[ii] = bytesToFloat(rx_bytes[ii]);
    }
}

