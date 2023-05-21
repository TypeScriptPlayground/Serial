import { checkForErrorCode } from "./check_for_error_code.ts";
import { dataBits } from "./constants/data_bits.ts";
import { parity } from "./constants/parity.ts";
import { stopBits } from "./constants/stop_bits.ts";
import { decode } from "./decode.ts";
import { Ports } from "./interfaces/ports.ts";
import { SerialFunctions } from "./interfaces/serial_functions.d.ts";
import { SerialOptions } from "./interfaces/serial_options.d.ts";
import { loadDL } from "./load_dl.ts";

export class Serial {
    private _isOpen : boolean;
    private _dl : SerialFunctions;

    /**
     * Create a new instance of a serial connection.
     */
    constructor() {
        this._isOpen = false;
        this._dl = loadDL('./lib/dls', Deno.build.os);
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
     * @param {string} port The port to connect
     * @param {number} baudrate The baudrate
     * @param {SerialOptions} serialOptions Additional options for the serial connection (`data bits`, `parity`, `stop bits`)
     */
    open(
        port : string,
        baudrate : number,
        serialOptions? : SerialOptions
    ) : number {
        const status = this._dl.open(
            port,
            baudrate,
            serialOptions?.dataBits || dataBits.EIGHT,
            serialOptions?.parity || parity.NONE,
            serialOptions?.stopBits || stopBits.ONE
        );
        
        checkForErrorCode(status);

        this._isOpen = true;

        return status;
    }

    /**
     * Closes the serial connection.
     */
    close() : number {
        const status = this._dl.close();

        checkForErrorCode(status);

        this._isOpen = false;

        return status;
    }

    /**
     * Read data from serial connection.
     * @param {Uint8Array} buffer Buffer to read the bytes into
     * @param {number} bytes The number of bytes to read
     * @param {number} timeout The timeout in `ms`
     * @param {number} multiplier The timeout between reading individual bytes in `ms`
     * @returns {number} Returns number of bytes read
     */
    read(
        buffer : Uint8Array,
        bytes : number,
        timeout = 0,
        multiplier = 10
    ) : number {
        const status = this._dl.read(
            buffer,
            bytes,
            timeout,
            multiplier
        );
        
        checkForErrorCode(status);

        return status;
    }

    /**
     * Read data from serial connection until a linebreak (`\n`) gets send.
     * @param {Uint8Array} buffer Buffer to read the bytes into
     * @param {number} bytes The number of bytes to read
     * @param {number} timeout The timeout in `ms`
     * @param {number} multiplier The timeout between reading individual bytes in `ms`
     * @param {string} searchString A string to search for
     * @returns {number} Returns number of bytes read
     */
    readUntil(
        buffer : Uint8Array,
        bytes : number,
        timeout = 0,
        multiplier = 10,
        searchString = '',
    ) : number {
        const status = this._dl.readUntil(
            buffer,
            bytes,
            timeout,
            multiplier,
            searchString
        )

        checkForErrorCode(status);

        return status
    }
    /**
     * Write data to serial connection.
     * @param {Uint8Array} buffer The data to write/send
     * @param {number} bytes The number of bytes to read
     * @param {number} timeout The timeout in `ms`
     * @param {number} multiplier The timeout between reading individual bytes in `ms`
     * @returns {number} Returns number of bytes written
     */
    write(
        buffer : Uint8Array,
        bytes : number,
        timeout = 0,
        multiplier = 10
    ) : number {
        const status = this._dl.write(
            buffer,
            bytes,
            timeout,
            multiplier
        )
        
        checkForErrorCode(status);

        return status
    }

    /**
     * Gat a list of the available ports.
     * @returns {Ports[]} Returns a list of available ports
     */
    getAvailablePorts() : Ports[] {
        const buffer = new Uint8Array(1024);
        const status = this._dl.getAvailablePorts(
            buffer,
            buffer.length,
            ','
        )
        
        checkForErrorCode(status);

        const ports = decode(buffer).replaceAll('\x00','').split(',').map((port) => {
            return {
                name: port
            };
        })

        return ports;
    }
}

