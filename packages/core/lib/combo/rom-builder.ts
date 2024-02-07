export type RomFile = {
  name?: string;
  type: 'compressed' | 'uncompressed' | 'dummy';
  game: 'mm' | 'oot' | 'custom' | 'raw';
  index?: number;
  injected: boolean;
  data: Buffer;
  paddr?: number;
  vaddr?: number;
};

type AddFileArgs = Omit<RomFile, 'injected'>;

export class RomBuilder {
  private files: RomFile[];
  private out: Buffer;
  private paddr: number;
  private vaddr: number;

  constructor() {
    this.files = [];
    this.out = Buffer.alloc(64 * 1024 * 1024);
    this.paddr = 0;
    this.vaddr = 0xc0000000;
  }

  build(): Buffer {

  }

  addFile(args: AddFileArgs): void {
    const file: RomFile = { ...args, injected: false };

    this.files.push(file);
  }
};
