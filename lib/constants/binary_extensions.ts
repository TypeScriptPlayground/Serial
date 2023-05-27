import { ObjectValuesMap } from "../types/object_values_map.ts";

export const binaryExtensions : {[key : string] : string} = {
    windows: 'dll',
    linux: 'so',
    darwin: '',
    freebsd: '',
    netbsd: '',
    aix: '',
    solaris: '',
    illumos: ''
} as const;

export type BinaryExtension = ObjectValuesMap<typeof binaryExtensions>
