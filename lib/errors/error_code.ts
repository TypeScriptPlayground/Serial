import { statusCodes } from "../constants/status_codes.ts";

export class ErrorCode extends Error {
    constructor(code : number) {
        const errorCodeKey = Object.keys(statusCodes).find(key => statusCodes[key] === code);
        super(`An error has occurred. Error code: ${errorCodeKey} (${code})`);
    }
}
