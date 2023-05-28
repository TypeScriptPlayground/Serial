#include "serial.h"

/**
* @fn void serialOpen(void* port, const int baudrate, const int dataBits, const int parity, const int stopBits)
* @brief Opens the specified connection to a serial device.
* @param port The port to open the serial connection to
* @param baudrate The baudrate for the serial connection when reading/writing
* @param dataBits The data bits
* @param parity The parity bits
* @param stopBits The stop bits
* @return Returns the current status code
*/
void serialOpen(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity,
    const int stopBits
) {
    return OPEN(port, baudrate, dataBits, parity, stopBits);
}

/**
* @fn void serialClose()
* @brief Closes the specified connection to a serial device.
* @return Returns the current status code
*/
void serialClose() {
    return CLOSE();
}

/**
* @fn auto serialRead(void* buffer, const int bufferSize, const int timeout, const int multiplier) -> int
* @brief Reads the specified number of bytes into the buffer.
* **It is not guaranteed that the complete buffer will be fully read.**
* @param buffer The buffer in which the bytes should be read into
* @param bufferSize The size of the buffer
* @param timeout Timeout to cancel the read
* @param multiplier The time multiplier between reading
* @return Returns the current status code (negative) or number of bytes read
*/
auto serialRead(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    return READ(buffer, bufferSize, timeout, multiplier);
}

/**
* @fn auto serialReadUntil(void* buffer, const int bufferSize, const int timeout, const int mutilplier, void* searchString) -> int
* @brief Reads until the specified string is found. If the specified string is not found, the buffer is read full until there are no more bytes to read.
* **It is not guaranteed that the complete buffer will be fully read.**
* @param buffer The buffer in which the bytes should be read into
* @param bufferSize The size of the buffer
* @param timeout Timeout to cancel the read
* @param multiplier The time multiplier between reading
* @param searchString The string to search for
* @return Returns the current status code (negative) or number of bytes read
*/
auto serialReadUntil(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* untilChar
) -> int {
    return READ_UNTIL(buffer, bufferSize, timeout, multiplier, untilChar);
}

/**
* @fn auto serialWrite(void* buffer, const int bufferSize, const int timeout, const int multiplier) -> int
* @brief Writes the buffer to the serial device.
* **It is not guaranteed that the complete buffer will be fully written.**
* @param buffer The buffer in which the bytes should be read into
* @param bufferSize The size of the buffer
* @param timeout Timeout to cancel the read
* @param multiplieer The time multiplier between writing
* @return Returns the current status code (negative) or number of bytes written
*/
auto serialWrite(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    return WRITE(buffer, bufferSize, timeout, multiplier);
}

/**
* @fn auto serialGetPortsInfo(void* buffer, const int bufferSize, void* separator) -> int
* @brief Get all the available serial ports.
* @param buffer The buffer in which the bytes should be read into
* @param bufferSize The size of the buffer
* @param separator The separator for the array buffer
* @return Returns the current status code (negative) or number of ports found
*/
auto serialGetPortsInfo(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int {
    return GET_PORTS_INFO(buffer, bufferSize, separator);
}

void serialOnError(void (*func)(int code)) {
    ON_ERROR(func);
}
