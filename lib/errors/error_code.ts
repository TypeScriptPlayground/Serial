import { statusCodes } from "../constants/status_codes.ts";

export class ErrorCode extends Error {
    constructor(code : number) {
        const errorCodeKey = Object.entries(statusCodes).find(([_,value]) => value === code)?.[0];
        super(`An error has occurred. Error code: ${errorCodeKey} (${code})`);
    }
}
