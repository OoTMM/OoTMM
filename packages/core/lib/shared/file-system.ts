export type GameFileType = 'compressed' | 'uncompressed' | 'dummy';

export type GameFileInject = {
  index: number;
  vrom: number;
};

export type GameFileCustomInject = {
  vrom: number;
};

export type GameFileInjects = {
  oot?: GameFileInject;
  mm?: GameFileInject;
  custom?: GameFileCustomInject;
}

export type GameFile = {
  name: string;
  type: GameFileType;
  data: Uint8Array;
  injects: GameFileInjects;
};

export type GameFileSystemBlob = {
  files: Map<string, [number, number]>;
  data: Uint8Array;
};

type AddFileArgs = {
  name?: string;
  compressed?: boolean;
  data: Uint8Array;
};

type GameFileSystemState = {
  nextUnkId: number;
  vrom: number;
}

export class GameFileSystem {
  public files: GameFile[];
  private state: GameFileSystemState;

  constructor(old?: GameFileSystem) {
    if (old) {
      this.files = old.files.map(x => ({ ...x, injects: { ...x.injects }, data: new Uint8Array(x.data) }));
      this.state = { ...old.state };
    } else {
      this.files = [];
      this.state = {
        nextUnkId: 0,
        vrom: 0x08000000,
      }
    }
  }

  addCustomFile(args: AddFileArgs) {
    const vrom = this.state.vrom;
    this.state.vrom = (this.state.vrom + args.data.length + 0xf) & ~0xf;

    const name = args.name ?? `unk_${this.state.nextUnkId++}`;
    this.files.push({ name, injects: { custom: { vrom } }, type: args.compressed ? 'compressed' : 'uncompressed', data: args.data });

    return vrom;
  }

  addRawFile(file: GameFile) {
    this.files.push(file);
  }

  getFile(name: string): GameFile | undefined {
    return this.files.find((file) => file.name === name);
  }

  getFileOrThrow(name: string): GameFile {
    const file = this.getFile(name);
    if (file === undefined) {
      throw new Error(`File not found: ${name}`);
    }
    return file;
  }

  blob(): GameFileSystemBlob {
    let size = 0;
    const files = new Map<string, [number, number]>();

    for (const file of this.files) {
      const sz = file.data.length;
      files.set(file.name, [size, sz]);
      size += sz;
    }

    size = (size + 3) & ~3;
    let offset = 0;
    const buf = new Uint8Array(size);
    for (const file of this.files) {
      buf.set(file.data, offset);
      offset += file.data.length;
    }

    return { files, data: buf };
  }
}