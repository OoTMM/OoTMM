import { bufReadU32BE } from './util/buffer';

export function validateOverlayRelocations(name: string, data: Uint8Array, base: number, end: number): number {
  const fail = (reason: string): never => {
    throw new Error(`Invalid overlay ${name}: ${reason}`);
  };
  if (data.length < 24 || data.length % 4 || end <= base) fail('invalid extent');
  const tail = bufReadU32BE(data, data.length - 4);
  if (tail < 24 || tail > data.length || tail % 4) fail('invalid relocation footer');
  const footer = data.length - tail;
  const sizes = [0, bufReadU32BE(data, footer), bufReadU32BE(data, footer + 4), bufReadU32BE(data, footer + 8)];
  const starts = [0, 0, sizes[1], sizes[1] + sizes[2]];
  const count = bufReadU32BE(data, footer + 16);
  if (sizes.some(size => size % 4) || sizes[1] + sizes[2] + sizes[3] !== footer || count > (tail - 24) / 4)
    fail('invalid relocation sections');
  const highs = new Map<number, number>();
  let active = 0;
  for (let i = 0; i < count; ++i) {
    const entry = bufReadU32BE(data, footer + 20 + i * 4);
    if (!entry) continue;
    const section = entry >>> 30, type = (entry >>> 24) & 63, offset = entry & 0xffffff;
    if (!section || offset % 4 || offset + 4 > sizes[section]) fail(`relocation ${i} is outside its section`);
    const at = starts[section] + offset, word = bufReadU32BE(data, at);
    const site = `site 0x${(base + at).toString(16)}, entry 0x${(base + footer + 20 + i * 4).toString(16)}`;
    let target: number | undefined;
    switch (type) {
      case 2:
        if (!(word & 0x0f000000)) target = word;
        break;
      case 4:
        if (word >>> 26 !== 2 && word >>> 26 !== 3) fail(`${site}: stale jump relocation on a non-jump`);
        target = (0x80000000 | ((word & 0x03ffffff) << 2)) >>> 0;
        break;
      case 5:
        if (word >>> 26 !== 15) fail(`${site}: HI16 is not LUI`);
        highs.set((word >>> 16) & 31, word);
        break;
      case 6: {
        const high = highs.get((word >>> 21) & 31);
        if (high === undefined) fail(`${site}: unpaired LO16`);
        const value = (((high! & 0xffff) << 16) + ((word << 16) >> 16)) >>> 0;
        if (!(value & 0x0f000000)) target = value;
        break;
      }
      default:
        fail(`${site}: unsupported relocation ${type}`);
    }

    if (target !== undefined && (target < base || target > end))
      fail(`${site}: target 0x${target.toString(16)} is outside this overlay; remove the stale relocation`);
    ++active;
  }
  return active;
}
