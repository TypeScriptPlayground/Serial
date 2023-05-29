import { ObjectValuesMap } from "../types/object_values_map.ts";

export const statusCodes = {
    SUCCESS: 0,
    CLOSE_HANDLE_ERROR: -1,
    INVALID_HANDLE_ERROR: -2,
    READ_ERROR: -3,
    WRITE_ERROR: -4,
    GET_PROPERTY_ERROR: -5,
    SET_PROPERTY_ERROR: -6,
    SET_TIMEOUT_ERROR: -7,
    BUFFER_ERROR: -8,
    NOT_FOUND_ERROR: -9
} as const;

export type StatusCode = ObjectValuesMap<typeof statusCodes>
