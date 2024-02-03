import { GameAddresses } from '../addresses';
import { Game } from '../config';
import { DecompressedRoms } from '../decompress';
import { Options } from '../options';
import { Patchfile } from '../patch-build/patchfile';
import { randString } from '../random';
import { Random, sample } from '../random';
import { toU32Buffer } from '../util';
import { png } from '../util/png';
import { Color, ColorArg, ColorRandom, COLORS } from './color';
import { recolorImage } from '../image';
import fs from 'fs';
import { enableModelOotLinkChild, enableModelOotLinkAdult } from './model';
import { BufferPath } from './type';
import { randomizeMusic } from './music';

export { makeCosmetics } from './util';
export { COSMETICS } from './data';
export type { Cosmetics } from './type';

const OBJECTS_TABLE_ADDR = 0x800f8ff8;

export async function cosmeticsAssets(opts: Options) {
  return {
    MASK_TUNIC: await png(opts, 'masks/tunic', 'bitmask'),
  }
}

type Unpromise<T extends Promise<any>> = T extends Promise<infer U> ? U : never;

export type CosmeticsOutput = {
  patch: Patchfile;
  overrides: {[k: string]: Buffer};
};

class CosmeticsPass {
  private vrom: number;
  private random: Random;
  private patch: Patchfile;
  private overrides: {[k: string]: Buffer} = {};
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

  private addNewFile(data: Buffer, compressed = true) {
    const size = (data.length + 0xf) & ~0xf;
    const vrom = this.vrom;
    this.vrom = (this.vrom + size) >>> 0;
    this.patch.addNewFile(vrom, data, compressed);
    return [vrom, (vrom + size) >>> 0];
  }

  private async getPathBuffer(path: BufferPath | null): Promise<Buffer | null> {
    if (path === null) {
      return null;
    }

    if (typeof path === 'string') {
      if (!process.env.BROWSER) {
        return fs.promises.readFile(path);
      } else {
        throw new Error(`Cannot load buffers from path`);
      }
    } else {
      return Buffer.from(path);
    }
  }

  private color(c: Color | number) {
    if (typeof c === 'number') {
      return c;
    } else {
      return COLORS[c].value;
    }
  }

  private colorArg(c: ColorArg, def: Color | number, auto: () => Color | number): number {
    switch (c) {
    case 'default':
      return this.color(def);
    case 'random':
      return sample(this.random, Object.values(COLORS)).value;
    case 'auto':
      return this.color(auto());
    default:
      return COLORS[c].value;
    }
  }

  private colorBufferRGB(color: number) {
    const buffer = Buffer.alloc(3);
    buffer.writeUInt8(color >>> 16, 0);
    buffer.writeUInt8((color >>> 8) & 0xff, 1);
    buffer.writeUInt8(color & 0xff, 2);
    return buffer;
  }

  private patchColorRGB(game: Game, addr: number, color: number) {
    const paddr = this.addresses[game].virtualToPhysical(addr);
    const buffer = Buffer.alloc(3);
    buffer.writeUInt8(color >>> 16, 0);
    buffer.writeUInt8((color >>> 8) & 0xff, 1);
    buffer.writeUInt8(color & 0xff, 2);
    this.patch.addDataPatch(game, paddr, buffer);
  }

