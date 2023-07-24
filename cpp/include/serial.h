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

    MODULE_API auto serialOpen(
        void* port,
        const int baudrate,
        const int dataBits,
        const int parity = 0,
        const int stopBits = 0
    ) -> int64_t;

    MODULE_API void serialClose(int64_t pointer);

    MODULE_API auto serialRead(
        int64_t pointer,
        void* buffer,
        const int bufferSize,
        const int timeout,
        const int multiplier
    ) -> int;

    MODULE_API auto serialReadUntil(
        int64_t pointer,
        void* buffer,
        const int bufferSize,
        const int timeout,
        const int multiplier,
        void* untilChar
    ) -> int;

    MODULE_API auto serialWrite(
        int64_t pointer,
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

    MODULE_API void serialClearBufferIn(int64_t pointer);

    MODULE_API void serialClearBufferOut(int64_t pointer);

    MODULE_API void serialAbortRead(int64_t pointer);

    MODULE_API void serialAbortWrite(int64_t pointer);
    
    MODULE_API auto serialOnError(void (*func)(int code)) -> void;

    MODULE_API auto serialOnRead(void (*func)(int bytes)) -> void;
    
    MODULE_API auto serialOnWrite(void (*func)(int bytes)) -> void;

#ifdef __cplusplus
}
#endif
