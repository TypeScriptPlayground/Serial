#pragma once
#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
#include <iostream>
#include <sstream>
#include <stdio.h>      // Standard input/output definitions
#include <string.h>     // String function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <system_error>	// For throwing std::system_error
#include <sys/ioctl.h> // Used for TCGETS2, which is required for custom baud rates
#include <cassert>
#include <asm/ioctls.h>
#include <asm/termbits.h>
#include <algorithm>
#include <iterator>
#include <filesystem>

#include "status_codes.h"

namespace UnixSystem {

    extern int hSerialPort;
    extern termios2 tty;

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
}
#endif
