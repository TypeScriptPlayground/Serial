import { binaryExtensions } from "./constants/binary_extensions.ts";
import { SerialFunctions } from "./interfaces/serial_functions.d.ts";
import { registerSerialFunctions } from "./register_serial_functions.ts";

/**
 * This function is used to load the specific binary based on the current os.
 * @param path The default path to the binaries directory
 * @param os The current OS
 * @returns A serial functions object with all the functions from the binary
 */
export function loadBinaryForOS(path : string, os : string) : SerialFunctions {
    if (!binaryExtensions[os]) {
        throw new Error(
            `There is currently no implementation for this operating system.
            We are currently working on an implementation for all missing operating systems.
            Current stage:

            - Windows (implemented)
            - Linux (in progress)
            
            For more information feel free to check out the repository:
            https://github.com/TypeScriptPlayground/Serial#compatibility`
        );
    }
    
    return registerSerialFunctions(path, os, binaryExtensions[os]);
}
