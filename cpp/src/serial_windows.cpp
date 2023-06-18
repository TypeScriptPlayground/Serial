#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <windows.h>
#include <iostream>
#include "serial.h"

namespace {
DCB dcbSerialParams = {0};
COMMTIMEOUTS timeouts = {0};
std::string data;
}

void (*errorCallback)(int errorCode);
void (*readCallback)(int bytes);
void (*writeCallback)(int bytes);

auto serialOpen(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity,
    const int stopBits
) -> int64_t {

    HANDLE hSerialPort;

    char *portName = static_cast<char*>(port);

    dcbSerialParams.DCBlength = sizeof(DCB);

    hSerialPort = CreateFile(
        portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL
    );

    // Error if open fails
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        errorCallback(status(StatusCodes::INVALID_HANDLE_ERROR));
        return -1;
    }

    // Error if configuration get fails
    if (!GetCommState(hSerialPort, &dcbSerialParams)) {
        // Error if close fails
        if (!CloseHandle(hSerialPort)) {
            errorCallback(status(StatusCodes::CLOSE_HANDLE_ERROR));
            return -1;
        }

        errorCallback(status(StatusCodes::GET_STATE_ERROR));

        return -1;
    }

    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = dataBits;
    dcbSerialParams.Parity = static_cast<BYTE>(parity);
    dcbSerialParams.StopBits = static_cast<BYTE>(stopBits);

    // Error if configuration set fails
    if (!SetCommState(hSerialPort, &dcbSerialParams)) {
        // Error if close fails
        if (!CloseHandle(hSerialPort)) {
            errorCallback(status(StatusCodes::CLOSE_HANDLE_ERROR));
            return -1;
        }
        
        errorCallback(status(StatusCodes::SET_STATE_ERROR));
        return -1;
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
            errorCallback(status(StatusCodes::CLOSE_HANDLE_ERROR));
            return -1;
        }

        errorCallback(status(StatusCodes::SET_TIMEOUT_ERROR));
        return -1;
    }
    return int64_t(hSerialPort);
}

void serialClose(int64_t pointer) {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);

    // Error if handle is invalid
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        errorCallback(status(StatusCodes::INVALID_HANDLE_ERROR));
        return;
    }

    // Error if close fails
    if (!CloseHandle(hSerialPort)) {
        errorCallback(status(StatusCodes::CLOSE_HANDLE_ERROR));
        return;
    }
}

auto serialRead(
    int64_t pointer,
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);

    // Error if handle is invalid
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        errorCallback(status(StatusCodes::INVALID_HANDLE_ERROR));
        return 0;
    }

    timeouts.ReadIntervalTimeout = timeout;
    timeouts.ReadTotalTimeoutConstant = multiplier;
    timeouts.ReadTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        errorCallback(status(StatusCodes::SET_TIMEOUT_ERROR));
        return 0;
    }

    DWORD bytesRead = 0;

    OVERLAPPED o{0,0,0,0};

    // Error if read fails
    if (!ReadFile(hSerialPort, buffer, bufferSize, &bytesRead, &o)) {
        errorCallback(status(StatusCodes::READ_ERROR));
        return 0;
    }
    
    readCallback(bytesRead);
    return bytesRead;
}

auto serialReadUntil(
    int64_t pointer,
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* searchString
) -> int {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);

    // Error if handle is invalid
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        errorCallback(status(StatusCodes::INVALID_HANDLE_ERROR));
        return 0;
    }

    timeouts.ReadIntervalTimeout = timeout;
    timeouts.ReadTotalTimeoutConstant = multiplier;
    timeouts.ReadTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        errorCallback(status(StatusCodes::SET_TIMEOUT_ERROR));
        return 0;
    }

    data = "";

    OVERLAPPED o{0,0,0,0};

    for (int i{0}; i < bufferSize && data.find(std::string(static_cast<char*>(searchString))) == std::string::npos; i++) {
        DWORD bytesRead;
        char bufferChar[1];

        // Error if read fails
        if (!ReadFile(hSerialPort, bufferChar, sizeof(bufferChar), &bytesRead, &o)) {
            errorCallback(status(StatusCodes::READ_ERROR));
            return 0;
        }

        if (bytesRead == 0) {
            break;
        }

        data.append(std::string(bufferChar, bytesRead));
    }

    memcpy(buffer, data.c_str(), data.length() + 1);

    readCallback(data.length());
    return data.length();
}

