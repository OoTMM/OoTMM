import { GameAddresses } from '../addresses';
import { GAMES } from '../config';
import { recolorImage } from '../image';
import { Options } from '../options';
import { Random, randString, sample } from '../random';
import { RomBuilder } from '../rom-builder';
import { png } from '../util/png';
import { COLORS, Color, ColorArg } from './color';

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

function patchSymbol(meta: any, name: string, builder: RomBuilder, buffer: Buffer) {
  for (const game of GAMES) {
    const syms = meta[game] || {};
    const addr = syms[name];
    if (!addr) continue;
    const payloadFile = builder.fileByNameRequired(`${game}/payload`);
    buffer.copy(payloadFile.data, addr);
  }
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

function patchOotShieldMirror(builder: RomBuilder, assets: Assets, color: number) {
  const buffer = colorBufferRGB(color);
  const fileObjectLinkBoy = builder.fileByNameRequired('oot/objects/object_link_boy');
  const fileIconItemStatic = builder.fileByNameRequired('oot/icon_item_static');
  const fileGi = builder.fileByNameRequired('oot/objects/object_gi_shield_3');

  /* Patch the field model */
  for (const off of [0x21270, 0x21768, 0x24278, 0x26560, 0x26980, 0x28dd0]) {
    buffer.copy(fileObjectLinkBoy.data, off + 4);
  }

  /* Patch icon */
  const iconOffset = 0x40 * 0x1000;
  const icon = fileIconItemStatic.data.subarray(iconOffset, iconOffset + 0x1000);
  const newIcon = recolorImage('rgba32', icon, assets.MASK_OOT_SHIELD_MIRROR, 0xff1313, color);
  newIcon.copy(icon);

  /* Patch gi */
  const primColor = colorBufferRGB(color);
  const envColor = colorBufferRGB(brightness(color, 0.2));
  primColor.copy(fileGi.data, 0xfc8 + 4);
  envColor.copy(fileGi.data, 0xfd0 + 4);
}

function patchOotTunic(builder: RomBuilder, assets: Assets, index: number, color: number) {
  const defaultColorIcons: number[] = [
    0x005a00,
    0x7a0000,
    0x0020b7,
  ];
  const defaultColorIcon = defaultColorIcons[index];
  const fileOotCode = builder.fileByNameRequired('oot/code');
  const fileOotIconItemStatic = builder.fileByNameRequired('oot/icon_item_static');
  const colorBuffer = colorBufferRGB(color);

  /* Patch the in-game color */
  colorBuffer.copy(fileOotCode.data, 0xe6a38 + index * 3);

  /* Patch the icon */
  const iconIndex = 0x41 + index;
  const iconOffset = 0x1000 * iconIndex;
  const icon = fileOotIconItemStatic.data.subarray(iconOffset, iconOffset + 0x1000);
  const newIcon = recolorImage('rgba32', icon, assets.MASK_TUNIC, defaultColorIcon, color);
  newIcon.copy(icon);

  /* Patch the GI */
  if (index !== 0) {
    const file = builder.fileByNameRequired('oot/objects/object_gi_clothes');
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

export async function cosmetics(opts: Options, addresses: GameAddresses, builder: RomBuilder, meta: any) {
  const c = opts.cosmetics;

  /* Create a random number generator */
  const random = new Random();
  random.seed(randString());

  /* Fetch assets */
  const assets = await cosmeticsAssets(opts);

  /* Resolve colors */
  const colorOotTunicKokiri = resolveColor(random, c.ootTunicKokiri);
  const colorOotTunicGoron = resolveColor(random, c.ootTunicGoron);
  const colorOotTunicZora = resolveColor(random, c.ootTunicZora);
  const colorMmTunicHuman = resolveColor(random, c.mmTunicHuman, () => colorOotTunicKokiri);
  const colorOotShieldMirror = resolveColor(random, c.ootShieldMirror);

  /* Patch human tunics */
  if (colorOotTunicKokiri !== null) {
    patchOotTunic(builder, assets, 0, colorOotTunicKokiri);
  }

  if (colorOotTunicGoron !== null) {
    patchOotTunic(builder, assets, 1, colorOotTunicGoron);
    patchSymbol(meta, 'MM_COLOR_TUNIC_GORON', builder, colorBufferRGB(colorOotTunicGoron));
  }

  if (colorOotTunicZora !== null) {
    patchOotTunic(builder, assets, 2, colorOotTunicZora);
    patchSymbol(meta, 'MM_COLOR_TUNIC_ZORA', builder, colorBufferRGB(colorOotTunicZora));
  }

  if (colorMmTunicHuman !== null) {
    patchSymbol(meta, 'MM_COLOR_TUNIC_KOKIRI', builder, colorBufferRGB(colorMmTunicHuman));
  }

  /* Patch OoT Mirror Shield */
  if (colorOotShieldMirror !== null) patchOotShieldMirror(builder, assets, colorOotShieldMirror);
}
