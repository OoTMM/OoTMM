import fs from 'fs';

import { GameAddresses } from '../addresses';
import { GAMES } from '../config';
import { recolorImage } from '../image';
import { Options } from '../options';
import { Random, randString, sample } from '../random';
import { RomBuilder } from '../rom-builder';
import { png } from '../util/png';
import { COLORS, ColorArg } from './color';
import { BufferPath } from './type';
import { toU32Buffer } from '../util';
import { enableModelOotLinkAdult, enableModelOotLinkChild } from './model';
import { randomizeMusic } from './music';

export { makeCosmetics } from './util';
export { COSMETICS } from './data';
export type { Cosmetics } from './type';

export async function cosmeticsAssets(opts: Options) {
  return {
    MASK_TUNIC: await png(opts, 'masks/tunic', 'bitmask'),
    MASK_OOT_SHIELD_MIRROR: await png(opts, 'masks/oot_shield_mirror', 'bitmask'),
  }
}

type Unpromise<T extends Promise<any>> = T extends Promise<infer U> ? U : never;
type Assets = Unpromise<ReturnType<typeof cosmeticsAssets>>;

function colorBufferRGB(color: number) {
  const buffer = Buffer.alloc(3);
  buffer.writeUInt8(color >>> 16, 0);
  buffer.writeUInt8((color >>> 8) & 0xff, 1);
  buffer.writeUInt8(color & 0xff, 2);
  return buffer;
}

function brightness(color: number, bright: number): number {
  const r = (color >>> 16) * bright;
  const g = ((color >>> 8) & 0xff) * bright;
  const b = (color & 0xff) * bright;
  return (r & 0xff) << 16 | (g & 0xff) << 8 | (b & 0xff);
}

function resolveColor(random: Random, c: ColorArg, auto?: () => number | null): number | null {
  switch (c) {
  case 'default':
    return null;
  case 'random':
    return sample(random, Object.values(COLORS)).value;
  case 'auto':
    return auto ? auto() : null;
  default:
    return COLORS[c].value;
  }
}

class CosmeticsPass {
  private assetsPromise: Promise<Assets> | null;
  private vrom: number;

  constructor(
    private opts: Options,
    private addresses: GameAddresses,
    private builder: RomBuilder,
    private meta: any,
  ) {
    this.assetsPromise = null;
    this.vrom = 0xc0000000;
  }

  private asset(key: keyof Assets): Promise<Buffer> {
    if (this.assetsPromise === null) {
      this.assetsPromise = cosmeticsAssets(this.opts);
    }
    return this.assetsPromise.then((assets) => assets[key]);
  }

  private patchSymbol(name: string, buffer: Buffer) {
    for (const game of GAMES) {
      const syms = this.meta[game] || {};
      const addr = syms[name];
      if (!addr) continue;
      const payloadFile = this.builder.fileByNameRequired(`${game}/payload`);
      buffer.copy(payloadFile.data, addr);
    }
  }

  private addNewFile(data: Buffer, compressed = true) {
    const size = (data.length + 0xf) & ~0xf;
    const vrom = this.vrom;
    this.vrom = (this.vrom + size) >>> 0;
    this.builder.addFile({ vaddr: vrom, data, type: compressed ? 'compressed' : 'uncompressed', game: 'custom' })
    return [vrom, (vrom + size) >>> 0];
  }

  private async patchMmTunicDeku(color: number) {
    const file = this.builder.fileByNameRequired('mm/objects/object_link_nuts');
    const lutOff = 0x4090;
    const lut = file.data.subarray(lutOff, lutOff + 16 * 2);
    const newLut = recolorImage('rgba16', lut, null, 0x00b439, color);
    newLut.copy(lut);
  }

  private patchMmTunicGoron(color: number) {
    const file = this.builder.fileByNameRequired('mm/objects/object_link_goron');

    const texOff = 0x2780;
    const tex = file.data.subarray(texOff, texOff + 8 * 16 * 2);
    const newTex = recolorImage('rgba16', tex, null, 0x00b439, color);
    newTex.copy(tex);

    const texOff2 = 0xceb8;
    const tex2 = file.data.subarray(texOff2, texOff2 + 8 * 16 * 2);
    const newTex2 = recolorImage('rgba16', tex2, null, 0x00b439, color);
    newTex2.copy(tex2);
  }

