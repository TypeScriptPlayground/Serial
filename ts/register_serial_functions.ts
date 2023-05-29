import { SerialFunctions } from "./interfaces/serial_functions.d.ts";

export function registerSerialFunctions(
    path : string,
    os : string,
    libSuffix : string
) : SerialFunctions {
    
    console.log(`${path}/${os}.${libSuffix}`);
    
    const serialFunctions = Deno.dlopen(new URL(`${import.meta.url}../../${path}/${os}.${libSuffix}`).pathname.replace(/^\//gm, ''), {
        'serialOpen': {
            parameters: [
                // Port
                'buffer',
                // Baudrate
                'i32',
                // Data Bits
                'i32',
                // Parity
                'i32',
                // Stop Bits
                'i32'
            ],
            // Status code
            result: 'void'
        },
        'serialClose': {
            parameters: [],
            // Status code
            result: 'i32'
        },
        'serialRead': {
            parameters: [
                // Buffer
                'buffer',
                // Buffer Size
                'i32',
                // Timeout
                'i32',
                // Multiplier
                'i32'
            ],
            // Bytes read
            result: 'i32'
        },
        'serialReadUntil': {
            parameters: [
                // Buffer
                'buffer',
                // Buffer Size
                'i32',
                // Timeout
                'i32',
                // Multiplier
                'i32',
                // SearchString
                'buffer'
            ],
            // Bytes read
            result: 'i32'
        },
        'serialWrite': {
            parameters: [
                // Buffer
                'buffer',
                // Buffer Size
                'i32',
                // Timeout
                'i32',
                // Multiplier
                'i32'
            ],
            // Bytes written
            result: 'i32'
        },
        'serialGetPortsInfo': {
            parameters: [
                // Buffer
                'buffer',
                // Buffer Size
                'i32',
                // Separator
                'buffer'
            ],
            // Amount of ports
            result: 'i32'
        },
        'serialOnError': {
            // on error callback function
            parameters: ['function'],
            result: 'void'
        },
        'serialOnRead': {
            // on error callback function
            parameters: ['function'],
            result: 'void'
        },
        'serialOnWrite': {
            // on error callback function
            parameters: ['function'],
            result: 'void'
        }
    }).symbols

    return {
        open: serialFunctions.serialOpen,
        close:  serialFunctions.serialClose,
        read: serialFunctions.serialRead,
        readUntil: serialFunctions.serialReadUntil,
        write: serialFunctions.serialWrite,
        getPortsInfo: serialFunctions.serialGetPortsInfo,
        onError: (callback) => {
            serialFunctions.serialOnError(new Deno.UnsafeCallback({
                parameters: ['i32'],
                result: "void",
            } as const,
            (errorCode) => {callback(errorCode)}).pointer)
        },
        onRead: (callback) => {
            serialFunctions.serialOnRead(new Deno.UnsafeCallback({
                parameters: ['i32'],
                result: "void",
            } as const,
            (bytes) => {callback(bytes)}).pointer)
        },
        onWrite: (callback) => {
            serialFunctions.serialOnWrite(new Deno.UnsafeCallback({
                parameters: ['i32'],
                result: "void",
            } as const,
            (bytes) => {callback(bytes)}).pointer)
        }
    }
}

