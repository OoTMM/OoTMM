import { Buffer } from 'buffer';

import { build } from "./build";
import { codegen } from "./codegen";
import { custom } from "./custom";
import { decompressGames } from "./decompress";
import { logic } from './logic';
import { Monitor, MonitorCallbacks } from './monitor';
import { Options } from "./options";
import { pack } from "./pack";
import { buildPatchfiles } from './patch-build';
import { Patchfile } from './patch-build/patchfile';
import { makeAddresses } from './addresses';
import { cosmetics } from './cosmetics';
import { applyRandomSettings } from './settings/random';
import { exportSettings } from './settings/string';

export type GeneratorOutput = {
  hash: string;
  log: string | null;
  roms: Buffer[];
  patches: Buffer[];
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
    const addresses = makeAddresses(roms);
    let patchfiles: Patchfile[];
    let log: string | null = null;

    /* Apply random settings (if enabled) */
    this.opts.settings = applyRandomSettings(this.opts.random, this.opts.settings);

    if (!this.opts.patch) {
      if (!process.env.ROLLUP) {
        await codegen(this.monitor);
      }
      const patchfile = new Patchfile;
      await custom(this.opts, this.monitor, roms, patchfile);
      const buildResult = await build(this.opts);
      /* Run logic */
      const logicResult = logic(this.monitor, this.opts);
      patchfile.setHash(logicResult.hash);
      patchfiles = buildPatchfiles({
        patch: patchfile,
        monitor: this.monitor,
        roms,
        addresses,
        build: buildResult,
        logic: logicResult,
        settings: this.opts.settings,
      });
      log = logicResult.log;
    } else {
      patchfiles = [new Patchfile(this.opts.patch)];
    }

    const cosmeticsPatchfile = await cosmetics(this.opts, addresses, roms);

    /* Build ROM(s) */
    let packedRoms: Buffer[] = [];
    if (patchfiles.length === 1 || this.opts.debug) {
      packedRoms = await Promise.all(patchfiles.map(x => pack(this.monitor, roms, [x, cosmeticsPatchfile])));
    }

    /* Build patch(es) */
    let patches: Buffer[] = [];
    if (!this.opts.patch) {
      patches = patchfiles.map(x => x.toBuffer());
    }

    return { roms: packedRoms, log, hash: patchfiles[0].hash, patches };
  }
};
