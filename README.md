# Serial
[![Build Win32 DLL](https://github.com/TypeScriptPlayground/Serial/actions/workflows/build_windows.yml/badge.svg)](https://github.com/TypeScriptPlayground/Serial/actions/workflows/build_windows.yml)
[![Build Linux SO](https://github.com/TypeScriptPlayground/Serial/actions/workflows/build_linux.yml/badge.svg)](https://github.com/TypeScriptPlayground/Serial/actions/workflows/build_linux.yml)

<a href="https://deno.land"><img align="right" src="https://deno.land/logo.svg" height="150px" alt="the deno mascot dinosaur standing in the rain"></a>

A [serial](https://en.wikipedia.org/wiki/Serial_communication) library written in TypeScript for [Deno](https://deno.land) without third party modules.

This library provides an interface for the communication with serial devices and **doesn't use any third party modules**. It uses C++ functions which are included in a [dynamic link library](https://de.wikipedia.org/wiki/Dynamic_Link_Library) or [shared object](https://en.wikipedia.org/wiki/Library_(computing)#Shared_libraries). These functions are then loaded by deno to establish a serial connection and talk to the devices.

A big thanks goes out to [@Katze719](https://github.com/Katze719) who wrote most of the C++ files and functions!

<br>

> <picture>
>   <source media="(prefers-color-scheme: light)" srcset="https://raw.githubusercontent.com/Mqxx/GitHub-Markdown/main/blockquotes/badge/light-theme/warning.svg">
>   <img alt="Warning" src="https://raw.githubusercontent.com/Mqxx/GitHub-Markdown/main/blockquotes/badge/dark-theme/warning.svg">
> </picture><br>
>
> This library and the documentation are still work in progress!

### Features
- Communication with serial devices.
- Create multiple serial connections at the same time.
- List available ports and their properties.
- Set timeouts for both reading and writing.
- All functions are async.
- Uses no third party modules.
- Works on different operating systems (check [compatibility](#compatibility) for mor info).

### Compatibility
- [x] Windows (implemented)
- [ ] Linux (in progress)

### Possible/Known issues
- What happens if you open multiple connections from the same instance.
- Every function returns the status code although it is previously checked.
- What happens if you async read 2 times directly after each other.

### Usage
#### `class` Serial
```typescript
/**
 * Create a new instance of a serial connection.
 */
new Serial()
```

<br>

#### `getter property` Serial.isOpen
```typescript
/**
 * Get the current connection status of the serial connection.
 * @returns {boolean} Returns `true` if the serial connection is open, otherwise returns `false`
 */
get isOpen() : boolean
```

<br>

#### `methode` Serial.open()
```typescript
/**
 * Opens the serial connection.
 * @param {string} port The port to connect
 * @param {number} baudrate The baudrate
 * @param {SerialOptions} serialOptions Additional options for the serial connection (`data bits`, `parity`, `stop bits`)
 * @returns {number} The status code
 */
open(
    port : string,
    baudrate : number,
    serialOptions? : SerialOptions
) : number
```

<br>

#### `methode` Serial.close()
```typescript
/**
 * Closes the serial connection.
 */
close() : number
```

<br>

#### `methode` Serial.read()
```typescript
/**
 * Read data from serial connection.
 * @param {Uint8Array} buffer Buffer to read the bytes into
 * @param {number} bytes The number of bytes to read
 * @param {number} timeout The timeout in `ms`
 * @param {number} multiplier The timeout between reading individual bytes in `ms`
 * @returns {number} Returns number of bytes read
 */
read(
    buffer : Uint8Array,
    bytes : number,
    timeout = 0,
    multiplier = 10
) : number
```

<br>

#### `methode` Serial.readUntil()
```typescript
/**
 * Read data from serial connection until a linebreak (`\n`) gets send.
 * @param {Uint8Array} buffer Buffer to read the bytes into
 * @param {number} bytes The number of bytes to read
 * @param {number} timeout The timeout in `ms`
 * @param {number} multiplier The timeout between reading individual bytes in `ms`
 * @param {string} searchString A string to search for
 * @returns {number} Returns number of bytes read
 */
readUntil(
    buffer : Uint8Array,
    bytes : number,
    timeout = 0,
    multiplier = 10,
    searchString = '',
) : number
```

<br>

#### `methode` Serial.write()
```typescript
/**
 * Write data to serial connection.
 * @param {Uint8Array} buffer The data to write/send
 * @param {number} bytes The number of bytes to read
 * @param {number} timeout The timeout in `ms`
 * @param {number} multiplier The timeout between reading individual bytes in `ms`
 * @returns {number} Returns number of bytes written
 */
write(
    buffer : Uint8Array,
    bytes : number,
    timeout = 0,
    multiplier = 10
) : number
```

<br>

### Examples

WIP

```typescript
import { Serial, baudrate } from "./mod.ts";

const serial = new Serial();

const availablePorts = serial.getAvailablePorts()

console.log('serial properties (available ports):', availablePorts);

console.log('serial properties (isOpen):', serial.isOpen);

console.log('serial.open():', serial.open(availablePorts[1].name, baudrate.B9600));

const textToSend = 'Hello World!'
const dataToSend = new TextEncoder().encode(textToSend);

console.log('serial.write():', serial.write(dataToSend, dataToSend.length, 10, 10));

console.log('writing:', textToSend);

const dataToRead = new Uint8Array(20);

console.log('serial.read()', serial.read(dataToRead, dataToRead.length, 10, 10));

console.log('reading:', new TextDecoder().decode(dataToRead));

console.log('serial.close()', serial.close());
```
Arduino:
```ino
char incomingBytes[10];

int charsRead = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  int available = Serial.available();
  if (available > 0) {
    charsRead = Serial.readBytes(incomingBytes, available);

    Serial.print(incomingBytes);
  }
}
```
