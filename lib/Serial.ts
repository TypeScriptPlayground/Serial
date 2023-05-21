import { baudrate } from "./constants/baudrate.ts";
import { dataBits } from "./constants/data_bits.ts";
import { parity } from "./constants/parity.ts";
import { stopBits } from "./constants/stop_bits.ts";
import { SerialFunctions } from "./interfaces/serial_functions.d.ts";
import { SerialOptions } from "./interfaces/serial_options.d.ts";
import { loadDL } from "./load_dl.ts";

export class Serial {
    private _port : string;
    private _baudrate : number;
    private _dataBits : dataBits;
    private _parity : parity;
    private _stopBits : stopBits;
    private _isOpen : boolean;
    private _dl : SerialFunctions;

    /**
     * Create a new instance of a serial connection.
     * @param {string} port The port to connect
     * @param {number} baudrate The baudrate
     * @param {SerialOptions} serialOptions Additional options for the serial connection (`data bits`, `parity`, `stop bits`)
     */
    constructor(
        port : string,
        baudrate : baudrate,
        serialOptions? : SerialOptions
    ) {
        this._port = port;
        this._baudrate = baudrate;
        this._dataBits = serialOptions?.dataBits || dataBits.EIGHT;
        this._parity = serialOptions?.parity || parity.NONE;
        this._stopBits = serialOptions?.stopBits || stopBits.ONE;
        this._isOpen = false;

        this._dl = loadDL(Deno.build.os, './dls');
    }

    /**
     * Get the current port for the serial connection.
     * @returns {string} Returns the port in a `string` format
     */
    get port() : string {
        return this._port;
    }

    /**
     * Get the current port for the serial connection.
     * @returns {string} Returns the baudrate
     */
    get baudrate() : number {
        return this._baudrate;
    }

    /**
     * Get the current connection status of the serial connection.
     * @returns {boolean} Returns `true` if the serial connection is open, otherwise returns `false`
     */
    get isOpen() : boolean {
        return this._isOpen;
    }

    /**
     * Opens the serial connection.
     */
    open() : void {
        this._dl.open(
            this._port,
            this._baudrate,
            this._dataBits,
            this._parity,
            this._stopBits
        );
        this._isOpen = true;
    }

    /**
     * Closes the serial connection.
     */
    close() : void {
        this._dl.close();
        this._isOpen = false;
    }

    /**
     * Read data from serial connection.
     * @param {Uint8Array} buffer Buffer to read the bytes into
     * @param {number} bytes The number of bytes to read
     * @param {number} timeout The timeout in `ms`
     * @returns {Promise<string>} Returns number of bytes read
     */
    read(
        buffer : Uint8Array,
        bytes : number,
        timeout? : number
    ) : Promise<string> {

    }

    /**
     * Read data from serial connection until a linebreak (`\n`) gets send.
     * @param {Uint8Array} buffer Buffer to read the bytes into
     * @param {string} searchString A string to search for
     * @param {number} timeout The timeout in `ms`
     * @returns {Promise<string>} Returns number of bytes read
     */
    readUntil(
        buffer : Uint8Array,
        searchString : string,
        timeout? : number
    ) : Promise<string> {

    }
    /**
     * Write data to serial connection.
     * @param {string} data The data to write/send
     * @param {number} timeout The timeout in `ms`
     * @returns {Promise<string>} Returns number of bytes written
     */
    write(
        data : string,
        timeout? : number
    ) : Promise<void> {

    }

    /**
     * Gat a list of the available ports.
     * @returns {Promise<string>} Returns a list of available ports
     */
    getPorts() : Promise<Ports[]> {

    }
}

