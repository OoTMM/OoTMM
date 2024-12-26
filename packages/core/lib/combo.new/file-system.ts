export type RandoFile = {
  id: number;
  name: string;
  data?: Uint8Array;
}

export class RandoFileSystem {
  public files: RandoFile[];

  constructor() {
    this.files = [];
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
