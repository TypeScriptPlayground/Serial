#include "serial.h"

auto open(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity,
    const int stopBits
) -> int {
    return _open(port, baudrate, dataBits, parity, stopBits);
}

auto close() -> int {
    return _close();
}

auto read(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    return _read(buffer, bufferSize, timeout, multiplier);
}

auto readUntil(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* untilChar
) -> int {
    return _readUntil(buffer, bufferSize, timeout, multiplier, untilChar);
}

auto write(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    return _write(buffer, bufferSize, timeout, multiplier);
}

auto getAvailablePorts(
    void* buffer,
    const int bufferSize,
    void* separator
) -> int {
    return _getAvailablePorts(buffer, bufferSize, separator);
}
