import { Game } from "../combo/config";

export type RandoFileSystemMetadata = {
  loaderOffsets: {
    configs: number;
  };
  games: { [k in Game]: { bootproc: number, ram: number } };
}

export type RandoFile = {
  id: number;
  name: string;
  data?: Uint8Array;
}

export class RandoFileSystem {
  public files: RandoFile[];
  public meta: RandoFileSystemMetadata;

  constructor() {
    this.files = [];
    this.meta = {
      loaderOffsets: {
        configs: 0,
      },
      games: {
        oot: { bootproc: 0, ram: 0 },
        mm: { bootproc: 0, ram: 0 },
      }
    }
  }

  addFile(file: RandoFile) {
    this.files.push(file);
  }

  deleteFile(name: string) {
    this.files = this.files.filter((f) => f.name !== name);
  }

  findFile(name: string) {
    return this.files.find((f) => f.name === name);
  }
}
