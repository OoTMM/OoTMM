import { Game, GAMES } from '../combo/config';
import { toU32Buffer } from '../combo/util';
import { bufReadU32BE, bufWriteU32BE } from '../combo/util/buffer';
import { RandoFile, RandoFileSystem } from './file-system';

const rol = (v: number, b: number) => (((v << b) | (v >>> (32 - b))) & 0xffffffff) >>> 0;

function checksum(rom: Uint8Array) {
  const seed = 0xdf26f436;
  let t1 = seed;
  let t2 = seed;
  let t3 = seed;
  let t4 = seed;
  let t5 = seed;
  let t6 = seed;

  for (let i = 0; i < 0x100000 / 4; ++i) {
    const offset = 0x1000 + i * 4;
    const d = bufReadU32BE(rom, offset);
    if ((((t6 + d) & 0xffffffff) >>> 0) < t6) {
      t4 = ((t4 + 1) & 0xffffffff) >>> 0;
    }
    t6 = ((t6 + d) & 0xffffffff) >>> 0;
    t3 = ((t3 ^ d) & 0xffffffff) >>> 0;
    const r = rol(d, ((d & 0x1f) >>> 0));
    t5 = ((t5 + r) & 0xffffffff) >>> 0;
    if (t2 > d) {
      t2 = (t2 ^ r) >>> 0;
    } else {
      t2 = (t2 ^ t6 ^ d) >>> 0;
    }
    const offset2 = 0x750 + ((i * 4) & 0xff);
    const x = bufReadU32BE(rom, offset2);
    t1 = ((t1 + ((x ^ d) >>> 0)) & 0xffffffff) >>> 0;
  }

  return [(t6 ^ t4 ^ t3) >>> 0, (t5 ^ t2 ^ t1) >>> 0];
}

class RomBuilder {
  private rom: Uint8Array;
  private fileSystem: RandoFileSystem;
  private offsets: Map<RandoFile, number>;
  private pos: number;

  constructor(fileSystem: RandoFileSystem) {
    this.rom = new Uint8Array(128 * 1024 * 1024);
    this.fileSystem = fileSystem;
    this.offsets = new Map();
    this.pos = 0;
  }

  private fixChecksum() {
    const [c1, c2] = checksum(this.rom);
    bufWriteU32BE(this.rom, 0x10, c1);
    bufWriteU32BE(this.rom, 0x14, c2);
  }

  private resolveFile(file: RandoFile | string) {
    if (typeof file === 'string') {
      const f = this.fileSystem.findFile(file);
      if (!f) {
        throw new Error(`File not found: ${file}`);
      }
      return f;
    }
    return file;
  }

  private inject(file: RandoFile | string) {
    file = this.resolveFile(file);

    /* Check if already injected */
    if (this.offsets.has(file)) {
      return;
    }

    /* Inject the file */
    const data = file.data!;
    this.rom.set(data, this.pos);
    this.offsets.set(file, this.pos);

    /* Update the position */
    this.pos += (data.length + 15) & 0xfffffff0;
  }

  private patchLoader() {
    const loaderFile = this.resolveFile('loader');
    const loaderOffset = this.offsets.get(loaderFile)!;

    const data = [];
    for (const game of GAMES) {
      const bootFile = this.resolveFile(`${game}/boot`);
      const ram = this.fileSystem.meta.games[game].ram;
      const rom = this.offsets.get(bootFile)!;
      const size = bootFile.data!.length;
      const entrypoint = this.fileSystem.meta.games[game].bootproc;
      data.push(ram, rom, size, entrypoint);

      if (ram & 0xf || size & 0xf || rom & 0xf) {
        throw new Error(`Game DMA: ${game} Invalid alignment`);
      }
    }
    const configBlock = toU32Buffer(data);
    this.rom.set(configBlock, loaderOffset + this.fileSystem.meta.loaderOffsets.configs);
  }

  private injectFileTable() {
    const filesSorted = Array.from(this.fileSystem.files).sort((a, b) => a.id - b.id);
    const fileTable = new Uint8Array(filesSorted.length * 4 * 4);

    for (let i = 0; i < filesSorted.length; ++i) {
      const f = filesSorted[i];
      bufWriteU32BE(fileTable, i * 16 + 0, f.id);
      bufWriteU32BE(fileTable, i * 16 + 4, this.offsets.get(f)!);
      bufWriteU32BE(fileTable, i * 16 + 8, f.data!.length);
      bufWriteU32BE(fileTable, i * 16 + 12, 0);
    }

    /* Inject the file table */
    const fileTableOffset = this.pos;
    this.rom.set(fileTable, fileTableOffset);
    this.pos += fileTable.length;

    /* Patch the loader */
    const loaderFile = this.resolveFile('loader');
    const loaderOffset = this.offsets.get(loaderFile)!;
    bufWriteU32BE(this.rom, loaderOffset + this.fileSystem.meta.loaderOffsets.file, fileTableOffset);
    bufWriteU32BE(this.rom, loaderOffset + this.fileSystem.meta.loaderOffsets.file + 4, filesSorted.length);
  }

  private makeDmaTable(game: Game, id: number) {
    const files = Array.from(this.fileSystem.files.filter((f) => f.dma[game])).sort((a, b) => a.dma[game]!.start - b.dma[game]!.start);
    const dmaTable = new Uint8Array(files.length * 4 * 3 + 4);

    bufWriteU32BE(dmaTable, 0, files.length);
    for (let i = 0; i < files.length; ++i) {
      const f = files[i];
      const dma = f.dma[game]!;
      bufWriteU32BE(dmaTable, i * 12 + 4, dma.start);
      bufWriteU32BE(dmaTable, i * 12 + 8, dma.size);
      bufWriteU32BE(dmaTable, i * 12 + 12, f.id);
    }

    /* Inject the DMA table in the files */
    this.fileSystem.addFile({ id, name: `${game}/dmadata`, data: dmaTable, dma: {} });
  }

  private makeDmaTables() {
    this.makeDmaTable('oot', 2);
  }

  build() {
    /* Build the DMA tables */
    this.makeDmaTables();

    /* We need to inject the makerom and loader first */
    this.inject('makerom');
    this.inject('loader');

    /* Inject all files */
    for (const file of this.fileSystem.files) {
      this.inject(file);
    }

    /* Inject the file table */
    this.injectFileTable();

    /* Patch loader */
    this.patchLoader();

    /* Fix the checksum */
    this.fixChecksum();

    return this.rom;
  }
}

export function buildRom(fileSystem: RandoFileSystem) {
  const builder = new RomBuilder(fileSystem);
  return builder.build();
}

