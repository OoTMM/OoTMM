import { codegen } from './codegen';
import { custom } from './custom';
import { decompressGames } from './decompress';
import { logic } from './logic';
import { Monitor, MonitorCallbacks } from './monitor';
import { Options } from './options';
import { pack } from './pack';
import { buildPatchfiles } from './patch-build';
import { Patchfile } from './patch-build/patchfile';
import { makeAddresses } from './addresses';
import { applyRandomSettings } from './settings/random';
import { makeResolver } from './resolve';

export type GeneratorOutputFile = {
  name: string;
  data: string | Buffer;
  mime: string;
};

export type GeneratorOutput = {
  hash: string;
  files: GeneratorOutputFile[];
};

function makeFile(opts: { name?: string, data: string | Buffer, mime: string, hash?: string, world?: number, ext: string }): GeneratorOutputFile {
  let name = 'OoTMM';

  if (opts.name) {
    name = name + '-' + opts.name;
  }

  if (opts.hash) {
    name = name + '-' + opts.hash;
  }

  if (opts.world !== undefined) {
    name = name + '-P' + opts.world;
  }

  name = name + '.' + opts.ext;

  return { name, data: opts.data, mime: opts.mime };
}

export class Generator {
  private monitor: Monitor;
  private oot: Buffer;
  private mm: Buffer;
  private opts: Options;

  constructor(
    oot: Buffer | ArrayBuffer,
    mm: Buffer | ArrayBuffer,
    opts: Options,
    monitorCallbacks: MonitorCallbacks,
  ) {
    this.oot = Buffer.from(oot);
    this.mm = Buffer.from(mm);
    this.opts = opts;
    this.monitor = new Monitor(monitorCallbacks, opts.debug);
  }

  async run(): Promise<GeneratorOutput> {
    const startTime = performance.now();
    const roms = await decompressGames(this.monitor, { oot: this.oot, mm: this.mm });
    const addresses = makeAddresses(roms);
    let patchfiles: Patchfile[];
    let log: string | null = null;

    /* Apply random settings (if enabled) */
    this.opts.settings = applyRandomSettings(this.opts.random, this.opts.settings);

    if (!this.opts.patch) {
      if (!process.env.BROWSER) {
        await codegen(this.monitor);
      }
      const patchfile = new Patchfile;
      await custom(this.opts, this.monitor, roms, patchfile);
      const resolver = await makeResolver(this.opts);

      /* Run logic */
      const logicResult = logic(this.monitor, this.opts);
      patchfile.setHash(logicResult.hash);
      patchfiles = await buildPatchfiles({
        opts: this.opts,
        patch: patchfile,
        monitor: this.monitor,
        roms,
        addresses,
        resolver,
        logic: logicResult,
        settings: this.opts.settings,
      });
      log = logicResult.log;
    } else {
      const patchfile = new Patchfile;
      await patchfile.deserialize(Buffer.from(this.opts.patch));
      patchfiles = [patchfile];
    }

    const hash = patchfiles[0].hash;
    const hashFileName = this.opts.debug ? undefined : hash;
    const files: GeneratorOutputFile[] = [];
    const playerNumber = (id: number) => patchfiles.length === 1 ? undefined : id + 1;

    /* Build ROM(s) */
    if (patchfiles.length === 1 || this.opts.debug) {
      for (let i = 0; i < patchfiles.length; i++) {
        const { rom, cosmeticLog } = await pack({ opts: this.opts, monitor: this.monitor, roms, patchfile: patchfiles[i], addresses });
        files.push(makeFile({ hash: hashFileName, data: rom, mime: 'application/octet-stream', world: playerNumber(i), ext: 'z64' }));
        if (cosmeticLog) {
          files.push(makeFile({ name: 'Cosmetics', hash: hashFileName, data: cosmeticLog, mime: 'text/plain', world: playerNumber(i), ext: 'txt' }));
        }
      }
    }

    /* Build patch(es) */
    if (!this.opts.patch) {
      for (let i = 0; i < patchfiles.length; i++) {
        const data = await patchfiles[i].serialize();
        files.push(makeFile({ name: 'Patch', hash: hashFileName, data, mime: 'application/octet-stream', world: playerNumber(i), ext: 'ootmm' }));
      }
    }

    if (log) {
      files.push(makeFile({ name: 'Spoiler', hash: hashFileName, data: Buffer.from(log), mime: 'text/plain', ext: 'txt' }));
    }

    const endTime = performance.now();
    const durationMs = Math.round(endTime - startTime);
    const duration = durationMs / 1000;
    this.monitor.debug(`Generation took ${duration.toFixed(3)}s`);

    return { hash, files };
  }
};
