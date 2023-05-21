# Serial
A serial device library written in TypeScript for Deno without third party libraries.

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
