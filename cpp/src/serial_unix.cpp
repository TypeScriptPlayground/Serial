#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
#include "serial.h"
#include <fstream>
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <sys/ioctl.h> // Used for TCGETS2, which is required for custom baud rates
#include <asm/termbits.h>
#include <cerrno>
#include <cstring>
#include <dirent.h>

namespace {
int hSerialPort;
termios2 tty;
std::string data;
}

void (*errorCallback)(int errorCode);
void (*readCallback)(int bytes);
void (*writeCallback)(int bytes);

auto serialOpen(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity,
    const int stopBits
) -> int64_t {
    char *portName = static_cast<char*>(port);
    // Open new serial connection
    hSerialPort = open(portName, O_RDWR);

    // Error if open fails
    if(hSerialPort == -1){
        errorCallback(status(StatusCodes::INVALID_HANDLE_ERROR));
        return -1;
    }
    
    // Get the current com configuration
    if(ioctl(hSerialPort, TCGETS2, &tty) == -1){
        errorCallback(status(StatusCodes::GET_STATE_ERROR));
        return -1;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE;  // Clear all bits that set the data size
    tty.c_cflag |= CS8;     // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;   // Disable echo
    tty.c_lflag &= ~ECHOE;  // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    // cfsetispeed(&tty, B9600);
    // cfsetospeed(&tty, B9600);
    tty.c_ispeed = baudrate;
    tty.c_ospeed = baudrate;

    // Data bits
    tty.c_cflag &=  ~CSIZE;			// CSIZE is a mask for the number of bits per character

    switch(dataBits) {
        case 5:
            tty.c_cflag |= CS5;
            break;
        case 6:
            tty.c_cflag |= CS6;
            break;
        case 7:
            tty.c_cflag |= CS7;
            break;
        default:
            tty.c_cflag |= CS8;
            break;
    }

    // parity
    switch(parity) {
        case 0:
            tty.c_cflag &= ~PARENB;
            break;
        case 1:	
            tty.c_cflag |= PARENB;
            tty.c_cflag &= ~PARODD; // Clearing PARODD makes the parity even
            break;
        case 2:
            tty.c_cflag |= PARENB;
            tty.c_cflag |= PARODD;
            break;
    }

    // stop bits
    // Set num. stop bits
    switch(stopBits) {
        case 0:
            tty.c_cflag &= ~CSTOPB;
            break;
        // TODO case 1: 1.5 would break the code
        case 2:
            tty.c_cflag |= CSTOPB;
            break;
    }

    // Save tty settings, also checking for error
    if (ioctl(hSerialPort, TCSETS2, &tty) == -1){
        errorCallback(status(StatusCodes::SET_STATE_ERROR));
        return -1;
    }

    return 0;
}

void serialClose(int64_t pointer) {
    // Error if close fails
    if (close(hSerialPort) == -1) {
        errorCallback(status(StatusCodes::CLOSE_HANDLE_ERROR));
        return;
    }

    return;
}

auto serialRead(
    int64_t pointer,
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {

    if (ioctl(hSerialPort, TCGETS2, &tty) == -1){
        errorCallback(status(StatusCodes::SET_STATE_ERROR));
        return 0;
    }

    if (timeout > 0 && timeout < 100) {
        tty.c_cc[VTIME] = 1;
    } else {
        tty.c_cc[VTIME] = timeout / 100;
    }

    tty.c_cc[VMIN] = bufferSize;

    if (ioctl(hSerialPort, TCSETS2, &tty) == -1) {
        errorCallback(status(StatusCodes::SET_STATE_ERROR));
        return 0;
    }

    int bytesRead = read(hSerialPort, static_cast<char*>(buffer), bufferSize);
    
    if (bytesRead >= 0){
        return bytesRead;
    }

    errorCallback(status(StatusCodes::READ_ERROR));
    return 0;
}

auto serialReadUntil(
    int64_t pointer,
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* searchString
) -> int {

    data = "";

    for (int i{0}; i < bufferSize && data.find(std::string(static_cast<char*>(searchString))) == std::string::npos; i++) {
        char bufferChar[1];

        // Error if read fails
        int bytesRead = read(hSerialPort, static_cast<char*>(bufferChar), 1);
        if (bytesRead == -1) {
            errorCallback(status(StatusCodes::READ_ERROR));
            return 0;
        }

        if (bytesRead == 0) {
            break;
        }

        data.append(std::string(bufferChar, bytesRead));
    }

    memcpy(buffer, data.c_str(), data.length() + 1);

    readCallback(data.length());
    
    return data.length();
}

auto serialWrite(
    int64_t pointer,
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {

    const char* tmp = static_cast<char*>(buffer);

    int bytesWritten = write(hSerialPort, tmp, bufferSize);

    if (bytesWritten == -1) {
        errorCallback(status(StatusCodes::WRITE_ERROR));
        return 0;
    }
    
    writeCallback(bytesWritten);
    return bytesWritten;
}

auto serialOnError(void (*func)(int code)) -> void {
    errorCallback = func;
};

auto serialOnRead(void (*func)(int bytes)) -> void {
    readCallback = func;
};

auto serialOnWrite(void (*func)(int bytes)) -> void {
    writeCallback = func;
};

auto serialGetPortsInfo(void *buffer, const int bufferSize, void *separator) -> int {
    std::string result = "";

    int portsCounter = 0;

    DIR* dir = opendir("/dev/serial/by-id");
    if (dir == nullptr) {
        // Handle directory not found error
        return -1; // Return an appropriate error code or define your own
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_LNK) {
            std::string symlinkPath = "/dev/serial/by-id/";
            symlinkPath += entry->d_name;

            char canonicalPath[PATH_MAX];
            if (realpath(symlinkPath.c_str(), canonicalPath) != nullptr) {
                result += std::string(canonicalPath) + std::string(static_cast<char*>(separator));
                portsCounter++;
            }
        }
    }

    closedir(dir);

    // Remove last trailing comma
    if (result.length() > 0) {
        result.erase(result.length() - 1);
    }

    if (result.length() + 1 <= bufferSize) {
        memcpy(buffer, result.c_str(), result.length() + 1);
    } else {
        errorCallback(status(StatusCodes::BUFFER_ERROR));
        return 0;
    }

    return portsCounter;
}

#endif
