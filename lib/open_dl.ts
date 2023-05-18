import { DynamicLibraryFunctions } from "./dynamic_library_functions.ts";
import { encode } from "./encode.ts";
import { parety } from "./parity.ts";

export function openDL(
    path : string,
    os : string,
    libSuffix : string
) : DynamicLibraryFunctions {
    const dlFunctions = Deno.dlopen(`${path}/${os}.${libSuffix}`, {
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
        open: (
            port : string,
            baudrate : number,
            dataBits : number,
            parity : parety,
            stopBits : number
        ) : number => dlFunctions.open(
            encode(port + '\0'),
            baudrate,
            dataBits,
            parity,
            stopBits
        ),
        close: () : number => dlFunctions.close(),
        read: (
            buffer : string,
            bufferSize : number,
            timeout : number,
            multiplier : number
        ) : number => dlFunctions.read(
            encode(buffer + '\0'),
            bufferSize,
            timeout,
            multiplier
        ),
        readUntil: (
            buffer : string,
            bufferSize : number,
            timeout : number,
            multiplier : number,
            searchString : string
        ) : number => dlFunctions.readUntil(
            encode(buffer + '\0'),
            bufferSize,
            timeout,
            multiplier,
            encode(searchString + '\0')
        ),
        write: (
            buffer : string,
            bufferSize : number,
            timeout : number,
            multiplier : number
        ) : number => dlFunctions.write(
            encode(buffer + '\0'),
            bufferSize,
            timeout,
            multiplier
        ),
        getAvailablePorts: (
            buffer : string,
            bufferSize : number,
            separator : string
        ) : number => dlFunctions.getAvailablePorts(
            encode(buffer + '\0'),
            bufferSize,
            encode(separator + '\0')
        )
    }
}
