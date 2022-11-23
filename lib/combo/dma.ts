import { Buffer } from 'buffer';

export type DmaDataRecord = {
  virtStart: number;
  virtEnd: number;
  physStart: number;
  physEnd: number;
};

export class DmaData {
  constructor(private buffer: Buffer) {}

  read(index: number): DmaDataRecord {
    const offset = index * 16;
    return {
      virtStart: this.buffer.readUInt32BE(offset + 0),
      virtEnd: this.buffer.readUInt32BE(offset + 4),
      physStart: this.buffer.readUInt32BE(offset + 8),
      physEnd: this.buffer.readUInt32BE(offset + 12),
    };
  }

  write(index: number, record: DmaDataRecord) {
    const offset = index * 16;
    this.buffer.writeUInt32BE(record.virtStart, offset + 0);
    this.buffer.writeUInt32BE(record.virtEnd, offset + 4);
    this.buffer.writeUInt32BE(record.physStart, offset + 8);
    this.buffer.writeUInt32BE(record.physEnd, offset + 12);
  }

  count() {
    return this.buffer.length / 16;
  }

  data() {
    return this.buffer;
  }
};
