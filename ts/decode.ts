const textDecoder = new TextDecoder()

/**
 * This function is used as a wrapper for the default text decoder.
 * @param buffer Data to decode
 * @returns The decoded data
 */
export const decode = (buffer : Uint8Array) : string => textDecoder.decode(buffer);