  private patchMmTunicZora(color: number) {
    const fileLink = this.builder.fileByNameRequired('mm/objects/object_link_zora');
    const fileKeep = this.builder.fileByNameRequired('mm/objects/gameplay_keep');

    const lutOff = 0x5000 + 9 * 16 * 2;
    const lut = fileLink.data.subarray(lutOff, lutOff + 16 * 2 * 2);
    const newLut = recolorImage('rgba16', lut, null, 0x00b439, color);
    newLut.copy(lut);

    const lutOff2 = 0xc578 + 9 * 16 * 2;
    const lut2 = fileLink.data.subarray(lutOff2, lutOff2 + 16 * 2 * 2);
    const newLut2 = recolorImage('rgba16', lut2, null, 0x00b439, color);
    newLut2.copy(lut2);

    const texOff = 0x10228 + 7 * 16 * 2;
    const tex = fileLink.data.subarray(texOff, texOff + (32 - 7) * 16 * 2);
    const newTex = recolorImage('rgba16', tex, null, 0x00b439, color);
    newTex.copy(tex);

    /* Fin */
    const texOff2 = 0x700b0 + 7 * 16 * 2;
    const tex2 = fileKeep.data.subarray(texOff2, texOff2 + (32 - 7) * 16 * 2);
    const newTex2 = recolorImage('rgba16', tex2, null, 0x00b439, color);
    newTex2.copy(tex2);
  }

  private patchMmTunicFierceDeity(color: number) {
    const file = this.builder.fileByNameRequired('mm/objects/object_link_boy');
    const lutOff = 0x8128;
    const lut = file.data.subarray(lutOff, lutOff + 16 * 2);
    const newLut = recolorImage('rgba16', lut, null, 0xffffff, color);
    newLut.copy(lut);
  }

  private async patchOotTunic(index: number, color: number) {
    const defaultColorIcons: number[] = [
      0x005a00,
      0x7a0000,
      0x0020b7,
    ];
    const defaultColorIcon = defaultColorIcons[index];
    const fileOotCode = this.builder.fileByNameRequired('oot/code');
    const fileOotIconItemStatic = this.builder.fileByNameRequired('oot/icon_item_static');
    const mask = await this.asset('MASK_TUNIC');
    const colorBuffer = colorBufferRGB(color);

    /* Patch the in-game color */
    colorBuffer.copy(fileOotCode.data, 0xe6a38 + index * 3);

    /* Patch the icon */
    const iconIndex = 0x41 + index;
    const iconOffset = 0x1000 * iconIndex;
    const icon = fileOotIconItemStatic.data.subarray(iconOffset, iconOffset + 0x1000);
    const newIcon = recolorImage('rgba32', icon, mask, defaultColorIcon, color);
    newIcon.copy(icon);

    /* Patch the GI */
    if (index !== 0) {
      const file = this.builder.fileByNameRequired('oot/objects/object_gi_clothes');
      let off = 0x14e0 + (index - 1) * 0x20;
      const colorPrim1 = colorBufferRGB(brightness(color, 0.76));
      const colorEnv1 = colorBufferRGB(brightness(color, 0.53));
      const colorPrim2 = colorBuffer;
      const colorEnv2 = colorBufferRGB(brightness(color, 0.59));

      colorPrim1.copy(file.data, off + 0x0c);
      colorEnv1.copy(file.data, off + 0x14);
      colorPrim2.copy(file.data, off + 0x4c);
      colorEnv2.copy(file.data, off + 0x54);
    }
  }

