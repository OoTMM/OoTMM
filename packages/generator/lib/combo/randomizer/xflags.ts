import { CHECKS_XFLAG_IDS } from '@ootmm/data';
import { bufWriteU16BE, bufWriteU32BE } from '../util/buffer';

/* TODO: This is fully static and should be embedded from data.zip instead */
export class RandomizerPatcherXflags {
  public static run() {
    return (new RandomizerPatcherXflags()).execute();
  }

  private execute(): Uint8Array {
    const keys = [...CHECKS_XFLAG_IDS.keys()].sort((a, b) => a < b ? -1 : 1);
    const buffer = new Uint8Array(keys.length * 8);

    for (let i = 0; i < keys.length; i++) {
      const key = keys[i];
      const value = CHECKS_XFLAG_IDS.get(key)!;
      const base = i * 8;
      bufWriteU32BE(buffer, base + 0, key);
      bufWriteU16BE(buffer, base + 4, value);
      bufWriteU16BE(buffer, base + 6, 0);
    }

    return buffer;
  }
}
