#include "serial.h"

void serialError(void (*func)(int code, void* buffer)) {
    systemError(func);
}

void serialOpen(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity,
    const int stopBits
) {
    return systemOpen(port, baudrate, dataBits, parity, stopBits);
}

void serialClose() {
    return systemClose();
}

auto serialRead(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    return systemRead(buffer, bufferSize, timeout, multiplier);
}

auto serialReadUntil(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* untilChar
) -> int {
    return systemReadUntil(buffer, bufferSize, timeout, multiplier, untilChar);
}

auto serialWrite(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    return systemWrite(buffer, bufferSize, timeout, multiplier);
}

auto serialGetAvailablePorts(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int {
    return systemGetAvailablePorts(buffer, bufferSize, separator);
}
