#pragma once
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#include <string>
#include <fstream>
#include <windows.h>
#include "status_codes.h"

extern void (*callback)(int code);

#define CALLBACK_STOP(errorCode) callback(status(errorCode)); return;

extern HANDLE hSerialPort;
extern DCB dcbSerialParams;
extern COMMTIMEOUTS timeouts;

void openWindows(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity = 0,
    const int stopBits = 0
);

void closeWindows();

auto readWindows(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto readUntilWindows(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* untilChar
) -> int;

auto writeWindows(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int;

auto getPortsInfoWindows(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int;

void onErrorWindows(void (*func)(int code));

#endif
