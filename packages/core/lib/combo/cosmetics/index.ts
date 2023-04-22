import { GameAddresses } from '../addresses';
import { Game } from '../config';
import { Options } from '../options';
import { Patchfile } from '../patch-build/patchfile';
import { randString } from '../random';
import { Random, sample } from '../random';
import { Color, ColorRandom, COLORS } from './color';

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

class CosmeticsPass {
  private random: Random;
  private patch: Patchfile;

  constructor(
    private opts: Options,
    private addresses: GameAddresses,
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

  run(): Patchfile {
    /* OoT tunics */
    this.patchColorRGB('oot', (0x800f7ad8 + 0x00) >>> 0, this.color(this.opts.cosmetics.ootTunicKokiri));
    this.patchColorRGB('oot', (0x800f7ad8 + 0x03) >>> 0, this.color(this.opts.cosmetics.ootTunicGoron));
    this.patchColorRGB('oot', (0x800f7ad8 + 0x06) >>> 0, this.color(this.opts.cosmetics.ootTunicZora));

    return this.patch;
  }
}

export function cosmetics(opts: Options, addresses: GameAddresses): Patchfile {
  return new CosmeticsPass(opts, addresses).run();
}
