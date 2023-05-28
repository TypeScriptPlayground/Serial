#pragma once
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#include <string>
#include <fstream>
#include <windows.h>
#include "status_codes.h"

extern void (*callback)(int code);

extern HANDLE hSerialPort;
extern DCB dcbSerialParams;
extern COMMTIMEOUTS timeouts;

void windowsSystemError(void (*func)(int code));

void windowsSystemOpen(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity = 0,
    const int stopBits = 0
);

void windowsSystemClose();

auto windowsSystemRead(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto windowsSystemReadUntil(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* untilChar
) -> int;

auto windowsSystemWrite(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto windowsSystemGetAvailablePorts(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int;

#endif
