import { Baudrate } from "../constants/baudrates.ts";
import { DataBits } from "../constants/data_bits.ts";
import { Parity } from "../constants/parity.ts";
import { StopBits } from "../constants/stop_bits.ts";

export interface SerialFunctions {
    open : (
        port : Uint8Array,
        baudrate : Baudrate,
        dataBits : DataBits,
        parity : Parity,
        stopBits : StopBits
    ) => void,
    close : () => void,
    read : (
        buffer : Uint8Array,
        bufferSize : number,
        timeout : number,
        multiplier : number
    ) => number,
    readUntil : (
        buffer : Uint8Array,
        bufferSize : number,
        timeout : number,
        multiplier : number,
        searchString : Uint8Array
    ) => number,
    write : (
        buffer : Uint8Array,
        bufferSize : number,
        timeout : number,
        multiplier : number
    ) => number,
    getPortsInfo : (
        buffer : Uint8Array,
        bufferSize : number,
        separator : Uint8Array
    ) => number,
    onError : (callback : (
        code : number
    ) => void) => void,
    onRead : (callback : (
        bytes : number
    ) => void) => void,
    onWrite : (callback : (
        bytes : number
    ) => void) => void
}
