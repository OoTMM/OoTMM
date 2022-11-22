import { build } from "./build";
import { codegen } from "./codegen";
import { custom } from "./custom";
import { decompressGames } from "./decompress";
import { Options } from "./options";
import { pack } from "./pack";
import { randomize } from "./randomizer";

type GeneratorOutput = {
  rom: Buffer;
  log: string;
};

export class Generator {
  constructor(
    private oot: string | Buffer,
    private mm: string | Buffer,
    private opts: Options
  ) {
  }

  async run(): Promise<GeneratorOutput> {
    const roms = await decompressGames({ oot: this.oot as string, mm: this.mm as string });
    if (!process.env.ROLLUP) {
      await codegen();
    }
    const customData = await custom(roms);
    const buildResult = await build(this.opts);
    const rom = await pack(roms, buildResult, customData, this.opts);
    const log = await randomize(rom, this.opts);
    return { rom, log };
  }
};
