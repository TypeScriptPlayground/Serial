import { ObjectValuesMap } from "../types/object_values_map.ts"

export const stopBits = {
    ONE: 0,
    ONE_DOT_FIVE: 1,
    TWO: 2
} as const;

export type StopBits = ObjectValuesMap<typeof stopBits>
