import { ObjectValuesMap } from "../types/object_values_map.ts";

export const baudrates = {
    B1200: 1200,
    B2400: 2400,
    B9600: 9600,
    B19200: 19200,
    B38400: 38400,
    B57600: 57600,
    B74880: 74880,
    B115200: 115200,
    B230400: 230400,
    B250000: 250000
} as const;

export type Baudrate = ObjectValuesMap<typeof baudrates>
