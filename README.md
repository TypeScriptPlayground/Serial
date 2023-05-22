# Serial
<img align="right" src="https://deno.land/logo.svg" height="150px" alt="the deno mascot dinosaur standing in the rain">

A serial device library written in TypeScript for [Deno](https://deno.land) without third party modules.

This library provides an interface for the communication with serial devices and **doesn't use any third party modules**. It uses C++ functions which are included in a dynamic library or shared object. These functions are then loaded by deno to establish a serial connection and talk to the devices.

A big thanks goes out to [@Katze719](https://github.com/Katze719) who wrote most of the C++ files and functions!

<br>

> <picture>
>   <source media="(prefers-color-scheme: light)" srcset="https://raw.githubusercontent.com/Mqxx/GitHub-Markdown/main/blockquotes/badge/light-theme/warning.svg">
>   <img alt="Warning" src="https://raw.githubusercontent.com/Mqxx/GitHub-Markdown/main/blockquotes/badge/dark-theme/warning.svg">
> </picture><br>
>
> This library is still work in progress!

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
- [ ] MacOS (planned)

### Usage
#### `class` Serial
##### `methode` open()
##### `methode` close()
##### `methode` read()
##### `methode` readUntil()
##### `methode` write()
##### `getter property` isOpen
...
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
