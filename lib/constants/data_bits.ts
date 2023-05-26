import { ObjectValuesMap } from "../types/object_values_map.ts";

export const dataBits = {
    FIVE: 5,
    SIX: 6,
    SEVEN: 7,
    EIGHT: 8,
} as const;

export type DataBits = ObjectValuesMap<typeof dataBits>
