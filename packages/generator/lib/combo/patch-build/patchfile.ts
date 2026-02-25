import JSZip from 'jszip';
import { concatUint8Arrays } from 'uint8array-extras';
import { Game } from '../config';

const VERSION = '1.2';

class BlobBuilder {
  private parts: Uint8Array[];
  private length: number;

  constructor() {
    this.parts = [];
    this.length = 0;
  }

  append(data: Uint8Array) {
    const offset = this.length;
    const size = data.length;
    this.parts.push(data);
    this.length += data.length;
    return { offset, size };
  }

  concat() {
    return concatUint8Arrays(this.parts);
  }
}

type Patch = {
  addr: number;
  data: Uint8Array;
}

type NewFile = {
  name?: string;
  vrom: number;
  vram?: { [k in Game]?: [number, number] };
  data: Uint8Array;
  compressed: boolean;
}

export class Patchfile {
  public patches: {[k: string]: Patch[]};
  public newFiles: NewFile[];
  public removedFiles: string[];
  public hash: string;
  public meta: any;
  public symbols: Record<Game, Map<string, number[]>>;
  public multiId: Uint8Array;

  constructor() {
    this.patches = {};
    this.newFiles = [];
    this.removedFiles = [];
    this.hash = 'XXXXXXXX';
    this.meta = {};
    this.symbols = { oot: new Map(), mm: new Map() };
    this.multiId = new Uint8Array(16);
  }

  setHash(hash: string) {
    this.hash = hash;
  }

  addPatch(file: string, addr: number, data: Uint8Array) {
    const patches = this.patches[file] || [];
    patches.push({ addr, data });
    this.patches[file] = patches;
  }

  addNewFile(args: NewFile) {
    this.newFiles.push({ ...args, vram: { ...args.vram } });
  }

  async deserialize(data: Uint8Array) {
    let meta: any;
    let symbols: any;
    let blob: Uint8Array;
    let multiId: Uint8Array;

    try {
      /* Load from the zip */
      const zip = await JSZip.loadAsync(data);
      meta = JSON.parse(await zip.file('meta.json')!.async('text'));
      blob = await zip.file('blob.bin')!.async('uint8array');
      symbols = JSON.parse(await zip.file('symbols.json')!.async('text'));
      multiId = await zip.file('multi-uuid.bin')!.async('uint8array');
    } catch (e) {
      throw new Error(`Failed to load patchfile: ${e}`);
    }

    if (meta.version !== VERSION) {
      throw new Error(`Unsupported patchfile version: ${meta.version}`);
    }

    if (multiId.length !== 16) {
      throw new Error('Invalid multi-id length');
    }

    this.hash = meta.hash;
    this.meta = meta.meta;
    this.removedFiles = meta.removedFiles;
    this.newFiles = [];
    this.patches = {};
    this.symbols = { oot: new Map(), mm: new Map() };
    this.multiId = multiId;

    /* Load new files */
    for (const f of meta.newFiles) {
      const data = blob.subarray(f.offset, f.offset + f.size);
      this.newFiles.push({ name: f.name, vrom: f.vrom, vram: f.vram, data, compressed: f.compressed });
    }

    /* Load patches */
    for (const k in meta.patches) {
      this.patches[k] = [];
      for (const p of meta.patches[k]) {
        const data = blob.subarray(p.offset, p.offset + p.size);
        this.patches[k].push({ addr: p.addr, data });
      }
    }

    /* Load symbols */
    for (const k in symbols.oot) {
      this.symbols.oot.set(k, symbols.oot[k]);
    }

    for (const k in symbols.mm) {
      this.symbols.mm.set(k, symbols.mm[k]);
    }
  }

  async serialize(): Promise<Uint8Array> {
    const zip = new JSZip();
    const blobBuilder = new BlobBuilder();
    const meta: any = {};
    meta.version = VERSION;
    meta.hash = this.hash;
    meta.meta = this.meta;
    meta.removedFiles = this.removedFiles;
    meta.newFiles = [];

    /* Store new files */
    for (const f of this.newFiles) {
      const nf: any = {};
      nf.name = f.name;
      nf.compressed = f.compressed;
      nf.vrom = f.vrom;
      nf.vram = f.vram;
      const { offset, size } = blobBuilder.append(f.data);
      nf.offset = offset;
      nf.size = size;
      meta.newFiles.push(nf);
    }

    /* Store patches */
    meta.patches = {};
    for (const k in this.patches) {
      meta.patches[k] = [];
      for (const p of this.patches[k]) {
        const patch: any = {};
        patch.addr = p.addr;
        const { offset, size } = blobBuilder.append(p.data);
        patch.offset = offset;
        patch.size = size;
        meta.patches[k].push(patch);
      }
    }

    /* Store meta and blob */
    zip.file('meta.json', JSON.stringify(meta));
    zip.file('blob.bin', blobBuilder.concat());

    /* Store multi id */
    zip.file('multi-uuid.bin', this.multiId);

    /* Store symbols */
    const symbols: any = { oot: {}, mm: {} };
    for (const [k, v] of this.symbols.oot) {
      symbols.oot[k] = v;
    }
    for (const [k, v] of this.symbols.mm) {
      symbols.mm[k] = v;
    }
    zip.file('symbols.json', JSON.stringify(symbols));

    return await zip.generateAsync({ type: 'uint8array', compression: 'DEFLATE' });
  }

  dup() {
    const ret = new Patchfile();
    ret.hash = this.hash;
    ret.meta = JSON.parse(JSON.stringify(this.meta));
    ret.patches = {};
    for (const k in this.patches) {
      ret.patches[k] = [...this.patches[k]];
    }
    ret.removedFiles = [...this.removedFiles];
    ret.newFiles = [...this.newFiles];
    ret.multiId = this.multiId.slice();
    ret.symbols = { oot: new Map(), mm: new Map() };
    for (const [k, v] of this.symbols.oot) {
      ret.symbols.oot.set(k, [...v]);
    }
    for (const [k, v] of this.symbols.mm) {
      ret.symbols.mm.set(k, [...v]);
    }
    return ret;
  }
}