  private patchOotTunic(index: number, color: number) {
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

    if (color === this.color(defaultColor))
      return;

    /* Patch the in-game color */
    this.patchColorRGB('oot', (0x800f7ad8 + index * 0x03) >>> 0, color);

    /* Patch the icon */
    const paddr = 0x7fe000 + 0x1000 * index;
    const icon = this.roms.oot.rom.subarray(paddr, paddr + 0x1000);
    const newIcon = recolorImage('rgba32', icon, this.assets.MASK_TUNIC, defaultColorIcon, color);
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

  private patchMmTunic(vrom: number, def: Color, color: number, offsets: number[]) {
    if (color === this.color(def)) {
      return;
    }
    const rgba = (((color << 8) | 0xff) >>> 0);

    /* Patch the in-game color */
    const buffer = Buffer.alloc(4);
    buffer.writeUInt32BE(rgba, 0);
    for (const offset of offsets) {
      this.patch.addDataPatch('mm', vrom + offset, buffer);
    }
  }

  private patchMmTunicDeku(color: number) {
    if (color === this.color('kokirigreen')) {
      return;
    }
    const paddr = 0x011a5000;
    const lutOff = 0x4090;
    const lut = Buffer.from(this.roms.mm.rom.subarray(paddr + lutOff, paddr + lutOff + 16 * 2));
    const newLut = recolorImage('rgba16', lut, null, 0x00b439, color);
    this.patch.addDataPatch('mm', paddr + lutOff, newLut);
  }

  private patchMmTunicGoron(color: number) {
    if (color === this.color('kokirigreen')) {
      return;
    }
    const paddr = 0x0117a000;
    const texOff = 0x2780;
    const tex = Buffer.from(this.roms.mm.rom.subarray(paddr + texOff, paddr + texOff + 8 * 16 * 2));
    const newTex = recolorImage('rgba16', tex, null, 0x00b439, color);
    this.patch.addDataPatch('mm', paddr + texOff, newTex);
    const texOff2 = 0xceb8;
    const tex2 = Buffer.from(this.roms.mm.rom.subarray(paddr + texOff2, paddr + texOff2 + 8 * 16 * 2));
    const newTex2 = recolorImage('rgba16', tex2, null, 0x00b439, color);
    this.patch.addDataPatch('mm', paddr + texOff2, newTex2);
  }

  private patchMmTunicZora(color: number) {
    if (color === this.color('kokirigreen')) {
      return;
    }
    const paddr = 0x01192000;
    const lutOff = 0x5000 + 9 * 16 * 2;
    const lut = Buffer.from(this.roms.mm.rom.subarray(paddr + lutOff, paddr + lutOff + 16 * 2 * 2));
    const newLut = recolorImage('rgba16', lut, null, 0x00b439, color);
    this.patch.addDataPatch('mm', paddr + lutOff, newLut);
    const lutOff2 = 0xc578 + 9 * 16 * 2;
    const lut2 = Buffer.from(this.roms.mm.rom.subarray(paddr + lutOff2, paddr + lutOff2 + 16 * 2 * 2));
    const newLut2 = recolorImage('rgba16', lut2, null, 0x00b439, color);
    this.patch.addDataPatch('mm', paddr + lutOff2, newLut2);

    const texOff = 0x10228 + 7 * 16 * 2;
    const tex = Buffer.from(this.roms.mm.rom.subarray(paddr + texOff, paddr + texOff + (32 - 7) * 16 * 2));
    const newTex = recolorImage('rgba16', tex, null, 0x00b439, color);
    this.patch.addDataPatch('mm', paddr + texOff, newTex);

    /* Fin */
    const paddr2 = 0x0108b000;
    const texOff2 = 0x700b0 + 7 * 16 * 2;
    const tex2 = Buffer.from(this.roms.mm.rom.subarray(paddr2 + texOff2, paddr2 + texOff2 + (32 - 7) * 16 * 2));
    const newTex2 = recolorImage('rgba16', tex2, null, 0x00b439, color);
    this.patch.addDataPatch('mm', paddr2 + texOff2, newTex2);
  }

  private patchMmTunicFierceDeity(color: number) {
    if (color === this.color('white')) {
      return;
    }
    const paddr = 0x0114d000;
    const lutOff = 0x8128;
    const lut = Buffer.from(this.roms.mm.rom.subarray(paddr + lutOff, paddr + lutOff + 16 * 2));
    const newLut = recolorImage('rgba16', lut, null, 0xffffff, color);
    this.patch.addDataPatch('mm', paddr + lutOff, newLut);
  }

  async run(): Promise<CosmeticsOutput> {
    const { cosmetics } = this.opts;
    this.assets = await cosmeticsAssets(this.opts);

    /* Resolve colors */
    const colorOotTunicKokiri = this.colorArg(cosmetics.ootTunicKokiri, 'kokirigreen', () => 'kokirigreen');
    const colorOotTunicGoron = this.colorArg(cosmetics.ootTunicGoron, 'goronred', () => 'goronred');
    const colorOotTunicZora = this.colorArg(cosmetics.ootTunicZora, 'zorablue', () => 'zorablue');
    const colorMmTunicHuman = this.colorArg(cosmetics.mmTunicHuman, 'kokirigreen', () => colorOotTunicKokiri);
    const colorMmTunicDeku = this.colorArg(cosmetics.mmTunicDeku, 'kokirigreen', () => colorMmTunicHuman);
    const colorMmTunicGoron = this.colorArg(cosmetics.mmTunicGoron, 'kokirigreen', () => colorMmTunicHuman);
    const colorMmTunicZora = this.colorArg(cosmetics.mmTunicZora, 'kokirigreen', () => colorMmTunicHuman);
    const colorMmTunicFierceDeity = this.colorArg(cosmetics.mmTunicFierceDeity, 'white', () => 'white');

    /* OoT tunics */
    this.patchOotTunic(0, colorOotTunicKokiri);
    this.patchOotTunic(1, colorOotTunicGoron);
    this.patchOotTunic(2, colorOotTunicZora);

    /* MM tunics */
    this.patchMmTunic(0x0115b000, 'kokirigreen', colorMmTunicHuman, [0xb39c, 0xb8c4, 0xbdcc, 0xbfa4, 0xc064, 0xc66c, 0xcae4, 0xcd1c, 0xcea4, 0xd1ec, 0xd374]);
    this.patchMmTunicDeku(colorMmTunicDeku);
    this.patchMmTunicGoron(colorMmTunicGoron);
    this.patchMmTunicZora(colorMmTunicZora);
    this.patchMmTunicFierceDeity(colorMmTunicFierceDeity);

    this.overrides['MM_COLOR_TUNIC_KOKIRI'] = this.colorBufferRGB(colorMmTunicHuman);
    this.overrides['MM_COLOR_TUNIC_GORON'] = this.colorBufferRGB(colorOotTunicGoron);
    this.overrides['MM_COLOR_TUNIC_ZORA'] = this.colorBufferRGB(colorOotTunicZora);

    /* Models */
    await this.patchOotChildModel();
    await this.patchOotAdultModel();

    /* Custom Music */
    if (cosmetics.music) {
      const data = await this.getPathBuffer(cosmetics.music);
      if (data)
        await randomizeMusic(this.roms, this.patch, this.random, data);
    }

    return { patch: this.patch, overrides: this.overrides };
  }
}

export async function cosmetics(opts: Options, addresses: GameAddresses, roms: DecompressedRoms): Promise<CosmeticsOutput> {
  const pass = new CosmeticsPass(opts, addresses, roms)
  return pass.run();
}
