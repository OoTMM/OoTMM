export function bufReadI8(data: Uint8Array, offset: number) {
  const d = data[offset];
  return d < 0x80 ? d : d - 0x100;
}

export function bufReadU8(data: Uint8Array, offset: number) {
  return data[offset];
}

export function bufReadU16LE(data: Uint8Array, offset: number) {
  return data[offset] | (data[offset + 1] << 8);
}

export function bufReadU16BE(data: Uint8Array, offset: number) {
  return (data[offset] << 8) | data[offset + 1];
}

export function bufReadU32LE(data: Uint8Array, offset: number) {
  return (data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24)) >>> 0;
}

export function bufReadU32BE(data: Uint8Array, offset: number) {
  return ((data[offset] << 24) | (data[offset + 1] << 16) | (data[offset + 2] << 8) | data[offset + 3]) >>> 0;
}

export function bufWriteU8(data: Uint8Array, offset: number, value: number) {
  data[offset] = value;
}

export function bufWriteI8(data: Uint8Array, offset: number, value: number) {
  if (value < 0) {
    value = 0x100 + value;
  }
  data[offset] = value;
}

export function bufWriteU16LE(data: Uint8Array, offset: number, value: number) {
  data[offset] = value & 0xff;
  data[offset + 1] = (value >> 8) & 0xff;
}

export function bufWriteU16BE(data: Uint8Array, offset: number, value: number) {
  data[offset] = (value >> 8) & 0xff;
  data[offset + 1] = value & 0xff;
}

export function bufWriteU32LE(data: Uint8Array, offset: number, value: number) {
  data[offset] = value & 0xff;
  data[offset + 1] = (value >>> 8) & 0xff;
  data[offset + 2] = (value >>> 16) & 0xff;
  data[offset + 3] = (value >>> 24) & 0xff;
}

export function bufWriteU32BE(data: Uint8Array, offset: number, value: number) {
  data[offset] = (value >>> 24) & 0xff;
  data[offset + 1] = (value >>> 16) & 0xff;
  data[offset + 2] = (value >>> 8) & 0xff;
  data[offset + 3] = value & 0xff;
}
