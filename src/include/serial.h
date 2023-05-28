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
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#   include "serial_windows.h"
#   define systemOpen(port, baudrate, dataBits, parity, stopBits) windowsSystemOpen(port, baudrate, dataBits, parity, stopBits)
#   define systemClose() windowsSystemClose()
#   define systemRead(buffer, bufferSize, timeout, multiplier) windowsSystemRead(buffer, bufferSize, timeout, multiplier)
#   define systemReadUntil(buffer, bufferSize, timeout, multiplier, untilChar) windowsSystemReadUntil(buffer, bufferSize, timeout, multiplier, untilChar)
#   define systemWrite(buffer, bufferSize, timeout, multiplier) windowsSystemWrite(buffer, bufferSize, timeout, multiplier)
#   define systemGetAvailablePorts(buffer, bufferSize, separator) windowsSystemGetAvailablePorts(buffer, bufferSize, separator)
#   define systemSetCallbackFunction(f_pointer) windowsSetCallbackFunction(f_pointer)
#endif

// Linux, Apple
#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
#   include "serial_unix.h"
#   define systemOpen(port, baudrate, dataBits, parity, stopBits) unixSystemOpen(port, baudrate, dataBits, parity, stopBits)
#   define systemClose() unixSystemClose()
#   define systemRead(buffer, bufferSize, timeout, multiplier) unixSystemRead(buffer, bufferSize, timeout, multiplier)
#   define systemReadUntil(buffer, bufferSize, timeout, multiplier, untilChar) unixSystemReadUntil(buffer, bufferSize, timeout, multiplier, untilChar)
#   define systemWrite(buffer, bufferSize, timeout, multiplier) unixSystemWrite(buffer, bufferSize, timeout, multiplier)
#   define systemGetAvailablePorts(buffer, bufferSize, separator) unixSystemGetAvailablePorts(buffer, bufferSize, separator)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    MODULE_API auto serialSetCallBackFunction(void (*func)()) -> void;

    MODULE_API auto serialOpen(
        void* port,
        const int baudrate,
        const int dataBits,
        const int parity = 0,
        const int stopBits = 0
    ) -> int;

    MODULE_API auto serialClose() -> int;

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

    MODULE_API auto serialGetAvailablePorts(
        void* buffer,
        const int bufferSize,
        void* separator
    ) -> int;

#ifdef __cplusplus
}
#endif
