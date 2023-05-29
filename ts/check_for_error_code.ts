import { ErrorCode } from "./errors/error_code.ts";

/**
 * This function is used to check for an error code. If the code is below 0 an error is thrown
 * @param code Error code
 * @throws The error with the code
 */
export function checkForErrorCode(code : number) {
    if (code >= 0) {
        return;
    }

    throw new ErrorCode(code);
}
