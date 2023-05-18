

export function loadDL(os : string, path : string) {
    let libSuffix = '';

    switch(os) {
        case 'windows': {
            libSuffix = 'dll';
            break;
        }

        case 'linux':
        case 'darwin':
        case 'freebsd':
        case 'netbsd':
        case 'aix':
        case 'solaris':
        case 'illumos': {
            throw new Error(
                `There is currently no implementation for this operating system.
                We are currently working on an implementation for all missing operating systems.
                Current stage:

                - Windows (implemented)
                - Linux (in progress)
                - MacOS (planned)
                
                For more information feel free to check out the repository:
                https://github.com/TypeScriptPlayground/Serial`
            );
        }

        default: {
            break;
        }
    }

    return Deno.dlopen(`${path}.${libSuffix}`, {
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
    })
}
