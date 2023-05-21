import { dataBits } from "../constants/data_bits.ts";
import { parity } from "../constants/parity.ts";
import { stopBits } from "../constants/stop_bits.ts";

export interface SerialOptions {
    dataBits? : dataBits,
    parity? : parity,
    stopBits? : stopBits
}
