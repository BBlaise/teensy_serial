# TeensyUART
This repository implements UART/Serial communication using the Teensy microcontrollers.

## Overview
It abstracts away the low-level formatting process needed to send an array/vector of integers or floats to another serial device. It allows the transmission and reception of data in binary or ASCII format. It also allows the use of multiple Serial buses via an object-oriented framework. This is useful if communicating using both the USB port and TX/RX pins simultaneously. However, it also provides a non-OOP implementation for those only interested in using the USB port. The non-OOP implementation is titled "teensy_uart" while the OOP implementation is titled "TeensyUART".

### Required Libraries
"bitwise.h" - Performs low-level bit operations in C++. Necesary to convert binary data to decimal and vice-versa.

# Functions - teensy_uart

## Transmit Functions

**uartTransmit(float tx)** - This is an overloaded function that can be used to transmit a single value as binary data. It converts the value into bytes then sends the bytes in a sequence. It is overloaded to accept the following types: float, int, int32_t, int16_t, int8_t, uint32_t, uint16_t, uint8_t (byte).
- tx: value to send via UART

Send an individual integer:
```
int tx = -12345;					// individual value (int32_t) to be transmitted
uartTransmit(tx);					// convert to bytes (four) and transmit them in a sequence
```

**uartTransmit(float* tx, int n_tx)** - This is an overloaded function that can be used to transmit an array (1-D) of values as binary data. It converts each value into bytes then sends the bytes in a sequence. It is overloaded to accept arrays of the following types: float, int, int32_t, int16_t, int8_t, uint32_t, uint16_t.
- tx: 1-D array (pointer) of values
- n_tx: the number of values in the array

```
float tx[2] = {-1.2, 3.4};				// array (vector) of float32s to be transmitted
int n_tx = sizeof(tx) / sizeof(tx[0]);	// number of values being sent
uartTransmit(tx, n_tx);					// convert the values to bytes (float = four bytes) and transmit them in a sequence (8 bytes total)
```

## Receive Functions

**int uartReceiveInt(void)** - This is an overloaded function that can be used to recive a single integer that available on the bus as binary data. It converts the binary data into decimal then returns that value. It is also implemented to receive other data types via similar functions:

- **float uartReceiveFloat(void)**
- **int32_t uartReceiveInt32(void)**
- **int16_t uartReceiveInt16(void)**
- **int8_t uartReceiveInt8(void)**
- **uint32_t uartReceiveUint32(void)**
- **uint16_t uartReceiveUint16(void)**

Receive an individual float:
```
float rx;										// decimal value to be read on serial line
int n_rx_bytes = sizeof(rx);					// number of bytes to be received
if Serial.available() >= n_rx_bytes{			// check if all bytes areavailable on the bus
	rx = uartReceive();							// receive four bytes representing a float and return that float
}
```

**uartReceive(float* rx, int n_rx)** - This is an overloaded function that can be used to receive an array (1-D) of values as binary data. It converts each value into bytes then sends the bytes in a sequence. It is overloaded to accept arrays of the following types: float, int, int32_t, int16_t, int8_t, uint32_t, uint16_t, uint8_t (byte).
- rx: 1-D buffer/array (pointer) of values to store received decimal values
- n_rx: the number of values in the array

```
int16_t rx[2] = {};							// 1-D buffer/array of int16_ts to be received
int n_rx = sizeof(rx) / sizeof(rx[0]);		// number of values to receive
int n_rx_bytes = sizeof(rx);				// number of bytes to receive (four total)
if(Serial.avaialble() >= n_rx_bytes){
	uartReceive(rx, n_rx);					// convert the values to bytes and transmit them in a sequence (4 bytes total)
}
```

# Examples - teensy_uart
The following example scripts are included in the folder **examples > teensy_uart_examples** to demonstrate the functionality of the library.

**transmit.cpp** - Transmit an array of either floats or ints periodically via UART\
**receive.cpp** - Receive an array of either floats or ints whenever the appropriate amount of bytes are available\
**bidirectional_driving.cpp** - Send and receive an array of numbers with the Teensy controlling the timing of communication\
**bidirectonal_driven.cpp** - Send and receive an array of numbers with the other device controlling the timing of communication\
**bidirectional_shared_start.cpp** - Bidirectional communication that includes protocol to start both devices at the same time\
**bidirectional_error_checking.cpp** - Bidirectional communication with error checking to ensure data transmissions/receptions are complete\

# Methods - TeensyUART

# Examples - TeensyUART


## Authors

* **Bryan Blaise** - *Initial work* -

## License

## Acknowledgments