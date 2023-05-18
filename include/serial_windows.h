#pragma once
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#include <string>
#include <fstream>
#include <windows.h>
#include "status_codes.h"

namespace WindowsSystem {

extern HANDLE hSerialPort;
extern DCB dcbSerialParams;
extern COMMTIMEOUTS timeouts;

auto open(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity = 0,
    const int stopBits = 0
) -> int;

auto close() -> int;

auto read(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto readUntil(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* untilChar
) -> int;

auto write(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto getAvailablePorts(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int;

}

#endif
