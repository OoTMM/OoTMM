import { bufReadU32BE, bufWriteU32BE } from './util/buffer';

export type DmaDataRecord = {
  virtStart: number;
  virtEnd: number;
  physStart: number;
  physEnd: number;
};

export class DmaData {
  constructor(private buffer: Uint8Array) {}

  read(index: number): DmaDataRecord {
    const offset = index * 16;
    return {
      virtStart: bufReadU32BE(this.buffer, offset + 0),
      virtEnd: bufReadU32BE(this.buffer, offset + 4),
      physStart: bufReadU32BE(this.buffer, offset + 8),
      physEnd: bufReadU32BE(this.buffer, offset + 12),
    };
  }

  write(index: number, record: DmaDataRecord) {
    const offset = index * 16;

    bufWriteU32BE(this.buffer, offset + 0, record.virtStart);
    bufWriteU32BE(this.buffer, offset + 4, record.virtEnd);
    bufWriteU32BE(this.buffer, offset + 8, record.physStart);
    bufWriteU32BE(this.buffer, offset + 12, record.physEnd);
  }

  count() {
    return this.buffer.length / 16;
  }

  data() {
    return this.buffer;
  }
};
