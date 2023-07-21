import { GameAddresses } from '../addresses';
import { Game } from '../config';
import { DecompressedRoms } from '../decompress';
import { Options } from '../options';
import { Patchfile } from '../patch-build/patchfile';
import { randString } from '../random';
import { Random, sample } from '../random';
import { toU32Buffer } from '../util';
import { png } from '../util/png';
import { Color, ColorRandom, COLORS } from './color';
import { recolorImage } from './image';
import fs from 'fs/promises';
import { enableModelOotLinkChild, enableModelOotLinkAdult } from './model';
import { BufferPath } from './type';

export { makeCosmetics } from './util';
export { COSMETICS } from './data';
export { Cosmetics } from './type';

const OBJECTS_TABLE_ADDR = 0x800f8ff8;

export async function cosmeticsAssets(opts: Options) {
  return {
    MASK_TUNIC: await png(opts, 'masks/tunic', 'bitmask'),
  }
}

type Unpromise<T extends Promise<any>> = T extends Promise<infer U> ? U : never;

class CosmeticsPass {
  private vrom: number;
  private random: Random;
  private patch: Patchfile;
  private assets!: Unpromise<ReturnType<typeof cosmeticsAssets>>;

  constructor(
    private opts: Options,
    private addresses: GameAddresses,
    private roms: DecompressedRoms,
  ) {
    this.random = new Random();
    this.random.seed(randString());
    this.patch = new Patchfile;
    this.vrom = 0xc0000000;
  }

  private addNewFile(data: Buffer) {
    const size = (data.length + 0xf) & ~0xf;
    const vrom = this.vrom;
    this.vrom = (this.vrom + size) >>> 0;
    this.patch.addNewFile(vrom, data, true);
    return [vrom, (vrom + size) >>> 0];
  }

  private async getPathBuffer(path: BufferPath | null): Promise<Buffer | null> {
    if (path === null) {
      return null;
    }

    if (Buffer.isBuffer(path)) {
      return path;
    }

    if (!process.env.ROLLUP) {
      return fs.readFile(path);
    } else {
      throw new Error(`Cannot load buffers from path`);
    }
  }

  private color(c: ColorRandom): number {
    let color: Color;

    if (c === 'random') {
      color = sample(this.random, Object.keys(COLORS) as Color[]);
    } else {
      color = c;
    }

    return COLORS[color].value;
  }

  private patchColorRGB(game: Game, addr: number, color: number) {
    const paddr = this.addresses[game].virtualToPhysical(addr);
    const buffer = Buffer.alloc(3);
    buffer.writeUInt8(color >>> 16, 0);
    buffer.writeUInt8((color >>> 8) & 0xff, 1);
    buffer.writeUInt8(color & 0xff, 2);
    this.patch.addDataPatch(game, paddr, buffer);
  }

  private patchOotTunic(index: number, color: ColorRandom) {
    const c = this.color(color);
    const defaultColors: Color[] = [
      'kokirigreen',
      'goronred',
      'zorablue',
    ];
    const defaultColorIcons: number[] = [
      0x005a00,
      0x7a0000,
      0x0020b7,
    ];
    const defaultColor = defaultColors[index];
    const defaultColorIcon = defaultColorIcons[index];

    if (c === this.color(defaultColor))
      return;

    /* Patch the in-game color */
    this.patchColorRGB('oot', (0x800f7ad8 + index * 0x03) >>> 0, c);

    /* Patch the icon */
    const paddr = 0x7fe000 + 0x1000 * index;
    const icon = this.roms.oot.rom.subarray(paddr, paddr + 0x1000);
    const newIcon = recolorImage(icon, this.assets.MASK_TUNIC, defaultColorIcon, c);
    this.patch.addDataPatch('oot', paddr, newIcon);
  }

  private async patchOotChildModel() {
    const model = await this.getPathBuffer(this.opts.cosmetics.modelOotChildLink);
    if (model) {
      const magic = model.indexOf(Buffer.from('MODLOADER64'));
      if (magic === -1) {
        throw new Error('Invalid model file');
      }

      /* Inject the new model */
      const obj = this.addNewFile(model);
      const objBuffer = toU32Buffer(obj);
      const vram = OBJECTS_TABLE_ADDR + 8 * 0x15;
      const vrom = this.addresses.oot.virtualToPhysical(vram);
      this.patch.addDataPatch('oot', vrom, objBuffer);

      /* Remove the old model */
      this.patch.removeFile('oot', 0x1f7);

      /* Enable the PlayAs hooks */
      const dfAddr = model.indexOf(Buffer.from([0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]));
      enableModelOotLinkChild(this.patch, dfAddr);
    }
  }

  private async patchOotAdultModel() {
    const model = await this.getPathBuffer(this.opts.cosmetics.modelOotAdultLink);
    if (model) {
      const magic = model.indexOf(Buffer.from('MODLOADER64'));
      if (magic === -1) {
        throw new Error('Invalid model file');
      }

      /* Inject the new model */
      const obj = this.addNewFile(model);
      const objBuffer = toU32Buffer(obj);
      const vram = OBJECTS_TABLE_ADDR + 8 * 0x14;
      const vrom = this.addresses.oot.virtualToPhysical(vram);
      this.patch.addDataPatch('oot', vrom, objBuffer);

      /* Remove the old model */
      this.patch.removeFile('oot', 0x1f6);

      /* Enable the PlayAs hooks */
      const dfAddr = model.indexOf(Buffer.from([0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]));
      enableModelOotLinkAdult(this.patch, dfAddr);
    }
  }

  async run(): Promise<Patchfile> {
    this.assets = await cosmeticsAssets(this.opts);

    /* OoT tunics */
    this.patchOotTunic(0, this.opts.cosmetics.ootTunicKokiri);
    this.patchOotTunic(1, this.opts.cosmetics.ootTunicGoron);
    this.patchOotTunic(2, this.opts.cosmetics.ootTunicZora);

    /* Models */
    await this.patchOotChildModel();
    await this.patchOotAdultModel();

    return this.patch;
  }
}

export async function cosmetics(opts: Options, addresses: GameAddresses, roms: DecompressedRoms): Promise<Patchfile> {
  const pass = new CosmeticsPass(opts, addresses, roms)
  return pass.run();
}
