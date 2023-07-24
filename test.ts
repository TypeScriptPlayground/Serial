import * as serialLink from "./mod.ts";

function sleep(time: number) : Promise<void> {
    return new Promise((resolve, reject) => setTimeout(resolve, time));
}

const serial1 = new serialLink.Serial();
// const serial2 = new serialLink.Serial();

let port1 = '';
// let port2 = '';

while(!port1 /* && !port2 */) {
    const availablePorts = serial1.getPortsInfo();

    console.log('serial properties (available ports):', availablePorts);

    port1 = prompt('Port1 to connect:') || '';
    // port2 = prompt('Port2 to connect:') || '';
}

serial1.open(port1, 9600);
// serial2.open(port2, 9600);

// serial1.serialTestEvent().then(result => console.log('serial1.serialTestEvent()', result));


const textToSend1 = prompt('Text1 to send:') || '';
// const textToSend2 = prompt('Text2 to send:') || '';

const dataToSend1 = new TextEncoder().encode(textToSend1);
// const dataToSend2 = new TextEncoder().encode(textToSend2);

console.log('serial1.write():', serial1.write(dataToSend1, dataToSend1.length, 10, 10));
console.log('writing1:', textToSend1);

await sleep(1000);

console.log('serial1.clearBufferIn():', serial1.clearBufferIn());
// console.log('serial1.clearBufferOut():', serial1.clearBufferOut());

console.log('serial1.write():', serial1.write(dataToSend1, dataToSend1.length, 10, 10));
console.log('writing1:', textToSend1);

// console.log('serial2.write():', serial2.write(dataToSend2, dataToSend2.length, 10, 10));
// console.log('writing2:', textToSend2);

await sleep(1000);


const dataToRead1 = new Uint8Array(1024);
// const dataToRead2 = new Uint8Array(20);


console.log('serial1.read():', serial1.read(dataToRead1, dataToRead1.length, 10, 10));
console.log('reading1:', new TextDecoder().decode(dataToRead1));

// console.log('serial2.read()', serial2.read(dataToRead2, dataToRead2.length, 10, 10));
// console.log('reading2:', new TextDecoder().decode(dataToRead2));




// serial1.close();
// serial2.close();

