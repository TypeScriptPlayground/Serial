import { dataBits } from "./constants/data_bits.d.ts";
import { parity } from "./constants/parity.d.ts";
import { stopBits } from "./constants/stop_bits.d.ts";

export interface SerialOptions {
    dataBits : typeof dataBits,
    parity : typeof parity,
    stopBits : typeof stopBits
}
