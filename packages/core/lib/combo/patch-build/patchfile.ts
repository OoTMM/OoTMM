import { Game } from "../config";
import { toU32BufferLE } from "../util";

const REVISION = 0;
const MAGIC = 'OoTMM-P2';

type DataPatch = {
  addr: number;
  data: Buffer;
}

type NewFile = {
  vrom: number;
  data: Buffer;
  compressed: boolean;
}

type GamePatches = {
  data: DataPatch[];
  removedFiles: Set<number>;
}

export class Patchfile {
  public gamePatches: {[k in Game]: GamePatches};
  public newFiles: NewFile[];
  public hash: string;

  constructor(data?: Buffer) {
    this.gamePatches = {
      oot: { data: [], removedFiles: new Set() },
      mm: { data: [], removedFiles: new Set() },
    };
    this.newFiles = [];
    this.hash = 'XXXXXXXX';

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

      let offset = 0x20;
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
    this.gamePatches[game].data.push({ addr, data: patch });
    return offset;
  }

  private parseNewFilePatch(data: Buffer, offset: number) {
    const vrom = data.readUInt32LE(offset);
    const compressed = data.readUInt32LE(offset + 4) === 1;
    const size = data.readUInt32LE(offset + 8);
    const patch = data.subarray(offset + 0x10, offset + 0x10 + size);
    offset += 0x10 + size;
    this.newFiles.push({ vrom, data: patch, compressed });
    return offset;
  }

  setHash(hash: string) {
    this.hash = hash;
  }

  addDataPatch(game: Game, addr: number, data: Buffer) {
    this.gamePatches[game].data.push({ addr, data });
  }

  addNewFile(vrom: number, data: Buffer, compressed: boolean) {
    this.newFiles.push({ vrom, data, compressed });
  }

  removeFile(game: Game, id: number) {
    this.gamePatches[game].removedFiles.add(id);
  }

  toBuffer(): Buffer {
    const buffers: Buffer[] = [];
    const header = Buffer.alloc(0x20, 0xff);
    header.write(MAGIC, 0x00, 0x08, 'utf8');
    header.write(this.hash, 0x08, 0x08, 'utf8');
    header.writeUInt32LE(REVISION, 0x10);
    header.writeUInt32LE(this.gamePatches.oot.data.length, 0x14);
    header.writeUInt32LE(this.gamePatches.mm.data.length, 0x18);
    header.writeUInt32LE(this.newFiles.length, 0x1c);
    buffers.push(header);

    const gameDataPatches = [...this.gamePatches.oot.data, ...this.gamePatches.mm.data];
    for (const p of gameDataPatches) {
      const h = toU32BufferLE([p.addr, p.data.length]);
      buffers.push(h);
      buffers.push(p.data);
    }

    for (const f of this.newFiles) {
      const h = toU32BufferLE([f.vrom, f.compressed ? 1 : 0, f.data.length, 0]);
      buffers.push(h);
      buffers.push(f.data);
    }

    return Buffer.concat(buffers);
  }

  dup() {
    const ret = new Patchfile();
    ret.hash = this.hash;
    ret.gamePatches.oot = { data: [...this.gamePatches.oot.data], removedFiles: new Set(this.gamePatches.oot.removedFiles) };
    ret.gamePatches.mm = { data: [...this.gamePatches.mm.data], removedFiles: new Set(this.gamePatches.mm.removedFiles) };
    ret.newFiles = [...this.newFiles];
    return ret;
  }
}
