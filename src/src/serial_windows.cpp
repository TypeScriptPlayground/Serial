#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include "serial_windows.h"


HANDLE hSerialPort;
DCB dcbSerialParams = {0};
COMMTIMEOUTS timeouts = {0};
std::string data;

/**
* @fn auto open(void* port, const int baudrate, const int dataBits, const int parity, const int stopBits) -> int
* @brief Opens the specified connection to a serial device.
* @param port The port to open the serial connection to
* @param baudrate The baudrate for the serial connection when reading/writing
* @param dataBits The data bits
* @param parity The parity bits
* @param stopBits The stop bits
* @return Returns the current status code
*/
auto windowsSystemOpen(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity,
    const int stopBits
) -> int {

    char *portName = static_cast<char*>(port);

    dcbSerialParams.DCBlength = sizeof(DCB);

    hSerialPort = CreateFile(
        portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // Error if open fails
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        returnStatus(StatusCodes::INVALID_HANDLE_ERROR);
    }

    // Error if configuration get fails
    if (!GetCommState(hSerialPort, &dcbSerialParams)) {
        CloseHandle(hSerialPort);
        returnStatus(StatusCodes::GET_STATE_ERROR);
    }

    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = dataBits;
    dcbSerialParams.Parity = static_cast<BYTE>(parity);
    dcbSerialParams.StopBits = static_cast<BYTE>(stopBits);

    // Error if configuration set fails
    if (!SetCommState(hSerialPort, &dcbSerialParams)) {
        CloseHandle(hSerialPort);
        returnStatus(StatusCodes::SET_STATE_ERROR);
    }
    
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        CloseHandle(hSerialPort);
        returnStatus(StatusCodes::SET_TIMEOUT_ERROR);
    }

    returnStatus(StatusCodes::SUCCESS);
}

/**
* @fn auto close() -> int
* @brief Closes the specified connection to a serial device.
* @return Returns the current status code
*/
auto windowsSystemClose() -> int {
    // Error if handle is invalid
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        returnStatus(StatusCodes::INVALID_HANDLE_ERROR);
    }

    // Error if close fails
    if (!CloseHandle(hSerialPort)) {
        returnStatus(StatusCodes::CLOSE_HANDLE_ERROR);
    }

    returnStatus(StatusCodes::SUCCESS);
}

/**
* @fn auto read(void* buffer, const int bufferSize, const int timeout, const int multiplier) -> int
* @brief Reads the specified number of bytes into the buffer.
* **It is not guaranteed that the complete buffer will be fully read.**
* @param buffer The buffer in which the bytes should be read into
* @param bufferSize The size of the buffer
* @param timeout Timeout to cancel the read
* @param multiplier The time multiplier between reading
* @return Returns the current status code (negative) or number of bytes read
*/
auto windowsSystemRead(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    // Error if handle is invalid
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        returnStatus(StatusCodes::INVALID_HANDLE_ERROR);
    }

    timeouts.ReadIntervalTimeout = timeout;
    timeouts.ReadTotalTimeoutConstant = timeout;
    timeouts.ReadTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        returnStatus(StatusCodes::SET_TIMEOUT_ERROR);
    }

    DWORD bytesRead;

    // Error if read fails
    if (!ReadFile(hSerialPort, buffer, bufferSize, &bytesRead, NULL)) {
        returnStatus(StatusCodes::READ_ERROR);
    }
    
    return bytesRead;
}

/**
* @fn auto readUntil(void* buffer, const int bufferSize, const int timeout, const int mutilplier, void* searchString) -> int
* @brief Reads until the specified string is found. If the specified string is not found, the buffer is read full until there are no more bytes to read.
* **It is not guaranteed that the complete buffer will be fully read.**
* @param buffer The buffer in which the bytes should be read into
* @param bufferSize The size of the buffer
* @param timeout Timeout to cancel the read
* @param multiplier The time multiplier between reading
* @param searchString The string to search for
* @return Returns the current status code (negative) or number of bytes read
*/
auto windowsSystemReadUntil(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* searchString
) -> int {

    if (hSerialPort == INVALID_HANDLE_VALUE) {
        returnStatus(StatusCodes::INVALID_HANDLE_ERROR);
    }

    timeouts.ReadIntervalTimeout = timeout;
    timeouts.ReadTotalTimeoutConstant = timeout;
    timeouts.ReadTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        returnStatus(StatusCodes::SET_TIMEOUT_ERROR);
    }

    data = "";

    for (int i{0}; i < bufferSize && data.find(std::string(static_cast<char*>(searchString))) == std::string::npos; i++) {
        DWORD bytesRead;
        char bufferChar[1];

        // Error if read fails
        if (!ReadFile(hSerialPort, bufferChar, sizeof(bufferChar), &bytesRead, NULL)) {
            returnStatus(StatusCodes::READ_ERROR);
        }

        if (bytesRead == 0) {
            break;
        }

        data.append(std::string(bufferChar, bytesRead));
    }

    memcpy(buffer, data.c_str(), data.length() + 1);

    return data.length();
}

/**
* @fn auto write(void* buffer, const int bufferSize, const int timeout, const int multiplier) -> int
* @brief Writes the buffer to the serial device.
* **It is not guaranteed that the complete buffer will be fully written.**
* @param buffer The buffer in which the bytes should be read into
* @param bufferSize The size of the buffer
* @param timeout Timeout to cancel the read
* @param multiplieer The time multiplier between writing
* @return Returns the current status code (negative) or number of bytes written
*/
auto windowsSystemWrite(void* buffer, const int bufferSize, const int timeout, const int multiplier) -> int {
    DWORD bytesWritten;

    timeouts.WriteTotalTimeoutConstant = timeout;
    timeouts.WriteTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        returnStatus(StatusCodes::SET_TIMEOUT_ERROR);
    }

    // Error if write fails
    if (!WriteFile(hSerialPort, buffer, bufferSize, &bytesWritten, NULL)) {
        returnStatus(StatusCodes::WRITE_ERROR);
    }
    
    return bytesWritten;
}

/**
* @fn auto getAvailablePorts(void* buffer, const int bufferSize, void* separator) -> int
* @brief Get all the available serial ports.
* @param buffer The buffer in which the bytes should be read into
* @param bufferSize The size of the buffer
* @param separator The separator for the array buffer
* @return Returns the current status code (negative) or number of ports found
*/
auto windowsSystemGetAvailablePorts(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int {
    std::string result;
    
    int portsCounter = 0;

    for (int i = 1; i <= 256; ++i) {
        std::string portName = "COM" + std::to_string(i);
        HANDLE hPort = CreateFileA(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        // Error if open fails
        if (hPort == INVALID_HANDLE_VALUE) {
            CloseHandle(hPort);
        }

        portsCounter++;
        result += portName.append(std::string(static_cast<char*>(separator)));

        CloseHandle(hPort);
    }
    // Error if buffer size is to small
    if (result.length() + 1 > bufferSize) {
        returnStatus(StatusCodes::BUFFER_ERROR);
    }

    memcpy(buffer, result.c_str(), result.length() + 1);
    
    return portsCounter;
}

#endif
