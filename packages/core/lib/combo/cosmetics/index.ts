import { GameAddresses } from '../addresses';
import { Game } from '../config';
import { DecompressedRoms } from '../decompress';
import { Options } from '../options';
import { Patchfile } from '../patch-build/patchfile';
import { randString } from '../random';
import { Random, sample } from '../random';
import { png } from '../util/png';
import { Color, ColorRandom, COLORS } from './color';
import { recolorImage } from './image';

export type Cosmetics = {
  ootTunicKokiri: ColorRandom;
  ootTunicGoron: ColorRandom;
  ootTunicZora: ColorRandom;
};

export const COSMETIC_NAMES: {[k in keyof Cosmetics]: string} = {
  ootTunicKokiri: 'OoT Kokiri Tunic',
  ootTunicGoron: 'OoT Goron Tunic',
  ootTunicZora: 'OoT Zora Tunic',
};

export const DEFAULT_COSMETICS: Cosmetics = {
  ootTunicKokiri: 'kokirigreen',
  ootTunicGoron: 'goronred',
  ootTunicZora: 'zorablue',
};

export function makeCosmetics(data: Partial<Cosmetics>) {
  return {
    ...DEFAULT_COSMETICS,
    ...data,
  };
}

export async function cosmeticsAssets() {
  return {
    MASK_TUNIC: await png('masks/tunic', 'bitmask'),
  }
}

type Unpromise<T extends Promise<any>> = T extends Promise<infer U> ? U : never;

class CosmeticsPass {
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
    this.patch = new Patchfile('');
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
    this.patch.addPatch(game, paddr, buffer);
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
    this.patch.addPatch('oot', paddr, newIcon);
  }

  async run(): Promise<Patchfile> {
    this.assets = await cosmeticsAssets();

    /* OoT tunics */
    this.patchOotTunic(0, this.opts.cosmetics.ootTunicKokiri);
    this.patchOotTunic(1, this.opts.cosmetics.ootTunicGoron);
    this.patchOotTunic(2, this.opts.cosmetics.ootTunicZora);

    return this.patch;
  }
}

export async function cosmetics(opts: Options, addresses: GameAddresses, roms: DecompressedRoms): Promise<Patchfile> {
  const pass = new CosmeticsPass(opts, addresses, roms)
  return pass.run();
}