  private async patchOotShieldMirror(color: number) {
    const buffer = colorBufferRGB(color);
    const fileObjectLinkBoy = this.builder.fileByNameRequired('oot/objects/object_link_boy');
    const fileIconItemStatic = this.builder.fileByNameRequired('oot/icon_item_static');
    const fileGi = this.builder.fileByNameRequired('oot/objects/object_gi_shield_3');
    const mask = await this.asset('MASK_OOT_SHIELD_MIRROR');

    /* Patch the field model */
    for (const off of [0x21270, 0x21768, 0x24278, 0x26560, 0x26980, 0x28dd0]) {
      buffer.copy(fileObjectLinkBoy.data, off + 4);
    }

    /* Patch icon */
    const iconOffset = 0x40 * 0x1000;
    const icon = fileIconItemStatic.data.subarray(iconOffset, iconOffset + 0x1000);
    const newIcon = recolorImage('rgba32', icon, mask, 0xff1313, color);
    newIcon.copy(icon);

    /* Patch gi */
    const primColor = colorBufferRGB(color);
    const envColor = colorBufferRGB(brightness(color, 0.2));
    primColor.copy(fileGi.data, 0xfc8 + 4);
    envColor.copy(fileGi.data, 0xfd0 + 4);
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

  private async patchOotChildModel() {
    const model = await this.getPathBuffer(this.opts.cosmetics.modelOotChildLink);
    if (model) {
      const magic = model.indexOf(Buffer.from('MODLOADER64'));
      if (magic === -1) {
        throw new Error('Invalid model file');
      }

      /* Inject the new model */
      const code = this.builder.fileByNameRequired('oot/code');
      const objEntryOffset = 0xe7f58 + 8 * 0x15;
      const obj = this.addNewFile(model);
      const objBuffer = toU32Buffer(obj);
      objBuffer.copy(code.data, objEntryOffset);

      /* Enable the PlayAs hooks */
      const dfAddr = model.indexOf(Buffer.from([0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]));
      enableModelOotLinkChild(this.builder, dfAddr);

      /* Delete the original */
      const original = this.builder.fileByNameRequired('oot/objects/object_link_child');
      original.type = 'dummy';
      original.data = Buffer.alloc(0);
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
      const code = this.builder.fileByNameRequired('oot/code');
      const objEntryOffset = 0xe7f58 + 8 * 0x14;
      const obj = this.addNewFile(model);
      const objBuffer = toU32Buffer(obj);
      objBuffer.copy(code.data, objEntryOffset);

      /* Enable the PlayAs hooks */
      const dfAddr = model.indexOf(Buffer.from([0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]));
      enableModelOotLinkAdult(this.builder, dfAddr);

      /* Delete the original */
      const original = this.builder.fileByNameRequired('oot/objects/object_link_boy');
      original.type = 'dummy';
      original.data = Buffer.alloc(0);
    }
  }

  async run() {
    const c = this.opts.cosmetics;

    /* Create a random number generator */
    const random = new Random();
    random.seed(randString());

    /* Resolve colors */
    const colorOotTunicKokiri = resolveColor(random, c.ootTunicKokiri);
    const colorOotTunicGoron = resolveColor(random, c.ootTunicGoron);
    const colorOotTunicZora = resolveColor(random, c.ootTunicZora);
    const colorMmTunicHuman = resolveColor(random, c.mmTunicHuman, () => colorOotTunicKokiri);
    const colorMmTunicDeku = resolveColor(random, c.mmTunicDeku, () => colorMmTunicHuman);
    const colorMmTunicGoron = resolveColor(random, c.mmTunicGoron, () => colorMmTunicHuman);
    const colorMmTunicZora = resolveColor(random, c.mmTunicZora, () => colorMmTunicHuman);
    const colorMmTunicFierceDeity = resolveColor(random, c.mmTunicFierceDeity);
    const colorOotShieldMirror = resolveColor(random, c.ootShieldMirror);

    /* Patch human tunics */
    if (colorOotTunicKokiri !== null) {
      await this.patchOotTunic(0, colorOotTunicKokiri);
    }

    if (colorOotTunicGoron !== null) {
      await this.patchOotTunic(1, colorOotTunicGoron);
      this.patchSymbol('MM_COLOR_TUNIC_GORON', colorBufferRGB(colorOotTunicGoron));
    }

    if (colorOotTunicZora !== null) {
      await this.patchOotTunic(2, colorOotTunicZora);
      this.patchSymbol('MM_COLOR_TUNIC_ZORA', colorBufferRGB(colorOotTunicZora));
    }

    if (colorMmTunicHuman !== null) {
      this.patchSymbol('MM_COLOR_TUNIC_KOKIRI', colorBufferRGB(colorMmTunicHuman));
    }

    /* Forms */
    if (colorMmTunicDeku !== null) this.patchMmTunicDeku(colorMmTunicDeku);
    if (colorMmTunicGoron !== null) this.patchMmTunicGoron(colorMmTunicGoron);
    if (colorMmTunicZora !== null) this.patchMmTunicZora(colorMmTunicZora);
    if (colorMmTunicFierceDeity !== null) this.patchMmTunicFierceDeity(colorMmTunicFierceDeity);

    /* Patch OoT Mirror Shield */
    if (colorOotShieldMirror !== null) this.patchOotShieldMirror(colorOotShieldMirror);

    /* Patch models */
    this.patchOotChildModel();
    this.patchOotAdultModel();

    /* Custom music */
    if (c.music) {
      const data = await this.getPathBuffer(c.music);
      if (data)
        await randomizeMusic(this.builder, random, data);
    }
  }
}

export async function cosmetics(opts: Options, addresses: GameAddresses, builder: RomBuilder, meta: any) {
  const x = new CosmeticsPass(opts, addresses, builder, meta);
  await x.run();
}
