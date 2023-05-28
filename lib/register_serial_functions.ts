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
            // Status code/Bytes read
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
            // Status code/Bytes read
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
            // Status code/Bytes written
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
            // Status code/Amount of ports
            result: 'i32'
        },
        'serialOnError': {
            parameters: ['function'],
            result: 'void'
        }
    }).symbols

    return {
        error: (callback) => {
            serialFunctions.serialOnError(new Deno.UnsafeCallback({
                parameters: ['i32'],
                result: "void",
            } as const,
            (code) => {callback(code)}).pointer)
        },
        open: serialFunctions.serialOpen,
        close:  serialFunctions.serialClose,
        read: serialFunctions.serialRead,
        readUntil: serialFunctions.serialReadUntil,
        write: serialFunctions.serialWrite,
        getPortsInfo: serialFunctions.serialGetPortsInfo
    }
}

