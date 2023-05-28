#include <string>
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include "serial_windows.h"


HANDLE hSerialPort;
DCB dcbSerialParams = {0};
COMMTIMEOUTS timeouts = {0};
std::string data;

void (*callback)(int code);

void openWindows(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity,
    const int stopBits
) {

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
        callback(status(StatusCodes::INVALID_HANDLE_ERROR));
    }

    // Error if configuration get fails
    if (!GetCommState(hSerialPort, &dcbSerialParams)) {
        CloseHandle(hSerialPort);
        callback(status(StatusCodes::GET_STATE_ERROR));
    }

    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = dataBits;
    dcbSerialParams.Parity = static_cast<BYTE>(parity);
    dcbSerialParams.StopBits = static_cast<BYTE>(stopBits);

    // Error if configuration set fails
    if (!SetCommState(hSerialPort, &dcbSerialParams)) {
        // Error if close fails
        if (!CloseHandle(hSerialPort)) {
            callback(status(StatusCodes::CLOSE_HANDLE_ERROR));
        }
        
        callback(status(StatusCodes::SET_STATE_ERROR));
    }
    
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        // Error if close fails
        if (!CloseHandle(hSerialPort)) {
            callback(status(StatusCodes::CLOSE_HANDLE_ERROR));
        }
        
        callback(status(StatusCodes::SET_TIMEOUT_ERROR));
    }
}

void closeWindows() {
    // Error if handle is invalid
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        callback(status(StatusCodes::INVALID_HANDLE_ERROR));
    }

    // Error if close fails
    if (!CloseHandle(hSerialPort)) {
        callback(status(StatusCodes::CLOSE_HANDLE_ERROR));
    }
}

auto readWindows(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    // Error if handle is invalid
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        return status(StatusCodes::INVALID_HANDLE_ERROR);
    }

    timeouts.ReadIntervalTimeout = timeout;
    timeouts.ReadTotalTimeoutConstant = timeout;
    timeouts.ReadTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        return status(StatusCodes::SET_TIMEOUT_ERROR);
    }

    DWORD bytesRead;

    // Error if read fails
    if (!ReadFile(hSerialPort, buffer, bufferSize, &bytesRead, NULL)) {
        return status(StatusCodes::READ_ERROR);
    }
    
    return bytesRead;
}

auto readUntilWindows(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* searchString
) -> int {

    if (hSerialPort == INVALID_HANDLE_VALUE) {
        return status(StatusCodes::INVALID_HANDLE_ERROR);
    }

    timeouts.ReadIntervalTimeout = timeout;
    timeouts.ReadTotalTimeoutConstant = timeout;
    timeouts.ReadTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        return status(StatusCodes::SET_TIMEOUT_ERROR);
    }

    data = "";

    for (int i{0}; i < bufferSize && data.find(std::string(static_cast<char*>(searchString))) == std::string::npos; i++) {
        DWORD bytesRead;
        char bufferChar[1];

        // Error if read fails
        if (!ReadFile(hSerialPort, bufferChar, sizeof(bufferChar), &bytesRead, NULL)) {
            return status(StatusCodes::READ_ERROR);
        }

        if (bytesRead == 0) {
            break;
        }

        data.append(std::string(bufferChar, bytesRead));
    }

    memcpy(buffer, data.c_str(), data.length() + 1);

    return data.length();
}

auto writeWindows(void* buffer, const int bufferSize, const int timeout, const int multiplier) -> int {
    DWORD bytesWritten;

    timeouts.WriteTotalTimeoutConstant = timeout;
    timeouts.WriteTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        return status(StatusCodes::SET_TIMEOUT_ERROR);
    }

    // Error if write fails
    if (!WriteFile(hSerialPort, buffer, bufferSize, &bytesWritten, NULL)) {
        return status(StatusCodes::WRITE_ERROR);
    }
    
    return bytesWritten;
}

auto getPortsInfoWindows(
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
            continue;
        }

        portsCounter++;
        result += portName.append(std::string(static_cast<char*>(separator)));

        CloseHandle(hPort);
    }

    // Remove last trailing comma
    if (result.length() > 0) {
        result.erase(result.length() - 1);
    }

    // Error if buffer size is to small
    if (result.length() + 1 > bufferSize) {
        return status(StatusCodes::BUFFER_ERROR);
    }

    memcpy(buffer, result.c_str(), result.length() + 1);
    
    return portsCounter;
}

void onErrorWindows(void (*func)(int code)){
    callback = func;
}

#endif
