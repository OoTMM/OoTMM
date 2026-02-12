import { codegen } from './codegen';
import { custom } from './custom';
import { decompressGames } from './decompress';
import { logic } from './logic';
import { Monitor, MonitorCallbacks } from './monitor';
import { options, Options, OptionsInput } from './options';
import { pack } from './pack';
import { buildPatchfiles } from './patch-build';
import { Patchfile } from './patch-build/patchfile';
import { makeAddresses } from './addresses';
import { applyRandomSettings } from './settings/random';

const env = process.env.NODE_ENV || 'development';
const isDev = (env !== 'production');

export type GeneratorOutputFile = {
  name: string;
  data: string | Uint8Array;
  mime: string;
};

export type GeneratorOutput = {
  hash: string;
  files: GeneratorOutputFile[];
};

function makeFile(opts: { name?: string, data: string | Uint8Array, mime: string, hash?: string, team?: number, world?: number, ext: string }): GeneratorOutputFile {
  let name = 'OoTMM';

  if (opts.name) {
    name = name + '-' + opts.name;
  }

  if (opts.hash) {
    name = name + '-' + opts.hash;
  }

  if (opts.team !== undefined) {
    name = name + '-Team' + opts.team;
  }

  if (opts.world !== undefined) {
    name = name + '-Player' + opts.world;
  }

  name = name + '.' + opts.ext;

  return { name, data: opts.data, mime: opts.mime };
}

export class Generator {
  private monitor: Monitor;
  private oot: Uint8Array;
  private mm: Uint8Array;
  private opts: Options;

  constructor(
    oot: Uint8Array,
    mm: Uint8Array,
    opts: Options,
    monitorCallbacks: MonitorCallbacks,
  ) {
    this.oot = oot;
    this.mm = mm;
    this.opts = opts;
    this.monitor = new Monitor(monitorCallbacks, (process.env.NODE_ENV !== 'production'));
  }

  async run(): Promise<GeneratorOutput> {
    const startTime = performance.now();
    const roms = await decompressGames(this.monitor, { oot: this.oot, mm: this.mm });
    const addresses = makeAddresses(roms);
    const teamId = 0; // TODO: support multiple teams
    let patchfiles: Patchfile[];
    let log: string | null = null;

    /* Apply random settings (if enabled) */
    if (this.opts.mode === 'random') {
      this.opts.settings = await applyRandomSettings(this.opts.random, this.opts.settings);
    }

    if (this.opts.mode !== 'patch') {
      if (!process.env.__IS_BROWSER__) {
        await codegen(this.monitor);
      }
      const patchfile = new Patchfile;
      await custom(this.monitor, roms, patchfile);

      /* Prepare multi stuff */
      const sessionId = new Uint8Array(16);
      const sessionSecret = new Uint8Array(4);
      if (this.opts.settings.mode !== 'single') {
        crypto.getRandomValues(sessionId);
        crypto.getRandomValues(sessionSecret);
        sessionId[15] = 0;
      }

      /* Run logic */
      const logicResult = await logic(this.monitor, this.opts);
      patchfile.setHash(logicResult.hash);
      patchfiles = await buildPatchfiles({
        opts: this.opts,
        patch: patchfile,
        monitor: this.monitor,
        roms,
        addresses,
        logic: logicResult,
        settings: this.opts.settings,
      });
      log = logicResult.log;
      for (let i = 0; i < patchfiles.length; ++i) {
        patchfiles[i].addSymbolPatch('MULTI_SESSION_ID', sessionId);
        patchfiles[i].addSymbolPatch('MULTI_SESSION_SECRET', sessionSecret);
      }
    } else {
      if (!this.opts.patch) {
        throw new Error('Patch mode requires a patch file');
      }
      const patchfile = new Patchfile;
      await patchfile.deserialize(this.opts.patch);
      patchfiles = [patchfile];
    }

    const hash = patchfiles[0].hash;
    const hashFileName = isDev ? undefined : hash;
    const files: GeneratorOutputFile[] = [];
    const playerNumber = (id: number) => patchfiles.length === 1 ? undefined : id + 1;

    /* Build ROM(s) */
    if (patchfiles.length === 1 || this.opts.patch || isDev) {
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
        const patchfile = patchfiles[i];
        const data = await patchfiles[i].serialize();
        files.push(makeFile({ name: 'Patch', hash: hashFileName, data, mime: 'application/octet-stream', world: playerNumber(i), ext: 'ootmm' }));
      }
    }

    if (log) {
      files.push(makeFile({ name: 'Spoiler', hash: hashFileName, data: log, mime: 'text/plain', ext: 'txt' }));
    }

    const endTime = performance.now();
    const durationMs = Math.round(endTime - startTime);
    const duration = durationMs / 1000;
    this.monitor.debug(`Generation took ${duration.toFixed(3)}s`);

    return { hash, files };
  }
};

export type GeneratorParams = {
  oot: Uint8Array,
  mm: Uint8Array,
  opts?: OptionsInput,
  monitor?: MonitorCallbacks
};

export const generate = (params: GeneratorParams): Generator => {
  const opts = options(params.opts || {});
  return new Generator(params.oot, params.mm, opts, params.monitor || {});
};
