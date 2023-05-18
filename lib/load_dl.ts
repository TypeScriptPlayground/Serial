import { DynamicLibraryFunctions } from "./dynamic_library_functions.ts";
import { openDL } from "./open_dl.ts";

export function loadDL(os : string, path : string) : DynamicLibraryFunctions {
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

    return openDL(path, os, libSuffix);
}
