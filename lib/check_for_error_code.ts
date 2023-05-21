import { ErrorCode } from "./errors/error_code.ts";

export function checkForErrorCode(code : number) {
    if (code >= 0) {
        return;
    }

    throw new ErrorCode(code);
}
