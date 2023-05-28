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

extern void (*callback)(int code);

void onErrorUnix(void (*func)(int code));

void openUnix(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity = 0,
    const int stopBits = 0
);

void closeUnix();

auto readUnix(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto readUntilUnix(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* untilChar
) -> int;

auto writeUnix(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto getPortsInfoUnix(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int;

#endif
