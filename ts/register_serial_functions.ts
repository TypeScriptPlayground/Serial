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
                'i32',
                // Data Bits
                'i32',
                // Parity
                'i32',
                // Stop Bits
                'i32'
            ],
            // Status code
            result: 'i64'
        },
        'serialClose': {
            parameters: [
                'i64'
            ],
            // Status code
            result: 'i32'
        },
        'serialRead': {
            parameters: [
                'i64',
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
                'i64',
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
                'i64',
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
        },
        'serialClearBufferIn': {
            parameters: ['i64'],
            result: 'void'
        },
        'serialClearBufferOut': {
            parameters: ['i64'],
            result: 'void'
        },
        'serialAbortRead': {
            parameters: ['i64'],
            result: 'void'
        },
        'serialAbortWrite': {
            parameters: ['i64'],
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
        clearBufferIn: serialFunctions.serialClearBufferIn,
        clearBufferOut: serialFunctions.serialClearBufferOut,
        abortRead: serialFunctions.serialAbortRead,
        abortWrite: serialFunctions.serialAbortWrite,
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

