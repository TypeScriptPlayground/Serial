#pragma once
#if defined(__unix__) || defined(__unix) || defined(__APPLE__)

#include <string.h>     // String function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <sys/ioctl.h> // Used for TCGETS2, which is required for custom baud rates
#include <asm/termbits.h>
#include <string>
// #include <filesystem>

#include "status_codes.h"

extern int hSerialPort;
extern termios2 tty;

auto unixSystemOpen(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity = 0,
    const int stopBits = 0
) -> int;

auto unixSystemClose() -> int;

auto unixSystemRead(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto unixSystemReadUntil(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* untilChar
) -> int;

auto unixSystemWrite(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto unixSystemGetAvailablePorts(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int;

#endif
