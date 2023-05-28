import { checkForErrorCode } from "./check_for_error_code.ts";
import { Baudrate } from "./constants/baudrates.ts";
import { dataBits } from "./constants/data_bits.ts";
import { parity } from "./constants/parity.ts";
import { stopBits } from "./constants/stop_bits.ts";
import { decode } from "./decode.ts";
import { encode } from "./encode.ts";
import { Ports } from "./interfaces/ports.ts";
import { SerialFunctions } from "./interfaces/serial_functions.d.ts";
import { SerialOptions } from "./interfaces/serial_options.d.ts";
import { loadBinaryForOS } from "./load_binary_for_os.ts";

const pathToBinariesDirectory = './lib/bin';

export class Serial {
    private _isOpen : boolean;
    private _dl : SerialFunctions;

    /**
     * Create a new instance of a serial connection.
     */
    constructor() {
        this._isOpen = false;
        this._dl = loadBinaryForOS(pathToBinariesDirectory, Deno.build.os);
        this._dl.error((code, message) => {
            throw new Error(`An error has occurred. Error code: ${code}, ${message}`)
        })
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
     * @param port The port to connect
     * @param baudrate The baudrate
     * @param serialOptions Additional options for the serial connection (`data bits`, `parity`, `stop bits`)
     */
    open(
        port : string,
        baudrate : Baudrate,
        serialOptions? : SerialOptions
    ) : void {
        this._dl.open(
            encode(port + '\0'),
            baudrate,
            serialOptions?.dataBits || dataBits.EIGHT,
            serialOptions?.parity || parity.NONE,
            serialOptions?.stopBits || stopBits.ONE
        );

        this._isOpen = true;
    }

    /**
     * Closes the serial connection.
     */
    close() {
        this._dl.close();

        this._isOpen = false;
    }

    /**
     * Read data from serial connection.
     * @param buffer Buffer to read the bytes into
     * @param bytes The number of bytes to read
     * @param timeout The timeout in `ms`
     * @param multiplier The timeout between reading individual bytes in `ms`
     * @returns The number of bytes read
     */
    read(
        buffer : Uint8Array,
        bytes : number,
        timeout = 0,
        multiplier = 10
    ) : number {
        const result = this._dl.read(
            buffer,
            bytes,
            timeout,
            multiplier
        );
        
        checkForErrorCode(result);

        return result;
    }

    /**
     * Read data from serial connection until the specified `searchString` is found.
     * @param buffer Buffer to read the bytes into
     * @param bytes The number of bytes to read
     * @param timeout The timeout in `ms`
     * @param multiplier The timeout between reading individual bytes in `ms`
     * @param searchString A string to search for
     * @returns The number of bytes read
     */
    readUntil(
        buffer : Uint8Array,
        bytes : number,
        timeout = 0,
        multiplier = 10,
        searchString = '',
    ) : number {
        const result = this._dl.readUntil(
            buffer,
            bytes,
            timeout,
            multiplier,
            encode(searchString + '\0')
        )

        checkForErrorCode(result);

        return result
    }
    /**
     * Write data to serial connection.
     * @param buffer The data to write/send
     * @param bytes The number of bytes to read
     * @param timeout The timeout in `ms`
     * @param multiplier The timeout between reading individual bytes in `ms`
     * @returns The number of bytes written
     */
    write(
        buffer : Uint8Array,
        bytes : number,
        timeout = 0,
        multiplier = 10
    ) : number {
        const result = this._dl.write(
            buffer,
            bytes,
            timeout,
            multiplier
        )
        
        checkForErrorCode(result);

        return result
    }

    /**
     * Gat a list of the available ports.
     * @returns {Ports[]} Returns a list of ports info
     */
    getPortsInfo() : Ports[] {
        const buffer = new Uint8Array(2048);
        const result = this._dl.getAvailablePorts(
            buffer,
            buffer.length,
            encode(',' + '\0')
        )
        
        checkForErrorCode(result);

        const ports = decode(buffer).replaceAll('\0','').split(',').map((port) => {
            return {
                name: port
            };
        })

        return ports;
    }
}

