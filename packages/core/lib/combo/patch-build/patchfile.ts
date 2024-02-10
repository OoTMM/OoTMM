import { Game } from '../config';
import { toU32BufferLE } from '../util';

const REVISION = 2;
const MAGIC = 'OoTMM-P2';

type DataPatch = {
  addr: number;
  data: Buffer;
}

type NewFile = {
  name: string | null;
  vrom: number;
  data: Buffer;
  compressed: boolean;
}

export class Patchfile {
  public gamePatches: {[k in Game]: DataPatch[] };
  public newFiles: NewFile[];
  public hash: string;
  public meta: any;

  constructor(data?: Buffer) {
    this.gamePatches = {
      oot: [],
      mm: [],
    };
    this.newFiles = [];
    this.hash = 'XXXXXXXX';
    this.meta = {};

    if (data) {
      if (data.toString('utf8', 0, 8) !== MAGIC) {
        throw new Error('Invalid patch file');
      }
      this.hash = data.toString('utf8', 0x08, 0x10);
      const rev = data.readUInt32LE(0x10);
      if (rev !== REVISION) {
        throw new Error(`Unsupported patch file revision ${rev}`);
      }
      const patchCountOot = data.readUInt32LE(0x14);
      const patchCountMm = data.readUInt32LE(0x18);
      const patchCountNewFile = data.readUInt32LE(0x1c);
      const metaOff = data.readUint32LE(0x20);
      const metaLen = data.readUint32LE(0x24);
      const metaRaw = data.subarray(metaOff, metaOff + metaLen);
      this.meta = JSON.parse(metaRaw.toString('utf8'));

      let offset = 0x40;
      for (let i = 0; i < patchCountOot; i++) {
        offset = this.parseGamePatch('oot', data, offset);
      }

      for (let i = 0; i < patchCountMm; i++) {
        offset = this.parseGamePatch('mm', data, offset);
      }

      for (let i = 0; i < patchCountNewFile; i++) {
        offset = this.parseNewFilePatch(data, offset);
      }
    }
  }

  private parseGamePatch(game: Game, data: Buffer, offset: number) {
    const addr = data.readUInt32LE(offset);
    const size = data.readUInt32LE(offset + 4);
    const patch = data.subarray(offset + 8, offset + 8 + size);
    offset += 8 + size;
    this.gamePatches[game].push({ addr, data: patch });
    return offset;
  }

  private parseNewFilePatch(data: Buffer, offset: number) {
    let name: string | null = data.toString('utf8', offset, offset + 0x40).replace(/\0/g, '');
    if (name === '') {
      name = null;
    }
    const vrom = data.readUInt32LE(offset + 0x40);
    const compressed = data.readUInt32LE(offset + 0x44) === 1;
    const size = data.readUInt32LE(offset + 0x48);
    const patch = data.subarray(offset + 0x50, offset + 0x50 + size);
    offset += 0x50 + size;
    this.newFiles.push({ name, vrom, data: patch, compressed });
    return offset;
  }

  setHash(hash: string) {
    this.hash = hash;
  }

  setMeta(meta: any) {
    this.meta = meta;
  }

  addDataPatch(game: Game, addr: number, data: Buffer) {
    this.gamePatches[game].push({ addr, data });
  }

  addNewFile(name: string | null, vrom: number, data: Buffer, compressed: boolean) {
    this.newFiles.push({ name, vrom, data, compressed });
  }

  toBuffer(): Buffer {
    const buffers: Buffer[] = [];
    const header = Buffer.alloc(0x40, 0xff);
    header.write(MAGIC, 0x00, 0x08, 'utf8');
    header.write(this.hash, 0x08, 0x08, 'utf8');
    header.writeUInt32LE(REVISION, 0x10);
    header.writeUInt32LE(this.gamePatches.oot.length, 0x14);
    header.writeUInt32LE(this.gamePatches.mm.length, 0x18);
    header.writeUInt32LE(this.newFiles.length, 0x1c);
    buffers.push(header);

    const gameDataPatches = [...this.gamePatches.oot, ...this.gamePatches.mm];
    for (const p of gameDataPatches) {
      const h = toU32BufferLE([p.addr, p.data.length]);
      buffers.push(h);
      buffers.push(p.data);
    }

    for (const f of this.newFiles) {
      const nameBuf = Buffer.alloc(0x40, 0);
      if (f.name) {
        nameBuf.write(f.name, 'utf-8');
      }
      const h = toU32BufferLE([f.vrom, f.compressed ? 1 : 0, f.data.length, 0]);
      buffers.push(nameBuf);
      buffers.push(h);
      buffers.push(f.data);
    }

    const meta = Buffer.from(JSON.stringify(this.meta), 'utf8');
    const metaOff = buffers.reduce((acc, b) => acc + b.length, 0);
    const metaLen = meta.length;
    buffers.push(meta);

    header.writeUInt32LE(metaOff, 0x20);
    header.writeUInt32LE(metaLen, 0x24);

    return Buffer.concat(buffers);
  }

  dup() {
    const ret = new Patchfile();
    ret.hash = this.hash;
    ret.meta = JSON.parse(JSON.stringify(this.meta));
    ret.gamePatches.oot = [...this.gamePatches.oot];
    ret.gamePatches.mm = [...this.gamePatches.mm];
    ret.newFiles = [...this.newFiles];
    return ret;
  }
}
