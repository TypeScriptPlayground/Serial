#pragma once

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#   ifdef SERIALPORT_EXPORTS
#       define MODULE_API __declspec(dllexport)
#   else
#       define MODULE_API __declspec(dllimport)
#   endif
#else
#   define MODULE_API
#endif

// Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   include "serial_windows.h"
#   define OPEN(port, baudrate, dataBits, parity, stopBits) openWindows(port, baudrate, dataBits, parity, stopBits)
#   define CLOSE() closeWindows()
#   define READ(buffer, bufferSize, timeout, multiplier) readWindows(buffer, bufferSize, timeout, multiplier)
#   define READ_UNTIL(buffer, bufferSize, timeout, multiplier, untilChar) readUntilWindows(buffer, bufferSize, timeout, multiplier, untilChar)
#   define WRITE(buffer, bufferSize, timeout, multiplier) writeWindows(buffer, bufferSize, timeout, multiplier)
#   define GET_PORTS_INFO(buffer, bufferSize, separator) getPortsInfoWindows(buffer, bufferSize, separator)
#   define ON_ERROR(f_pointer) onErrorWindows(f_pointer)
#endif

// Linux, Apple
#if defined(__unix) || defined(__unix__) || defined(__APPLE__)
#   include "serial_unix.h"
#   define OPEN(port, baudrate, dataBits, parity, stopBits) openUnix(port, baudrate, dataBits, parity, stopBits)
#   define CLOSE() closeUnix()
#   define READ(buffer, bufferSize, timeout, multiplier) readUnix(buffer, bufferSize, timeout, multiplier)
#   define READ_UNTIL(buffer, bufferSize, timeout, multiplier, untilChar) readUntilUnix(buffer, bufferSize, timeout, multiplier, untilChar)
#   define WRITE(buffer, bufferSize, timeout, multiplier) writeUnix(buffer, bufferSize, timeout, multiplier)
#   define GET_PORTS_INFO(buffer, bufferSize, separator) getPortsInfoUnix(buffer, bufferSize, separator)
#   define ON_ERROR(f_pointer) onErrorUnix(f_pointer)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    MODULE_API void serialOpen(
        void* port,
        const int baudrate,
        const int dataBits,
        const int parity = 0,
        const int stopBits = 0
    );

    MODULE_API void serialClose();

    MODULE_API auto serialRead(
        void* buffer,
        const int bufferSize,
        const int timeout,
        const int multiplier
    ) -> int;

    MODULE_API auto serialReadUntil(
        void* buffer,
        const int bufferSize,
        const int timeout,
        const int multiplier,
        void* untilChar
    ) -> int;

    MODULE_API auto serialWrite(
        void* buffer,
        const int bufferSize,
        const int timeout,
        const int multiplier
    ) -> int;

    MODULE_API auto serialGetPortsInfo(
        void* buffer,
        const int bufferSize,
        void* separator
    ) -> int;
    
    MODULE_API auto serialOnError(void (*func)(int code)) -> void;

#ifdef __cplusplus
}
#endif
