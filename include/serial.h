#pragma once

#ifdef SERIALPORT_EXPORTS
/*Enabled as "export" while compiling the dll project*/
#define DLL_IMPORT_EXPORT __declspec(dllexport)
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define DLL_IMPORT_EXPORT __declspec(dllimport)
#endif

// Windows
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #include "serial_windows.h"
    #define _open(port, baudrate, dataBits, parity, stopBits) WindowsSystem::open(port, baudrate, dataBits, parity, stopBits)
    #define _close() WindowsSystem::close()
    #define _read(buffer, bufferSize, timeout, multiplier) WindowsSystem::read(buffer, bufferSize, timeout, multiplier)
    #define _readUntil(buffer, bufferSize, timeout, multiplier, untilChar) WindowsSystem::readUntil(buffer, bufferSize, timeout, multiplier, untilChar)
    #define _write(buffer, bufferSize, timeout, multiplier) WindowsSystem::write(buffer, bufferSize, timeout, multiplier)
    #define _getAvailablePorts(buffer, bufferSize, separator) WindowsSystem::getAvailablePorts(buffer, bufferSize, separator)
#endif

// Linux, Apple
#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #include "serial_unix.h"
    #define _open(port, baudrate, dataBits, parity, stopBits) UnixSystemOopen(port, baudrate, dataBits, parity, stopBits)
    #define _close() UnixSystemClose()
    #define _read(buffer, bufferSize, timeout, multiplier) UnixSystemRead(buffer, bufferSize, timeout, multiplier)
    #define _readUntil(buffer, bufferSize, timeout, multiplier, untilChar) UnixSystemReadUntil(buffer, bufferSize, timeout, multiplier, untilChar)
    #define _write(buffer, bufferSize, timeout, multiplier) UnixSystemWrite(buffer, bufferSize, timeout, multiplier)
    // #define _getAvailablePorts() UnixSystemGetAvailablePorts()
#endif

extern "C" {

    DLL_IMPORT_EXPORT auto open(
        void* port,
        const int baudrate,
        const int dataBits,
        const int parity = 0,
        const int stopBits = 0
    ) -> int;

    DLL_IMPORT_EXPORT auto close() -> int;

    DLL_IMPORT_EXPORT auto read(
        void* buffer,
        const int bufferSize,
        const int timeout,
        const int multiplier
    ) -> int;

    DLL_IMPORT_EXPORT auto readUntil(
        void* buffer,
        const int bufferSize,
        const int timeout,
        const int multiplier,
        void* untilChar
    ) -> int;

    DLL_IMPORT_EXPORT auto write(
        void* buffer,
        const int bufferSize,
        const int timeout,
        const int multiplier
    ) -> int;

    DLL_IMPORT_EXPORT auto getAvailablePorts(
        void* buffer,
        const int bufferSize,
        void* separator
    ) -> int;

}
