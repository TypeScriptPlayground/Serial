#pragma once
#include <string>

#include "status_codes.h"


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#   ifdef SERIALPORT_EXPORTS
#       define MODULE_API __declspec(dllexport)
#   else
#       define MODULE_API __declspec(dllimport)
#   endif
#else
#   define MODULE_API
#endif

extern void (*errorCallback)(int errorCode);
extern void (*readCallback)(int bytes);
extern void (*writeCallback)(int bytes);

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
    
    MODULE_API auto inline serialOnError(void (*func)(int code)) -> void {
        errorCallback = func;
    };

    MODULE_API auto inline serialOnRead(void (*func)(int bytes)) -> void {
        readCallback = func;
    };
    
    MODULE_API auto inline serialOnWrite(void (*func)(int bytes)) -> void {
        writeCallback = func;
    };

#ifdef __cplusplus
}
#endif
