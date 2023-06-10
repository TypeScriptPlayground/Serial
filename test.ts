import * as serialLink from "./mod.ts";

const serial1 = new serialLink.Serial();

let port;

while(!port) {
    const availablePorts = serial1.getPortsInfo();

    console.log('serial properties (available ports):', availablePorts);

    port = prompt('Port to connect:');
}

serial1.open(port, 9600);

const textToSend = prompt('Text to send:') || '';

const dataToSend = new TextEncoder().encode(textToSend);

console.log('serial.write():', serial1.write(dataToSend, dataToSend.length, 10, 10));

console.log('writing:', textToSend);

const dataToRead = new Uint8Array(20);

console.log('serial.read()', serial1.read(dataToRead, dataToRead.length, 10, 10));

console.log('reading:', new TextDecoder().decode(dataToRead));

serial1.close();
