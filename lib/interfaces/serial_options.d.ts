import { DataBits } from "../constants/data_bits.ts";
import { Parity } from "../constants/parity.ts";
import { StopBits } from "../constants/stop_bits.ts";

export interface SerialOptions {
    dataBits? : DataBits,
    parity? : Parity,
    stopBits? : StopBits
}