auto serialWrite(
    int64_t pointer,
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);

    DWORD bytesWritten = 0;

    timeouts.WriteTotalTimeoutConstant = timeout;
    timeouts.WriteTotalTimeoutMultiplier = multiplier;

    // Error if timeout set fails
    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        errorCallback(status(StatusCodes::SET_TIMEOUT_ERROR));
        return 0;
    }

    OVERLAPPED o;
    ZeroMemory(&o, sizeof(o));
    

    // Error if write fails
    if (!WriteFile(hSerialPort, buffer, bufferSize, &bytesWritten, &o)) {
        DWORD dwRet = GetLastError();
        LPSTR errorMessage = nullptr;
        DWORD result = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dwRet,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&errorMessage),
            0,
            nullptr
        );
        std::cerr << "Fehlermeldung: " << errorMessage << std::endl;
        LocalFree(errorMessage); // Speicher freigeben
        std::cerr << dwRet << "\n";
        errorCallback(status(StatusCodes::WRITE_ERROR));
        return 0;
    }
    
    writeCallback(bytesWritten);
    return bytesWritten;
}

auto serialGetPortsInfo(
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
        errorCallback(status(StatusCodes::BUFFER_ERROR));
        return 0;
    }

    memcpy(buffer, result.c_str(), result.length() + 1);
    
    return portsCounter;
}

void serialClearBufferIn(int64_t pointer) {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);
    if (!PurgeComm(hSerialPort, PURGE_RXCLEAR)){
        errorCallback(status(StatusCodes::CLEAR_BUFFER_IN_ERROR));
        return;
    }
    return;
}

void serialClearBufferOut(int64_t pointer) {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);
    if (!PurgeComm(hSerialPort, PURGE_TXCLEAR)){
        errorCallback(status(StatusCodes::CLEAR_BUFFER_OUT_ERROR));
        return;
    }
    return;
}

void serialAbortRead(int64_t pointer) {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);
    if (!PurgeComm(hSerialPort, PURGE_RXABORT)){
        errorCallback(status(StatusCodes::ABORT_READ_ERROR));
        return;
    }
    return;
}

void serialAbortWrite(int64_t pointer) {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);
    if (!PurgeComm(hSerialPort, PURGE_TXABORT)){
        errorCallback(status(StatusCodes::ABORT_WRITE_ERROR));
        return;
    }
    return;
}

auto serialTestEvent(int64_t pointer) -> bool {
    HANDLE hSerialPort = reinterpret_cast<void*>(pointer);

    DWORD dwEventMask;
    
    if (!SetCommMask(hSerialPort, EV_BREAK | EV_RXCHAR)) {
        std::cerr << "setcommMask is fucked\n";
        return false;
    }

    // Warte auf das Ereignis
    DWORD dwEvent;


    if (!WaitCommEvent(hSerialPort, &dwEvent, NULL)) {
        std::cerr << "waitcommEvent is fucked\n";
        return false;
    }

    // Überprüfe das erhaltene Ereignis
    if (dwEvent & EV_RXCHAR || dwEvent & EV_BREAK) {
        return true;
    }
    std::cerr << "event nicht ausgeführt\n";
    return false;
}


auto serialOnError(void (*func)(int code)) -> void {
    errorCallback = func;
};

auto serialOnRead(void (*func)(int bytes)) -> void {
    readCallback = func;
};

auto serialOnWrite(void (*func)(int bytes)) -> void {
    writeCallback = func;
};

#endif
