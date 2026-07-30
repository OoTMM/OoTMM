import type { Options, OptionsInput, Settings, MonitorCallbacks } from '@ootmm/core';

import { Monitor, Random, applyRandomSettings, options } from '@ootmm/core';
import { logic } from '@ootmm/logic';
import { codegen } from './codegen';
import { custom } from './custom';
import { decompressGames } from './decompress';
import { pack } from './pack';
import { buildPatchfiles } from './patch-build';
import { Patchfile } from './patch-build/patchfile';
import { makeAddresses } from './addresses';
import { makeSpoilerLog } from './spoiler';

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

/* For settings that require randomization always */
async function randomVariants(settings: Settings) {
  const random = new Random();
  const randBytes = crypto.getRandomValues(new Uint8Array(32));
  const randBytesAsHex = Array.from(randBytes).map(b => b.toString(16).padStart(2, '0')).join('');
  await random.seed(randBytesAsHex);

  if (settings.startingAge === 'random') {
    if (random.next() & 1) {
      settings.startingAge = 'adult';
    } else {
      settings.startingAge = 'child';
    }
  }
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
    let patchfiles: Patchfile[];
    const files: GeneratorOutputFile[] = [];

    /* Apply random settings (if enabled) */
    if (this.opts.mode === 'random') {
      this.opts.settings = await applyRandomSettings(this.opts.random, this.opts.settings);
    } else if (this.opts.mode === 'create') {
      await randomVariants(this.opts.settings);
    }

    if (this.opts.mode !== 'patch') {
      if (!process.env.__IS_BROWSER__) {
        await codegen(this.monitor);
      }
      const patchfile = new Patchfile;
      await custom(this.monitor, roms, patchfile);

      /* Run logic */
      const logicResult = await logic(this.monitor, this.opts);
      patchfile.setHash(logicResult.hash);
      patchfiles = await buildPatchfiles({
        patch: patchfile,
        monitor: this.monitor,
        roms,
        addresses,
        logic: logicResult,
        settings: this.opts.settings,
      });

      const hash = patchfiles[0].hash;
      const hashFileName = isDev ? undefined : hash;

      /* Generate spoiler log */
      if (this.opts.settings.generateSpoilerLog) {
        const log = makeSpoilerLog(logicResult, this.opts);
        files.push(makeFile({ name: 'Spoiler', hash: hashFileName, data: log, mime: 'text/plain', ext: 'txt' }));
      }

      /* Prepare multi stuff */
      const sessionIds: Uint8Array[] = [];
      const sessionSecrets: Uint8Array[] = [];
      for (let i = 0; i < this.opts.settings.teams; ++i) {
        const sessionId = new Uint8Array(16);
        const sessionSecret = new Uint8Array(8);
        crypto.getRandomValues(sessionId);
        crypto.getRandomValues(sessionSecret);
        sessionIds.push(sessionId);
        sessionSecrets.push(sessionSecret);
      }

      /* Emit the patchfiles */
      const playerNumber = (id: number) => patchfiles.length === 1 ? undefined : id + 1;
      const teamNumber = (id: number) => this.opts.settings.teams === 1 ? undefined : id + 1;

      for (let i = 0; i < patchfiles.length; i++) {
        const patchfile = patchfiles[i];
        patchfile.meta['mode'] = this.opts.settings.mode;
        patchfile.meta['worldId'] = (i + 1);
        for (let team = 0; team < this.opts.settings.teams; ++team) {
          const sessionId = sessionIds[team];
          const sessionSecret = sessionSecrets[team];
          patchfile.meta['sessionId'] = sessionId.toHex();
          patchfile.meta['sessionSecret'] = sessionSecret.toHex();
          patchfile.addSymbolPatch('MULTI_SESSION_ID', sessionId);
          patchfile.addSymbolPatch('MULTI_SESSION_SECRET', sessionSecret);
          const data = await patchfile.serialize();
          files.push(makeFile({ name: 'Patch', hash: hashFileName, data, mime: 'application/vnd.ootmm.patch+zip', world: playerNumber(i), team: teamNumber(team), ext: 'ootmm' }));
        }
      }
    } else {
      if (!this.opts.patch) {
        throw new Error('Patch mode requires a patch file');
      }
      const patchfile = new Patchfile;
      await patchfile.deserialize(this.opts.patch);
      patchfiles = [patchfile];
    }

    /* Build ROM(s) */
    if ((patchfiles.length === 1 && this.opts.settings.teams === 1) || this.opts.patch) {
      for (let i = 0; i < patchfiles.length; i++) {
        const hashFileName = isDev ? undefined : patchfiles[0].hash;
        const { rom, cosmeticLog } = await pack({ opts: this.opts, monitor: this.monitor, roms, patchfile: patchfiles[i], addresses });
        files.push(makeFile({ hash: hashFileName, data: rom, mime: 'application/octet-stream', ext: 'z64' }));
        if (cosmeticLog) {
          files.push(makeFile({ name: 'Cosmetics', hash: hashFileName, data: cosmeticLog, mime: 'text/plain', ext: 'txt' }));
        }
      }
    }

    const endTime = performance.now();
    const durationMs = Math.round(endTime - startTime);
    const duration = durationMs / 1000;
    this.monitor.debug(`Generation took ${duration.toFixed(3)}s`);

    return { hash: patchfiles[0].hash, files };
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
