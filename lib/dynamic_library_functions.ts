import { parety } from "./parity.ts";

export interface DynamicLibraryFunctions {
    open: (
        port : string,
        baudrate : number,
        dataBits : number,
        parity : parety,
        stopBits : number
    ) => number,
    close: () => number,
    read: (
        buffer : string,
        bufferSize : number,
        timeout : number,
        multiplier : number
    ) => number,
    readUntil: (
        buffer : string,
        bufferSize : number,
        timeout : number,
        multiplier : number,
        searchString : string
    ) => number,
    write: (
        buffer : string,
        bufferSize : number,
        timeout : number,
        multiplier : number
    ) => number,
    getAvailablePorts: (
        buffer : string,
        bufferSize : number,
        separator : string
    ) => number
}
