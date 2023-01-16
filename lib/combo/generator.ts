import { Buffer } from 'buffer';

import { build } from "./build";
import { codegen } from "./codegen";
import { custom } from "./custom";
import { decompressGames } from "./decompress";
import { Monitor, MonitorCallbacks } from './monitor';
import { Options } from "./options";
import { pack } from "./pack";
import { randomize } from "./randomizer";

type GeneratorOutput = {
  rom: Buffer;
  log: string;
};

export class Generator {
  private monitor: Monitor;

  constructor(
    private oot: Buffer,
    private mm: Buffer,
    private opts: Options,
    monitorCallbacks: MonitorCallbacks,
  ) {
    this.monitor = new Monitor(monitorCallbacks, opts.debug);
  }

  async run(): Promise<GeneratorOutput> {
    const roms = await decompressGames(this.monitor, { oot: this.oot, mm: this.mm });
    if (!process.env.ROLLUP) {
      await codegen(this.monitor);
    }
    const customData = await custom(this.monitor, roms);
    const buildResult = await build(this.opts);
    const rom = await pack(this.monitor, roms, buildResult, customData, this.opts);
    const log = randomize(this.monitor, rom, this.opts);
    return { rom, log };
  }
};
