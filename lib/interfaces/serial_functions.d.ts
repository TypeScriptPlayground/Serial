import { parity } from "../constants/parity.ts";

export interface SerialFunctions {
    open: (
        port : string,
        baudrate : number,
        dataBits : number,
        parity : parity,
        stopBits : number
    ) => number,
    close: () => number,
    read: (
        buffer : Uint8Array,
        bufferSize : number,
        timeout : number,
        multiplier : number
    ) => number,
    readUntil: (
        buffer : Uint8Array,
        bufferSize : number,
        timeout : number,
        multiplier : number,
        searchString : string
    ) => number,
    write: (
        buffer : Uint8Array,
        bufferSize : number,
        timeout : number,
        multiplier : number
    ) => number,
    getAvailablePorts: (
        buffer : Uint8Array,
        bufferSize : number,
        separator : string
    ) => number
}
