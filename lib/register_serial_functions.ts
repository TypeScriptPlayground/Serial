import { SerialFunctions } from "./interfaces/serial_functions.d.ts";

export function registerSerialFunctions(
    path : string,
    os : string,
    libSuffix : string
) : SerialFunctions {
    const serialFunctions = Deno.dlopen(`${path}/${os}.${libSuffix}`, {
        'serialOpen': {
            parameters: [
                // Port
                'buffer',
                // Baudrate
                'u32',
                // Data Bits
                'u32',
                // Parity
                'u32',
                // Stop Bits
                'u32'
            ],
            // Status code
            result: 'void'
        },
        'serialClose': {
            parameters: [],
            // Status code
            result: 'u32'
        },
        'serialRead': {
            parameters: [
                // Buffer
                'buffer',
                // Buffer Size
                'u32',
                // Timeout
                'u32',
                // Multiplier
                'u32'
            ],
            // Bytes read
            result: 'u32'
        },
        'serialReadUntil': {
            parameters: [
                // Buffer
                'buffer',
                // Buffer Size
                'u32',
                // Timeout
                'u32',
                // Multiplier
                'u32',
                // SearchString
                'buffer'
            ],
            // Bytes read
            result: 'u32'
        },
        'serialWrite': {
            parameters: [
                // Buffer
                'buffer',
                // Buffer Size
                'u32',
                // Timeout
                'u32',
                // Multiplier
                'u32'
            ],
            // Bytes written
            result: 'u32'
        },
        'serialGetPortsInfo': {
            parameters: [
                // Buffer
                'buffer',
                // Buffer Size
                'u32',
                // Separator
                'buffer'
            ],
            // Amount of ports
            result: 'u32'
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
        onError: (callback) => {
            serialFunctions.serialOnError(new Deno.UnsafeCallback({
                parameters: ['u32'],
                result: "void",
            } as const,
            (errorCode) => {callback(errorCode)}).pointer)
        },
        onRead: (callback) => {
            serialFunctions.serialOnRead(new Deno.UnsafeCallback({
                parameters: ['u32'],
                result: "void",
            } as const,
            (bytes) => {callback(bytes)}).pointer)
        },
        onWrite: (callback) => {
            serialFunctions.serialOnWrite(new Deno.UnsafeCallback({
                parameters: ['u32'],
                result: "void",
            } as const,
            (bytes) => {callback(bytes)}).pointer)
        },
        open: serialFunctions.serialOpen,
        close:  serialFunctions.serialClose,
        read: serialFunctions.serialRead,
        readUntil: serialFunctions.serialReadUntil,
        write: serialFunctions.serialWrite,
        getPortsInfo: serialFunctions.serialGetPortsInfo
    }
}

