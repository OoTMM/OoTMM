import { Buffer } from 'buffer';

import { build } from "./build";
import { codegen } from "./codegen";
import { custom } from "./custom";
import { decompressGames } from "./decompress";
import { logic } from './logic';
import { Monitor, MonitorCallbacks } from './monitor';
import { Options } from "./options";
import { pack } from "./pack";
import { buildPatchfile } from './patch-build';

type GeneratorOutput = {
  rom: Buffer;
  log: string;
  hash: string;
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

    /* Run logic */
    const logicResult = logic(this.monitor, this.opts);

    const dma = { oot: roms.oot.dma, mm: roms.mm.dma };
    const patchfile = buildPatchfile({
      monitor: this.monitor,
      roms,
      build: buildResult,
      custom: customData,
      logic: logicResult,
      settings: this.opts.settings,
    });
    const packedRom = await pack(this.monitor, roms, patchfile);
    const hash = "XXX";
    const log = "TODO";
    //const rom = await pack(this.monitor, roms, buildResult, customData, this.opts);
    //const { log, hash } = randomize(this.monitor, rom, this.opts);
    return { rom: packedRom, log, hash };
  }
};
