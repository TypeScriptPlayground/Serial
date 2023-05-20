import { baudrate } from "./constants/baudrate.d.ts";
import { dataBits } from "./constants/data_bits.d.ts";
import { parity } from "./constants/parity.d.ts";
import { stopBits } from "./constants/stop_bits.d.ts";
import { SerialOptions } from "./serial_options.d.ts";

export class Serial {
    private _port : string;
    private _baudrate : number;
    private _isOpen : boolean;
    private _dataBits : typeof dataBits;
    private _parity : typeof parity;
    private _stopBits : typeof stopBits;

    /**
     * Create a new instance of a serial connection.
     * @param {string} port The port to connect
     * @param {number} baudrate The baudrate
     * @param {SerialOptions} serialOptions 
     */
    constructor(port : string, baudrate : baudrate, serialOptions : SerialOptions) {
        this._port = port;
        this._baudrate = baudrate
        this._dataBits = serialOptions.dataBits
        this._parity = serialOptions.parity
        this._stopBits = serialOptions.stopBits
        this._isOpen = false;
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
        this._isOpen = true;
    }

    /**
     * Closes the serial connection.
     */
    close() : void {
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

