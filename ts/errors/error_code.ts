export class ErrorCode extends Error {
    code : number

    constructor(code : number) {
        super(`An error has occurred. Error code: ${code}`);
        this.code = code;
    }
}
