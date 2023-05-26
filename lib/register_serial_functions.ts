import { SerialFunctions } from "./interfaces/serial_functions.d.ts";

export function registerSerialFunctions(
    path : string,
    os : string,
    libSuffix : string
) : SerialFunctions {
    const serialFunctions = Deno.dlopen(`${path}/${os}.${libSuffix}`, {
        'open': {
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
            result: 'i32'
        },
        'close': {
            parameters: [],
            // Status code
            result: 'i32'
        },
        'read': {
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
        'readUntil': {
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
        'write': {
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
        'getAvailablePorts': {
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
        }
    }).symbols
    
    return {
        open: serialFunctions.open,
        close:  serialFunctions.close,
        read: serialFunctions.read,
        readUntil: serialFunctions.readUntil,
        write: serialFunctions.write,
        getAvailablePorts: serialFunctions.getAvailablePorts
    }
}
