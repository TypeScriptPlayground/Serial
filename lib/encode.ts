const textEncoder = new TextEncoder()

/**
 * This function is used as a wrapper for the default text encoder
 * @param buffer Data to encode
 * @returns The encoded data
 */
export const encode = (string : string) => textEncoder.encode(string);
