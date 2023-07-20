import { Game } from "../config";

const REVISION = 1;

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
  payload?: Buffer;
}

export class Patchfile {
  public gamePatches: {[k in Game]: GamePatches};
  public globalPatches: DataPatch[];
  public newFiles: NewFile[];
  public hash: string;

  constructor(data?: Buffer) {
    this.gamePatches = {
      oot: { data: [] },
      mm: { data: [] },
    };
    this.globalPatches = [];
    this.newFiles = [];
    this.hash = 'XXXXXXXX';

    if (data) {
      const header = data.subarray(0, 0x18);
      if (header.toString('utf8', 0, 8) !== 'OoTMM-PF') {
        throw new Error('Invalid patch file');
      }
      const rev = header.readUInt32LE(0x8);
      if (rev !== REVISION) {
        throw new Error(`Unsupported patch file revision ${rev}`);
      }
      this.hash = header.toString('utf8', 0x10, 0x18);
      let offset = 0x18;
      const patchCount = header.readUInt32LE(0xc);
    }
  }

  setHash(hash: string) {
    this.hash = hash;
  }

  addDataPatch(game: Game, addr: number, data: Buffer) {
    this.gamePatches[game].data.push({ addr, data });
  }

  addGlobalPatch(addr: number, data: Buffer) {
    this.globalPatches.push({ addr, data });
  }

  addNewFile(vrom: number, data: Buffer, compressed: boolean) {
    this.newFiles.push({ vrom, data, compressed });
  }

  addPayload(game: Game, payload: Buffer) {
    this.gamePatches[game].payload = payload;
  }

  toBuffer(): Buffer {
    const buffers: Buffer[] = [];
    const header = Buffer.alloc(0x18, 0xff);
    return Buffer.concat(buffers);
  }

  dup() {
    const ret = new Patchfile();
    ret.hash = this.hash;
    ret.gamePatches.oot = { data: [...this.gamePatches.oot.data], payload: this.gamePatches.oot.payload };
    ret.gamePatches.mm = { data: [...this.gamePatches.mm.data], payload: this.gamePatches.mm.payload };
    ret.globalPatches = [...this.globalPatches];
    ret.newFiles = [...this.newFiles];
    return ret;
  }
}
