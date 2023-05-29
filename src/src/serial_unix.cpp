#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
#include "serial.h"
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <sys/ioctl.h> // Used for TCGETS2, which is required for custom baud rates
#include <asm/termbits.h>
// #include <filesystem>


// namespace fs = std::filesystem;

int hSerialPort;
termios2 tty;

void (*errorCallback)(int errorCode);
void (*readCallback)(int bytes);
void (*writeCallback)(int bytes);

namespace helper {
    void Callback(StatusCodes errorCode){        
        static std::string msg = "";
        callback(status(errorCode));
    }
}

void serialOpen(
    void* port,
    const int baudrate,
    const int dataBits,
    const int parity,
    const int stopBits
) {
    char *portName = static_cast<char*>(port);
    // Open new serial connection
    hSerialPort = open(portName, O_RDWR);
    
    // Error if open fails
    ioctl(hSerialPort, TCGETS2, &tty);
    // if (tcgetattr(hSerialPort, &tty) != 0) {
    //     returnStatus(StatusCodes::INVALID_HANDLE_ERROR);
    // }

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
    ioctl(hSerialPort, TCSETS2, &tty);

    // returnStatus(StatusCodes::SUCCESS);
}

void serialClose() {
    close(hSerialPort);
}

auto serialRead(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {
    return read(hSerialPort, static_cast<char*>(buffer), bufferSize);
}

auto serialReadUntil(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier,
    void* searchString
) -> int {
    return 0;
}

auto serialWrite(
    void* buffer,
    const int bufferSize,
    const int timeout,
    const int multiplier
) -> int {

    std::string tmp(static_cast<char*>(buffer), bufferSize);

    return write(hSerialPort, tmp.c_str(), tmp.length() + 1);
}

// auto getPortsInfoUnix(
//     void* buffer,
//     const int bufferSize,
//     void* separator
// ) -> int {
//     std::string result;

//     int portsCounter = 0;

//     fs::path p("/dev/serial/by-id");
    
//     try {
//         if (!exists(p)) {
//             returnStatus(StatusCodes::NOT_FOUND_ERROR);
//         }
        
//         else {
//             for (auto de : fs::directory_iterator(p)) {
//                 if (is_symlink(de.symlink_status())) {
//                     fs::path symlink_points_at = read_symlink(de);
//                     fs::path canonical_path = fs::canonical(p / symlink_points_at);
//                     result += canonical_path.generic_string().append(std::string(static_cast<char*>(separator)));
//                     portsCounter++;
//                 }
//             }
//         }
//     } catch (const fs::filesystem_error &exeption) {
//     }

//     if (result.length() + 1 <= bufferSize){
//         memcpy(buffer, result.c_str(), result.length() + 1);
//     } else {
//         returnStatus(StatusCodes::BUFFER_ERROR);
//     }
//     return portsCounter;
// }

#endif
