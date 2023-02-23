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
import { Patchfile } from './patch-build/patchfile';

type GeneratorOutput = {
  rom: Buffer;
  log: string | null;
  hash: string;
  patch: Buffer | null;
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
    let patchfile: Patchfile;
    let log: string | null = null;

    if (!this.opts.patch) {
      if (!process.env.ROLLUP) {
        await codegen(this.monitor);
      }
      const customData = await custom(this.monitor, roms);
      const buildResult = await build(this.opts);
      /* Run logic */
      const logicResult = logic(this.monitor, this.opts);
      patchfile = buildPatchfile({
        monitor: this.monitor,
        roms,
        build: buildResult,
        custom: customData,
        logic: logicResult,
        settings: this.opts.settings,
      });
      log = logicResult.log;
    } else {
      patchfile = new Patchfile(this.opts.patch);
    }

    const packedRom = await pack(this.monitor, roms, patchfile);
    let patch: Buffer | null = null;
    if (!this.opts.patch) {
      patch = patchfile.toBuffer();
    }
    return { rom: packedRom, log, hash: patchfile.hash, patch };
  }
};
