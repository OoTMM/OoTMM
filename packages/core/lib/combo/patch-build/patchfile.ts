import JSZip from 'jszip';
import { concatUint8Arrays } from 'uint8array-extras';

const VERSION = '1.1';

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
  vram?: [number, number];
  data: Uint8Array;
  compressed: boolean;
}

export class Patchfile {
  public patches: {[k: string]: Patch[]};
  public newFiles: NewFile[];
  public removedFiles: string[];
  public hash: string;
  public meta: any;

  constructor() {
    this.patches = {};
    this.newFiles = [];
    this.removedFiles = [];
    this.hash = 'XXXXXXXX';
    this.meta = {};
  }

  setHash(hash: string) {
    this.hash = hash;
  }

  setMeta(meta: any) {
    this.meta = meta;
  }

  addPatch(file: string, addr: number, data: Uint8Array) {
    const patches = this.patches[file] || [];
    patches.push({ addr, data });
    this.patches[file] = patches;
  }

  addNewFile(args: NewFile) {
    this.newFiles.push({ ...args, vram: args.vram ? [...args.vram] : undefined });
  }

  async deserialize(data: Uint8Array) {
    let meta: any;
    let blob: Uint8Array;

    try {
      /* Load from the zip */
      const zip = await JSZip.loadAsync(data);
      meta = JSON.parse(await zip.file('meta.json')!.async('text'));
      blob = await zip.file('blob.bin')!.async('uint8array');
    } catch (e) {
      throw new Error(`Failed to load patchfile: ${e}`);
    }

    if (meta.version !== VERSION) {
      throw new Error(`Unsupported patchfile version: ${meta.version}`);
    }

    this.hash = meta.hash;
    this.meta = meta.meta;
    this.removedFiles = meta.removedFiles;
    this.newFiles = [];
    this.patches = {};

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
    return ret;
  }
}
