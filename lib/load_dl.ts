import { SerialFunctions } from "./interfaces/serial_functions.d.ts";
import { registerSerialFunctions } from "./register_serial_functions.ts";

export function loadDL(path : string, os : string) : SerialFunctions {
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
    
    return registerSerialFunctions(path, os, libSuffix);
}
