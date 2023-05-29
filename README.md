<!-- Badges -->
[Build binaries]: https://img.shields.io/github/actions/workflow/status/TypeScriptPlayground/Serial/build_windows.yml?label=Build%20binaries&labelColor=343b42&logo=github&logoColor=959DA5 'Build binaries'
[Release Downloads]: https://img.shields.io/github/downloads/TypeScriptPlayground/Serial/total?label=Downloads%20&labelColor=343b42&logo=docusign&logoColor=959DA5 'Total Release Downloads'

# Serial
[![Build binaries]](https://github.com/TypeScriptPlayground/Serial/actions/workflows/build.yml)
[![Release Downloads]](https://github.com/TypeScriptPlayground/Serial/releases)

<a href="https://deno.land"><img align="right" src="https://deno.land/logo.svg" height="150px" alt="the deno mascot dinosaur standing in the rain"></a>

A [serial](https://en.wikipedia.org/wiki/Serial_communication) library written in TypeScript for [Deno](https://deno.land) without any third party modules.

This library provides an interface for the communication with serial devices and **doesn't use any third party modules**. It uses C++ functions which are included in a [dynamic link library](https://de.wikipedia.org/wiki/Dynamic_Link_Library) or [shared object](https://en.wikipedia.org/wiki/Library_(computing)#Shared_libraries). These functions are then loaded by deno to establish a serial connection and talk to the devices.

<br>

> <picture>
>   <source media="(prefers-color-scheme: light)" srcset="https://raw.githubusercontent.com/Mqxx/GitHub-Markdown/main/blockquotes/badge/light-theme/warning.svg">
>   <img alt="Warning" src="https://raw.githubusercontent.com/Mqxx/GitHub-Markdown/main/blockquotes/badge/dark-theme/warning.svg">
> </picture><br>
>
> This library and the documentation are still work in progress!

## Features
- Communication with serial devices.
- Create multiple serial connections at the same time.
- List available ports and their properties.
- Set timeouts for both reading and writing.
- All functions are async.
- Uses no third party modules.
- Works on different operating systems (check [compatibility](#compatibility) for mor info).

## Compatibility
| OS      | Tested version          | Current state |
|---------|-------------------------|---------------|
| Windows | Windows 10 (x64)        | implemented   |
| Linux   | Ubuntu Server 22.04 LTS | implemented   |

## Possible/Known issues
- What happens if you open multiple connections from the same instance.
- Every function returns the status code although it is previously checked.
- What happens if you async read 2 times directly after each other.
- Linux write currently not working

## Examples - How to use
To use this library you need the following flags to run it:
- `--unstable`
- `--allow-ffi`

### Ports
Get a list with all serial ports and their info that are currently available on your system.

`main.ts`
```typescript
import { Serial } from "./mod.ts";

// create new instance of a serial object
const serial = new Serial();

// get all available ports
const availablePorts = serial.getPortsInfo();

// console log the list
console.log(availablePorts);
```

> Example output:
> ```
> [
>   { name: 'COM1' },
>   { name: 'COM2' },
>   ...
>   { name: 'tty/USB1' }
> ]
> ```

### Sending
Send data to a serial device. For exampe to an [Arduino](https://www.arduino.cc/).

`main.ts`
```typescript
import { Serial, baudrate } from "./mod.ts";

// create new instance of a serial object
const serial = new Serial();

// open the connection
serial.open('COM1', baudrate.B9600);

// encode the message to a Uint8Array
const textToSend = 'Hello from TypeScript!';
const encodedTextToSend = new TextEncoder().encode(textToSend);

// send the message
serial.send(encodedTextToSend, encodedTextToSend.length);
```

### Reading
Read data from a serial device. Again, in our example from an Arduino.
> Depending on your Arduino board you may need to press the reset button on the board after uploading the sketch, in order to receive data.

`sketch.ino`
```ino
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  Serial.println("Hello from Arduino!");
}
```

`main.ts`
```typescript
import { Serial, baudrate } from "./mod.ts";

// create new instance of a serial object
const serial = new Serial();

// open the connection
serial.open('COM1', baudrate.B9600);

// create a new buffer to store incoming bytes,
// in this example we want to read a maximum of 100 bytes
const bufferToRead = new Uint8Array(100);

// read data into the buffer
serial.read(bufferToRead, bufferToRead.length);

// decode the data from the buffer
const decodedTextToRead = new TextDecoder().decode(bufferToRead);

// console log the text
console.log(decodedTextToRead);
```

> Example output:
> ```
> Hello from Arduino!
> ```

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
## Credits

- Big thanks goes out to [@Katze719](https://github.com/Katze719) who wrote most of the C++ files and functions!
- Thanks to [@AapoAlas](https://github.com/aapoalas) for the great support and help on the [Deno Discord](https://discord.gg/deno)!

## Licence
Apache-2.0. Check [LICENSE](./LICENSE) for more details. Feel free to contribute to this project.

Copyright 2023 © Max
