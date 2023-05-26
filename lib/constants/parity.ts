import { ObjectValuesMap } from "../types/object_values_map.ts";

export const parity = {
    NONE: 0,
    ODD: 1,
    EVEN: 2,
    MARK: 3,
    SPACE: 4
} as const;

export type Parity = ObjectValuesMap<typeof parity>
