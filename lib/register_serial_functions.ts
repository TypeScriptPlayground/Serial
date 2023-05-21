import { SerialFunctions } from "./interfaces/serial_functions.d.ts";
import { encode } from "./encode.ts";
import { parity } from "./constants/parity.ts";

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
        open: (
            port : string,
            baudrate : number,
            dataBits : number,
            parity : parity,
            stopBits : number
        ) : number => serialFunctions.open(
            encode(port + '\0'),
            baudrate,
            dataBits,
            parity,
            stopBits
        ),
        close: () : number => serialFunctions.close(),
        read: (
            buffer : Uint8Array,
            bytes : number,
            timeout : number,
            multiplier : number
        ) : number => serialFunctions.read(
            buffer,
            bytes,
            timeout,
            multiplier
        ),
        readUntil: (
            buffer : Uint8Array,
            bytes : number,
            timeout : number,
            multiplier : number,
            searchString : string
        ) : number => serialFunctions.readUntil(
            buffer,
            bytes,
            timeout,
            multiplier,
            encode(searchString + '\0')
        ),
        write: (
            buffer : Uint8Array,
            bytes : number,
            timeout : number,
            multiplier : number
        ) : number => serialFunctions.write(
            buffer,
            bytes,
            timeout,
            multiplier
        ),
        getAvailablePorts: (
            buffer : Uint8Array,
            bytes : number,
            separator : string
        ) : number => serialFunctions.getAvailablePorts(
            buffer,
            bytes,
            encode(separator + '\0')
        )
    }
}
