import { Game } from "../config";

const REVISION = 1;

const TYPES_TO_VALUES = {
  global: 0,
  oot: 1,
  mm: 2,
};

export type PatchType = keyof typeof TYPES_TO_VALUES;

const VALUES_TO_TYPES: {[k: number]: PatchType} = {};
for (const [k, v] of Object.entries(TYPES_TO_VALUES)) {
  VALUES_TO_TYPES[v] = k as PatchType;
}

type DataPatch = {
  addr: number;
  data: Buffer;
}

type GamePatches = {
  data: DataPatch[];
}

export class Patchfile {
  public gamePatches: {[k in Game]: GamePatches};
  public globalPatches: DataPatch[];
  public readonly hash: string;

  constructor(hashOrBuffer: string | Buffer) {
    this.gamePatches = {
      oot: { data: [] },
      mm: { data: [] },
    };
    this.globalPatches = [];

    if (typeof hashOrBuffer === 'string') {
      this.hash = hashOrBuffer;
    } else {
      const header = hashOrBuffer.subarray(0, 0x18);
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

  addDataPatch(game: Game, addr: number, data: Buffer) {
    this.gamePatches[game].data.push({ addr, data });
  }

  addGlobalPatch(addr: number, data: Buffer) {
    this.globalPatches.push({ addr, data });
  }

  toBuffer(): Buffer {
    const buffers: Buffer[] = [];
    const header = Buffer.alloc(0x18, 0xff);
    return Buffer.concat(buffers);
  }

  dup() {
    const ret = new Patchfile(this.hash);
    ret.gamePatches.oot = { data: [...this.gamePatches.oot.data] };
    ret.gamePatches.mm = { data: [...this.gamePatches.mm.data] };
    ret.globalPatches = [...this.globalPatches];
    return ret;
  }
}
