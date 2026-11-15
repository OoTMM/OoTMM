import { EXTRA_COMBAT_PORTS } from './extra-port-data';
import { align, toU32Buffer } from '../util';
import { debugEnemyForms } from './debug-catalog';
import { matchesRomBytes, bufReadU32BE, bufReadU16BE, bufWriteU32BE, bufReadI16BE, bufWriteU16BE } from '../util/buffer';
import CRC32 from 'crc-32';
import { CONFIG } from '../config';
import type { Addresses, GameAddresses } from '../addresses';
import {
  native_extra_oot_sources as extraOotSources,
  native_extra_oot_facts as extraOotPorts,
  native_extra_mm_sources as extraMmSources,
  native_extra_mm_facts as extraMmPorts,
  native_sources as nativeSources,
  generated_native_ports as generatedPorts,
  native_ports as nativePorts,
  native_garo_facts as garoPort,
  keep_assets as facts,
  native_port_keeps as nativePortKeepsFacts,
  animation_audit as animationFacts,
  defeat_respawn as respawnFacts,
} from './enemy-rom-data';
import { SFX_IDS, ENGINE_ACTORS, ENEMY_ACTOR_CATALOG, FILES, ENEMY_SOUNDS_MM, ENEMY_SOUNDS_OOT, type Game } from '@ootmm/data';
import { GI } from '../data';
import type { RomBuilder } from '../rom-builder';
import { enemy_oot_source_corrections as ootSourceCorrectionsFacts } from '../../../../core/src/enemy-data';
import {
  OOT_GAMEPLAY_POLICY,
  scriptedMmSource,
  scriptedMmTargetAllowed,
  standaloneEnemyTransfer,
  assertForeignEnemyExclusive,
  enemyDefinition,
  enemyEncounterCompatible,
  enemyActorSpecies,
  foreignEnemyForms,
  foreignEnemyRuntimeReady,
  nativeEnemyCounterparts,
  additionalForeignEnemyTarget,
  isEnemyEncounter,
  standaloneEnemyTarget,
  type EnemyPlan,
} from '@ootmm/core';
import { CustomObjectsBuilder, type CustomObject } from '../custom/custom-objects-builder';
import type { DecompressedRoms } from '../decompress';
import { Patchfile } from '../patch-build/patchfile';

function engineActorByName(game: Game, name: string) {
  return ENGINE_ACTORS.find(actor => actor.game === game && actor.actor === name);
}

function engineActorById(game: Game, id: number) {
  return ENGINE_ACTORS.find(actor => actor.game === game && actor.actorId === id);
}

function enemyCatalogActor(game: Game, actorId: number) {
  return ENEMY_ACTOR_CATALOG.find(actor => actor.game === game && actor.actorId === actorId);
}

type OverlayLayout = {
  textSize: number;
  dataSize: number;
  rodataSize: number;
  bssSize: number;
  relocationOffset: number;
  relocationSize: number;
  relocations: readonly { section: number; type: number; offset: number }[];
};

export function readOverlayLayout(data: Uint8Array): OverlayLayout {
  if (data.length < 0x1c || data.length % 4) throw new Error('Invalid actor overlay length');
  const relocationSize = bufReadU32BE(data, data.length - 4);
  if (relocationSize < 0x18 || relocationSize > data.length || relocationSize % 4)
    throw new Error('Invalid actor overlay relocation footer');
  const relocationOffset = data.length - relocationSize;
  const [textSize, dataSize, rodataSize, bssSize, count] = Array.from({ length: 5 }, (_, i) => bufReadU32BE(data, relocationOffset + i * 4));
  if (textSize % 4 || dataSize % 4 || rodataSize % 4 || textSize + dataSize + rodataSize !== relocationOffset)
    throw new Error('Actor overlay sections do not match relocation footer');
  if (count > (relocationSize - 0x18) / 4) throw new Error('Truncated actor overlay relocations');
  if (data.length + bssSize > 0x800000) throw new Error('Actor overlay exceeds RDRAM');
  const sections = [0, 0, textSize, textSize + dataSize];
  const sizes = [0, textSize, dataSize, rodataSize];
  const relocations = [];
  const visited = new Set<number>();
  for (let i = 0; i < count; ++i) {
    const value = bufReadU32BE(data, relocationOffset + 0x14 + i * 4);
    const section = value >>> 30;
    const type = (value >>> 24) & 0x3f;
    const relative = value & 0xffffff;
    if (!section || relative % 4 || relative + 4 > sizes[section]) throw new Error('Invalid actor relocation address');
    if (![2, 4, 5, 6].includes(type)) throw new Error(`Unsupported actor relocation type: ${type}`);
    const offset = sections[section] + relative;
    if (visited.has(offset)) throw new Error('Duplicate actor overlay relocation');
    visited.add(offset);
    relocations.push({ section, type, offset });
  }
  return { textSize, dataSize, rodataSize, bssSize, relocationOffset, relocationSize, relocations };
}

const ENEMY_ARCHIVE_START = 0xd0000000;

const ENEMY_ARCHIVE_END = 0xd1000000;

export function archiveEnemyPort(patch: Patchfile, targetGame: Game, actorId: number, source: Uint8Array): number {
  if (!Number.isInteger(actorId) || actorId < 0 || actorId >= 0x400) throw new Error('Invalid native enemy port ID');
  return archiveEnemyImage(patch, `enemy/port/${targetGame}/${actorId.toString(16)}`, source);
}

function archiveEnemyObject(patch: Patchfile, sourceGame: Game, objectId: number, source: Uint8Array): number {
  if (!Number.isInteger(objectId) || objectId < 1 || objectId >= 0x1000 || !source.length) throw new Error('Invalid pristine enemy object');
  return archiveEnemyImage(patch, `enemy/object/${sourceGame}/${objectId.toString(16)}`, source, false);
}

function archiveEnemyImage(patch: Patchfile, name: string, source: Uint8Array, overlay = true): number {
  if (overlay) readOverlayLayout(source);
  const existing = patch.newFiles.find(f => f.name === name);
  if (existing) {
    if (existing.vrom < ENEMY_ARCHIVE_START || existing.vrom + existing.data.length > ENEMY_ARCHIVE_END ||
      existing.data.length !== source.length || !existing.data.every((value, i) => value === source[i]))
      throw new Error(`Conflicting pristine enemy overlay: ${name}`);
    return existing.vrom;
  }
  let vrom = ENEMY_ARCHIVE_START;
  const size = align(source.length, 16);
  const files = [...patch.newFiles].sort((a, b) => a.vrom - b.vrom);
  for (const file of files) {
    const end = file.vrom + align(file.data.length, 16);
    if (end <= vrom) continue;
    if (file.vrom >= vrom + size) break;
    vrom = end;
  }
  if (vrom + size > ENEMY_ARCHIVE_END) throw new Error('Pristine enemy overlay archive exhausted');
  patch.addNewFile({ name, vrom, data: new Uint8Array(source), compressed: source.length % 16 === 0 });
  return vrom;
}

const ACTOR_HEADER_SIZE = { oot: 0x13c, mm: 0x144 } as const;

type ForeignEnemyChildLayout = {
  sourceActorId: number;
  sourceVrom: number;
  sourceVram: number;
  profileOffset: number;
  fileSize: number;
  crc32: number;
  instanceSize: number;
};

function validateForeignEnemyChildLayout(source: Game, child: ForeignEnemyChildLayout) {
  if (!child || Object.keys(child).length !== 7 ||
    (['sourceActorId', 'sourceVrom', 'sourceVram', 'profileOffset', 'fileSize', 'crc32', 'instanceSize'] as const).some(key => !Number.isSafeInteger(child[key])) ||
    child.sourceActorId < 0 || child.sourceActorId >= 0x400 || child.sourceVrom < 1 ||
    child.sourceVram < 0x80800000 || child.sourceVram >= 0x81000000 ||
    child.fileSize < 32 || child.fileSize > 0x1000000 || child.sourceVram + child.fileSize > 0x81000000 ||
    child.profileOffset < 0 || child.profileOffset + 32 > child.fileSize || child.profileOffset % 4 ||
    child.crc32 < 0 || child.crc32 > 0xffffffff || child.instanceSize < ACTOR_HEADER_SIZE[source] || child.instanceSize > 0xffff)
    throw new Error('Invalid foreign child layout evidence');
}

function verifyForeignEnemyChildSource(source: Game, rom: Uint8Array, child: ForeignEnemyChildLayout) {
  validateForeignEnemyChildLayout(source, child);
  const config = CONFIG[source], entry = config.actorsOvlAddr + child.sourceActorId * 32;
  if (child.sourceActorId >= config.actorsOvlCount || entry + 32 > rom.length || child.sourceVrom + child.fileSize > rom.length ||
    bufReadU32BE(rom, entry) !== child.sourceVrom || bufReadU32BE(rom, entry + 4) !== child.sourceVrom + child.fileSize ||
    bufReadU32BE(rom, entry + 8) !== child.sourceVram || bufReadU32BE(rom, entry + 20) !== child.sourceVram + child.profileOffset)
    throw new Error('Foreign child source actor table mismatch');
  const overlay = rom.subarray(child.sourceVrom, child.sourceVrom + child.fileSize), layout = readOverlayLayout(overlay);
  if ((CRC32.buf(overlay) >>> 0) !== child.crc32 || child.profileOffset < layout.textSize || child.profileOffset + 32 > layout.relocationOffset ||
    bufReadU16BE(overlay, child.profileOffset) !== donorProfileId(source, child) || bufReadU32BE(overlay, child.profileOffset + 12) !== child.instanceSize)
    throw new Error('Foreign child source profile revision mismatch');
}

export function installedEnemyDependency(patch: Patchfile, game: Game, addresses: Pick<Addresses, 'fileFromROM'>,
  id: number, category: number, name: string, profileId = id) {
  const matches = patch.newFiles.filter(f => f.name === name);
  if (matches.length !== 1 || patch.removedFiles.includes(name) || patch.patches[name]?.length)
    throw new Error('Missing or modified receiving dependency overlay: ' + name);
  const overlay = matches[0];
  const entry = addresses.fileFromROM(CONFIG[game].actorsOvlAddr + id * 0x20);
  const rows = patch.patches[entry.name]?.filter(p => p.addr < entry.offset + 24 && p.addr + p.data.length > entry.offset);
  const vram = overlay.vram?.[game];
  if (!vram || rows?.length !== 1 || rows[0].addr !== entry.offset || rows[0].data.length !== 24)
    throw new Error('Missing installed receiving dependency entry');
  const row = rows[0].data, layout = readOverlayLayout(overlay.data), profile = bufReadU32BE(row, 20) - vram[0];
  if (bufReadU32BE(row, 0) !== overlay.vrom || bufReadU32BE(row, 4) !== overlay.vrom + overlay.data.length ||
    bufReadU32BE(row, 8) !== vram[0] || bufReadU32BE(row, 12) !== vram[1] || bufReadU32BE(row, 16) !== 0 ||
    vram[1] - vram[0] !== overlay.data.length + layout.bssSize ||
    profile % 4 || profile < layout.textSize || profile + 32 > layout.textSize + layout.dataSize ||
    bufReadU16BE(overlay.data, profile) !== profileId || overlay.data[profile + 2] !== category)
    throw new Error('Wrong receiving dependency overlay or profile');
  return { overlay, vram, profile, layout };
}

function validateForeignEnemyRootProfile(manifest: ForeignEnemyManifest, image: Uint8Array): void {
  const offset = manifest.profileOffset;
  if (!Number.isInteger(offset) || offset < 0 || offset % 4 || offset + 32 > image.length)
    throw new Error('Invalid foreign enemy root profile bounds');
  if (manifest.patches.length) throw new Error('Native enemy profiles cannot contain donor code patches');
  const category = (bufReadU32BE(image, offset) >>> 8) & 0xff;
  if (category !== 5 && category !== 9)
    throw new Error('Foreign enemy root profile must use ENEMY or BOSS category');
}

export function nativeCombatProfileCategory(category: number, actorId: number, params: number): boolean {
  return category === 5 || (category === 9 && actorId === 0x113 && (params === 0xff82 || params === 0xff83));
}

type ForeignEnemyManifest = {
  sourceVrom: number;
  sourceCrc32: number;
  fileSize: number;
  vramStart: number;
  memorySize: number;
  profileOffset: number;
  patches: readonly { offset: number; expected: number; replacement: number }[];
};

function serializeForeignEnemyManifest(manifest: ForeignEnemyManifest): Uint8Array {
  const fields = [manifest.sourceVrom, manifest.fileSize, manifest.vramStart, manifest.memorySize, manifest.profileOffset, manifest.sourceCrc32];
  if (fields.some(v => !Number.isInteger(v) || v < 0 || v > 0xffffffff) ||
    !manifest.sourceVrom || manifest.fileSize < 0x20 || manifest.fileSize % 4 || manifest.memorySize < manifest.fileSize || manifest.memorySize > 0x800000 ||
    manifest.profileOffset % 4 || manifest.profileOffset + 0x20 > manifest.fileSize || manifest.patches.length !== 0)
    throw new Error('Invalid foreign enemy manifest');
  const out = new Uint8Array(28);
  [manifest.sourceVrom, manifest.fileSize, manifest.vramStart, manifest.memorySize, manifest.profileOffset, 0, manifest.sourceCrc32]
    .forEach((value, i) => bufWriteU32BE(out, i * 4, value));
  return out;
}

export type ForeignEnemyProgram = {
  randomDrops?: readonly EnemyRandomDropSite[];
  version: 1;
  sourceGame: Game;
  sourceActorId: number;
  sourceObjectId: number;
  manifest: Omit<ForeignEnemyManifest, 'patches'>;
  actorIds: readonly (readonly [source: number, target: number])[];
  runtimeImports: readonly { offset: number; expected: number; symbol: string; addend: number; part: 'word' | 'jump' | 'hi16' | 'lo16' }[];
  romData?: readonly ForeignEnemyRomData[];
  foreignChildren?: readonly ForeignEnemyChildLayout[];
};

export type NativeEnemyPort = {
  version: 1;
  prefix: string;
  objectVrom: number;
  objectSize: number;
  objectCrc32: number;
  damageEffects?: boolean;
  requiredRuntimeExports?: readonly string[];
  randomDropRows?: readonly number[];
  randomDrops?: readonly EnemyRandomDropSite[];
  nativeChildren?: readonly number[];
  sourceChildren?: readonly number[];
  nativeObject?: 1;
  overlay?: string;
  defeat?: boolean;
};

function instantiateNativeEnemyPort(patch: Patchfile, target: Game, program: ForeignEnemyProgram,
  port: NativeEnemyPort, sourceRom: Uint8Array, targetRom?: Uint8Array) {
  if (target === program.sourceGame || !['oot', 'mm'].includes(program.sourceGame) || port.version !== 1 ||
    !/^FOREIGN_ENEMY_PORT_[A-Z0-9_]+$/.test(port.prefix) ||
    !patch.symbols[target].has('FOREIGN_ENEMY_NATIVE_PORTS_V1'))
    throw new Error('Missing native combat-port capability');
  for (const child of port.nativeChildren ?? []) {
    const entry = CONFIG.mm.actorsOvlAddr + child * 32;
    if (target !== 'mm' || program.sourceGame !== 'oot' || child !== 3 || !targetRom || entry + 32 > targetRom.length ||
      bufReadU32BE(targetRom, entry) !== 0xcdf760 || bufReadU32BE(targetRom, entry + 4) !== 0xcdfd80 ||
      bufReadU32BE(targetRom, entry + 8) !== 0x80865370 || bufReadU32BE(targetRom, entry + 20) !== 0x808658e0 ||
      CRC32.buf(targetRom.subarray(0xcdf760, 0xcdfd80)) >>> 0 !== 0x2e9cde39 ||
      CRC32.buf(targetRom.subarray(0xb3c000 + 0x800bbcec - 0x800a5ac0, 0xb3c000 + 0x800bbcec - 0x800a5ac0 + 0xc0)) >>> 0 !== 0x7d673975)
      throw new Error('Native combat-port body-fragment dependency revision mismatch');
  }
  verifyNativeEnemyPortSource(program, port, sourceRom);
  const keepExports = verifyNativePortKeeps(program, port, sourceRom, targetRom);
  for (const key of [...(port.requiredRuntimeExports ?? []), ...keepExports])
    if (patch.symbols[target].get(key)?.length !== 1) throw new Error('Missing native enemy adapter: ' + key);
  return instantiateNativeEnemyOverlay(patch, target, program, port);
}

export function verifyNativeEnemyPortSource(program: ForeignEnemyProgram, port: NativeEnemyPort, sourceRom: Uint8Array) {
  if (!['oot', 'mm'].includes(program.sourceGame) || port.version !== 1) throw new Error('Unsupported native combat-port donor');
  const original = program.manifest;
  const actorEntry = CONFIG[program.sourceGame].actorsOvlAddr + program.sourceActorId * 32;
  const objectEntry = CONFIG[program.sourceGame].objectTableAddr + program.sourceObjectId * 8;
  if (actorEntry + 32 > sourceRom.length || objectEntry + 8 > sourceRom.length ||
    bufReadU32BE(sourceRom, actorEntry) !== original.sourceVrom ||
    bufReadU32BE(sourceRom, actorEntry + 4) !== original.sourceVrom + original.fileSize ||
    bufReadU32BE(sourceRom, actorEntry + 8) !== original.vramStart ||
    bufReadU32BE(sourceRom, actorEntry + 20) !== original.vramStart + original.profileOffset ||
    bufReadU32BE(sourceRom, objectEntry) !== port.objectVrom ||
    bufReadU32BE(sourceRom, objectEntry + 4) !== port.objectVrom + port.objectSize)
    throw new Error('Native combat-port donor table identity mismatch');
  if (original.sourceVrom + original.fileSize > sourceRom.length ||
    CRC32.buf(sourceRom.subarray(original.sourceVrom, original.sourceVrom + original.fileSize)) >>> 0 !== original.sourceCrc32)
    throw new Error('Native combat-port donor overlay revision mismatch');
  const source = sourceRom.subarray(original.sourceVrom, original.sourceVrom + original.fileSize);
  const sourceLayout = readOverlayLayout(source);
  if (original.profileOffset < sourceLayout.textSize || original.profileOffset + 32 > sourceLayout.relocationOffset ||
    bufReadU16BE(source, original.profileOffset) !== donorProfileId(program.sourceGame, {
      sourceActorId: program.sourceActorId, sourceVrom: original.sourceVrom, sourceVram: original.vramStart,
      profileOffset: original.profileOffset, fileSize: original.fileSize, crc32: original.sourceCrc32,
      instanceSize: bufReadU32BE(source, original.profileOffset + 12),
    }) ||
    bufReadU16BE(source, original.profileOffset + 8) !== program.sourceObjectId)
    throw new Error('Native combat-port donor profile mismatch');
  if (![port.objectVrom, port.objectSize, port.objectCrc32].every(Number.isSafeInteger) ||
    port.objectVrom < 1 || port.objectSize < 1 || port.objectCrc32 < 0 || port.objectCrc32 > 0xffffffff ||
    port.objectVrom + port.objectSize > sourceRom.length ||
    CRC32.buf(sourceRom.subarray(port.objectVrom, port.objectVrom + port.objectSize)) >>> 0 !== port.objectCrc32)
    throw new Error('Native combat-port model revision mismatch');
}

function instantiateNativeEnemyOverlay(patch: Patchfile, target: Game,
  program: ForeignEnemyProgram, port: NativeEnemyPort) {
  const stem = port.overlay?.replace(/^Native_Enemy_/, '');
  if (!stem || !/^(oot|mm)_[a-z0-9_]+$/.test(stem) || !stem.startsWith(program.sourceGame + '_') ||
    port.prefix !== 'FOREIGN_ENEMY_PORT_' + stem.toUpperCase())
    throw new Error('Invalid generated native enemy overlay identity');
  const name = `ovl/${target}/${target}_ovl_${port.overlay}.zovlx`;
  const files = patch.newFiles.filter(f => f.name === name);
  const file = files.length === 1 ? files[0] : undefined;
  const span = file?.vram?.[target];
  if (!file || !span || file.data.length % 16 || file.vrom < ENEMY_ARCHIVE_START || file.vrom + file.data.length > ENEMY_ARCHIVE_END ||
    patch.removedFiles.includes(name) || patch.patches[name]?.length ||
    span[0] < 0x80800000 || span[1] <= span[0])
    throw new Error('Missing or modified generated native enemy overlay: ' + name);
  const layout = readOverlayLayout(file.data);
  if (span[1] - span[0] !== file.data.length + layout.bssSize)
    throw new Error('Generated native enemy overlay extent mismatch');
  const symbol = (suffix: string, size: number, section: 'text' | 'data') => {
    const key = port.prefix + '_' + suffix, rows = patch.symbols[target].get(key);
    const offset = (rows?.[0] ?? 0) - span[0];
    if (rows?.length !== 1 || patch.symPatches.has(key) || offset % 4 || offset < 0 ||
      offset + size > (section === 'text' ? layout.textSize : layout.relocationOffset) ||
      (section === 'data' && offset < layout.textSize))
      throw new Error('Invalid generated native enemy symbol: ' + key);
    return offset;
  };
  const profile = symbol('PROFILE', 32, 'data');
  const actorId = 0xc00 + program.sourceActorId, objectId = port.nativeObject ?? (0x1000 | program.sourceObjectId);
  const size = bufReadU32BE(file.data, profile + 12);
  if (bufReadU16BE(file.data, profile) !== actorId || bufReadU16BE(file.data, profile + 8) !== objectId ||
    file.data[profile + 2] >= 12 || size < (target === 'oot' ? 0x13c : 0x144) || size > 0xffff || size % 4)
    throw new Error('Generated native enemy profile layout mismatch');
  for (const [i, suffix] of ['INIT', 'DESTROY', 'UPDATE', 'DRAW'].entries()) {
    const callback = bufReadU32BE(file.data, profile + 16 + i * 4);
    if (!callback && (suffix === 'DESTROY' || suffix === 'DRAW')) continue;
    if (callback !== span[0] + symbol(suffix, 4, 'text'))
      throw new Error('Generated native enemy callback mismatch');
  }
  const origin = symbol('ORIGIN', 20, 'data');
  const expected = [0x4e505431, program.sourceGame === 'oot' ? 0 : 1,
    program.sourceActorId, program.sourceObjectId, program.manifest.sourceCrc32];
  if (!expected.every((word, i) => bufReadU32BE(file.data, origin + i * 4) === word))
    throw new Error('Generated native enemy source provenance mismatch');
  const manifest: ForeignEnemyManifest = {
    sourceVrom: file.vrom, fileSize: file.data.length,
    memorySize: span[1] - span[0], sourceCrc32: CRC32.buf(file.data) >>> 0,
    vramStart: span[0], profileOffset: profile, patches: []
  };
  let nativeDefeat: readonly [number, number, number] | undefined;
  if (port.defeat) {
    const at = symbol('DEFEAT', 12, 'data');
    const field = bufReadU32BE(file.data, at), from = bufReadU32BE(file.data, at + 4), to = bufReadU32BE(file.data, at + 8);
    if (field % 4 || field < (target === 'oot' ? 0x13c : 0x144) || field + 4 > size ||
      from === to || (from | to) % 4 || from < span[0] || to < span[0] ||
      from >= span[0] + layout.textSize || to >= span[0] + layout.textSize)
      throw new Error('Invalid native enemy defeat transition');
    nativeDefeat = [field, from - span[0], to - span[0]];
  }
  return { manifest, actorId, objectId, nativeDefeat };
}

const garoProgram = () => ({ ...nativeSources.programs['mm-garo-native.json'], romData: [MM_DEATH_FLAME_PALETTE] });

const GARO_SOURCE_SOUNDS = SFX_IDS.mm;

const garoSounds = garoPort.sourceSounds.map(id => {
  const entry = Object.entries(GARO_SOURCE_SOUNDS).find(([, value]) => value === id);
  if (!entry) throw new Error('Missing verified Garo source sound: ' + id.toString(16));
  return entry[0];
});

const tail = nativeSources.programs['oot-en-tp.json'];

const larva = nativeSources.programs['oot-en-goma.json'];

const badBat = nativeSources.programs['mm-en-bat.json'];

const torchSlug = nativeSources.programs['oot-en-bw.json'];

const anubisTag = nativeSources.programs['oot-en-anubice-tag.json'];

const anubisBody = nativeSources.programs['oot-en-anubice.json'];

const anubisFire = nativeSources.programs['oot-en-anubice-fire.json'];

const hiploop = nativeSources.programs['mm-en-pp.json'];

const desbreko = nativeSources.programs['mm-en-pr.json'];

const desbrekoYoung = nativeSources.programs['mm-en-prz.json'];

const dragonfly = nativeSources.programs['mm-en-grasshopper.json'];

const eeno = nativeSources.programs['mm-en-snowman.json'];

const bioBaba = nativeSources.programs['mm-boss-05.json'];

const realBombchu = nativeSources.programs['mm-en-rat.json'];

const snapper = nativeSources.programs['mm-en-kame.json'];

const bee = nativeSources.programs['mm-en-bee.json'];

const babyDodongo = nativeSources.programs['oot-en-dodojr.json'];

const biri = nativeSources.programs['oot-en-bili.json'];

const bari = nativeSources.programs['oot-en-vali.json'];

const groundStinger = nativeSources.programs['oot-en-eiyer.json'];

const waterStinger = nativeSources.programs['oot-en-weiyer.json'];

const spike = nativeSources.programs['oot-en-ny.json'];

const flareCore = nativeSources.programs['oot-en-fw.json'];

const flareDancer = nativeSources.programs['oot-en-fd.json'];

const flareFire = nativeSources.programs['oot-en-fd-fire.json'];

const skullKidNeedle = nativeSources.programs['oot-en-skjneedle.json'];

const nejiron = nativeSources.programs['mm-en-baguo.json'];

const boe = nativeSources.programs['mm-en-mkk.json'];

const slime = nativeSources.programs['mm-en-slime.json'];

const iceBlock = nativeSources.programs['mm-obj-iceblock.json'];

function bundledForeignEnemies(): ForeignEnemyArtifacts {
  type PendingPort = Omit<ForeignEnemyRuntimeMetadata, 'nativePort'> & { nativePort?: NativeEnemyPort };
  const actors = { oot: new Map<number, PendingPort>(), mm: new Map<number, PendingPort>() };
  const add = (program: ForeignEnemyProgram, soulGi: number, mainKeep: boolean, sounds: string[], options: Pick<ForeignEnemyRuntimeMetadata, 'subKeep'> = {}) => {
    const game = program.sourceGame === 'oot' ? 'mm' : 'oot';
    actors[game].set(program.sourceActorId, { program, soulGi, mainKeep, sounds, ...options, sourceDamage: true, sourceAudio: true });
  };
  add(slime as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_CHUCHU.index, true, [
    'NA_SE_EN_COMMON_FREEZE', 'NA_SE_EN_SLIME_BREAK', 'NA_SE_EN_SLIME_DAMAGE', 'NA_SE_EN_SLIME_DEAD',
    'NA_SE_EN_SLIME_DEFENCE', 'NA_SE_EN_SLIME_JUMP', 'NA_SE_EN_SLIME_SURFACE', 'NA_SE_EV_ICE_FREEZE',
  ]);
  add(iceBlock as unknown as ForeignEnemyProgram, 0, false, [
    'NA_SE_EV_BLOCK_BOUND', 'NA_SE_EV_DIVE_INTO_WATER_L', 'NA_SE_PL_SLIP_ICE_LEVEL',
  ]);
  add(tail as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_TAILPASARN.index, false, [
    'NA_SE_EN_GOMA_JR_FREEZE', 'NA_SE_EN_TAIL_CRY', 'NA_SE_EN_TAIL_DEAD',
    'NA_SE_EN_TAIL_FLY',
  ]);
  add(larva as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_GOHMA_LARVA.index, false, [
    'NA_SE_EN_EXTINCT', 'NA_SE_EN_GOMA_BJR_CRY', 'NA_SE_EN_GOMA_BJR_DAM1',
    'NA_SE_EN_GOMA_BJR_DAM2', 'NA_SE_EN_GOMA_BJR_DEAD', 'NA_SE_EN_GOMA_BJR_EGG1',
    'NA_SE_EN_GOMA_BJR_EGG2', 'NA_SE_EN_GOMA_BJR_FREEZE', 'NA_SE_EN_GOMA_BJR_LAND',
    'NA_SE_EN_GOMA_BJR_LAND2', 'NA_SE_EN_GOMA_BJR_WALK', 'NA_SE_EN_GOMA_EGG1',
    'NA_SE_EN_GOMA_EGG2', 'NA_SE_EN_GOMA_JR_CRY', 'NA_SE_EN_GOMA_JR_DAM1',
    'NA_SE_EN_GOMA_JR_DAM2', 'NA_SE_EN_GOMA_JR_DEAD', 'NA_SE_EN_GOMA_JR_FREEZE',
    'NA_SE_EN_GOMA_JR_LAND', 'NA_SE_EN_GOMA_JR_LAND2', 'NA_SE_EN_GOMA_JR_WALK',
  ], { subKeep: 'dungeon' });
  add(badBat as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_BAD_BAT.index, true, [
    'NA_SE_EN_COMMON_FREEZE', 'NA_SE_EN_EXTINCT', 'NA_SE_EN_FFLY_ATTACK',
    'NA_SE_EN_FFLY_DEAD', 'NA_SE_EN_FFLY_FLY', 'NA_SE_EV_ICE_FREEZE',
  ]);
  add(torchSlug as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_TORCH_SLUG.index, true, [
    'NA_SE_EN_BUBLEWALK_AIM', 'NA_SE_EN_BUBLEWALK_DAMAGE', 'NA_SE_EN_BUBLEWALK_DEAD',
    'NA_SE_EN_BUBLEWALK_REVERSE', 'NA_SE_EN_BUBLEWALK_WALK', 'NA_SE_EN_DODO_M_GND',
    'NA_SE_EN_GOMA_JR_FREEZE', 'NA_SE_EN_STAL_JUMP', 'NA_SE_PL_BODY_HIT',
  ]);
  add(anubisTag as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_ANUBIS.index, true, [

  ]);
  add(anubisBody as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_ANUBIS.index, true, [
    'NA_SE_EN_ANUBIS_DEAD', 'NA_SE_EN_NUTS_CUTBODY',
  ]);
  add(anubisFire as unknown as ForeignEnemyProgram, 0, true, [
    'NA_SE_EN_ANUBIS_FIRE', 'NA_SE_EN_ANUBIS_FIREBOMB', 'NA_SE_IT_SHIELD_REFLECT_SW',
  ]);
  add(hiploop as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_HIPLOOP.index, true, [
    'NA_SE_EN_COMMON_FREEZE', 'NA_SE_EN_HIPLOOP_DAMAGE', 'NA_SE_EN_HIPLOOP_DEAD',
    'NA_SE_EN_HIPLOOP_FOOT', 'NA_SE_EN_HIPLOOP_FOOTSTEP', 'NA_SE_EN_HIPLOOP_MASC_OFF',
    'NA_SE_EN_HIPLOOP_PAUSE', 'NA_SE_EN_HIPLOOP_RUN', 'NA_SE_EV_BOMB_DROP_WATER',
    'NA_SE_IT_SHIELD_BOUND',
  ]);
  add(desbreko as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_SKULLFISH.index, true, [
    'NA_SE_EN_BUBLEWALK_DAMAGE', 'NA_SE_EN_BUBLEWALK_DEAD',
  ]);
  add(desbrekoYoung as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_SKULLFISH.index, true, [
    'NA_SE_EN_BUBLEWALK_DEAD',
  ]);
  add(dragonfly as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_DRAGONFLY.index, true, [
    'NA_SE_EN_BATTA_ATTACK', 'NA_SE_EN_BATTA_DAMAGE', 'NA_SE_EN_BATTA_DEAD',
    'NA_SE_EN_BATTA_FLY', 'NA_SE_EN_COMMON_EXTINCT_LEV', 'NA_SE_EV_BOMB_DROP_WATER',
  ]);
  add(eeno as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_EENO.index, true, [
    'NA_SE_EN_COMMON_FREEZE', 'NA_SE_EN_YMAJIN_DAMAGE', 'NA_SE_EN_YMAJIN_DEAD_BREAK',
    'NA_SE_EN_YMAJIN_HIDE', 'NA_SE_EN_YMAJIN_HOLD_SNOW', 'NA_SE_EN_YMAJIN_MINI_DAMAGE',
    'NA_SE_EN_YMAJIN_MINI_HOLD', 'NA_SE_EN_YMAJIN_MINI_MOVE', 'NA_SE_EN_YMAJIN_MINI_THROW',
    'NA_SE_EN_YMAJIN_MOVE', 'NA_SE_EN_YMAJIN_SPLIT', 'NA_SE_EN_YMAJIN_SURFACE',
    'NA_SE_EN_YMAJIN_THROW', 'NA_SE_EN_YMAJIN_UNITE', 'NA_SE_EV_SMALL_SNOWBALL_BROKEN',
    'NA_SE_EV_SNOWBALL_BROKEN', 'NA_SE_IT_REFLECTION_WOOD',
  ]);
  add(bioBaba as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_BIO_BABA.index, true, [
    'NA_SE_EN_MIZUBABA1_ATTACK', 'NA_SE_EN_MIZUBABA1_DAMAGE', 'NA_SE_EN_MIZUBABA1_MOUTH',
    'NA_SE_EN_MIZUBABA2_ATTACK', 'NA_SE_EN_MIZUBABA2_DAMAGE', 'NA_SE_EN_MIZUBABA2_VOICE',
    'NA_SE_EN_MIZUBABA2_WALK', 'NA_SE_EN_MIZUBABA_DEAD', 'NA_SE_EN_MIZUBABA_TRANSFORM',
    'NA_SE_EV_ICE_BROKEN', 'NA_SE_EV_OUT_OF_WATER',
  ]);
  add(realBombchu as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_REAL_BOMBCHU.index, true, [
    'NA_SE_EN_BOMCHU_AIM', 'NA_SE_EN_BOMCHU_RUN', 'NA_SE_EN_BOMCHU_VOICE',
    'NA_SE_EN_BOMCHU_WALK', 'NA_SE_EN_COMMON_FREEZE',
  ]);
  add(snapper as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_SNAPPER.index, true, [
    'NA_SE_EN_COMMON_EXTINCT_LEV', 'NA_SE_EN_COMMON_FREEZE', 'NA_SE_EN_HIPLOOP_LAND',
    'NA_SE_EN_PAMET_CUTTER_OFF', 'NA_SE_EN_PAMET_CUTTER_ON', 'NA_SE_EN_PAMET_DAMAGE',
    'NA_SE_EN_PAMET_DEAD', 'NA_SE_EN_PAMET_REVERSE', 'NA_SE_EN_PAMET_ROAR',
    'NA_SE_EN_PAMET_ROLL', 'NA_SE_EN_PAMET_VOICE', 'NA_SE_EN_PAMET_WAKEUP',
    'NA_SE_EN_PAMET_WALK', 'NA_SE_EV_ICE_FREEZE',
  ]);
  add(bee as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_GIANT_BEE.index, false, [
    'NA_SE_EN_BEE_FLY', 'NA_SE_EN_CUTBODY', 'NA_SE_EN_EXTINCT',
  ]);
  add(babyDodongo as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_BABY_DODONGO.index, false, [
    'NA_SE_EN_DODO_K_DRINK', 'NA_SE_EN_DODO_M_CRY', 'NA_SE_EN_DODO_M_DEAD',
    'NA_SE_EN_DODO_M_DOWN', 'NA_SE_EN_DODO_M_EAT', 'NA_SE_EN_DODO_M_GND',
    'NA_SE_EN_DODO_M_MOVE', 'NA_SE_EN_DODO_M_UP', 'NA_SE_EN_GOMA_JR_FREEZE',
    'NA_SE_IT_BOMB_EXPLOSION',
  ]);
  add(biri as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_BIRI.index, false, [
    'NA_SE_EN_BIRI_BUBLE', 'NA_SE_EN_BIRI_DEAD', 'NA_SE_EN_BIRI_FLY',
    'NA_SE_EN_BIRI_JUMP', 'NA_SE_EN_BIRI_SPARK', 'NA_SE_EN_DODO_M_GND',
    'NA_SE_EN_GOMA_JR_FREEZE',
  ]);
  add(bari as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_BARI.index, false, [
    'NA_SE_EN_BARI_DAMAGE', 'NA_SE_EN_BARI_DEAD', 'NA_SE_EN_BARI_ROLL',
    'NA_SE_EN_BARI_SPLIT', 'NA_SE_EN_BIRI_SPARK', 'NA_SE_EN_DODO_M_GND',
    'NA_SE_EN_GOMA_JR_FREEZE',
  ]);
  add(groundStinger as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_STINGER.index, false, [
    'NA_SE_EN_DODO_M_GND', 'NA_SE_EN_EIER_DAMAGE', 'NA_SE_EN_EIER_DEAD',
    'NA_SE_EN_EIER_FLUTTER', 'NA_SE_EN_EIER_FLY', 'NA_SE_EN_GOMA_JR_FREEZE',
    'NA_SE_EN_OCTAROCK_JUMP', 'NA_SE_EN_OCTAROCK_SINK',
  ]);
  add(waterStinger as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_STINGER.index, false, [
    'NA_SE_EN_DODO_M_GND', 'NA_SE_EN_EIER_ATTACK', 'NA_SE_EN_EIER_CRY',
    'NA_SE_EN_EIER_DEAD', 'NA_SE_EN_EIER_FLUTTER', 'NA_SE_EN_GOMA_JR_FREEZE',
    'NA_SE_EN_OCTAROCK_JUMP', 'NA_SE_EN_OCTAROCK_SINK',
  ]);
  add(spike as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_SPIKE.index, false, [
    'NA_SE_EN_DODO_M_GND', 'NA_SE_EN_NYU_DEAD', 'NA_SE_EN_NYU_HIT_STOP',
    'NA_SE_EN_NYU_MOVE',
  ]);
  add(flareCore as unknown as ForeignEnemyProgram, 0, true, [
    'NA_SE_EN_DODO_M_GND', 'NA_SE_EN_FLAME_MAN_DAMAGE', 'NA_SE_EN_FLAME_MAN_RUN',
    'NA_SE_EN_FLAME_MAN_SLIDE', 'NA_SE_EN_FLAME_MAN_SURP', 'NA_SE_EN_STAL_JUMP',
  ]);
  add(flareDancer as unknown as ForeignEnemyProgram, GI.OOT_SOUL_ENEMY_FLARE_DANCER.index, true, [
    'NA_SE_EN_FLAME_DAMAGE', 'NA_SE_EN_FLAME_FIRE_ATTACK', 'NA_SE_EN_FLAME_KICK',
    'NA_SE_EN_FLAME_LAUGH', 'NA_SE_EN_FLAME_ROLL', 'NA_SE_EN_FLAME_RUN',
    'NA_SE_PL_BODY_HIT',
  ]);
  add(flareFire as unknown as ForeignEnemyProgram, 0, true, []);
  add(skullKidNeedle as unknown as ForeignEnemyProgram, 0, false, []);
  add(nejiron as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_NEJIRON.index, true, [
    'NA_SE_EN_BAKUO_APPEAR', 'NA_SE_EN_BAKUO_DEAD', 'NA_SE_EN_BAKUO_ROLL',
    'NA_SE_EN_BAKUO_VOICE', 'NA_SE_IT_BOMB_EXPLOSION',
  ]);
  add(boe as unknown as ForeignEnemyProgram, GI.MM_SOUL_ENEMY_BOE.index, false, [
    'NA_SE_EN_EXTINCT', 'NA_SE_EN_KUROSUKE_ATTACK', 'NA_SE_EN_KUROSUKE_MOVE',
    'NA_SE_EN_PO_DEAD',
  ]);
  const addPort = (name: keyof typeof nativePorts, soulGi: number, sounds: string[], options: Partial<NativeEnemyPort> = {}) => {
    const row = nativePorts[name];
    actors.mm.set(row.program.sourceActorId, {
      program: row.program as unknown as ForeignEnemyProgram,
      nativePort: { ...row.nativePort, ...options } as NativeEnemyPort, soulGi, sounds,
      mainKeep: false, sourceDamage: true, sourceAudio: true
    });
  };
  addPort('STALFOS', GI.OOT_SOUL_ENEMY_STALFOS.index, [
    'NA_SE_EN_DODO_M_GND', 'NA_SE_EN_GOMA_JR_FREEZE', 'NA_SE_EN_RIZA_DOWN', 'NA_SE_EN_STAL_DAMAGE',
    'NA_SE_EN_STAL_DEAD', 'NA_SE_EN_STAL_JUMP', 'NA_SE_EN_STAL_SAKEBI', 'NA_SE_EN_STAL_WALK', 'NA_SE_EN_STAL_WARAU',
  ], { randomDropRows: [0xd0], nativeChildren: [3] });
  addPort('LIZALFOS', GI.OOT_SOUL_ENEMY_LIZALFOS.index, [
    'NA_SE_EN_GOMA_JR_FREEZE', 'NA_SE_EN_RIZA_ATTACK', 'NA_SE_EN_RIZA_CRY', 'NA_SE_EN_RIZA_DAMAGE',
    'NA_SE_EN_RIZA_DEAD', 'NA_SE_EN_RIZA_DOWN', 'NA_SE_EN_RIZA_JUMP', 'NA_SE_EN_RIZA_ONGND', 'NA_SE_EN_RIZA_WALK',
  ], { randomDropRows: [0x40] });
  addPort('SKULL_KID', GI.OOT_SOUL_ENEMY_SKULL_KID.index, []);
  addPort('MOBLIN', GI.OOT_SOUL_ENEMY_MOBLIN.index, [
    'NA_SE_EN_GOMA_JR_FREEZE', 'NA_SE_EN_MONBLIN_HAM_DOWN', 'NA_SE_EN_MONBLIN_HAM_LAND', 'NA_SE_EN_MONBLIN_HAM_UP',
    'NA_SE_EN_MORIBLIN_ATTACK', 'NA_SE_EN_MORIBLIN_DASH', 'NA_SE_EN_MORIBLIN_DEAD', 'NA_SE_EN_MORIBLIN_SLIDE',
    'NA_SE_EN_MORIBLIN_SPEAR_AT', 'NA_SE_EN_MORIBLIN_SPEAR_NORM', 'NA_SE_EN_MORIBLIN_VOICE', 'NA_SE_EN_MORIBLIN_WALK',
    'NA_SE_EN_RIZA_DOWN', 'NA_SE_EN_MONBLIN_GNDWAVE',
  ], { randomDropRows: [0xc0, 0xe0], sourceChildren: [7] });
  addPort('MOBLIN_WAVE', 0, ['NA_SE_EN_MONBLIN_GNDWAVE'], { nativeObject: 1 });
  actors.mm.get(7)!.bodyPart = true;
  actors.oot.set(0x113, {
    program: garoProgram() as unknown as ForeignEnemyProgram,
    nativePort: {
      ...garoPort.nativePort, sourceChildren: [],
      requiredRuntimeExports: [...new Set([...garoPort.requiredRuntimeExports, 'FOREIGN_ENEMY_API_SFX_STOP_POSITION'])]
    } as NativeEnemyPort,
    soulGi: GI.MM_SOUL_ENEMY_GARO.index, mainKeep: true, sourceDamage: true, sourceAudio: false,
    sounds: garoSounds
  });
  const extraPorts = [...extraMmPorts.actors, ...extraOotPorts.actors, ...EXTRA_COMBAT_PORTS.actors];
  const extraPrograms = { ...extraMmSources.programs, ...extraOotSources.programs, ...EXTRA_COMBAT_PORTS.programs };
  for (const row of extraPorts) {
    const original = extraPrograms[row.program as keyof typeof extraPrograms] as unknown as ForeignEnemyProgram | undefined;
    const program = original?.sourceGame === 'mm' && original.runtimeImports.some(ref => ref.symbol === 'FOREIGN_ENEMY_API_DEATH_FLAME_PRESET') &&
      !original.romData?.some(data => data.nativePointer === MM_DEATH_FLAME_PALETTE.nativePointer)
      ? { ...original, romData: [...(original.romData ?? []), MM_DEATH_FLAME_PALETTE] } : original;
    if (!program || program.sourceGame !== row.sourceGame || program.sourceActorId !== row.sourceActorId || program.sourceObjectId !== row.sourceObjectId) throw new Error('Additional native enemy source mismatch');
    const shuffleSoul = row.sourceGame === 'mm' ?
      row.sourceActorId === 0x23d ? GI.MM_SOUL_ENEMY_MOTH :
        row.sourceActorId === 0x23f ? GI.MM_SOUL_ENEMY_GARAHNA : undefined : undefined;
    const soul = row.soul ? GI[row.soul as keyof typeof GI] : shuffleSoul;
    if (row.soul && !soul) throw new Error('Unknown additional native enemy soul');
    const sounds = program.sourceGame === 'mm' && program.sourceActorId === 0x12c
      ? [...row.sourceSounds, 'NA_SE_EN_ME_EXIST', 'NA_SE_EN_IKURA_DAMAGE', 'NA_SE_EN_IKURA_DEAD', 'NA_SE_EN_B_SLIME_EAT']
      : program.sourceGame === 'mm' && program.sourceActorId === 0x43
        ? [...row.sourceSounds, 'NA_SE_EN_SHARP_FLOAT', 'NA_SE_EN_DEATH_VOICE', 'NA_SE_EN_DEATH_ATTACK',
          'NA_SE_EN_DEATH_ROLL', 'NA_SE_EN_FFLY_DEAD', 'NA_SE_EN_FFLY_ATTACK', 'NA_SE_EV_BAT_FLY', 'NA_SE_EN_DEATH_SCYTHE_LEV',
          'NA_SE_EN_DEATH_SCYTHE', 'NA_SE_EN_DEATH_SCYTHE_ONGND', 'NA_SE_EN_DEATH_HEARTBREAK', 'NA_SE_EN_COMMON_EXTINCT_LEV', 'NA_SE_EN_EXTINCT']
        : program.sourceGame === 'mm' && program.sourceActorId === 0x15d
          ? [...row.sourceSounds, 'NA_SE_EN_WIZ_DISAPPEAR', 'NA_SE_EN_WIZ_RUN', 'NA_SE_EN_WIZ_VOICE',
            'NA_SE_EN_WIZ_LAUGH2', 'NA_SE_EN_WIZ_EXP', 'NA_SE_EV_ICE_FREEZE', 'NA_SE_EV_ICE_MELT',
            'NA_SE_EV_BURN_OUT', 'NA_SE_PL_MAGIC_FIRE', 'NA_SE_IT_SHIELD_REFLECT_MG']
          : program.sourceGame === 'mm' && program.sourceActorId === 0x182
            ? [...row.sourceSounds, 'NA_SE_EN_ANSATSUSYA_MOVING', 'NA_SE_EN_ANSATSUSYA_CRYING',
              'NA_SE_EN_ANSATSUSYA_SKIP', 'NA_SE_EN_ANSATSUSYA_ENTRY', 'NA_SE_EN_ANSATSUSYA_DASH_2', 'NA_SE_IT_SWORD_SWING_HARD', 'NA_SE_EN_COMMON_FREEZE']
            : program.sourceGame === 'mm' && program.sourceActorId === 0x115
              ? [...row.sourceSounds, 'NA_SE_EN_BOSU_DEAD', 'NA_SE_EN_YASE_DEAD', 'NA_SE_EN_DEBU_DEAD',
                'NA_SE_EN_YASE_DEAD_VOICE', 'NA_SE_EN_DEBU_DEAD_VOICE', 'NA_SE_EN_STAL_FREEZE_LIGHTS',
                'NA_SE_EN_BOSU_LAUGH_K', 'NA_SE_EN_YASE_PAUSE_K', 'NA_SE_EN_DEBU_PAUSE_K',
                'NA_SE_EN_YASE_LAUGH_K', 'NA_SE_EN_DEBU_LAUGH_K',
                'NA_SE_EN_BOSU_HEAD_FLOAT', 'NA_SE_EN_BOSU_LAUGH_DEMO_K',
                'NA_SE_EN_BOSU_ATTACK_W', 'NA_SE_EN_YASE_ATTACK_W', 'NA_SE_EN_DEBU_ATTACK_W',
                'NA_SE_EN_YASE_DAMAGE', 'NA_SE_EN_DEBU_DAMAGE',
                'NA_SE_EN_BOSU_WALK', 'NA_SE_EN_YASE_WALK', 'NA_SE_EN_DEBU_WALK',
                'NA_SE_EN_GERUDOFT_DOWN', 'NA_SE_EN_EYEGOLE_ATTACK', 'NA_SE_EN_TEKU_JUMP',
                'NA_SE_EN_BOSU_HEAD_SHORT', 'NA_SE_EN_BOSU_CYNICAL',
                'NA_SE_EN_MIBOSS_JUMP1', 'NA_SE_EN_MIBOSS_JUMP2', 'NA_SE_EN_BOSU_LAUGH_DEMO']
              : program.sourceGame === 'mm' && program.sourceActorId === 0x28f
                ? [...row.sourceSounds, 'NA_SE_EN_KTIA_PAUSE_K', 'NA_SE_EN_KITA_LAUGH_K', 'NA_SE_EN_TEKU_JUMP',
                  'NA_SE_EN_EYEGOLE_ATTACK', 'NA_SE_IT_SHIELD_BOUND', 'NA_SE_EN_COMMON_FREEZE']
                : program.sourceGame === 'mm' && (program.sourceActorId === 0x7 || program.sourceActorId === 0x65)
                  ? [...row.sourceSounds, 'NA_SE_EN_FROG_JUMP', 'NA_SE_EN_FROG_KICK', 'NA_SE_EN_FROG_PUNCH2',
                    'NA_SE_EN_COMMON_FREEZE', 'NA_SE_EN_NPC_APPEAR', 'NA_SE_EN_FROG_DAMAGE',
                    'NA_SE_EN_FROG_DEAD', 'NA_SE_EN_FROG_PUNCH1', 'NA_SE_EN_FROG_GREET', 'NA_SE_EV_ICE_FREEZE',
                  ...(program.sourceActorId === 0x7
                    ? ['NA_SE_EN_FROG_VOICE1', 'NA_SE_EN_FROG_VOICE2', 'NA_SE_EN_PAMET_CUTTER_ON', 'NA_SE_EN_PAMET_CUTTER_OFF', 'NA_SE_EN_B_PAMET_ROLL',
                      'NA_SE_EN_B_PAMET_REVERSE', 'NA_SE_EN_B_PAMET_VOICE', 'NA_SE_EN_B_PAMET_BREAK', 'NA_SE_EN_HIPLOOP_LAND', 'NA_SE_EN_NPC_FADEAWAY', 'NA_SE_EN_FROG_RUNAWAY',
                      'NA_SE_EN_FROG_RUNAWAY2', 'NA_SE_EN_FROG_REAL', 'NA_SE_EN_BOMCHU_WALK', 'NA_SE_EV_WALK_WATER',
                      'NA_SE_EN_FROG_GREET']
                    : ['NA_SE_EN_B_SLIME_COMBINE', 'NA_SE_EN_B_SLIME_JUMP1', 'NA_SE_EN_B_SLIME_JUMP2', 'NA_SE_EN_B_SLIME_EAT', 'NA_SE_EN_B_SLIME_BREAK', 'NA_SE_EV_ICE_BROKEN', 'NA_SE_EN_B_SLIME_REVERSE', 'NA_SE_EN_B_SLIME_PUNCH_MOVE',
                      'NA_SE_EN_SLIME_JUMP1', 'NA_SE_EN_SLIME_JUMP2', 'NA_SE_EV_ICE_MELT_LEVEL', 'NA_SE_EN_FROG_DOWN',
                      'NA_SE_EN_FROG_HOLD_SLIME', 'NA_SE_EN_FROG_JUMP_MID', 'NA_SE_EN_FROG_THROW_SLIME', 'NA_SE_EN_FROG_JUMP_ABOVE', 'NA_SE_EN_UTSUBO_APPEAR_TRG', 'NA_SE_EV_WALK_WATER'])]
                  : row.sourceSounds;
    const nativePort = row.nativePort as NativeEnemyPort;
    const gekko = program.sourceGame === 'mm' && (program.sourceActorId === 0x7 || program.sourceActorId === 0x65);
    actors[program.sourceGame === 'mm' ? 'oot' : 'mm'].set(row.sourceActorId, {
      program: program as unknown as ForeignEnemyProgram,
      nativePort: gekko ? {
        ...nativePort, damageEffects: true,
        requiredRuntimeExports: [...(nativePort.requiredRuntimeExports ?? []),
          'FOREIGN_ENEMY_API_DAMAGE_EFFECTS', 'FOREIGN_ENEMY_API_ICE_SHATTER']
      } : nativePort,
      soulGi: soul?.index ?? 0, mainKeep: false, sourceDamage: true, sourceAudio: false, sounds
    });
  }
  for (const generated of generatedPorts.actors) {
    const game = generated.targetGame as 'oot' | 'mm';
    const actor = actors[game].get(generated.sourceActorId);
    if (!actor || (actor.nativePort && !extraPorts.some(p => p.sourceGame === generated.sourceGame && p.sourceActorId === generated.sourceActorId && p.nativePort.prefix === generated.nativePort.prefix && p.nativePort.objectVrom === generated.nativePort.objectVrom && p.nativePort.objectSize === generated.nativePort.objectSize && p.nativePort.objectCrc32 === generated.nativePort.objectCrc32)) || actor.program.sourceGame !== generated.sourceGame ||
      actor.program.sourceObjectId !== generated.sourceObjectId ||
      actor.program.manifest.sourceCrc32 !== generated.sourceOverlayCrc32)
      throw new Error('Generated native port roster/provenance mismatch');
    actor.nativePort = { ...actor.nativePort, ...generated.nativePort } as NativeEnemyPort;
    if (game === 'oot' && generated.sourceActorId === 0x43) {
      actor.nativePort.damageEffects = true;
      actor.nativePort.requiredRuntimeExports = [...new Set([...(actor.nativePort.requiredRuntimeExports ?? []),
        'FOREIGN_ENEMY_API_SFX_POSITION', 'FOREIGN_ENEMY_API_SFX_STOP_POSITION', 'FOREIGN_ENEMY_API_SFX_VOLUME_TRANSITION',
        'FOREIGN_ENEMY_API_SFX_WORLD',
        'FOREIGN_ENEMY_API_DAMAGE_EFFECTS'])];
    }
    if (game === 'oot' && (generated.sourceActorId === 0x28f || generated.sourceActorId === 0x15d || generated.sourceActorId === 0x115)) {
      actor.nativePort.damageEffects = true;
      actor.nativePort.requiredRuntimeExports = [...new Set([...(actor.nativePort.requiredRuntimeExports ?? []),
        'FOREIGN_ENEMY_API_DAMAGE_EFFECTS', 'FOREIGN_ENEMY_API_ICE_SHATTER'])];
    }
    if (game === 'mm' && generated.sourceActorId === 0x33) {
      actor.nativePort.randomDrops = [{ offset: 0, params: 0xc0, self: true }];
      actor.sounds = [...actor.sounds, 'NA_SE_VO_LI_SWORD_N', 'NA_SE_VO_LI_SWORD_L', 'NA_SE_VO_LI_LASH', 'NA_SE_VO_LI_AUTO_JUMP', 'NA_SE_VO_LI_DAMAGE_S', 'NA_SE_VO_BL_DOWN'];
    }
    actor.sourceAudio = false;
  }
  const complete = (rows: Map<number, PendingPort>) => new Map([...rows].map(([id, actor]) => {
    if (!actor.nativePort) throw new Error('Missing receiving-native enemy port');
    return [id, { ...actor, nativePort: actor.nativePort }] as const;
  }));
  return { oot: complete(actors.oot), mm: complete(actors.mm) };
}

export function nativeEnemyObjectDependencies(game: Game, actorId: number,
  profileObject: (actorId: number) => number | undefined): ReadonlySet<number> | undefined {
  const pending = new Set([actorId]), objects = new Set<number>();
  for (const id of pending) {
    const actor = enemyCatalogActor(game, id);
    if (!actor) {
      const object = profileObject(id);
      if (object === undefined || object < 0) return undefined;
      objects.add(object);
      continue;
    }
    if (actor.unresolvedSpawnCalls.length) return undefined;
    objects.add(actor.objectId);
    actor.requiredObjects.forEach(object => objects.add(object));
    for (const name of actor.spawnActors) {
      const child = engineActorByName(game, name);
      if (!child) return undefined;
      pending.add(child.actorId);
    }
  }
  objects.delete(1);
  return objects;
}

const MM_EXTENDED_OBJECT_PATCHES = [
  { address: 0x801663a8, words: [0x0c040b7b, 0x00000000] },
  { address: 0x8012f608, words: [0x00053c00, 0x00073c03] },
  { address: 0x8012f668, words: [0x00057100, 0x01c57021] },
  { address: 0x800b67fc, words: [0x80ae001e, 0x3c190001] },
  { address: 0x800b9b6c, words: [0x820d001e, 0x01a50019, 0x00007012, 0x022e7821, 0x01e6c021, 0x8f197d98] },
  { address: 0x800b9ba0, words: [0x8209001e, 0x01250019, 0x27a5002c, 0x00005012, 0x022a5821, 0x01666021, 0x8d8d7d98] },
] as const;

function validateMmExtendedObjectPatches(rom: Uint8Array): void {
  const codeVrom = 0xb3c000, codeVram = 0x800a5ac0;
  for (const patch of MM_EXTENDED_OBJECT_PATCHES) {
    const offset = codeVrom + patch.address - codeVram;
    for (let i = 0; i < patch.words.length; i++)
      if (offset + i * 4 + 4 > rom.length || bufReadU32BE(rom, offset + i * 4) !== patch.words[i])
        throw new Error(`MM extended object instruction revision mismatch at 0x${(patch.address + i * 4).toString(16)}`);
  }
}

function bindPristineEnemyObject(patch: Patchfile, target: Game, sourceRom: Uint8Array, objectId: number, port: NativeEnemyPort, addresses: GameAddresses): boolean {
  const source = target === 'oot' ? 'mm' : 'oot', config = CONFIG[source];
  if (!Number.isInteger(objectId) || objectId < 1 || objectId >= config.objectCount) throw new Error('Invalid pristine enemy object identity');
  const entry = config.objectTableAddr + objectId * 8;
  if (entry + 8 > sourceRom.length || bufReadU32BE(sourceRom, entry) !== port.objectVrom || bufReadU32BE(sourceRom, entry + 4) !== port.objectVrom + port.objectSize ||
    port.objectSize < 1 || port.objectVrom + port.objectSize > sourceRom.length) throw new Error('Pristine enemy object donor table mismatch');
  const bytes = sourceRom.subarray(port.objectVrom, port.objectVrom + port.objectSize);
  const model = addresses[source].fileFromROM(port.objectVrom);
  if (model.offset) throw new Error('Modified native combat-port source model: nonzero file offset');
  if (!patch.removedFiles.includes(model.name) && !patch.patches[model.name]?.length && !patch.newFiles.some(f => f.name === model.name)) return false;
  const files = patch.newFiles.filter(f => f.name === `${target}/payload`), payload = files.length === 1 ? files[0] : undefined, span = payload?.vram?.[target];
  const capName = `FOREIGN_ENEMY_OBJECT_TABLE_${target.toUpperCase()}_V1`;
  const caps = patch.symbols[target].get(capName);
  const tableSize = config.objectCount * 8;
  if (!span || span[1] - span[0] !== payload!.data.length || caps?.length !== 1 || caps[0] % 16 || caps[0] < span[0] || caps[0] + tableSize > span[1]) throw new Error('Missing native foreign object table export');
  const tableOffset = caps[0] - span[0], offset = objectId * 8;
  const originalTable = payload!.data.slice(tableOffset, tableOffset + tableSize);
  for (let i = 0; i < config.objectCount; i++)for (let field = 0; field < 2; field++)
    if (bufReadU32BE(originalTable, i * 8 + field * 4) !== ((bufReadU32BE(sourceRom, config.objectTableAddr + i * 8 + field * 4) | 0x04000000) >>> 0)) throw new Error('Native foreign object table source row mismatch');
  if ((patch.patches[payload!.name!] ?? []).some(edit => edit.addr < tableOffset + tableSize && edit.addr + edit.data.length > tableOffset)) throw new Error('Modified native foreign object binding');
  const prior = patch.symPatches.get(capName);
  if (prior && prior.length !== tableSize) throw new Error('Modified native foreign object binding');
  const table = prior ? Uint8Array.from(prior) : originalTable;
  for (let i = 0; i < config.objectCount; i++) {
    if ([0, 4].every(f => bufReadU32BE(table, i * 8 + f) === bufReadU32BE(originalTable, i * 8 + f))) continue;
    const archives = patch.newFiles.filter(f => f.name === `enemy/object/${source}/${i.toString(16)}`);
    const archive = archives.length === 1 ? archives[0] : undefined;
    const begin = bufReadU32BE(sourceRom, config.objectTableAddr + i * 8), end = bufReadU32BE(sourceRom, config.objectTableAddr + i * 8 + 4);
    if (!archive || end <= begin || end > sourceRom.length || archive.data.length !== end - begin ||
      archive.vrom < 0xd0000000 || archive.vrom + archive.data.length > 0xd1000000 ||
      patch.removedFiles.includes(archive.name!) || patch.patches[archive.name!]?.length ||
      !archive.data.every((v, k) => v === sourceRom[begin + k]) ||
      bufReadU32BE(table, i * 8) !== archive.vrom || bufReadU32BE(table, i * 8 + 4) !== archive.vrom + archive.data.length) throw new Error('Modified native foreign object binding');
  }
  const existing = patch.newFiles.filter(f => f.name === `enemy/object/${source}/${objectId.toString(16)}`);
  if (existing.length > 1 || existing.some(f => patch.removedFiles.includes(f.name!) || !!patch.patches[f.name!]?.length) || existing.some(f => f.data.length !== bytes.length || !f.data.every((v, i) => v === bytes[i]))) throw new Error('Conflicting pristine enemy object archive');
  const vrom = archiveEnemyObject(patch, source, objectId, bytes);
  bufWriteU32BE(table, offset, vrom);
  bufWriteU32BE(table, offset + 4, vrom + bytes.length);
  patch.addSymbolPatch(capName, table);
  return true;
}

const KEEP_ASSET_GAMES: Record<Game, {
  entries: (typeof facts.games)[Game]['entries'];
  objects: Record<string, { start: number; size: number; crc32: number }>;
}> = facts.games;

function selectiveKeepAssetConfig(game: Game, sourceRom: Uint8Array): Uint8Array {
  const source = game === 'oot' ? 'mm' : 'oot';
  const data = KEEP_ASSET_GAMES[source];
  const { entries } = data;
  if (!entries.length || entries.length > 128 || entries.reduce((total, entry) => total + entry.size, 0) > 0x20000)
    throw Error('Invalid selective keep asset capacity');
  const contains = (id: number, offset: number) => entries.findIndex(entry =>
    entry.objectId === id && offset >= entry.sourceOffset && offset < entry.sourceOffset + entry.size);
  const entriesEnd = 16 + entries.length * 24;
  let size = entriesEnd;
  entries.forEach((e, i) => {
    const original = data.objects[e.objectId];
    const end = e.sourceOffset + e.size;
    if (!original || !Number.isInteger(e.sourceOffset) || !Number.isInteger(e.size) ||
      e.sourceOffset < 0 || e.size < 1 || e.size > 0x10000 ||
      end > original.size || original.start + original.size > sourceRom.length)
      throw Error('Invalid selective source asset bounds');
    const prior = entries[i - 1];
    if (i && (prior.objectId > e.objectId ||
      (prior.objectId === e.objectId && prior.sourceOffset + prior.size > e.sourceOffset)))
      throw Error('Overlapping or unsorted selective source assets');
    const asset = sourceRom.subarray(original.start + e.sourceOffset, original.start + end);
    if ((CRC32.buf(asset) >>> 0) !== e.crc32) throw Error('Modified selective keep source asset');
    let previous = -4;
    for (const fixup of e.fixups) {
      if (fixup.offset % 4 || fixup.offset < 0 || fixup.offset + 4 > e.size ||
        fixup.offset <= previous || contains(fixup.targetObjectId, fixup.targetSourceOffset) < 0)
        throw Error('Invalid selective keep dependency');
      previous = fixup.offset;
      const segment = fixup.targetObjectId === 0x1001 ? 4 : 5;
      const expected = ((segment << 24) | fixup.targetSourceOffset) >>> 0;
      if (bufReadU32BE(asset, fixup.offset) !== expected)
        throw Error('Modified selective source pointer');
    }
    size += e.fixups.length * 12;
  });
  const walk = (index: number, path: Set<number>) => {
    if (path.has(index) || path.size >= 32) throw Error('Cyclic or deep selective keep dependency');
    const next = new Set(path).add(index);
    for (const fixup of entries[index].fixups)
      walk(contains(fixup.targetObjectId, fixup.targetSourceOffset), next);
  };
  entries.forEach((_, i) => walk(i, new Set()));
  if (size > 0x10000) throw Error('Selective keep metadata overflow');

  let cursor = entriesEnd;
  const blob = new Uint8Array(size);
  blob.set(toU32Buffer([0x454b5041, 1, game === 'oot' ? 0 : 1, entries.length]));
  entries.forEach((entry, i) => {
    const words = [entry.objectId, entry.sourceOffset, entry.size, entry.crc32, cursor, entry.fixups.length];
    words.forEach((word, j) => bufWriteU32BE(blob, 16 + i * 24 + j * 4, word));
    for (const fixup of entry.fixups) {
      bufWriteU32BE(blob, cursor, fixup.offset);
      bufWriteU32BE(blob, cursor + 4, fixup.targetObjectId);
      bufWriteU32BE(blob, cursor + 8, fixup.targetSourceOffset);
      cursor += 12;
    }
  });
  return blob;
}

function installSelectiveKeepAssets(patch: Patchfile, game: Game, sourceRom: Uint8Array): void {
  const symbol = game === 'oot' ? 'FOREIGN_ENEMY_KEEP_ASSETS_OOT' : 'FOREIGN_ENEMY_KEEP_ASSETS_MM';
  const refs = patch.symbols[game].get(symbol);
  const payloads = patch.newFiles.filter(file => file.name === game + '/payload');
  if (refs?.length !== 1 || payloads.length !== 1 || !Number.isInteger(refs[0]) ||
    refs[0] % 4 || patch.symPatches.has(symbol))
    throw Error('Missing selective enemy keep capability');
  const payload = payloads[0];
  const span = payload.vram?.[game];
  if (!span || span[1] - span[0] !== payload.data.length || refs[0] < span[0] || refs[0] + 4 > span[1])
    throw Error('Invalid selective enemy keep capability');
  const data = selectiveKeepAssetConfig(game, sourceRom);
  const vrom = game === 'oot' ? 0xd9a00000 : 0xd9a10000;
  if (patch.newFiles.some(file => vrom < file.vrom + file.data.length && file.vrom < vrom + data.length))
    throw Error('Selective keep metadata VROM collision');
  const source = game === 'oot' ? 'mm' : 'oot';
  const sourceFacts = KEEP_ASSET_GAMES[source];
  const config = CONFIG[source];
  const dmaIndices = new Map<string, number>();
  const dmaKey = (start: number, end: number) => start + '/' + end;
  for (let i = 0; i < config.dmaCount; ++i) {
    const offset = config.dmaAddr + i * 16;
    const key = dmaKey(bufReadU32BE(sourceRom, offset), bufReadU32BE(sourceRom, offset + 4));
    dmaIndices.set(key, dmaIndices.has(key) ? -2 : i);
  }
  const ids = new Set(sourceFacts.entries.map(entry => entry.objectId));
  const records = [...ids].map(id => {
    const object = sourceFacts.objects[id];
    const end = object.start + object.size;
    const index = dmaIndices.get(dmaKey(object.start, end)) ?? -1;
    if (index === -2) throw Error('Ambiguous selective keep source DMA identity');
    const name = FILES[source][index];
    if (index < 6 || !name) throw Error('Missing selective keep source DMA identity');
    return { game: source, index, name: source + '/' + name, vromStart: object.start, vromEnd: end, crc32: object.crc32 };
  });
  const prior = patch.meta.enemyKeepRawSources ?? [];
  if (!Array.isArray(prior)) throw Error('Invalid selective keep raw source metadata');
  const additions = records.filter(row => {
    const matching = prior.filter(old => old.name === row.name);
    if (matching.some(old => JSON.stringify(old) !== JSON.stringify(row)))
      throw Error('Conflicting selective keep source identity');
    return !matching.length;
  });
  patch.addNewFile({ name: game + '/enemy-keep-assets', vrom, data, compressed: false });
  patch.addSymbolPatch(symbol, toU32Buffer([vrom]));
  patch.meta.enemyKeepRawSources = [...prior, ...additions].sort((a, b) => a.name.localeCompare(b.name));
}

export function forceSelectiveKeepSourcesRaw(builder: Pick<RomBuilder, 'fileByNameRequired'>, patch: Patchfile,
  phase: 'original' | 'patched' = 'original'): void {
  const records = patch.meta.enemyKeepRawSources ?? [];
  if (!Array.isArray(records)) throw Error('Invalid selective keep source packaging metadata');
  for (const game of ['oot', 'mm'] as const) {
    if (!patch.newFiles.some(file => file.name === game + '/enemy-keep-assets')) continue;
    const source = game === 'oot' ? 'mm' : 'oot';
    const { entries, objects } = KEEP_ASSET_GAMES[source];
    for (const id of new Set(entries.map(entry => entry.objectId))) {
      const object = objects[id];
      if (!records.some(row => row.game === source && row.vromStart === object.start &&
        row.vromEnd === object.start + object.size && row.crc32 === object.crc32))
        throw Error('Missing raw source for selective enemy keep assets');
    }
  }
  const seen = new Set<string>();
  const verifiedFiles: ReturnType<RomBuilder['fileByNameRequired']>[] = [];
  for (const row of records) {
    if (!row || (row.game !== 'oot' && row.game !== 'mm') || !Number.isInteger(row.index) ||
      row.index < 6 || seen.has(row.name))
      throw Error('Invalid selective keep native file identity');
    const game: Game = row.game;
    if (row.name !== game + '/' + FILES[game][row.index])
      throw Error('Invalid selective keep native file identity');
    seen.add(row.name);

    const { objects, entries } = KEEP_ASSET_GAMES[game];
    const matching = Object.entries(objects).filter(([, object]) =>
      object.start === row.vromStart && object.start + object.size === row.vromEnd);
    if (!matching.some(([, object]) => object.crc32 === row.crc32))
      throw Error('Uncertified raw keep source file');

    const file = builder.fileByNameRequired(row.name);
    if (file.game !== game || file.index !== row.index || file.vaddr !== row.vromStart ||
      file.data.length !== row.vromEnd - row.vromStart || file.injected || file.alias ||
      file.type === 'dummy' || patch.removedFiles.includes(row.name) ||
      patch.newFiles.some(newFile => newFile.name === row.name) ||
      (phase === 'original' && (CRC32.buf(file.data) >>> 0) !== row.crc32))
      throw Error('Modified or replaced selective keep home file');

    if (phase === 'patched') {
      const objectId = Number(matching[0][0]);
      for (const entry of entries) {
        if (entry.objectId !== objectId) continue;
        const end = entry.sourceOffset + entry.size;
        if (entry.sourceOffset < 0 || end > file.data.length ||
          (CRC32.buf(file.data.subarray(entry.sourceOffset, end)) >>> 0) !== entry.crc32)
          throw Error('Native edit overlaps selective keep source asset');
      }
    }
    verifiedFiles.push(file);
  }
  for (const file of verifiedFiles) file.type = 'uncompressed';
}

const MUSCLE_BLOCK = {
  actor: 0xb6, object: 0xac, crc: 0x0eda151a, modelCrc: 0x8c4b9a89,
  header: 0x1a70, hook: 'FOREIGN_ENEMY_API_BROB_COLLISION'
} as const;

function verifyMuscleBlockModel(model: Uint8Array) {
  if (model.length !== 0x1aa0 || (CRC32.buf(model) >>> 0) !== MUSCLE_BLOCK.modelCrc)
    throw new Error('Unexpected retail muscle block model');
  const h = MUSCLE_BLOCK.header, word = (at: number) => bufReadU32BE(model, at), half = (at: number) => bufReadU16BE(model, at);
  if (![-600, 0, -600, 600, 1200, 600].every((v, i) => bufReadI16BE(model, h + 2 * i) === v) ||
    half(h + 0xc) !== 8 || word(h + 0x10) !== 0x06001a40 || half(h + 0x14) !== 12 || word(h + 0x18) !== 0x06001980 ||
    word(h + 0x1c) !== 0x06001978 || word(h + 0x20) !== 0x06001970 || half(h + 0x24) !== 0 || word(h + 0x28) !== 0)
    throw new Error('Unexpected muscle block collision header');
  if (word(0x1978) !== 0 || word(0x197c) !== 0x7c3 ||
    half(0x1970) > 1 || half(0x1972) !== 0 || word(0x1974) !== 0) throw new Error('Unmapped muscle block surface or camera');
  for (let at = 0x1980; at < 0x1a40; at += 16) {
    if (half(at) !== 0 || (half(at + 2) & 0x1fff) >= 8 || half(at + 4) >= 8 || half(at + 6) >= 8)
      throw new Error('Unmapped muscle block polygon');
  }
}

const OOT_TILE = {
  actor: 0x6b, object: 0x5f, overlayCrc: 0x160a633a, modelCrc: 0x42b98b42,
  burstPc: 0x268, escapePc: 0x1ec,
  burst: 'FOREIGN_ENEMY_API_TILE_FRAGMENTS', escape: 'FOREIGN_ENEMY_API_ESCAPE',
} as const;

function verifyTileModel(model: Uint8Array) {
  if (model.length !== 0x14f0 || (CRC32.buf(model) >>> 0) !== OOT_TILE.modelCrc)
    throw new Error('Flying floor tile object revision mismatch');
  const commands = new Set([0x01, 0x03, 0x05, 0x06, 0xd7, 0xd9, 0xdf, 0xe2, 0xe3, 0xe6, 0xe7, 0xe8, 0xe9, 0xf2, 0xf3, 0xf4, 0xf5, 0xfa, 0xfb, 0xfc, 0xfd]);
  let mesh = false, texture = false;
  for (let pc = 0xa60; pc < 0xaf0; pc += 8) {
    const cmd = bufReadU32BE(model, pc), arg = bufReadU32BE(model, pc + 4), op = cmd >>> 24;
    if (!commands.has(op) || (op === 0xdf) !== (pc === 0xae8)) throw new Error('Unexpected floor tile fragment command');
    if (op === 1) {
      if (arg !== 0x06000a30 || (cmd >>> 12 & 255) !== 3) throw new Error('Unexpected floor tile fragment vertices');
      mesh = true;
    }
    if (op === 0xfd) {
      if (arg !== 0x060012f0) throw new Error('Unexpected floor tile fragment texture');
      texture = true;
    }
  }
  if (!mesh || !texture) throw new Error('Incomplete floor tile fragment geometry');
}

function verifyNativePortKeeps(program: ForeignEnemyProgram, port: NativeEnemyPort, sourceRom: Uint8Array, targetRom?: Uint8Array): readonly string[] {
  const spike = port.prefix === 'FOREIGN_ENEMY_PORT_OOT_EN_NY';
  const proof = spike ? {
    actorId: 0xec, objectId: 0xe5, sourceCrc32: 0x2e892cc9,
    objectVrom: 23019520, objectSize: 8560, objectCrc32: 4125912761,
    roots: [0x1dd0, 0x1ea8, 0x1fc8],
  } : nativePortKeepsFacts.ports.find(p => p.prefix === port.prefix);
  if (!proof) return [];
  if (program.sourceGame !== 'oot' || program.sourceActorId !== proof.actorId || program.sourceObjectId !== proof.objectId ||
    program.manifest.sourceCrc32 !== proof.sourceCrc32 || port.objectVrom !== proof.objectVrom || port.objectSize !== proof.objectSize || port.objectCrc32 !== proof.objectCrc32)
    throw Error('Native combat-port keep model proof mismatch');
  const visited = new Set<number>(), external: { offset: number; word: number; pointer: number }[] = [];
  const walk = (offset: number) => {
    if (visited.has(offset)) return;
    visited.add(offset);
    if (offset % 8 || offset < 0 || offset >= port.objectSize) throw Error('Invalid native combat-port display list');
    for (let at = offset; at + 8 <= port.objectSize; at += 8) {
      const word = bufReadU32BE(sourceRom, port.objectVrom + at), pointer = bufReadU32BE(sourceRom, port.objectVrom + at + 4), op = word >>> 24;
      if ([1, 0xde, 0xfd, 0xda].includes(op) && [4, 5].includes(pointer >>> 24)) external.push({ offset: at, word, pointer });
      if (op === 0xde) {
        if (pointer >>> 24 === 6) walk(pointer & 0xffffff);
        if ((word >>> 16) & 255) return;
      }
      if (op === 0xdf) return;
    }
    throw Error('Unterminated native combat-port display list');
  };
  proof.roots.forEach(walk);
  if (proof.actorId !== 0x25 && !spike) {
    if (external.length) throw Error('Unexpected donor keep dependency in native combat port');
    return [];
  }
  const k = nativePortKeepsFacts.lizalfos;
  const offsets = spike ? [0x1ef8, 0x2018] : [k.modelOffset];
  if (external.length !== offsets.length || external.some((entry, i) => entry.offset !== offsets[i] || entry.word !== k.words[0] || entry.pointer !== k.words[1]))
    throw Error('Native reflection display-list revision mismatch');
  if (!targetRom || targetRom.length < k.targetKeepEnd || sourceRom.length < k.sourceKeepEnd ||
    bufReadU32BE(sourceRom, CONFIG.oot.objectTableAddr + 8) !== k.sourceKeepStart || bufReadU32BE(sourceRom, CONFIG.oot.objectTableAddr + 12) !== k.sourceKeepEnd ||
    bufReadU32BE(targetRom, CONFIG.mm.objectTableAddr + 8) !== k.targetKeepStart || bufReadU32BE(targetRom, CONFIG.mm.objectTableAddr + 12) !== k.targetKeepEnd)
    throw Error('Native reflection keep table revision mismatch');
  const source = sourceRom.subarray(k.sourceKeepStart + k.sourceOffset, k.sourceKeepStart + k.sourceOffset + k.bytes), target = targetRom.subarray(k.targetKeepStart + k.targetOffset, k.targetKeepStart + k.targetOffset + k.bytes);
  if (CRC32.buf(source) >>> 0 !== k.crc32 || !source.every((value, i) => value === target[i])) throw Error('Native reflection keep texture revision mismatch');
  return spike ? [] : ['FOREIGN_ENEMY_PORT_LIZALFOS_NATIVE_KEEP_V1'];
}

type AnimationSegment = { id: number; data: Uint8Array };

function resolve(segments: readonly AnimationSegment[], address: number, size: number) {
  const segment = segments.find(s => s.id === address >>> 24);
  const offset = address & 0xffffff;
  if (!segment || offset % 2 || size < 0 || offset + size > segment.data.length)
    throw new Error(`Foreign animation pointer 0x${address.toString(16)} is outside its DMA segment`);
  return { data: segment.data, offset };
}

function validateForeignAnimationHeader(segments: readonly AnimationSegment[], address: number, jointCount: number): void {
  if (!Number.isSafeInteger(jointCount) || jointCount < 1 || jointCount > 256) throw new Error('Invalid enemy animation joint count');
  const header = resolve(segments, address, 16);
  const frameCount = bufReadU16BE(header.data, header.offset);
  const frameData = bufReadU32BE(header.data, header.offset + 4);
  const jointIndices = bufReadU32BE(header.data, header.offset + 8);
  const staticIndexMax = bufReadU16BE(header.data, header.offset + 12);
  if (!frameCount) throw new Error('Enemy animation has no frames');
  const joints = resolve(segments, jointIndices, jointCount * 6);
  let samples = staticIndexMax;
  for (let i = 0; i < jointCount * 3; ++i) {
    const index = bufReadU16BE(joints.data, joints.offset + i * 2);
    samples = Math.max(samples, index + (index < staticIndexMax ? 1 : frameCount));
  }
  resolve(segments, frameData, samples * 2);
}

type AnimationObjectFact = typeof animationFacts.objects[number];

function local(data: Uint8Array, address: number, size: number, alignment: number): number {
  const offset = address & 0xffffff;
  if (address >>> 24 !== 6 || offset % alignment || size < 0 || offset + size > data.length)
    throw new Error(`Foreign skeleton pointer outside source object: 0x${address.toString(16)}`);
  return offset;
}

function validateForeignSkeleton(data: Uint8Array, skeleton: AnimationObjectFact['skeletons'][number]) {
  const offset = skeleton.offset;
  if (offset % 4 || offset < 0 || offset + (skeleton.type === 'Flex' ? 12 : 8) > data.length)
    throw new Error('Foreign skeleton header outside source object');
  const limbCount = data[offset + 4];
  if (!limbCount || limbCount + 1 !== skeleton.jointCount ||
    !['Normal', 'Flex'].includes(skeleton.type) || !['Standard', 'LOD'].includes(skeleton.limbType))
    throw new Error('Foreign skeleton type or joint count mismatch');
  if (skeleton.type === 'Flex' && data[offset + 8] > limbCount)
    throw new Error('Foreign skeleton display-list count exceeds limbs');
  const list = local(data, bufReadU32BE(data, offset), limbCount * 4, 4);
  const links: [number, number][] = [];
  for (let i = 0; i < limbCount; ++i) {
    const limb = local(data, bufReadU32BE(data, list + i * 4), skeleton.limbType === 'LOD' ? 16 : 12, 4);
    const child = data[limb + 6];
    const sibling = data[limb + 7];
    if ((child !== 255 && child >= limbCount) || (sibling !== 255 && sibling >= limbCount))
      throw new Error('Foreign skeleton limb link outside table');
    links.push([child, sibling]);
    for (let j = 0; j < (skeleton.limbType === 'LOD' ? 2 : 1); ++j) {
      const dl = bufReadU32BE(data, limb + 8 + j * 4);
      if ((dl >>> 24) === 6) local(data, dl, 8, 8);
    }
  }
  const active = new Set<number>();
  const done = new Set<number>();
  function visit(index: number) {
    if (index === 255 || done.has(index)) return;
    if (active.has(index)) throw new Error('Foreign skeleton cyclic limb links');
    active.add(index);
    for (const next of links[index]) visit(next);
    active.delete(index);
    done.add(index);
  }
  for (let i = 0; i < limbCount; ++i) visit(i);
}

function validateForeignAnimationObject(data: Uint8Array, object: AnimationObjectFact) {
  if (data.length !== object.size || (CRC32.buf(data) >>> 0) !== object.crc32) throw new Error(`Foreign animation object fingerprint mismatch: ${object.game}/${object.objectId}`);
  for (const skeleton of object.skeletons) validateForeignSkeleton(data, skeleton);
  const segments = [{ id: 6, data }];
  for (const { offset, jointCount } of object.animations) {
    if (offset % 4) throw new Error('Foreign animation header is not word aligned');
    validateForeignAnimationHeader(segments, 0x06000000 + offset, jointCount);
  }
}

function validateForeignEnemyAnimations(rom: Uint8Array, game: Game, actorId: number, checkedObjects: Set<number>) {
  const actor = animationFacts.actors.find(a => a.game === game && a.actorId === actorId);
  if (!actor) throw new Error(`Missing foreign animation audit actor: ${game}/${actorId}`);
  if (!actor.objectId || checkedObjects.has(actor.objectId)) return;
  const object = animationFacts.objects.find(o => o.game === game && o.objectId === actor.objectId);
  if (!object || object.vrom + object.size > rom.length) throw new Error('Foreign animation object outside original ROM');
  const table = CONFIG[game].objectTableAddr + actor.objectId * 8;
  if (table + 8 > rom.length || bufReadU32BE(rom, table) !== object.vrom || bufReadU32BE(rom, table + 4) !== object.vrom + object.size)
    throw new Error('Foreign animation source object table mismatch');
  validateForeignAnimationObject(rom.subarray(object.vrom, object.vrom + object.size), object);
  checkedObjects.add(actor.objectId);
}

const MAX_FOREIGN_ENEMY_DATA_SIZE = 0x8000;

const FOREIGN_ENEMY_DATA_CAPACITIES = { oot: 0x1000, mm: 0x40 };

const ENEMY_NATIVE_DATA_REFERENCES: Readonly<Record<string, { source: Game; api: string; size: number; format: 'rgba' }>> = {
  FOREIGN_ENEMY_DATA_REF_DEATH_FLAME_PALETTE: { source: 'mm', api: 'func_800B3030', size: 20, format: 'rgba' },
};

type ForeignEnemyRomData = {
  symbol: string;
  nativePointer?: string;
  sourceVrom: number;
  sourceVram: number;
  size: number;
  alignment: number;
  crc32: number;
  format: 'gfx' | 'zero' | 'matrix' | 'rgba' | 'bits';
};

type ForeignEnemyRomDataInput = ForeignEnemyRomData & { sourceName: string };

const MM_DEATH_FLAME_PALETTE: ForeignEnemyRomDataInput = {
  sourceName: 'func_800B3030', symbol: 'FOREIGN_ENEMY_DATA_MM_DEATH_FLAME_PALETTE',
  nativePointer: 'FOREIGN_ENEMY_DATA_REF_DEATH_FLAME_PALETTE', sourceVrom: 0xc449cc,
  sourceVram: 0x801ae48c, size: 20, alignment: 4, format: 'rgba', crc32: 0xd9180104,
};

function verifyDeathFlameProgram(source: Game, imports: readonly { symbol: string }[], data: readonly ForeignEnemyRomData[], rom?: Uint8Array) {
  if (!imports.some(i => i.symbol === 'FOREIGN_ENEMY_API_DEATH_FLAME_PRESET')) return;
  const palette = data.find(d => d.nativePointer === MM_DEATH_FLAME_PALETTE.nativePointer);
  if (source !== 'mm' || !palette ||
    (['symbol', 'sourceVram', 'sourceVrom', 'crc32', 'size', 'format', 'alignment'] as const).some(k => palette[k] !== MM_DEATH_FLAME_PALETTE[k]))
    throw new Error('Missing original MM death-flame preset palette');
  if (rom) {
    const helper = 0xb3c000 + 0x800b3030 - 0x800a5ac0;
    if (helper + 0x58 > rom.length || CRC32.buf(rom.subarray(helper, helper + 0x58)) >>> 0 !== 0x8bd7c124)
      throw new Error('MM death-flame helper revision mismatch');
    validateEnemyRomData(palette, rom);
  }
}

function validateEnemyRomData(facts: ForeignEnemyRomData, sourceRom?: Uint8Array) {
  if (!/^FOREIGN_ENEMY_DATA_[A-Z0-9_]+$/.test(facts.symbol) ||
    !Number.isInteger(facts.sourceVrom) || facts.sourceVrom < 1 ||
    !Number.isInteger(facts.sourceVram) || facts.sourceVram < 0x80000000 ||
    !Number.isInteger(facts.size) || facts.size < 1 || facts.size > MAX_FOREIGN_ENEMY_DATA_SIZE ||
    facts.sourceVrom + facts.size > 0x04000000 || facts.sourceVram + facts.size > 0x80800000 ||
    ![4, 8, 16].includes(facts.alignment) || facts.sourceVram % facts.alignment || facts.sourceVrom % facts.alignment ||
    !Number.isInteger(facts.crc32) || facts.crc32 < 0 || facts.crc32 > 0xffffffff ||
    !['gfx', 'zero', 'matrix', 'rgba', 'bits'].includes(facts.format) ||
    facts.format === 'gfx' && (facts.size % 8 || facts.alignment < 8) ||
    facts.format === 'matrix' && (facts.size % 64 || facts.alignment < 8) ||
    facts.format === 'rgba' && facts.size % 4 ||
    facts.format === 'bits' && (facts.size !== 128 || facts.alignment !== 4))
    throw new Error('Invalid foreign enemy ROM data metadata');
  if (facts.nativePointer !== undefined) {
    const reference = ENEMY_NATIVE_DATA_REFERENCES[facts.nativePointer];
    if (!reference || reference.size !== facts.size || reference.format !== facts.format)
      throw new Error('Invalid native enemy data reference');
  }
  if (!sourceRom) return;
  const bytes = sourceRom.subarray(facts.sourceVrom, facts.sourceVrom + facts.size);
  if (bytes.length !== facts.size || (CRC32.buf(bytes) >>> 0) !== facts.crc32)
    throw new Error(`Foreign enemy data ROM revision mismatch: ${facts.symbol}`);
  if (facts.format === 'bits') for (let index = 0; index < 32; ++index)
    if (bufReadU32BE(bytes, index * 4) !== 2 ** index) throw new Error('Foreign enemy bit table is not the reviewed powers-of-two table');
  if (facts.format === 'zero' && bytes.some(value => value !== 0)) throw new Error('Foreign enemy zero data is not zero');
  if (facts.format === 'gfx') {
    const commands = new Set([0xd7, 0xd9, 0xdf, 0xe2, 0xe3, 0xe6, 0xe7, 0xe8, 0xe9,
      0xed, 0xee, 0xef, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc]);
    for (let offset = 0; offset < bytes.length; offset += 8)
      if (!commands.has(bytes[offset])) throw new Error(`Foreign enemy graphics data requires pointer relocation: ${facts.symbol}`);
  }
}

function packEnemyRomData(target: Game, requested: readonly ForeignEnemyRomData[], sourceRom: Uint8Array) {
  const byName = new Map<string, ForeignEnemyRomData>();
  for (const facts of requested) {
    const previous = byName.get(facts.symbol);
    if (previous && (['sourceVrom', 'sourceVram', 'size', 'alignment', 'crc32', 'format', 'nativePointer'] as const).some(key => previous[key] !== facts[key]))
      throw new Error(`Conflicting foreign enemy data symbol: ${facts.symbol}`);
    byName.set(facts.symbol, facts);
  }
  const facts = [...byName.values()].sort((a, b) => a.sourceVrom - b.sourceVrom || a.symbol.localeCompare(b.symbol));
  for (const block of facts) {
    validateEnemyRomData(block, sourceRom);
    if (block.nativePointer && ENEMY_NATIVE_DATA_REFERENCES[block.nativePointer].source === target)
      throw new Error('Native enemy data reference requires the other source game');
  }
  const groups: { start: number; end: number; offset: number; delta: number }[] = [];
  for (const block of facts) {
    const end = block.sourceVrom + block.size, delta = block.sourceVram - block.sourceVrom;
    const last = groups.at(-1);
    if (last && block.sourceVrom < last.end) {
      if (last.delta !== delta) throw new Error('Foreign enemy data aliases disagree about source addresses');
      last.end = Math.max(last.end, end);
    } else groups.push({ start: block.sourceVrom, end, offset: 0, delta });
  }
  let size = 0;
  for (const group of groups) {
    group.offset = align(size, 16) + group.start % 16;
    size = group.offset + group.end - group.start;
  }
  size = align(size, 16);
  if (size > FOREIGN_ENEMY_DATA_CAPACITIES[target]) throw new Error('Foreign enemy ROM data capacity exceeded');
  const bytes = new Uint8Array(size), offsets = new Map<string, { offset: number; size: number }>();
  for (const group of groups) bytes.set(sourceRom.subarray(group.start, group.end), group.offset);
  for (const block of facts) {
    const group = groups.find(group => block.sourceVrom >= group.start && block.sourceVrom + block.size <= group.end)!;
    offsets.set(block.symbol, { offset: group.offset + block.sourceVrom - group.start, size: block.size });
  }
  const bits = [...new Set(facts.filter(fact => fact.format === 'bits').map(fact => offsets.get(fact.symbol)!.offset))].sort((a, b) => a - b);
  const headerSize = bits.length ? 24 + bits.length * 12 : 20;
  const data = new Uint8Array(headerSize + bytes.length);
  [0x454e4441, bits.length ? 2 : 1, target === 'oot' ? 0 : 1, bytes.length, CRC32.buf(bytes) >>> 0]
    .forEach((value, i) => bufWriteU32BE(data, i * 4, value));
  if (bits.length) {
    bufWriteU32BE(data, 20, bits.length);
    bits.forEach((offset, index) => [offset, 128, 5].forEach((value, field) => bufWriteU32BE(data, 24 + index * 12 + field * 4, value)));
  }
  data.set(bytes, headerSize);
  return { data, offsets };
}

function installEnemyRomData(patch: Patchfile, target: Game, requested: readonly ForeignEnemyRomData[], sourceRom: Uint8Array) {
  if (!requested.length) return patch.symbols[target];
  const symbols = new Map<string, readonly number[]>(patch.symbols[target]);
  const config = `FOREIGN_ENEMY_DATA_CONFIG_${target.toUpperCase()}`, arena = `FOREIGN_ENEMY_DATA_BASE_${target.toUpperCase()}`;
  const base = symbols.get(arena), pointer = symbols.get(config);
  if (patch.symPatches.has(config)) throw new Error('Foreign enemy ROM data is already installed');
  for (const [name, values] of [[arena, base], [config, pointer]] as const)
    if (!values || values.length !== 1 || !Number.isInteger(values[0]) || values[0] < 0x80000000 || values[0] % (name === arena ? 16 : 4) ||
      values[0] + (name === arena ? FOREIGN_ENEMY_DATA_CAPACITIES[target] : 4) > 0x80800000)
      throw new Error(`Missing or invalid native enemy data buffer: ${name}`);
  if (requested.some(fact => fact.format === 'bits')) {
    const capability = symbols.get('FOREIGN_ENEMY_DATA_BITS_V2');
    if (!capability || capability.length !== 1 || !Number.isInteger(capability[0]) || capability[0] < 0x80000000 || capability[0] >= 0x80800000 || capability[0] % 4)
      throw new Error('Rebuild the native payload to include verified enemy bit-table data support');
  }
  const { data, offsets } = packEnemyRomData(target, requested, sourceRom);
  const size = align(data.length, 16);
  let next = target === 'oot' ? 0xd5000000 : 0xd5800000;
  const end = next + 0x800000;
  for (const file of [...patch.newFiles].sort((a, b) => a.vrom - b.vrom)) {
    const after = file.vrom + align(file.data.length, 16);
    if (after <= next) continue;
    if (file.vrom >= next + size) break;
    next = after;
  }
  if (next + size > end) throw new Error('Foreign enemy ROM data DMA space exhausted');
  for (const [symbol, block] of offsets) {
    if (symbols.has(symbol)) throw new Error(`Foreign enemy data alias conflicts with native payload symbol: ${symbol}`);
    symbols.set(symbol, [base![0] + block.offset]);
  }
  const references = new Map<string, number>();
  for (const fact of requested) {
    if (!fact.nativePointer) continue;
    const native = symbols.get(fact.nativePointer), address = symbols.get(fact.symbol)![0];
    if (!native || native.length !== 1 || !Number.isInteger(native[0]) || native[0] < 0x80000000 ||
      native[0] + 4 > 0x80800000 || native[0] % 4 || patch.symPatches.has(fact.nativePointer))
      throw new Error(`Missing or invalid native enemy data reference: ${fact.nativePointer}`);
    if (references.has(fact.nativePointer) && references.get(fact.nativePointer) !== address)
      throw new Error('Conflicting native enemy data reference');
    references.set(fact.nativePointer, address);
  }
  const address = new Uint8Array(4);
  bufWriteU32BE(address, 0, next);
  patch.addNewFile({ name: `enemy/data/${target}/constants`, vrom: next, data, compressed: false });
  patch.addSymbolPatch(config, address);
  for (const [symbol, value] of references) {
    const pointer = toU32Buffer([value]);
    patch.addSymbolPatch(symbol, pointer);
  }
  return symbols;
}

const nativePeahatFacts = { 'oot': { 'actorId': 29, 'objectId': 24, 'actorTable': 11921456, 'actorTableBytes': '00c2f8d000c32fd0808920008089570000000000808952200000000000000000', 'vromStart': 12777680, 'vromEnd': 12791760, 'vramStart': 2156470272, 'vramEnd': 2156484352, 'crc32': 2937947234, 'profileOffset': 12790512, 'profileBytes': '001d050001000015001800000000041c8089200c8089227c8089493880895124', 'objectTable': 11989016, 'objectTableBytes': '0105200001053c90', 'objectStart': 17113088, 'objectEnd': 17120400, 'objectCrc32': 3375471487 }, 'mm': { 'actorId': 20, 'objectId': 20, 'actorTable': 12867472, 'actorTableBytes': '00d1115000d13b8080896f308089996000000000808994100000000000000000', 'vromStart': 13701456, 'vromEnd': 13712256, 'vramStart': 2156490544, 'vramEnd': 2156501344, 'crc32': 2617921401, 'profileOffset': 13710896, 'profileBytes': '0014050000000015001400000000054080896f30808970f480898a2880899218', 'objectTable': 12946720, 'objectTableBytes': '0122b0000122cc90', 'objectStart': 19050496, 'objectEnd': 19057808, 'objectCrc32': 3375471487 } };

const BAD_BAT = { actor: 0x15b, crc: 0x3629d509, params: 0x0101, invalidSwitch: 'FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH' } as const;

export function verifyOotEnemySourceCorrections(rom: Uint8Array): void {
  for (const row of ootSourceCorrectionsFacts) {
    if (row.originalHex.length !== 32 || !matchesRomBytes(rom, row.vrom, row.originalHex))
      throw Error('Water Temple enemy source correction revision mismatch');
  }
}

const BARI_SPLIT = {
  owner: 0x63, member: 0x34, ownerCrc: 0x832b9f33, memberCrc: 0x4aa6a9fd,
  call: 0x60c, expected: 0x0c009444, symbol: 'FOREIGN_ENEMY_API_BARI_SPLIT',
} as const;

const TAILPASARAN = {
  actor: 0x35, crc: 0x140be849, calls: [0x19c, 0x994], symbol: 'FOREIGN_ENEMY_API_TAIL_MEMBER',
} as const;

const LARVA = {
  actor: 0x2b, object: 0x1e, vrom: 0xc51860, vram: 0x808b3fc0, profile: 0x2810,
  size: 0x2c90, instance: 0x394, crc: 0x811cbb0f, hatch: 0x2598, hook: 'FOREIGN_ENEMY_API_EGG_DEBRIS'
} as const;

function donorProfileId(game: Game, child: ForeignEnemyChildLayout) {
  if (game === 'oot' && child.sourceActorId === LARVA.actor && child.sourceVrom === LARVA.vrom &&
    child.sourceVram === LARVA.vram && child.profileOffset === LARVA.profile && child.fileSize === LARVA.size &&
    child.instanceSize === LARVA.instance && child.crc32 === LARVA.crc) return 0x28;
  return child.sourceActorId;
}

const FACTS: readonly (readonly [Game, number, number, number, readonly (readonly [number, number, number, number])[]])[] = [['oot', 41979904, 41995584, 3772543354, [[94, 2, 258, 2], [110, 2, 258, 2]]], ['mm', 37126144, 37209680, 1058700328, [[9860, 4, 903, 100], [11182, 4, 80, 16], [11230, 4, 5243140, 1048675], [11278, 4, 17039735, 6488164], [11350, 4, 80, 16], [11398, 4, 5243130, 1048675], [11446, 4, 16384375, 6488164]]], ['mm', 41881600, 41939408, 4115665958, [[8179, 1, 254, 255], [258, 2, 4095, 3071]]], ['mm', 42831872, 42948416, 2198482389, [[6671, 1, 254, 255]]]];

export function reviewedNativeLayoutFile(rom: Uint8Array, game: Game, start: number, end: number, addresses: GameAddresses, patch: Patchfile): boolean {
  try {
    const f = addresses[game].fileFromROM(start), last = addresses[game].fileFromROM(end - 1);
    if (f.offset || end <= start || end > rom.length || f.name !== last.name || last.offset !== end - start - 1 || patch.removedFiles.includes(f.name) || patch.newFiles.some(r => r.name === f.name)) return false;
    const rows = patch.patches[f.name] ?? [];
    if (!rows.some(r => r.data.length)) return true;
    const fact = FACTS.find(f => f[0] === game && f[1] === start && f[2] === end);
    if (!fact || CRC32.buf(rom.subarray(start, end)) >>> 0 !== fact[3]) return false;
    const word = (bytes: Uint8Array, at: number, n: number) => {
      let v = 0;
      for (let i = 0; i < n; i++)v = v * 256 + bytes[at + i];
      return v;
    };
    return rows.every(r => fact[4].some(([at, n, old, value]) => r.addr === at && r.data.length === n && word(rom, start + at, n) === old && word(r.data, 0, n) === value));
  } catch {
    return false;
  }
}

const GROUND_STINGER = { actor: 0x3a, params: 10, crc: 0x3c860de1 } as const;

function verifyStandaloneStinger(game: Game, registration: { sourceActorId: number; root: boolean; manifest: ForeignEnemyManifest; nativePort?: boolean; sourceOverlayCrc32?: number },
  symbols: ReadonlyMap<string, readonly number[]>) {
  if (game !== 'mm' || registration.sourceActorId !== GROUND_STINGER.actor || !registration.root) return;
  if (registration.nativePort) {
    if (registration.sourceOverlayCrc32 !== GROUND_STINGER.crc || symbols.get('FOREIGN_ENEMY_API_SPAWN_CHILD')?.length !== 1)
      throw new Error('Unverified native ground Stinger source contract');
    return;
  }
  throw new Error('Ground Stinger requires a receiving-native port');
}

export function installOotEnemyNativeArena(rom: Uint8Array, addresses: GameAddresses, patch: Patchfile): void {
  const refs = patch.symbols.oot.get('ENEMY_NATIVE_ARENA_OOT_V1'), enable = patch.symbols.oot.get('ENEMY_NATIVE_ARENA_OOT_ENABLE'), files = patch.newFiles.filter(f => f.name === 'oot/payload');
  if (refs?.length !== 1 || enable?.length !== 1 || files.length !== 1) throw Error('Rebuild OoT native enemy arena capability');
  const file = files[0], span = file.vram?.oot, record = refs[0], toggle = enable[0];
  if (!span || span[0] !== 0x80400000 || span[1] - span[0] !== file.data.length || span[1] > 0x80480000 || ![record, toggle].every(x => Number.isInteger(x) && x % 4 === 0 && x >= span[0]) || record + 24 > span[1] || toggle + 4 > span[1] || toggle < record + 24 && record < toggle + 4) throw Error('Invalid OoT enemy arena capability span');
  const at = record - span[0], values = Array.from({ length: 6 }, (_, i) => bufReadU32BE(file.data, at + i * 4));
  if (values.slice(0, 5).some((word, i) => word !== [0x45415231, 2, 0x80480000, 0x100000, 0x8009ade8][i]) || values[5] % 4 || values[5] < span[0] || values[5] + 4 > span[1] || bufReadU32BE(file.data, toggle - span[0]) !== 0) throw Error('Unknown OoT enemy arena layout');
  const hook = 0xb10d48;
  if (hook + 8 > rom.length || bufReadU32BE(rom, hook) !== 0x0c019b5b || bufReadU32BE(rom, hook + 4) !== 0x00c02025) throw Error('Modified native OoT arena initialization call');
  const source = addresses.oot.fileFromROM(hook), end = addresses.oot.fileFromROM(hook + 7);
  if (source.name !== end.name || end.offset !== source.offset + 7 || patch.removedFiles.includes(source.name) || patch.newFiles.some(f => f.name === source.name)) throw Error('Replaced native OoT arena call file');
  const words = Uint8Array.from(rom.subarray(hook, hook + 8));
  for (const edit of patch.patches[source.name] ?? []) {
    const from = Math.max(source.offset, edit.addr), to = Math.min(source.offset + 8, edit.addr + edit.data.length);
    if (from < to) words.set(edit.data.subarray(from - edit.addr, to - edit.addr), from - source.offset);
  }
  if (bufReadU32BE(words, 0) !== ((0x0c000000 | ((values[5] >>> 2) & 0x3ffffff)) >>> 0) || bufReadU32BE(words, 4) !== 0x00c02025) throw Error('Missing native OoT arena initialization wrapper');
  const gameplay = patch.symbols.oot.get('ENEMY_GAMEPLAY_RESERVE_OOT_V1');
  const policy = OOT_GAMEPLAY_POLICY;
  const expected = [0x47504d31, 1, policy.nativeReserve, policy.customReserve, policy.fragmentationReserve,
    policy.contiguousReserve, policy.actorSlots, policy.customBase];
  if (gameplay?.length !== 1 || gameplay[0] < span[0] || gameplay[0] + expected.length * 4 > span[1] ||
    expected.some((word, i) => bufReadU32BE(file.data, gameplay[0] - span[0] + i * 4) !== word))
    throw Error('OoT gameplay headroom policy and compiled allocator disagree');
  if (patch.symPatches.has('ENEMY_NATIVE_ARENA_OOT_ENABLE')) throw Error('Duplicate native OoT arena activation');
  const data = new Uint8Array(4);
  bufWriteU32BE(data, 0, 1);
  patch.addSymbolPatch('ENEMY_NATIVE_ARENA_OOT_ENABLE', data);
}

const SNOW_FRAGMENT_LISTS = [0x06005cb0, 0x06006190, 0x06006620] as const;

function verifySnowFragmentObject(object: Uint8Array) {
  if (object.length !== 0x7730 || (CRC32.buf(object) >>> 0) !== 0x5d48a385)
    throw new Error('Unexpected original Eeno object');
  for (const [index, list] of SNOW_FRAGMENT_LISTS.entries()) {
    const start = list & 0xffffff;
    const next = SNOW_FRAGMENT_LISTS[index + 1];
    const limit = next === undefined ? object.length : next & 0xffffff;
    let ended = false;
    for (let at = start; at + 8 <= limit; at += 8) {
      if (bufReadU32BE(object, at) === 0xdf000000 && bufReadU32BE(object, at + 4) === 0) {
        ended = true;
        break;
      }
    }
    if (!ended) throw new Error('Incomplete Eeno snow-fragment display list');
  }
}

const MM_ENEMY_SETUP_FILTER_WORDS = [
  [0x800b9420, 0x86180000],
  [0x800b9424, 0x33080800],
  [0x800b9428, 0x55000005],
  [0x800b942c, 0x26310001],
] as const;

function verifyMmEnemySetupFilter(rom: Uint8Array, addresses: GameAddresses, patch: Patchfile): void {
  for (const [address, word] of MM_ENEMY_SETUP_FILTER_WORDS) {
    const file = addresses.mm.fileFromRAM(address);
    if (file.name !== 'mm/code' || bufReadU32BE(rom, file.addr) !== word)
      throw new Error('MM enemy setup-filter instruction revision mismatch');
  }
  const exported = patch.symbols.mm.get('FOREIGN_ENEMY_MM_SCHEDULE_V1');
  const payloads = patch.newFiles.filter(file => file.name === 'mm/payload');
  const payload = payloads.length === 1 ? payloads[0] : undefined, span = payload?.vram?.mm;
  const symbol = exported?.[0];
  if (!exported || exported.length !== 1 || !Number.isInteger(symbol) || symbol! % 4 !== 0 ||
    !span || span[0] !== 0x80700000 || span[1] > 0x80780000 || span[1] <= span[0] ||
    symbol! < span[0] || symbol! + 4 > span[1] || symbol! - span[0] + 4 > payload!.data.length)
    throw new Error('Rebuild MM payload for foreign enemy half-day schedule support');
}

function verifyNativePeahatSource(game: Game, rom: Uint8Array, addresses: GameAddresses, patch: Patchfile) {
  const f = nativePeahatFacts[game];
  if (!matchesRomBytes(rom, f.actorTable, f.actorTableBytes) || !matchesRomBytes(rom, f.objectTable, f.objectTableBytes) ||
    !matchesRomBytes(rom, f.profileOffset, f.profileBytes) || f.vromEnd > rom.length || f.objectEnd > rom.length ||
    (CRC32.buf(rom.subarray(f.vromStart, f.vromEnd)) >>> 0) !== f.crc32 ||
    (CRC32.buf(rom.subarray(f.objectStart, f.objectEnd)) >>> 0) !== f.objectCrc32)
    throw new Error('Native Peahat source/profile/model revision mismatch');

  const ranges = [
    [f.vromStart, f.vromEnd],
    [f.actorTable, f.actorTable + 32],
    [f.objectStart, f.objectEnd],
    [f.objectTable, f.objectTable + 8],
  ];
  for (const [start, end] of ranges) {
    const first = addresses[game].fileFromROM(start);
    const last = addresses[game].fileFromROM(end - 1);
    if (first.name !== last.name || last.offset - first.offset !== end - start - 1 ||
      patch.removedFiles.includes(first.name) || patch.newFiles.some(file => file.name === first.name) ||
      patch.patches[first.name]?.some(edit => edit.addr < last.offset + 1 &&
        edit.addr + edit.data.length > first.offset))
      throw new Error('Modified native Peahat source/profile/model');
  }
}

export function validateNativePeahatPlacement(game: Game, rom: Uint8Array,
  row: { replacement: Uint8Array; targetId: number; targetParams: number },
  addresses: GameAddresses, patch: Patchfile) {
  const { actorId } = nativePeahatFacts[game];
  const params = game === 'oot' ? [0, 0xffff] : [0];
  const actorMask = game === 'mm' ? 0x1fff : 0xffff;
  if (row.targetId !== actorId || !params.includes(row.targetParams) || row.replacement.length !== 16 ||
    (bufReadU16BE(row.replacement, 0) & actorMask) !== actorId ||
    bufReadU16BE(row.replacement, 14) !== row.targetParams)
    throw new Error('Invalid selected native Peahat placement');
  verifyNativePeahatSource(game, rom, addresses, patch);
}

type EnemyObjectClosure = {
  pinned: ReadonlyMap<number, number>;
  nativeChildren: ReadonlySet<number>;
};

function foreignEnemyObjects(game: Game, root: number, rom: Uint8Array,
  registrations: readonly ForeignEnemyRegistration[], rules: readonly EnemySpawnRule[],
  lightRayObject: { id: number; size: number } | undefined,
  actors: ReadonlyMap<number, ForeignEnemyRuntimeMetadata>): EnemyObjectClosure {
  const source = game === 'oot' ? 'mm' : 'oot';
  const config = CONFIG[source], pinned = new Map<number, number>(), nativeChildren = new Set<number>();
  function add(id: number) {
    if (pinned.has(0x1000 | id)) return;
    const entry = config.objectTableAddr + id * 8;
    if (id < 1 || id >= config.objectCount || entry + 8 > rom.length) throw new Error('Invalid foreign dependency object');
    const start = bufReadU32BE(rom, entry), end = bufReadU32BE(rom, entry + 4);
    if (!start || end <= start || end > rom.length) throw new Error('Missing foreign dependency object');
    const fragmentList = game === 'mm' && id === 0x5f ? 32 : game === 'oot' && id === 0x1c4 ? 96 : 0;
    pinned.set(0x1000 | id, id <= 3 ? 16 : align(end - start, 16) + fragmentList);
  }
  const pending = new Set([root]);
  for (const id of pending) {
    const registration = registrations.find(r => r.sourceActorId === id);
    const actor = enemyCatalogActor(source, id);
    if (!registration || !actor || actor.unresolvedSpawnCalls.length) throw new Error('Incomplete foreign object dependency graph');
    const port = actors.get(id)?.nativePort;
    for (const child of port?.sourceChildren ?? []) pending.add(child);
    for (const child of port?.nativeChildren ?? []) nativeChildren.add(child);
    const nativeObject = port?.nativeObject;
    if (nativeObject && (game !== 'mm' || id !== 7 || actor.objectId !== 1)) throw new Error('Unreviewed native support object');
    if (!nativeObject) add(actor.objectId);
    for (const object of actor.requiredObjects) {
      if (!nativeObject || object !== actor.objectId) add(object);
    }
    if (registration.mainKeep) add(1);
    if (registration.subKeep) add(registration.subKeep === 'field' ? 2 : 3);
    for (const name of enemyPortSpawnActors(source, id)) {
      const child = engineActorByName(source, name);
      const dispatch = child && rules.filter(rule => rule.sourceActorId === child.actorId);
      if (!dispatch?.length) throw new Error('Missing foreign child object dispatch');
      for (const rule of dispatch) {
        if (rule.targetActorId === 0xc00 + rule.sourceActorId) pending.add(rule.sourceActorId);
        else if (game === 'oot' && rule.sourceActorId === MM_LARGE_RAYS.sourceActorId && rule.targetActorId === MM_LARGE_RAYS.targetActorId) {
          if (!lightRayObject) throw new Error('Missing installed light-ray object');
          pinned.set(lightRayObject.id, lightRayObject.size);
        } else nativeChildren.add(rule.targetActorId);
      }
    }
  }
  return { pinned, nativeChildren };
}

export type ForeignEnemyPlacement = {
  sourceGame: Game;
  targetGame: Game;
  sourceActorId: number;
  sourceObjectId: number;
  actorId: number;
  objectId: number;
  objectSize: number;
  objects: EnemyObjectClosure;
};

type ForeignEnemyRuntimeMetadata = {
  program: ForeignEnemyProgram;
  nativePort: NativeEnemyPort;
  soulGi: number;
  bodyPart?: boolean;
  mainKeep?: boolean;
  subKeep?: 'field' | 'dungeon';
  sourceDamage: boolean;
  sourceAudio: boolean;
  sounds: readonly string[];
};

type ForeignEnemyArtifacts = Record<Game, ReadonlyMap<number, ForeignEnemyRuntimeMetadata>>;

export function prepareForeignEnemies(plan: EnemyPlan, roms: DecompressedRoms, addresses: GameAddresses, patch: Patchfile) {
  const targets = new Map<string, ForeignEnemyPlacement>();
  const roots: Record<Game, Map<number, ReturnType<typeof enemyDefinition>>> = { oot: new Map(), mm: new Map() };
  for (const pair of plan) {
    if (pair.kind && pair.kind !== 'scripted') continue;
    const source = enemyDefinition(pair.source), target = enemyDefinition(pair.target);
    const exactStandalone = !!pair.ootr || !!pair.placement;
    const scripted = pair.kind === 'scripted' && !!scriptedMmSource(source.key) && scriptedMmTargetAllowed(target, true, source);
    if (pair.kind === 'scripted' && !scripted) throw new Error('Unreviewed scripted MM encounter target');
    if (!exactStandalone && !scripted && !enemyEncounterCompatible(source.game, source.actorId, target.game, target.actorId))
      throw new Error('Enemy replacement requires a compatible scripted arena');
    if (source.game === target.game) continue;
    if (!exactStandalone && !scripted && !standaloneEnemyTransfer(source, target))
      throw new Error('Cross-game replacement is outside the standalone enemy shuffle pools');
    assertForeignEnemyExclusive(target.game, target.actorId, source.game, target.spawnParams);
    if (target.nativeOnly) throw new Error('Enemy variant is verified for native placements only');
    roots[source.game].set(target.actorId, target);
  }
  for (const game of ['oot', 'mm'] as const) {
    if (!patch.symbols[game].has('DEBUG_ENEMY_PICKER')) continue;
    for (const { enemy, foreign } of debugEnemyForms(game)) {
      if (foreign && !roots[game].has(enemy.actorId)) roots[game].set(enemy.actorId, enemy);
    }
  }
  if (!roots.oot.size && !roots.mm.size) return targets;
  const artifacts = bundledForeignEnemies();
  const staged = patch.dup();
  for (const game of ['oot', 'mm'] as const) {
    if (!roots[game].size) continue;
    verifyEnemyRoomEntryHook(game, roms[game].rom);
    verifyNativeEnemyEffectBoundary(game, roms[game].rom, addresses, staged);
    if (game === 'mm') {
      validateMmExtendedObjectPatches(roms.mm.rom);
      verifyMmEnemySetupFilter(roms.mm.rom, addresses, staged);
    }
    const gameActors = artifacts[game];
    const foreign = game === 'oot' ? 'mm' : 'oot';
    const checkedAnimationObjects = new Set<number>();
    const selected = new Set(roots[game].keys());
    for (const id of selected) {
      for (const child of gameActors.get(id)?.nativePort?.sourceChildren ?? []) {
        if (!Number.isInteger(child) || !gameActors.has(child)) throw new Error('Missing native combat-port support actor');
        selected.add(child);
      }
      const rules = buildEnemySpawnRules(game, [id]);
      const actor = enemyCatalogActor(foreign, id);
      if (!actor || actor.role === 'dungeon-boss' || actor.role === 'noncombat')
        throw new Error(`Invalid foreign enemy dependency: ${foreign}/${id.toString(16)}`);
      for (const symbol of enemyPortSpawnActors(foreign, id)) {
        const dependency = engineActorByName(foreign, symbol);
        if (!dependency) throw new Error(`Unknown enemy spawn dependency: ${symbol}`);
        if (!rules.some(rule => rule.sourceActorId === dependency.actorId && rule.targetActorId < 0xc00)) selected.add(dependency.actorId);
      }
    }
    if (game === 'oot' && selected.has(0x109)) installMmEnemyShadow(staged, roms.mm.rom);
    const spawnRules = buildEnemySpawnRules(game, [...selected]);
    assertEnemySpawnDependencies(game, [...selected], spawnRules);
    const romData = [...selected].flatMap(id => gameActors.get(id)?.program.romData ?? []);
    if (romData.length) installEnemyRomData(staged, game, romData, roms[foreign].rom);
    const registrations: ForeignEnemyRegistration[] = [];
    const requestedSounds = new Set<string>();
    let needsDamage = false, needsAudio = false, checkedDamageEffects = false, checkedParticles = false;
    for (const id of [...selected].sort((a, b) => a - b)) {
      const actorFacts = gameActors.get(id);
      if (!actorFacts) throw new Error(`Missing verified native actor metadata: ${foreign}/${id.toString(16)}`);
      const rom = roms[foreign].rom, config = CONFIG[foreign];
      const entry = config.actorsOvlAddr + id * 0x20;
      if (id >= config.actorsOvlCount || entry + 0x20 > rom.length) throw new Error('Invalid source actor table');
      const start = bufReadU32BE(rom, entry), end = bufReadU32BE(rom, entry + 4), vram = bufReadU32BE(rom, entry + 8);
      const profile = bufReadU32BE(rom, entry + 0x14);
      if (!start || end <= start || end > rom.length || profile < vram || profile - vram + 0x20 > end - start)
        throw new Error(`Invalid source profile for foreign enemy: ${foreign}/${id.toString(16)}`);
      const objectId = bufReadU16BE(rom, start + profile - vram + 8);
      const root = roots[game].get(id);
      const catalog = enemyCatalogActor(foreign, id);
      if (catalog?.objectId !== objectId || (root && root.objectId !== objectId))
        throw new Error(`Foreign enemy profile/object mismatch: ${foreign}/${id.toString(16)}`);
      const objectEntry = config.objectTableAddr + objectId * 8;
      if (objectId >= config.objectCount || objectEntry + 8 > rom.length) throw new Error('Invalid source enemy object table');
      const objectStart = bufReadU32BE(rom, objectEntry), objectEnd = bufReadU32BE(rom, objectEntry + 4);
      if (!objectStart || objectEnd <= objectStart || objectEnd > rom.length) throw new Error('Missing source enemy object');
      if (!actorFacts.nativePort) throw new Error(`Missing native enemy port: ${foreign}/${id.toString(16)}`);
      const compiled = instantiateNativeEnemyPort(staged, game, actorFacts.program, actorFacts.nativePort, rom, roms[game].rom);
      if (objectId > 3) bindPristineEnemyObject(staged, game, rom, objectId, actorFacts.nativePort, addresses);
      if ((foreign === 'oot' && (id === OOT_TILE.actor || id === MUSCLE_BLOCK.actor)) || (foreign === 'mm' && id === 0x1e6)) {
        const model = rom.subarray(objectStart, objectEnd);
        if (foreign === 'mm') verifySnowFragmentObject(model);
        else if (id === OOT_TILE.actor) verifyTileModel(model);
        else verifyMuscleBlockModel(model);
      }
      if (actorFacts.program.sourceGame !== foreign || actorFacts.program.sourceActorId !== id || actorFacts.program.sourceObjectId !== objectId)
        throw new Error('Native enemy metadata identity mismatch');
      validateForeignEnemyAnimations(rom, foreign, id, checkedAnimationObjects);
      verifyDeathFlameProgram(foreign, actorFacts.program.runtimeImports, actorFacts.program.romData ?? [], rom);
      if (!actorFacts.sourceDamage && actorFacts.program.runtimeImports.some(ref => ref.symbol.startsWith('FOREIGN_ENEMY_API_COLLISION_')))
        throw new Error('Native enemy collider adapter requires the source damage policy');
      const damageEffects = !!actorFacts.nativePort.damageEffects || actorFacts.program.runtimeImports.some(ref => ref.symbol === 'FOREIGN_ENEMY_API_DAMAGE_EFFECTS');
      if (damageEffects) {
        if (foreign !== 'mm' || game !== 'oot') throw new Error('MM damage-effect adapter requires its source gameplay_keep');
        if (!checkedDamageEffects) validateMmDamageEffectAssets(rom);
        checkedDamageEffects = true;
        needsAudio = true;
        for (const sound of MM_DAMAGE_EFFECT_SOUNDS) requestedSounds.add(sound);
      }
      if (actorFacts.program.runtimeImports.some(ref => ref.symbol === 'FOREIGN_ENEMY_API_PARTICLE_ICE_BLOCK')) {
        if (foreign !== 'mm' || game !== 'oot') throw new Error('Source ice particles require the MM source object');
        if (!checkedParticles) validateMmEnemyParticleAssets(rom);
        checkedParticles = true;
      }
      for (const child of actorFacts.program.foreignChildren ?? []) verifyForeignEnemyChildSource(foreign, rom, child);
      if (compiled.actorId !== 0xc00 + id || compiled.objectId !== (actorFacts.nativePort.nativeObject ?? (0x1000 | objectId)))
        throw new Error('Native enemy profile revision mismatch');
      if (root) {
        const image = staged.newFiles.find(file => file.vrom === compiled.manifest.sourceVrom)?.data;
        if (!image) throw new Error('Missing compiled foreign root profile image');
        validateForeignEnemyRootProfile(compiled.manifest, image);
      }
      const manifest = compiled.manifest;
      registrations.push({
        sourceActorId: id, root: !!root, soulGi: actorFacts.soulGi, bodyPart: actorFacts.bodyPart,
        nativePort: true,
        sourceOverlayCrc32: actorFacts.program.manifest.sourceCrc32,
        nativeDefeat: compiled.nativeDefeat,
        groundBound: (root?.habitat === 'ground' || foreign === 'oot' && id === 0xe0),
        airBound: root?.habitat === 'air' && !(foreign === 'oot' ? id === 0x37 : id === 0x24),
        sourceDamage: actorFacts.sourceDamage, sourceAudio: actorFacts.sourceAudio, mainKeep: actorFacts.mainKeep || damageEffects,
        subKeep: actorFacts.subKeep, manifest
      });
      needsDamage ||= actorFacts.sourceDamage;
      needsAudio ||= actorFacts.sourceAudio || actorFacts.program.runtimeImports.some(ref =>
        ref.symbol.startsWith('FOREIGN_ENEMY_API_SFX_') || ref.symbol === 'FOREIGN_ENEMY_API_OOT_PLAYER_SFX');
      for (const sound of actorFacts.sounds) requestedSounds.add(sound);
      if (foreign === 'mm') {
        const overlay = engineActorById('mm', id)?.overlay;
        if (overlay) for (const name of mmComputedDamageVoices(overlay)) requestedSounds.add(name);
      }
    }
    installSelectiveKeepAssets(staged, game, roms[foreign].rom);
    installEnemyRandomDrops(staged, game, roms[foreign].rom, [...selected].flatMap(id => {
      const actor = gameActors.get(id)!;
      if (!actor.nativePort) throw new Error('Missing native enemy port');
      return actor.nativePort.randomDrops ?? (actor.nativePort.randomDropRows ?? []).map(params => ({ offset: 0, params, self: true }));
    }));
    if (needsDamage) {
      installEnemyDamagePresets(staged, game, roms, defaultEnemyDamagePolicy(foreign, game));
    }
    if (requestedSounds.size || needsAudio) installForeignEnemyAudio(staged, game, requestedSounds, roms, addresses);
    const lightRayAssets = game === 'oot' && spawnRules.some(rule => rule.sourceActorId === MM_LARGE_RAYS.sourceActorId)
      ? { addresses: addresses.oot, object: new CustomObjectsBuilder(roms).makeClearTag() } : undefined;
    const { lightRayObject } = installForeignEnemyRegistry(staged, game, registrations, lightRayAssets);
    const closures = new Map<number, EnemyObjectClosure>();
    for (const pair of plan) {
      const source = enemyDefinition(pair.source), target = enemyDefinition(pair.target);
      if (source.game !== game || target.game !== foreign || targets.has(target.key)) continue;
      const id = target.actorId, objectId = 0x1000 | target.objectId;
      let objects = closures.get(id);
      if (!objects) {
        objects = foreignEnemyObjects(game, id, roms[foreign].rom, registrations, spawnRules, lightRayObject, gameActors);
        closures.set(id, objects);
      }
      const objectSize = objects.pinned.get(objectId);
      if (!objectSize || gameActors.get(id)?.program.sourceObjectId !== target.objectId) throw new Error('Missing foreign root object');
      targets.set(target.key, {
        sourceGame: foreign, targetGame: game, sourceActorId: id, sourceObjectId: target.objectId,
        actorId: 0xc00 + id, objectId, objectSize, objects
      });
    }
  }
  patch.patches = staged.patches;
  patch.newFiles = staged.newFiles;
  patch.symPatches = staged.symPatches;
  patch.meta = staged.meta;
  return targets;
}

type SpawnBits = readonly [mask: number, value: number];

type EnemyNativeSpawn = {
  sourceActorId: number;
  targetActorId: number;
  matchParams?: SpawnBits;
  matchRotX?: SpawnBits;
  requireZeroRotation?: boolean;
  params?: SpawnBits;
  rotations?: readonly [SpawnBits, SpawnBits, SpawnBits];
};

type EnemySpawnRule = Required<EnemyNativeSpawn>;

const KEEP: SpawnBits = [0xffff, 0];

const ANY: SpawnBits = [0, 0];

const word = (value: number) => Number.isInteger(value) && value >= 0 && value <= 0xffff;

function rule(input: EnemyNativeSpawn): EnemySpawnRule {
  const requireZeroRotation = input.requireZeroRotation ?? false;
  if (typeof requireZeroRotation !== 'boolean' || (requireZeroRotation && input.matchRotX !== undefined &&
    (input.matchRotX[0] !== 0xffff || input.matchRotX[1] !== 0)))
    throw new Error('Invalid zero-rotation spawn condition');
  const result = {
    ...input, requireZeroRotation, matchParams: input.matchParams ?? ANY,
    matchRotX: requireZeroRotation ? [0xffff, 0] : input.matchRotX ?? ANY,
    params: input.params ?? KEEP, rotations: input.rotations ?? [KEEP, KEEP, KEEP]
  } as EnemySpawnRule;
  if (result.rotations.length !== 3) throw new Error('A spawn rule needs three rotation transforms');
  for (const [mask, value] of [result.matchParams, result.matchRotX])
    if (!word(mask) || !word(value) || (value & ~mask)) throw new Error('Invalid spawn variant match');
  for (const [mask, value] of [result.params, ...result.rotations])
    if (!word(mask) || !word(value) || (mask & value)) throw new Error('Invalid spawn bit transform');
  return result;
}

function bombRules(source: Game, target: Game): EnemyNativeSpawn[] {
  const sourceActorId = engineActorByName(source, 'ACTOR_EN_BOM')!.actorId;
  const targetActorId = engineActorByName(target, 'ACTOR_EN_BOM')!.actorId;
  return [{
    sourceActorId, targetActorId, matchParams: [0xfffe, 0],
    matchRotX: source === 'mm' ? [1, 0] : ANY, rotations: [ANY, KEEP, KEEP]
  }];
}

function enemyPortSpawnActors(game: Game, actorId: number): readonly string[] {
  const actor = enemyCatalogActor(game, actorId);
  if (!actor) throw Error(`Missing enemy port dependency catalog: ${game}/${actorId.toString(16)}`);
  if (game === 'mm' && [0x43, 0x15d, 0x182, 0x12c, 0x7, 0x65, 0x115, 0x28f].includes(actorId)) return [];

  if (game === 'oot' && actorId === 0x69) return actor.spawnActors.filter(name => name !== 'ACTOR_EN_BB');
  return game === 'mm' && actorId === 0x2d ? actor.spawnActors.filter(name => name !== 'ACTOR_EN_TEST') : actor.spawnActors;
}

function buildEnemySpawnRules(target: Game, imported: readonly number[]): readonly EnemySpawnRule[] {
  const source = target === 'oot' ? 'mm' : 'oot';
  const rows = imported.flatMap(sourceActorId => {
    if (!Number.isInteger(sourceActorId) || sourceActorId < 0 || sourceActorId >= 0x400 || !engineActorById(source, sourceActorId))
      throw new Error('Invalid imported spawn actor');
    const actor = enemyCatalogActor(source, sourceActorId);
    if (!actor || actor.role === 'dungeon-boss' || actor.role === 'noncombat') throw new Error(`Spawn import is outside the enemy dependency scope: ${source}/${sourceActorId.toString(16)} (${actor?.role ?? 'unlisted'})`);
    const mixed = (actor.role === 'enemy' || actor.role === 'miniboss') && nativeEnemyCounterparts(source, sourceActorId, target).length > 0;
    const forms = mixed ? foreignEnemyForms(source, sourceActorId, target).filter(params => foreignEnemyRuntimeReady(source, sourceActorId, params)) : [];
    if (mixed) {
      if (!forms.length) throw new Error('No exclusive imported enemy forms');
      const unique = [...new Set(forms)];
      unique.forEach(params => assertForeignEnemyExclusive(source, sourceActorId, target, params));

      if (target === 'mm' && sourceActorId === 0x1d) unique.push(1);
      return unique.map(params => rule({ sourceActorId, targetActorId: 0xc00 + sourceActorId, matchParams: [0xffff, params] }));
    }
    if (actor.role === 'enemy' || actor.role === 'miniboss') assertForeignEnemyExclusive(source, sourceActorId, target);
    const matchParams = mixed && forms.length === 1 ? [0xffff, forms[0]] as const : target === 'mm' && sourceActorId === GROUND_STINGER.actor
      ? [0xffff, GROUND_STINGER.params] as const : undefined;
    return rule({ sourceActorId, targetActorId: 0xc00 + sourceActorId, matchParams });
  });
  for (const input of [...bombRules(source, target), ...nativeEnemyEffectRules(target, imported)]) {
    const entry = rule(input), a = engineActorById(source, entry.sourceActorId);
    const b = engineActorById(target, entry.targetActorId);
    if (!a || !b || entry.targetActorId >= 0xc00) throw new Error('Invalid native spawn identity');
    const from = enemyCatalogActor(source, entry.sourceActorId), to = enemyCatalogActor(target, entry.targetActorId);
    if (from?.role === 'dungeon-boss' || to?.role === 'dungeon-boss') throw new Error('Dungeon bosses remain in boss shuffle');
    if (from?.role === 'enemy' || from?.role === 'miniboss') {
      if (!to || !['enemy', 'miniboss'].includes(to.role)) throw new Error('Native spawn counterpart must be an enemy');
      const species = enemyActorSpecies(source, entry.sourceActorId, (entry.matchParams[0] & 0xff) === 0xff ? entry.matchParams[1] : undefined);
      const host = enemyActorSpecies(target, entry.targetActorId);
      if (!species.every(s => host.includes(s))) throw new Error('Native spawn rule needs an exact species/variant counterpart');
    } else if (a.overlay !== b.overlay) throw new Error('Unreviewed support actor counterpart');
    rows.push(entry);
  }
  if (rows.length > 4096) throw new Error('Spawn namespace table exceeds its runtime limit');
  rows.sort((a, b) => a.sourceActorId - b.sourceActorId || a.matchParams[0] - b.matchParams[0] || a.matchParams[1] - b.matchParams[1] || a.matchRotX[0] - b.matchRotX[0] || a.matchRotX[1] - b.matchRotX[1]);
  for (let i = 0; i < rows.length; ++i) for (let j = i + 1; j < rows.length && rows[j].sourceActorId === rows[i].sourceActorId; ++j) {
    const a = rows[i], b = rows[j];
    const overlap = (x: SpawnBits, y: SpawnBits) => !((x[1] ^ y[1]) & x[0] & y[0]);
    if (overlap(a.matchParams, b.matchParams) && overlap(a.matchRotX, b.matchRotX)) throw new Error('Overlapping enemy spawn variants');
  }
  return rows;
}

function serializeEnemySpawnRules(target: Game, rows: readonly EnemySpawnRule[]): Uint8Array {
  if (rows.length > 4096) throw new Error('Spawn namespace table exceeds its runtime limit');
  const data = new Uint8Array(16 + rows.length * 32);
  const version = rows.some(row => row.requireZeroRotation) ? 2 : 1;
  [0x454e5350, version, target === 'oot' ? 0 : 1, rows.length].forEach((v, i) => bufWriteU32BE(data, i * 4, v));
  rows.forEach((entry, i) => {
    const values = [entry.sourceActorId, entry.targetActorId, ...entry.matchParams, ...entry.matchRotX, ...entry.params, ...entry.rotations.flat()];
    values.forEach((value, j) => bufWriteU16BE(data, 16 + i * 32 + j * 2, value));
    if (entry.requireZeroRotation) bufWriteU32BE(data, 16 + i * 32 + 28, 1);
  });
  return data;
}

function assertEnemySpawnDependencies(target: Game, imported: readonly number[], rules: readonly EnemySpawnRule[], roots: readonly number[] = []): void {
  const source = target === 'oot' ? 'mm' : 'oot';
  for (const id of imported) {
    const actor = enemyCatalogActor(source, id);
    if (!actor) throw new Error(`Missing enemy dependency catalog entry: ${source}/${id}`);
    if (actor.unresolvedSpawnCalls.length) throw new Error(`Dynamic spawn IDs require compiler-derived dependencies: ${source}/${actor.overlay}`);
    for (const symbol of enemyPortSpawnActors(source, id)) {
      const dependency = engineActorByName(source, symbol);
      if (!dependency || !rules.some(rule => rule.sourceActorId === dependency.actorId))
        throw new Error(`Missing enemy spawn dependency: ${source}/${actor.overlay} -> ${symbol}`);
      const split = target === 'mm' && id === BARI_SPLIT.owner && dependency.actorId === BARI_SPLIT.member;
      const rows = rules.filter(r => r.sourceActorId === id);
      const solo = target === 'mm' && id === GROUND_STINGER.actor ? GROUND_STINGER :
        target === 'oot' && id === BAD_BAT.actor ? BAD_BAT : undefined;
      const standalone = solo && dependency.actorId === id &&
        rows.length === 1 && rows[0].targetActorId === 0xc00 + id && rows[0].matchParams[0] === 0xffff &&
        rows[0].matchParams[1] === solo.params && rows[0].params[0] === 0xffff && !rows[0].params[1];
      const tailFamily = target === 'mm' && id === TAILPASARAN.actor && dependency.actorId === id;
      const hatch = target === 'mm' && (id === LARVA.actor || id === 0x1d) && dependency.actorId === id;
      const terminalSelfFamily = dependency.actorId === id && ((target === 'oot' && [0x12d, 0x180, 0x1e6, 0x1e9, BAD_BAT.actor].includes(id)) || (target === 'mm' && id === 0xde)) &&
        rows.length === 1 && rows[0].targetActorId === 0xc00 + id &&
        !rows[0].matchParams[0] && !rows[0].matchParams[1] && !rows[0].matchRotX[0] && !rows[0].matchRotX[1] &&
        !rows[0].requireZeroRotation && rows[0].params[0] === 0xffff && !rows[0].params[1] &&
        rows[0].rotations.every(pair => pair[0] === 0xffff && !pair[1]);
      if (!split && !tailFamily && !hatch && !standalone && !terminalSelfFamily && roots.includes(dependency.actorId) && rules.some(rule => rule.sourceActorId === dependency.actorId && rule.targetActorId >= 0xc00))
        throw new Error(`Spawned dependency also requires authored room ownership: ${source}/${symbol}`);
    }
  }
}

export function verifyEnemyRoomEntryHook(game: Game, rom: Uint8Array) {
  const [offset, jal, delay] = game === 'oot' ? [0xa99d48, 0x0c009571, 0x02203025] : [0xb4f970, 0x0c02ecb4, 0x02e03025];
  if (offset + 8 > rom.length || bufReadU32BE(rom, offset) !== jal || bufReadU32BE(rom, offset + 4) !== delay)
    throw new Error('Room actor-entry hook source revision mismatch: ' + game);
  if (game === 'mm' && bufReadU32BE(rom, 0xb4fd28) !== 0x0c02e4cd) throw new Error('Native-first MM setup hook source revision mismatch');
}

type Root = { sourceActorId: number; root: boolean; manifest: ForeignEnemyManifest; nativePort?: boolean; sourceOverlayCrc32?: number; nativeDefeat?: readonly [number, number, number] };

function enemyDefeatTransition(target: Game, root: Root): readonly [field: number, from: number, to: number] | undefined {
  if (!root.root || target !== 'oot') return;
  const proof = respawnFacts.find(p => p.actorId === root.sourceActorId);
  if (root.nativePort) {
    const expected = proof?.sourceCrc32 ?? (root.sourceActorId === 0x14a ? 0xe81d2667 : undefined);
    if (expected === undefined) {
      if (root.nativeDefeat) throw new Error('Unexpected native enemy defeat descriptor');
      return;
    }
    if (root.sourceOverlayCrc32 !== expected || !root.nativeDefeat)
      throw new Error('Missing recompiled native enemy defeat contract');
    const [field, from, to] = root.nativeDefeat;
    if ((field | from | to) & 3 || field < 0x13c || field > 0xfffc || from === to ||
      from >= root.manifest.profileOffset || to >= root.manifest.profileOffset)
      throw new Error('Invalid recompiled native enemy defeat contract');
    return root.nativeDefeat;
  }

  throw new Error('Enemy defeat handling requires a receiving-native port');
}

type EnemyRandomDropSite = { offset: number; params: number; lastParams?: number; self: boolean };

function retailTables(source: Game, rom: Uint8Array) {
  const mm = source === 'mm', size = (mm ? 17 : 15) * 16;
  const idsCrc = mm ? 0x9179f96d : 0x078b3c5a, qtyCrc = mm ? 0x52b49a34 : 0x2434d848;
  const start = mm ? 0xc444b4 : 0xa87000, end = mm ? start + size * 2 : Math.min(rom.length, 0xa87000 + 0x103d30);
  let found = -1;
  for (let pos = start; pos + size * 2 <= end && pos + size * 2 <= rom.length; pos += 4) {
    if (rom[pos] !== 0 || rom[pos + 1] !== (mm ? 0 : 1) || rom[pos + 2] !== (mm ? 1 : 255) ||
      rom[pos + 3] !== 255 || CRC32.buf(rom.subarray(pos, pos + size)) >>> 0 !== idsCrc ||
      CRC32.buf(rom.subarray(pos + size, pos + size * 2)) >>> 0 !== qtyCrc) continue;
    if (found !== -1) throw new Error('Ambiguous source enemy drop tables');
    found = pos;
  }
  if (found === -1) throw new Error('Source enemy drop table revision mismatch');
  return { ids: rom.subarray(found, found + size), quantities: rom.subarray(found + size, found + size * 2) };
}

function hostDrop(source: Game, item: number): number | undefined {
  if (item === 255 || item <= 4 || [11, 13, 14, 15, 19, 20].includes(item)) return item;
  if (source === 'mm') {
    if (item === 12) return 0xfb;
    if (item === 5 || item === 16) return 9;
    if (item === 8) return 10;
    if (item === 18) return 0xfe;
  } else {
    if (item === 8) return 0xfd;
    if (item === 12) return 0xfc;
    if (item === 18) return 0xfe;
    if (item === 9) return 5;
    if (item === 10) return 8;
    if (item === 16) return 0x1e;
  }
  return undefined;
}

function installEnemyRandomDrops(patch: Patchfile, game: Game, rom: Uint8Array, sites: readonly EnemyRandomDropSite[]) {
  if (!sites.length) return;
  const source = game === 'oot' ? 'mm' : 'oot';
  const { ids, quantities } = retailTables(source, rom);
  const tail = 4 + 17 * 32;
  const data = new Uint8Array(tail + (source === 'oot' ? 14 : 0));
  let singleNut = false;
  function installCell(at: number, index: number) {
    const mapped = hostDrop(source, ids[index]), quantity = quantities[index];
    if (mapped === undefined || quantity > 3) throw new Error('Unproven random enemy drop cell ' + index);
    if (mapped === 0xfb) singleNut = true;
    data[at] = mapped;
    data[at + 1] = quantity;
  }
  if (source === 'oot' && sites.some(s => s.self)) {
    [0x1b, 0x16, 0x69, 0x3b, 0x6c, 0x00, 0x01].forEach((index, bit) => installCell(tail + bit * 2, index));
  }
  let mask = 0;
  const rows = new Set<number>();
  for (const site of sites) {
    const last = site.lastParams ?? site.params;
    if (!Number.isInteger(site.params) || !Number.isInteger(last) || ((site.params | last) & 15) !== 0 ||
      site.params < 0 || last < site.params || last > (source === 'mm' ? 0x100 : 0xe0))
      throw new Error('Invalid random enemy drop row range');
    for (let params = site.params; params <= last; params += 16) rows.add(params >>> 4);
  }
  for (const row of rows) {
    mask |= 1 << row;
    for (let roll = 0; roll < 16; ++roll) {
      const index = row * 16 + roll;
      installCell(4 + index * 2, index);
    }
  }
  bufWriteU32BE(data, 0, mask);
  const symbol = game === 'mm' ? 'FOREIGN_ENEMY_RANDOM_DROPS_V2_MM' :
    singleNut ? 'FOREIGN_ENEMY_RANDOM_DROPS_V2_OOT' : 'FOREIGN_ENEMY_RANDOM_DROPS_OOT';
  if (patch.symbols[game].get(symbol)?.length !== 1 || patch.symPatches.has(symbol) ||
    game === 'oot' && ['FOREIGN_ENEMY_RANDOM_DROPS_V2_OOT', 'FOREIGN_ENEMY_RANDOM_DROPS_OOT'].some(s => patch.symPatches.has(s)))
    throw new Error('Missing or duplicate native enemy random-drop table');
  patch.addSymbolPatch(symbol, data);
}

const FOREIGN_ENEMY_ACTOR_BASE = 0xc00;

const FOREIGN_ENEMY_ACTOR_COUNT = 0x400;

type ForeignEnemyRegistration = {
  sourceActorId: number;
  soulGi: number;
  root: boolean;
  bodyPart?: boolean;
  nativePort: true;
  sourceOverlayCrc32?: number;
  nativeDefeat?: readonly [number, number, number];
  sourceDamage?: boolean;
  sourceAudio?: boolean;
  mainKeep?: boolean;
  groundBound?: boolean;
  airBound?: boolean;
  subKeep?: 'field' | 'dungeon';
  manifest: ForeignEnemyManifest;
};

function foreignEnemyActorId(sourceActorId: number): number {
  if (!Number.isInteger(sourceActorId) || sourceActorId < 0 || sourceActorId >= FOREIGN_ENEMY_ACTOR_COUNT)
    throw new Error('Foreign actor ID does not fit the scene actor field');
  return FOREIGN_ENEMY_ACTOR_BASE + sourceActorId;
}

function installForeignEnemyRegistry(patch: Patchfile, targetGame: Game, registrations: readonly ForeignEnemyRegistration[], lightRayAssets?: LightRayAssets) {
  if (registrations.some(row => !row.nativePort)) throw new Error('Enemy registry requires receiving-native ports');
  if (registrations.length > 32 && !patch.symbols[targetGame].has('DEBUG_ENEMY_PICKER')) throw new Error('Foreign enemy registry exceeds resident overlay capacity');
  const sourceGame = targetGame === 'oot' ? 'mm' : 'oot';
  if (registrations.some(registration => registration.sourceDamage) &&
    (!patch.symPatches.has(`FOREIGN_ENEMY_DAMAGE_TABLES_${targetGame.toUpperCase()}`) ||
      !patch.newFiles.some(file => file.name === `enemy/damage/${targetGame}/presets`)))
    throw new Error('Source damage actors require an installed host-to-source attack policy');
  if (registrations.some(registration => registration.sourceAudio) &&
    (!patch.symPatches.has(`FOREIGN_ENEMY_SFX_MAP_${targetGame.toUpperCase()}`) ||
      !patch.newFiles.some(file => file.name === `enemy/audio/${targetGame}/sfx-map`)))
    throw new Error('Source audio actors require an installed sound namespace');
  if (targetGame === 'oot' && registrations.some(registration => registration.sourceAudio))
    throw new Error('Native OoT ports cannot use donor audio-header emulation');
  const prefix = `enemy/import/${targetGame}/`;
  if (patch.newFiles.some(f => f.name?.startsWith(prefix))) throw new Error(`Foreign registry already installed: ${targetGame}`);
  const sorted = [...registrations].sort((a, b) => a.sourceActorId - b.sourceActorId);
  const sourceCrc = (row: ForeignEnemyRegistration | undefined) => row && (row.sourceOverlayCrc32 ?? row.manifest.sourceCrc32);
  const family = targetGame === 'oot' ? [[0x14b, 0x770f3d95], [0x181, 0xad532661]] :
    [[0xf6, 0x596432a8], [0xe0, 0x8c91de7f], [0xe1, 0x5c758d8c]];
  if (family.some(([id]) => sorted.some(row => row.sourceActorId === id)) &&
    !family.every(([id, crc], i) => sorted.some(row => row.sourceActorId === id && row.root === (i === 0) && sourceCrc(row) === crc)))
    throw new Error('Missing verified controller or dependent enemy program');
  const splitOwner = targetGame === 'mm' && sorted.find(r => r.sourceActorId === BARI_SPLIT.owner);
  if (splitOwner && (!splitOwner.root || sourceCrc(splitOwner) !== BARI_SPLIT.ownerCrc ||
    sourceCrc(sorted.find(r => r.sourceActorId === BARI_SPLIT.member)!) !== BARI_SPLIT.memberCrc ||
    patch.symbols.mm.get(BARI_SPLIT.symbol)?.length !== 1)) throw new Error('Missing Bari split encounter support');
  const tail = targetGame === 'mm' && sorted.find(r => r.sourceActorId === TAILPASARAN.actor);
  if (tail) {
    const hook = patch.symbols.mm.get(TAILPASARAN.symbol);
    if (!tail.root || sourceCrc(tail) !== TAILPASARAN.crc || hook?.length !== 1 ||
      !Number.isInteger(hook[0]) || hook[0] < 0x80000000 || hook[0] >= 0x90000000 || hook[0] % 4) throw new Error('Missing Tailpasaran encounter support');
  }
  const egg = targetGame === 'mm' && sorted.find(r => r.sourceActorId === LARVA.actor);
  if (egg) {
    const hook = patch.symbols.mm.get(LARVA.hook);
    if (!egg.root || egg.subKeep !== 'dungeon' || sourceCrc(egg) !== LARVA.crc || hook?.length !== 1 ||
      !Number.isInteger(hook[0]) || hook[0] < 0x80000000 || hook[0] >= 0x90000000 || hook[0] % 4)
      throw new Error('Missing ordinary Gohma Larva encounter support');
  }
  const bat = targetGame === 'oot' && sorted.find(r => r.sourceActorId === BAD_BAT.actor);
  if (bat && (!bat.root || sourceCrc(bat) !== BAD_BAT.crc))
    throw new Error('Missing standalone Bad Bat source contract');
  const ids = new Set<number>();
  const files: { name: string; vrom: number; data: Uint8Array; compressed: boolean }[] = [];
  const start = targetGame === 'oot' ? 0xd1000000 : 0xd1800000, end = start + 0x800000;
  let next = start;
  const allocate = (name: string, data: Uint8Array) => {
    const size = align(data.length, 16);
    for (const file of [...patch.newFiles, ...files].sort((a, b) => a.vrom - b.vrom)) {
      const fileEnd = file.vrom + align(file.data.length, 16);
      if (fileEnd <= next) continue;
      if (file.vrom >= next + size) break;
      next = fileEnd;
    }
    if (next + size > end) throw new Error('Foreign enemy manifest DMA space exhausted');
    const vrom = next;
    next += size;
    files.push({ name, vrom, data, compressed: false });
    return vrom;
  };
  const transitions = sorted.map(root => enemyDefeatTransition(targetGame, root));
  if (targetGame === 'oot' && sorted.some((root, i) => transitions[i] && [0x164, 0x16f].includes(root.sourceActorId))) {
    const capability = patch.symbols.oot.get('FOREIGN_ENEMY_RESPAWN_DEFEAT_V1');
    const payloads = patch.newFiles.filter(file => file.name === 'oot/payload');
    const payload = payloads.length === 1 ? payloads[0] : undefined, range = payload?.vram?.oot;
    if (capability?.length !== 1 || !Number.isInteger(capability[0]) || capability[0] % 4 || !payload || !range ||
      range[1] - range[0] !== payload.data.length || capability[0] < range[0] || capability[0] + 4 > range[1])
      throw new Error('Rebuild the native payload for certified respawning enemy defeats');
  }
  const stride = transitions.some(t => t !== undefined) ? 24 : 12;
  const table = new Uint8Array(16 + sorted.length * stride);
  [0x454e4d59, stride === 24 ? 2 : 1, targetGame === 'oot' ? 0 : 1, sorted.length].forEach((v, i) => bufWriteU32BE(table, i * 4, v));
  const giNames = new Map(Object.entries(GI).map(([name, item]) => [item.index, name]));
  sorted.forEach((registration, i) => {
    const id = foreignEnemyActorId(registration.sourceActorId);
    verifyStandaloneStinger(targetGame, registration, patch.symbols[targetGame]);
    if (ids.has(id)) throw new Error(`Duplicate foreign actor registration: ${id.toString(16)}`);
    ids.add(id);
    const actor = enemyCatalogActor(sourceGame, registration.sourceActorId);
    if (!actor || actor.role === 'dungeon-boss' || (registration.root && !isEnemyEncounter(actor)))
      throw new Error(`Foreign actor is outside the requested enemy scope: ${sourceGame}/${registration.sourceActorId.toString(16)}`);
    if (actor.role === 'enemy' || actor.role === 'miniboss') {
      const forms = foreignEnemyForms(sourceGame, registration.sourceActorId, targetGame).filter(params => foreignEnemyRuntimeReady(sourceGame, registration.sourceActorId, params));
      const mixed = nativeEnemyCounterparts(sourceGame, registration.sourceActorId, targetGame).length > 0;
      if (mixed) {
        if (!forms.length) throw new Error('No exclusive imported enemy forms');
        for (const params of new Set(forms))
          assertForeignEnemyExclusive(sourceGame, registration.sourceActorId, targetGame, params);
      } else {
        assertForeignEnemyExclusive(sourceGame, registration.sourceActorId, targetGame);
      }
    }
    if (registration.root && targetGame === 'oot' && [0x12d, 0x1e6, 0x1e9].includes(registration.sourceActorId)) {
      const family = patch.symbols.oot.get(registration.sourceActorId === 0x12d ? 'FOREIGN_ENEMY_MM_SELF_FAMILIES_V3' : 'FOREIGN_ENEMY_MM_SELF_FAMILIES_V2');
      if (family?.length !== 1 || !Number.isInteger(family[0]) || family[0] % 4 || family[0] < 0x80000000 || family[0] >= 0x80800000)
        throw new Error('Rebuild the native payload for MM enemy family ownership');
    }
    if (registration.root && targetGame === 'oot' && registration.sourceActorId === 0x1e9) {
      const family = patch.symbols.oot.get('FOREIGN_ENEMY_MM_HIPLOOP_MASK_V1');
      if (family?.length !== 1 || !Number.isInteger(family[0]) || family[0] % 4 || family[0] < 0x80000000 || family[0] >= 0x80800000)
        throw new Error('Rebuild the native payload for masked Hiploop family ownership');
    }
    if (registration.root && targetGame === 'oot' && registration.sourceActorId === 0x1e6 &&
      patch.symbols.oot.get('FOREIGN_ENEMY_MM_EENO_FAMILY_V1')?.length !== 1)
      throw new Error('Rebuild the native payload for Eeno split-family ownership');
    if (registration.root && targetGame === 'oot' && registration.sourceActorId === 0x15b) {
      const family = patch.symbols.oot.get('FOREIGN_ENEMY_MM_BAD_BAT_FAMILY_V1');
      if (family?.length !== 1 || !Number.isInteger(family[0]) || family[0] % 4 || family[0] < 0x80000000 || family[0] >= 0x80800000)
        throw new Error('Rebuild the native payload for MM Bad Bat family ownership');
    }
    if (registration.root && !standaloneEnemyTarget(sourceGame, registration.sourceActorId) && !additionalForeignEnemyTarget(sourceGame, registration.sourceActorId))
      throw new Error('Foreign root is outside its standalone enemy shuffle pool');
    if (!Number.isInteger(registration.soulGi) || registration.soulGi < 0 || registration.soulGi > 0x7fff)
      throw new Error('Invalid source soul GI');
    if (registration.root && registration.bodyPart) throw new Error('An enemy root cannot be a body-break fragment');
    const manifestVrom = allocate(prefix + registration.sourceActorId.toString(16), serializeForeignEnemyManifest(registration.manifest));
    const offset = 16 + i * stride;
    bufWriteU16BE(table, offset, id);
    const sourceSoul = giNames.get(registration.soulGi);
    const hostSoul = sourceSoul?.startsWith(sourceGame.toUpperCase() + '_SOUL_ENEMY_')
      ? GI[targetGame.toUpperCase() + sourceSoul.slice(sourceGame.length)]?.index
      : undefined;
    bufWriteU16BE(table, offset + 2, hostSoul ?? registration.soulGi);
    bufWriteU32BE(table, offset + 4, (registration.root ? 1 : 0) | (registration.bodyPart ? 2 : 0) | (registration.nativePort ? 256 : 0) |
      (registration.groundBound ? 512 : 0) | (registration.airBound ? 1024 : 0) | (hostSoul !== undefined ? 2048 : 0) | (registration.sourceAudio ? 128 : 0) | (registration.sourceDamage ? 64 : 0) | (registration.mainKeep ? 4 : 0) | (registration.subKeep === 'field' ? 8 : registration.subKeep === 'dungeon' ? 16 : 0));
    bufWriteU32BE(table, offset + 8, manifestVrom);
    transitions[i]?.forEach((value, j) => bufWriteU32BE(table, offset + 12 + j * 4, value));
  });
  const spawnRules = buildEnemySpawnRules(targetGame, sorted.map(registration => registration.sourceActorId));
  const lightRayObject = targetGame === 'oot' && spawnRules.some(rule => rule.sourceActorId === MM_LARGE_RAYS.sourceActorId)
    ? verifyMmLargeLightRays(patch, sorted, lightRayAssets) : undefined;
  assertEnemySpawnDependencies(targetGame, sorted.map(r => r.sourceActorId), spawnRules, sorted.filter(r => r.root).map(r => r.sourceActorId));
  const spawnsVrom = allocate(prefix + 'spawns', serializeEnemySpawnRules(targetGame, spawnRules));
  const spawnPointer = new Uint8Array(4);
  bufWriteU32BE(spawnPointer, 0, spawnsVrom);
  const vrom = allocate(prefix + 'registry', table);
  const pointer = toU32Buffer([vrom]);
  for (const file of files) patch.addNewFile(file);
  patch.addSymbolPatch(`FOREIGN_ENEMY_SPAWNS_${targetGame.toUpperCase()}`, spawnPointer);
  patch.addSymbolPatch(`FOREIGN_ENEMY_REGISTRY_${targetGame.toUpperCase()}`, pointer);
  return { vrom, spawnsVrom, count: sorted.length, lightRayObject };
}

const DAMAGE_ATTACKS = {
  oot: ['nut', 'stick', 'slingshot', 'explosive', 'boomerang', 'arrow', 'hammer', 'hookshot',
    'kokiriSlash', 'masterSlash', 'giantSlash', 'fireArrow', 'iceArrow', 'lightArrow',
    'unknownArrow1', 'unknownArrow2', 'unknownArrow3', 'fireMagic', 'iceMagic', 'lightMagic',
    'shield', 'lightRay', 'kokiriSpin', 'giantSpin', 'masterSpin', 'kokiriJump', 'giantJump',
    'masterJump', 'unknown28', 'unblockable', 'hammerJump', 'unknown31'],
  mm: ['nut', 'stick', 'horse', 'explosive', 'zoraBoomerang', 'arrow', 'unknown6', 'hookshot',
    'goronPunch', 'sword', 'goronPound', 'fireArrow', 'iceArrow', 'lightArrow', 'goronSpikes',
    'dekuSpin', 'dekuBubble', 'dekuLaunch', 'unknown18', 'zoraBarrier', 'shield', 'lightRay',
    'thrownObject', 'zoraPunch', 'swordSpin', 'swordBeam', 'roll', 'unknown27', 'unknown28',
    'unblockable', 'unknown30', 'powderKeg'],
} as const;

type DamageAttack = typeof DAMAGE_ATTACKS[Game][number];

type HitDamagePolicy = { hit: readonly DamageAttack[]; fallback?: DamageAttack };

type DamagePolicy = Partial<Record<DamageAttack, DamageAttack | HitDamagePolicy | null>>;

const ENEMY_DAMAGE_EQUIVALENTS = {
  oot: { goronPunch: 'hammer', goronPound: 'hammerJump', dekuBubble: 'slingshot', zoraBoomerang: 'boomerang' },
  mm: { hammer: 'goronPunch', hammerJump: 'goronPound', slingshot: 'dekuBubble', boomerang: 'zoraBoomerang' },
} as const satisfies Record<Game, Partial<Record<DamageAttack, DamageAttack>>>;

function defaultEnemyDamagePolicy(source: Game, target: Game): DamagePolicy {
  if (source === target) return {};
  if (source === 'oot' && target === 'mm') return {
    sword: { hit: ['kokiriSlash', 'masterSlash', 'giantSlash', 'kokiriJump', 'masterJump', 'giantJump', 'kokiriSpin', 'masterSpin', 'giantSpin'] },
    swordSpin: { hit: ['kokiriSpin', 'masterSpin', 'giantSpin'] },
    goronPunch: { hit: ['hammer', 'hammerJump'] },
    fireArrow: { hit: ['fireArrow', 'fireMagic'], fallback: 'fireArrow' },
    horse: 'giantSlash', thrownObject: 'stick',
    goronSpikes: 'hammer', roll: 'hammer', dekuSpin: 'stick',
    dekuLaunch: 'hammerJump', zoraPunch: 'kokiriSlash',
    zoraBarrier: 'lightMagic', swordBeam: 'giantSlash', powderKeg: 'explosive',
    unknown6: null, unknown18: null, unknown27: null, unknown28: null, unknown30: null,
  };
  return {
    kokiriSlash: 'sword', masterSlash: 'sword', giantSlash: 'sword',
    kokiriSpin: 'swordSpin', masterSpin: 'swordSpin', giantSpin: 'swordSpin',
    kokiriJump: 'sword', masterJump: 'sword', giantJump: 'sword',
    fireMagic: 'fireArrow', iceMagic: 'iceArrow', lightMagic: 'lightArrow',
    unknownArrow1: null, unknownArrow2: null, unknownArrow3: null, unknown28: null, unknown31: null,
  };
}

function attackIndex(game: Game, name: DamageAttack) {
  const index = (DAMAGE_ATTACKS[game] as readonly string[]).indexOf(name);
  if (index < 0) throw new Error(`Attack ${name} does not exist in ${game}`);
  return index;
}

function damageAttackCandidates(source: Game, target: Game, policy: DamagePolicy = {}): readonly (readonly number[])[] {
  const names = new Set<string>(DAMAGE_ATTACKS[source]);
  return DAMAGE_ATTACKS[target].map(name => {
    if (Object.hasOwn(policy, name)) {
      const replacement = policy[name];
      if (replacement === null) return [];
      if (typeof replacement === 'object') {
        if (!Array.isArray(replacement.hit) || replacement.hit.length < 2 || new Set(replacement.hit).size !== replacement.hit.length)
          throw new Error('Invalid hit-specific damage policy');
        const indices = replacement.hit.map(attack => attackIndex(source, attack));
        if (replacement.fallback !== undefined && !indices.includes(attackIndex(source, replacement.fallback)))
          throw new Error('Hit-specific fallback must be a candidate attack');
        return indices;
      }
      return [attackIndex(source, replacement!)];
    }
    if (names.has(name) && (source === target || !name.startsWith('unknown'))) return [attackIndex(source, name)];
    const equivalent = (ENEMY_DAMAGE_EQUIVALENTS[source] as Partial<Record<DamageAttack, DamageAttack>>)[name];
    if (source !== target && equivalent) return [attackIndex(source, equivalent)];
    throw new Error(`Missing ${source} -> ${target} damage policy for ${name}`);
  });
}

const PRESETS = { oot: { vrom: 0xb65620, crc: 0x5735879e }, mm: { vrom: 0xc50460, crc: 0x2c0e4dff } };

function compileEnemyDamagePresets(target: Game, sourceRom: Uint8Array, policy: DamagePolicy): Uint8Array {
  const source = target === 'oot' ? 'mm' : 'oot', presetFacts = PRESETS[source];
  const bytes = sourceRom.subarray(presetFacts.vrom, presetFacts.vrom + 23 * 32);
  if (bytes.length !== 23 * 32 || (CRC32.buf(bytes) >>> 0) !== presetFacts.crc) throw new Error('Source damage preset table revision mismatch');
  const candidates = damageAttackCandidates(source, target, policy);
  const dynamic = candidates.some(indices => indices.length > 1);
  const tableOffset = dynamic ? 176 : 48;
  const data = new Uint8Array(tableOffset + bytes.length);
  [0x45444d47, dynamic ? 3 : 2, target === 'oot' ? 0 : 1, 23].forEach((v, i) => bufWriteU32BE(data, i * 4, v));
  data.set(Uint8Array.from(candidates, (indices, index) => {
    if (indices.length < 2) return indices[0] ?? 0xff;
    const entry = policy[DAMAGE_ATTACKS[target][index]];
    return entry && typeof entry === 'object' && entry.fallback !== undefined
      ? 0x80 | (DAMAGE_ATTACKS[source] as readonly string[]).indexOf(entry.fallback) : 0xfe;
  }), 16);
  if (dynamic) candidates.forEach((indices, index) => bufWriteU32BE(data, 48 + index * 4, indices.reduce((mask, bit) => (mask | 2 ** bit) >>> 0, 0)));
  data.set(bytes, tableOffset);
  return data;
}

function installEnemyDamagePresets(patch: Patchfile, target: Game, roms: DecompressedRoms, policy: DamagePolicy) {
  const symbol = `FOREIGN_ENEMY_DAMAGE_TABLES_${target.toUpperCase()}`;
  if (patch.symPatches.has(symbol)) throw new Error('Enemy damage presets already installed');
  if (!patch.symbols[target].has(symbol)) throw new Error('Rebuild the native payload to include source damage table support');
  const data = compileEnemyDamagePresets(target, roms[target === 'oot' ? 'mm' : 'oot'].rom, policy);
  if (bufReadU32BE(data, 4) === 3 && !patch.symbols[target].has('FOREIGN_ENEMY_DAMAGE_POLICY_V3'))
    throw new Error('Rebuild the native payload to include hit-specific enemy damage policy support');
  let next = target === 'oot' ? 0xd4000000 : 0xd4800000;
  const end = next + 0x800000, size = (data.length + 15) & ~15;
  for (const file of [...patch.newFiles].sort((a, b) => a.vrom - b.vrom)) {
    const fileEnd = file.vrom + ((file.data.length + 15) & ~15);
    if (fileEnd <= next) continue;
    if (file.vrom >= next + size) break;
    next = fileEnd;
  }
  if (next + size > end) throw new Error('Enemy damage preset DMA space exhausted');
  const pointer = toU32Buffer([next]);
  patch.addNewFile({ name: `enemy/damage/${target}/presets`, vrom: next, data, compressed: false });
  patch.addSymbolPatch(symbol, pointer);
  return next;
}

const MM_LARGE_RAYS: EnemyNativeSpawn = {
  sourceActorId: 0xa2, targetActorId: 0x13b, matchParams: [0xffff, 4], requireZeroRotation: true,
};

type ClearTagCall = { paramsAt: number; call: number; fields: readonly number[]; variant?: 0 | 1 | 2 | 3 | 4 | 35 };

type ClearTagCaller = { actorId: number; crc32: number; sites: readonly ClearTagCall[] };

const CLEAR_TAG_CALLERS: readonly ClearTagCaller[] = [
  {
    actorId: 0x0b, crc32: 0x4dfdaf2a, sites: [
      { paramsAt: 0x5d0, call: 0x620, fields: [0x5dc, 0x5e0, 0x600] },
    ]
  },
  {
    actorId: 0x66, crc32: 0x5801bdbe, sites: [
      { paramsAt: 0x280, call: 0x2a8, fields: [0x2a4], variant: 3 },
    ]
  },
  {
    actorId: 0x291, crc32: 0x70a465b6, sites: [
      { paramsAt: 0x247c, call: 0x24c8, fields: [0x2498] },
    ]
  },
  {
    actorId: 0x155, crc32: 0xdb43c397, sites: [
      { paramsAt: 0xb28, call: 0xb44, fields: [0xb40], variant: 2 },
    ]
  },
  {
    actorId: 0x184, crc32: 0xb6d3d288, sites: [
      { paramsAt: 0x250c, call: 0x2540, fields: [0x250c], variant: 0 },
      { paramsAt: 0x274c, call: 0x2784, fields: [0x274c, 0x275c, 0x2760, 0x276c] },
    ]
  },
  {
    actorId: 0x13a, crc32: 0xf547c0ea, sites: [
      { paramsAt: 0xd40, call: 0xd68, fields: [0xd40], variant: 0 },
    ]
  },
  {
    actorId: 0x12c, crc32: 0x780861a3, sites: [
      { paramsAt: 0x8dc, call: 0x8f8, fields: [0x8fc], variant: 35 },
      { paramsAt: 0x13a0, call: 0x13c0, fields: [0x13bc], variant: 1 },
    ]
  },
  {
    actorId: 0x15b, crc32: 0x3629d509, sites: [
      { paramsAt: 0xb58, call: 0xb94, fields: [0xb5c, 0xb64, 0xb84], variant: 3 },
    ]
  },
  {
    actorId: 0x28f, crc32: 0x67992926, sites: [
      { paramsAt: 0x3578, call: 0x3594, fields: [0x3598] },
    ]
  },
  {
    actorId: 0x43, crc32: 0x5112de68, sites: [
      { paramsAt: 0x3110, call: 0x3134, fields: [0x3130] },
    ]
  },
  {
    actorId: 0x109, crc32: 0x064df7e9, sites: [
      { paramsAt: 0x1f20, call: 0x1f3c, fields: [0x1f40] },
    ]
  },
  {
    actorId: 0x113, crc32: 0xc8d13e73, sites: [
      { paramsAt: 0x28cc, call: 0x28e8, fields: [0x28e4] },
    ]
  },
  {
    actorId: 0x182, crc32: 0x74fc26b5, sites: [
      { paramsAt: 0x314c, call: 0x3168, fields: [0x3164] },
    ]
  },
  {
    actorId: 0x1ba, crc32: 0x7b405b21, sites: [
      { paramsAt: 0x1994, call: 0x19b8, fields: [0x19bc] },
    ]
  },
  {
    actorId: 0x7, crc32: 0x4ca5a6ff, sites: [
      { paramsAt: 0xa04, call: 0xa44, fields: [0xa10, 0xa24] },
      { paramsAt: 0x361c, call: 0x365c, fields: [0x3628, 0x363c] },
    ]
  },
  {
    actorId: 0x1e9, crc32: 0xdcfc8d6d, sites: [
      { paramsAt: 0x2b50, call: 0x2b6c, fields: [0x2b68] },
    ]
  },
  {
    actorId: 0x14b, crc32: 0x770f3d95, sites: [
      { paramsAt: 0xef8, call: 0xf18, fields: [0xf10] },
    ]
  },
  {
    actorId: 0x1e6, crc32: 0xe0e4e793, sites: [
      { paramsAt: 0x2648, call: 0x2670, fields: [0x264c] },
    ]
  },
  {
    actorId: 0x15d, crc32: 0x01232f00, sites: [
      { paramsAt: 0x25ec, call: 0x260c, fields: [0x2608] },
    ]
  },
  {
    actorId: 0x14a, crc32: 0xe81d2667, sites: [
      { paramsAt: 0x23c4, call: 0x23ec, fields: [0x23c8] },
    ]
  },
  {
    actorId: 0x65, crc32: 0xb0ae9cde, sites: [
      { paramsAt: 0x6d30, call: 0x6d64, fields: [0x6d38, 0x6d44] },
      { paramsAt: 0x6eb4, call: 0x6ee8, fields: [0x6ebc, 0x6ec8] },
    ]
  },
];

function nativeEnemyEffectRules(target: Game, imported: readonly number[]): readonly EnemyNativeSpawn[] {
  if (target !== 'oot') return [];
  const callers = imported.filter(id => ENEMY_ACTOR_CATALOG.some(actor => actor.game === 'mm' &&
    actor.actorId === id && actor.spawnActors.some(name => name === 'ACTOR_EN_CLEAR_TAG')));
  if (callers.some(id => !CLEAR_TAG_CALLERS.some(caller => caller.actorId === id))) throw new Error('Unverified MM Clear Tag spawn variant');
  const variants = new Set(callers.flatMap(id => CLEAR_TAG_CALLERS.find(c => c.actorId === id)!.sites.map(s => s.variant ?? 4)));
  return [...variants].sort((a, b) => a - b).map(params => ({ ...MM_LARGE_RAYS, matchParams: [0xffff, params] as const }));
}

type Caller = Pick<ForeignEnemyRegistration, 'sourceActorId'>;

type LightRayAssets = {
  addresses: Pick<Addresses, 'fileFromROM'>;
  object: Pick<CustomObject, 'data' | 'offsets'>;
};

function verifyMmLargeLightRays(patch: Patchfile, callers: readonly Caller[], assets?: LightRayAssets) {
  const dispatch = patch.symbols.oot.get('FOREIGN_ENEMY_API_SPAWN');
  if (dispatch?.length !== 1 || !Number.isInteger(dispatch[0]) || dispatch[0] < 0x80000000 ||
    dispatch[0] >= 0x90000000 || dispatch[0] % 4)
    throw new Error('Missing native light-ray spawn dispatcher');
  const file = (name: string) => {
    const matches = patch.newFiles.filter(f => f.name === name);
    if (matches.length !== 1 || patch.removedFiles.includes(name) || patch.patches[name]?.length)
      throw new Error('Missing or modified native light-ray file: ' + name);
    return matches[0];
  };
  if (!assets) throw new Error('Missing native light-ray assets');
  const { overlay, vram, profile, layout } = installedEnemyDependency(patch, 'oot', assets.addresses,
    MM_LARGE_RAYS.targetActorId, 7, 'ovl/oot/oot_ovl_En_Clear_Tag.zovlx');

  const symbol = 'FOREIGN_ENEMY_MM_LARGE_RAYS_V1';
  const marker = patch.symbols.oot.get(symbol);
  if (marker?.length !== 1 || !Number.isInteger(marker[0]) || marker[0] % 4 || patch.symPatches.has(symbol) ||
    marker[0] - vram[0] < layout.textSize + layout.dataSize || marker[0] - vram[0] + 12 > layout.relocationOffset)
    throw new Error('Missing receiving light-ray capability');
  if (callers.some(c => CLEAR_TAG_CALLERS.some(r => r.actorId === c.sourceActorId && r.sites.some(s => s.variant === 3)))) {
    const name = 'FOREIGN_ENEMY_MM_SMALL_RAYS_V1', small = patch.symbols.oot.get(name);
    if (small?.length !== 1 || small[0] !== marker[0] || patch.symPatches.has(name))
      throw new Error('Missing small light-ray capability');
  }
  const tableVrom = bufReadU32BE(overlay.data, marker[0] - vram[0]);
  const tables = patch.newFiles.filter(f => f.vrom === tableVrom);
  const asset = file('custom/clear_tag');
  const object = bufReadU16BE(overlay.data, profile + 8) - 0x2000;
  if (tableVrom < 0x08000000 || tables.length !== 1 || object < 0 || object >= 128 || (object + 1) * 8 > tables[0].data.length ||
    !asset.data.length || asset.vrom < 0x08000000 || bufReadU32BE(tables[0].data, object * 8) !== asset.vrom ||
    bufReadU32BE(tables[0].data, object * 8 + 4) !== asset.vrom + asset.data.length)
    throw new Error('Wrong installed light-ray object');
  const expected = assets.object;
  if (asset.data.length !== expected.data.length || !asset.data.every((v, i) => v === expected.data[i]))
    throw new Error('Light-ray object differs from original MM assets');
  for (const index of [6, 7]) {
    const dl = bufReadU32BE(overlay.data, marker[0] - vram[0] + (index - 5) * 4);
    if (dl !== expected.offsets[index] || dl >>> 24 !== 6 || dl % 8 || (dl & 0xffffff) + 8 > asset.data.length)
      throw new Error('Incorrect receiving light-ray display list');
  }
  const burst = callers.some(c => CLEAR_TAG_CALLERS.some(r => r.actorId === c.sourceActorId &&
    r.sites.some(s => s.variant !== undefined && s.variant !== 3 && s.variant !== 4)));
  if (burst) {
    const name = 'FOREIGN_ENEMY_MM_CLEAR_TAG_BURST_V1';
    const marker = patch.symbols.oot.get(name), offset = (marker?.[0] ?? 0) - vram[0];
    const keep = [0x0401a550, 0x04028290, ...Array.from({ length: 8 }, (_, i) => 0x04025a90 + i * 0x500)];
    if (marker?.length !== 1 || !Number.isInteger(offset) || offset % 4 || patch.symPatches.has(name) ||
      offset < layout.textSize + layout.dataSize || offset + (8 + keep.length) * 4 > layout.relocationOffset)
      throw new Error('Missing receiving Clear Tag burst capability');
    for (let index = 0; index < 8; ++index) {
      const dl = bufReadU32BE(overlay.data, offset + index * 4);
      if (dl !== expected.offsets[index] || dl >>> 24 !== 6 || dl % 8 || (dl & 0xffffff) + 8 > asset.data.length)
        throw new Error('Incorrect receiving Clear Tag burst display list');
    }
    if (keep.some((addr, i) => addr !== bufReadU32BE(overlay.data, offset + (i + 8) * 4)))
      throw new Error('Incorrect receiving Clear Tag native keep asset');
  }
  return { id: 0x2000 | object, size: align(asset.data.length, 16) };
}

const material = [0xe7000000, 0, 0xe3001001, 0, 0xd7000002, 0xffffffff, 0xdf000000, 0];

const model = [0xfc3097ff, 0x5ffefe38, 0xe200001c, 0x0c184f50, 0xd9f0fbff, 0, 0x01004008, 0x801c5d20, 0x06000204, 0x00000406, 0xdf000000, 0];

const vertexRom = 0xb3c000 + 0x120260;

function installMmEnemyShadow(patch: Patchfile, rom: Uint8Array) {
  const symbol = 'FOREIGN_ENEMY_MM_SHADOW_ASSETS', refs = patch.symbols.oot.get(symbol), size = 720;
  if (!refs || refs.length !== 1 || !Number.isInteger(refs[0]) || refs[0] % 16 || refs[0] < 0x80000000 ||
    refs[0] + size > 0x80800000 || patch.symPatches.has(symbol)) throw new Error('Invalid MM shadow asset export');
  const matches = (p: number, n: number, crc: number) => p + n <= rom.length && (CRC32.buf(rom.subarray(p, p + n)) >>> 0) === crc;
  if (!matches(0xc74b20, 0x2b0, 0x4dd59f2d) || !matches(vertexRom, 64, 0x978b561d) ||
    vertexRom + 144 > rom.length || ![...material, ...model].every((w, i) => bufReadU32BE(rom, vertexRom + 64 + i * 4) === w))
    throw new Error('Unexpected MM shadow graphics revision');
  const data = new Uint8Array(size);
  data.set(rom.subarray(0xc74b20, 0xc74b20 + 576));
  if (data.subarray(0, 576).some(x => x > 1)) throw new Error('Unexpected MM shadow coverage value');
  data.set(rom.subarray(vertexRom, vertexRom + 144), 576);
  bufWriteU32BE(data, 576 + 64 + 32 + 7 * 4, refs[0] + 576);
  patch.addSymbolPatch(symbol, data);
}

const MM_ICE_BLOCK_PARTICLE = {
  objectId: 0x167,
  objectSize: 0x1ae0,
  objectCrc32: 0xa740e256,
  model: 0x06000a38,
  life: 40,
} as const;

function validateMmEnemyParticleAssets(rom: Uint8Array): void {
  const entry = CONFIG.mm.objectTableAddr + MM_ICE_BLOCK_PARTICLE.objectId * 8;
  if (entry + 8 > rom.length) throw new Error('Missing MM particle source object table');
  const start = bufReadU32BE(rom, entry), end = bufReadU32BE(rom, entry + 4);
  if (!start || end - start !== MM_ICE_BLOCK_PARTICLE.objectSize || end > rom.length ||
    (CRC32.buf(rom.subarray(start, end)) >>> 0) !== MM_ICE_BLOCK_PARTICLE.objectCrc32)
    throw new Error('MM ice-block particle source object revision mismatch');
}

const MM_DAMAGE_EFFECT_SOUNDS = [
  'NA_SE_EV_BURN_OUT', 'NA_SE_EV_ICE_FREEZE',
  'NA_SE_EN_COMMON_EXTINCT_LEV', 'NA_SE_EN_COMMON_DEADLIGHT',
] as const;

const MM_DAMAGE_EFFECT_ASSETS = {
  ICE_MATERIAL: 0x04050648, ICE_MODEL: 0x040506e0,
  STEAM_MATERIAL: 0x04051180, STEAM_MODEL: 0x04051238,
  FIRE_MODEL: 0x0407d590, ORB_MATERIAL: 0x04023348, ORB_MODEL: 0x04023428,
  SPARK_MATERIAL: 0x04023480, SPARK_MODEL: 0x040234f0,
  SPARK_TEXTURE_0: 0x04091de0, SPARK_TEXTURE_1: 0x04091fe0,
  SPARK_TEXTURE_2: 0x040921e0, SPARK_TEXTURE_3: 0x040923e0,
} as const;

function validateMmDamageEffectAssets(rom: Uint8Array): void {
  const entry = CONFIG.mm.objectTableAddr + 8;
  if (entry + 8 > rom.length) throw new Error('Missing MM damage-effect source object table');
  const start = bufReadU32BE(rom, entry), end = bufReadU32BE(rom, entry + 4);
  if (!start || end - start !== 0x925e0 || end > rom.length ||
    (CRC32.buf(rom.subarray(start, end)) >>> 0) !== 0x20a1350e)
    throw new Error('MM damage-effect gameplay_keep revision mismatch');
  for (const address of Object.values(MM_DAMAGE_EFFECT_ASSETS)) {
    const offset = address & 0xffffff;
    if (address >>> 24 !== 4 || offset + 8 > end - start)
      throw new Error('Invalid MM damage-effect asset metadata');
    if (!KEEP_ASSET_GAMES.mm.entries.some(entry => entry.objectId === 0x1001 &&
      offset >= entry.sourceOffset && offset + 8 <= entry.sourceOffset + entry.size))
      throw new Error('Missing selective MM damage-effect asset: ' + address.toString(16));
  }
}

function verifyNativeEnemyEffectBoundary(game: Game, rom: Uint8Array, addresses: GameAddresses, patch: Patchfile) {
  const entry = game === 'oot' ? 0x8001b468 : 0x800b04d4;
  const file = addresses[game].fileFromRAM(entry);
  if (bufReadU32BE(rom, file.addr) !== 0x27bdffc0 || bufReadU32BE(rom, file.addr + 4) !== 0xafbf001c)
    throw new Error('Native effect constructor prologue revision mismatch');
  const payloads = patch.newFiles.filter(f => f.name === `${game}/payload`);
  const payload = payloads.length === 1 ? payloads[0] : undefined, span = payload?.vram?.[game];
  const symbol = (name: string, size = 4) => {
    const rows = patch.symbols[game].get(name);
    if (!span || span[1] - span[0] !== payload!.data.length || rows?.length !== 1 || rows[0] % 4 ||
      rows[0] < span[0] || rows[0] + size > span[1]) throw new Error('Missing native effect constructor boundary: ' + name);
    return rows[0];
  };
  const hook = symbol('FOREIGN_ENEMY_NATIVE_EFFECT_INIT_V1');
  const trampoline = symbol('FOREIGN_ENEMY_NATIVE_EFFECT_TRAMPOLINE_V1', 16);
  const offset = trampoline - span![0];
  const expectedTrampoline = [0x27bdffc0, 0xafbf001c, (0x08000000 | (((entry + 8) >>> 2) & 0x03ffffff)) >>> 0, 0];
  if (!expectedTrampoline.every((v, i) => bufReadU32BE(payload!.data, offset + i * 4) === v))
    throw new Error('Native effect constructor trampoline mismatch');
  const staged = rom.slice(file.addr, file.addr + 8), expected = new Uint8Array(8);
  bufWriteU32BE(expected, 0, (0x08000000 | ((hook >>> 2) & 0x03ffffff)) >>> 0);
  for (const edit of patch.patches[file.name] ?? []) {
    const start = Math.max(file.offset, edit.addr), end = Math.min(file.offset + 8, edit.addr + edit.data.length);
    if (start < end) staged.set(edit.data.subarray(start - edit.addr, end - edit.addr), start - file.offset);
  }
  if (!staged.every((v, i) => v === expected[i])) throw new Error('Native effect constructor hook missing or modified');
}

type EnemySound = { name: string; id: number; fixed?: boolean };

type EnemySoundBinding = { source: EnemySound; target?: EnemySound; mode: 'native' | 'import' };

function bindEnemySounds(source: readonly EnemySound[], target: readonly EnemySound[]): EnemySoundBinding[] {
  const targetByName = new Map<string, EnemySound>();
  for (const sfx of target) {
    if (targetByName.has(sfx.name)) throw new Error(`Duplicate target sound: ${sfx.name}`);
    targetByName.set(sfx.name, sfx);
  }
  const sourceIds = new Set<number>();
  return source.map(sfx => {
    if (!Number.isInteger(sfx.id) || sfx.id < 0 || sfx.id > 0xffff || sourceIds.has(sfx.id)) throw new Error('Invalid or duplicate source sound ID');
    sourceIds.add(sfx.id);
    const native = sfx.fixed ? undefined : targetByName.get(sfx.name);
    return { source: sfx, target: native, mode: native ? 'native' : 'import' };
  });
}

function serializeEnemySoundMap(game: Game, bindings: readonly EnemySoundBinding[]): Uint8Array {
  const ids = new Map<number, number>();
  for (const binding of bindings) {
    if (!binding.target) throw new Error(`Missing imported enemy sound program: ${binding.source.name}`);
    for (const id of [binding.source.id, binding.target.id])
      if (!Number.isInteger(id) || id < 0 || id > 0x6fff) throw new Error('Invalid enemy sound namespace ID');
    const source = binding.source.id & 0xf3ff, target = binding.target.id & 0xf3ff;
    if (ids.has(source) && ids.get(source) !== target) throw new Error('Ambiguous enemy sound namespace mapping');
    ids.set(source, target);
  }
  const sorted = [...ids].sort((a, b) => a[0] - b[0]);
  if (sorted.length > 7168) throw new Error('Enemy sound namespace exceeds its runtime limit');
  const data = new Uint8Array(16 + sorted.length * 4);
  [0x45534658, 1, game === 'oot' ? 0 : 1, sorted.length].forEach((value, i) => bufWriteU32BE(data, i * 4, value));
  sorted.forEach(([source, target], i) => {
    bufWriteU16BE(data, 16 + i * 4, source);
    bufWriteU16BE(data, 18 + i * 4, target);
  });
  return data;
}

function installEnemySoundMap(patch: Patchfile, game: Game, bindings: readonly EnemySoundBinding[]): void {
  const symbol = `FOREIGN_ENEMY_SFX_MAP_${game.toUpperCase()}`;
  if (patch.symPatches.has(symbol)) throw new Error('Foreign enemy sound namespace already installed');
  if (!patch.symbols[game].has(symbol)) throw new Error('Rebuild the native payload to include enemy sound namespace support');
  const data = serializeEnemySoundMap(game, bindings), size = align(data.length, 16);
  let next = game === 'oot' ? 0xd3000000 : 0xd3800000;
  const end = next + 0x800000;
  for (const file of [...patch.newFiles].sort((a, b) => a.vrom - b.vrom)) {
    const fileEnd = file.vrom + align(file.data.length, 16);
    if (fileEnd <= next) continue;
    if (file.vrom >= next + size) break;
    next = fileEnd;
  }
  if (next + size > end) throw new Error('Foreign enemy sound namespace DMA space exhausted');
  const pointer = toU32Buffer([next]);
  patch.addNewFile({ name: `enemy/audio/${game}/sfx-map`, vrom: next, data, compressed: false });
  patch.addSymbolPatch(symbol, pointer);
}

const OOT_MOBLIN_AUDIO = new Set([
  'NA_SE_EN_MONBLIN_GNDWAVE',
  'NA_SE_EN_MONBLIN_HAM_DOWN',
  'NA_SE_EN_MONBLIN_HAM_UP',
]);

const ENEMY_SOUND_METADATA = { oot: ENEMY_SOUNDS_OOT, mm: ENEMY_SOUNDS_MM };

const ENEMY_SOUND_IDS: Record<Game, Readonly<Record<string, number>>> = SFX_IDS;

const FIXED_ENEMY_SOUNDS = {
  oot: new Set((ENEMY_SOUND_METADATA.oot.sharedSounds as readonly EnemySound[]).filter(sound => sound.fixed).map(sound => sound.name)),
  mm: new Set((ENEMY_SOUND_METADATA.mm.sharedSounds as readonly EnemySound[]).filter(sound => sound.fixed).map(sound => sound.name)),
};

function requiresForeignEnemySound(game: Game, foreign: Game, name: string): boolean {
  return FIXED_ENEMY_SOUNDS[foreign].has(name) || (game === 'mm' && OOT_MOBLIN_AUDIO.has(name));
}

function partitionEnemySoundRequests(game: Game, requested: ReadonlySet<string>) {
  const foreign = game === 'oot' ? 'mm' : 'oot';
  const metadata = ENEMY_SOUND_METADATA[foreign];
  const enemies = new Set([...metadata.sounds, ...metadata.sharedSounds].map(sound => sound.name));
  const source = ENEMY_SOUND_IDS[foreign];
  const target = ENEMY_SOUND_IDS[game];
  const enemyRequests: string[] = [], nativeBindings: EnemySoundBinding[] = [];
  for (const name of [...requested].sort()) {
    if (!Object.hasOwn(source, name)) throw new Error(`Unknown source sound: ${name}`);
    if (requiresForeignEnemySound(game, foreign, name)) {
      enemyRequests.push(name);
      continue;
    }
    if (Object.hasOwn(target, name)) {
      nativeBindings.push({ source: { name, id: source[name] }, target: { name, id: target[name] }, mode: 'native' });
      continue;
    }
    if (enemies.has(name)) {
      enemyRequests.push(name);
      continue;
    }
    if ((source[name] >>> 12) === 3) throw new Error(`Enemy sound requires sequence metadata: ${name}`);
    throw new Error(`Missing shared sound requires a foreign bank adapter: ${name}`);
  }
  return { enemyRequests, nativeBindings };
}

function sharedEnemySoundBindings(game: Game): EnemySoundBinding[] {
  const foreign = game === 'oot' ? 'mm' : 'oot';
  const source = ENEMY_SOUND_IDS[foreign];
  const target = ENEMY_SOUND_IDS[game];
  return Object.keys(source).filter(name => Object.hasOwn(target, name) &&
    !requiresForeignEnemySound(game, foreign, name)).sort().map(name => ({
      source: { name, id: source[name] }, target: { name, id: target[name] }, mode: 'native',
    }));
}

function installForeignEnemyAudio(patch: Patchfile, game: Game, requested: ReadonlySet<string>,
  roms: DecompressedRoms, addresses: GameAddresses): void {
  const foreign = game === 'oot' ? 'mm' : 'oot';
  const { enemyRequests, nativeBindings } = partitionEnemySoundRequests(game, requested);
  const sharedBindings = sharedEnemySoundBindings(game);
  if (!enemyRequests.length) {
    installEnemySoundMap(patch, game, [...sharedBindings, ...nativeBindings]);
    return;
  }
  const sourceSequence = (source: Game) => {
    const rom = roms[source].rom;
    const code = addresses[source].fileFromRAM(source === 'oot' ? 0x800110a0 : 0x800a5ac0);
    const tableOffset = source === 'oot' ? 0x102ae0 : 0x13bb80;
    const audioseq = source === 'oot' ? 0x29de0 : 0x46af0;
    const offset = bufReadU32BE(rom, code.addr + tableOffset), size = bufReadU32BE(rom, code.addr + tableOffset + 4);
    if (size !== ENEMY_SOUND_METADATA[source].size || audioseq + offset + size > rom.length) throw new Error('Source sound sequence table revision mismatch');
    return rom.slice(audioseq + offset, audioseq + offset + size);
  };
  const parameters = (source: Game, bank: 3 | 6 = 3) => {
    const rom = roms[source].rom;
    const ptr = addresses[source].fileFromRAM(source === 'oot' ? 0x8010433c : 0x801db450).addr;
    const base = bufReadU32BE(rom, ptr + bank * 4);
    const count = bank === 6 ? (source === 'oot' ? 128 : 400) : (source === 'oot' ? 499 : 736);
    const first = addresses[source].fileFromRAM(base), last = addresses[source].fileFromRAM(base + count * 4 - 1);
    if (first.name !== `${source}/code` || last.name !== first.name || last.addr !== first.addr + count * 4 - 1)
      throw new Error('Native SFX parameter table is outside the original code file');
    return rom.slice(first.addr, last.addr + 1);
  };
  const foreignSharedParameters: Record<string, EnemySoundParameters> = {};
  for (const sound of ENEMY_SOUND_METADATA[foreign].sharedSounds) {
    if (!enemyRequests.includes(sound.name)) continue;
    const rom = roms[foreign].rom, bank = sound.id >>> 12, index = sound.id & 0x3ff;
    if (bank >= 7) throw new Error('Invalid shared source sound bank');
    const table = addresses[foreign].fileFromRAM(foreign === 'oot' ? 0x8010433c : 0x801db450);
    const base = bufReadU32BE(rom, table.addr + bank * 4);
    const first = addresses[foreign].fileFromRAM(base + index * 4), last = addresses[foreign].fileFromRAM(base + index * 4 + 3);
    if (first.name !== `${foreign}/code` || last.name !== first.name) throw new Error('Shared sound parameters are outside the original code file');
    foreignSharedParameters[sound.name] = { importance: rom[first.addr], flags2: rom[first.addr + 1], params: bufReadU16BE(rom, first.addr + 2) };
  }
  const original = sourceSequence(game);
  const adultVoice = game === 'mm' ? patch.newFiles.find(f => f.name === 'mm/seq_0_oot_adult_voice') : undefined;
  const sequence = adultVoice?.data ?? original;
  const nativeMetadata = ENEMY_SOUND_METADATA[game], foreignMetadata = ENEMY_SOUND_METADATA[foreign];
  const result = buildHybridEnemySoundBank({
    game, sequence,
    nativeSounds: nativeMetadata.sounds, nativeParameters: parameters(game),
    nativeVoiceParameters: foreignMetadata.sharedSounds.some(s => (s.id >>> 12) === 6 && enemyRequests.includes(s.name)) ? parameters(game, 6) : undefined,
    foreignSequence: sourceSequence(foreign), foreignGraph: foreignMetadata as unknown as SoundSequenceGraph,
    foreignSounds: [...foreignMetadata.sounds, ...foreignMetadata.sharedSounds].map(sound =>
      game === 'mm' && OOT_MOBLIN_AUDIO.has(sound.name) ? { ...sound, fixed: true } : sound),
    foreignParameters: parameters(foreign), foreignSharedParameters, requested: enemyRequests,
    mmSpatial: game === 'oot' && patch.symbols.oot.has('FOREIGN_ENEMY_AUDIO_SPATIAL_OOT')
  });
  const bindings = [...sharedBindings, ...result.bindings, ...nativeBindings];
  if (!result.imports) {
    installEnemySoundMap(patch, game, bindings);
    return;
  }
  const symbols = [2, 3].map(v => `FOREIGN_ENEMY_AUDIO_V${v}_${game.toUpperCase()}`);
  const symbol = symbols[result.voiceParameters ? 1 : 0];
  if (symbols.some(s => patch.symPatches.has(s))) throw new Error('Foreign enemy audio is already installed');
  if (!patch.symbols[game].has(symbol)) throw new Error('Rebuild the native payload to include foreign enemy audio support');
  const staged = patch.dup();
  if (game === 'oot' && result.enemyImports) {
    for (const [vram, expected] of [[0x800c8358, 0x30ff01ff], [0x800c83d4, 0x308e01ff],
    [0x800c8410, 0x308f01ff], [0x800c8664, 0x30f901ff], [0x800c91c4, 0x30a50100]]) {
      const file = addresses.oot.fileFromRAM(vram);
      if (bufReadU32BE(roms.oot.rom, file.addr) !== expected) throw new Error('OoT SFX engine revision mismatch');
      const data = new Uint8Array(4);
      bufWriteU32BE(data, 0, (expected & 0xffff0000) | (vram === 0x800c91c4 ? 0x300 : 0x3ff));
      staged.addPatch(file.name, file.offset, data);
    }
  }
  const foreignBanks = staged.newFiles.find(f => f.name === `${foreign}/bank_table`)?.data;
  if (!foreignBanks) throw new Error('Custom cross-game soundfont table is missing');
  const align = (n: number) => (n + 15) & ~15;
  const growth = align(result.sequence.length) - align(sequence.length) +
    align(bufReadU32BE(foreignBanks, 4)) + align(bufReadU32BE(foreignBanks, 0x14));
  const heapFile = addresses[game].fileFromRAM(game === 'oot' ? 0x8010a1b4 : 0x801e1104);
  const heap = roms[game].rom.slice(heapFile.addr, heapFile.addr + 12);
  for (const previous of staged.patches[heapFile.name] ?? []) {
    const start = Math.max(previous.addr, heapFile.offset), end = Math.min(previous.addr + previous.data.length, heapFile.offset + 12);
    if (start < end) heap.set(previous.data.subarray(start - previous.addr, end - previous.addr), start - heapFile.offset);
  }
  for (const field of [4, 8]) bufWriteU32BE(heap, field, align(bufReadU32BE(heap, field) + growth));
  if (bufReadU32BE(heap, 4) >= bufReadU32BE(heap, 0)) throw new Error('Foreign SFX exhaust the native audio heap');
  staged.addPatch(heapFile.name, heapFile.offset, heap);
  const prefix = `enemy/audio/${game}/`, end = game === 'oot' ? 0xd2800000 : 0xd3000000;
  let next = game === 'oot' ? 0xd2000000 : 0xd2800000;
  const add = (name: string, data: Uint8Array) => {
    const size = align(data.length);
    for (const file of [...staged.newFiles].sort((a, b) => a.vrom - b.vrom)) {
      if (file.vrom + align(file.data.length) <= next) continue;
      if (file.vrom >= next + size) break;
      next = file.vrom + align(file.data.length);
    }
    if (next + size > end) throw new Error('Foreign sound DMA space exhausted');
    const vrom = next;
    next += size;
    staged.addNewFile({ name: prefix + name, vrom, data, compressed: false });
    return vrom;
  };
  const allParams = new Uint8Array(result.parameters.length + (result.voiceParameters?.length ?? 0));
  allParams.set(result.parameters);
  if (result.voiceParameters) allParams.set(result.voiceParameters, result.parameters.length);
  const seqVrom = add('sequence', result.sequence), paramsVrom = add('parameters', allParams);
  const words = [0x46585331, result.voiceParameters ? 3 : 2, game === 'oot' ? 0 : 1, seqVrom, result.sequence.length,
    paramsVrom, result.parameters.length / 4, result.programStart, result.programEnd];
  if (result.voiceParameters) words.push(result.voiceParameters.length / 4);
  const pointer = toU32Buffer([add('config', toU32Buffer(words))]);
  staged.addSymbolPatch(symbol, pointer);
  installEnemySoundMap(staged, game, bindings);
  patch.newFiles = staged.newFiles;
  patch.patches = staged.patches;
  patch.symPatches = staged.symPatches;
}

type SoundSequenceReference = { offset: number; width: 1 | 2; target: number; relative?: boolean };

type SoundSequenceNode = {
  offset: number;
  size: number;
  section: 'channel' | 'layer' | 'envelope' | 'array' | 'table' | 'filter';
  next: boolean;
  references: readonly SoundSequenceReference[];
  fonts?: readonly number[];
  unsupportedOn?: readonly Game[];
};

type SoundSequenceAdapters = { mmLayerFlags?: boolean; mmSampleStartZero?: boolean; channelCalls?: ReadonlyMap<number, number> };

type SoundSequenceGraph = {
  game: Game;
  size: number;
  crc32: number;
  nodes: readonly SoundSequenceNode[];
  entries: Readonly<Record<string, number>>;
};

function compileForeignSoundPrograms(source: Uint8Array, graph: SoundSequenceGraph, target: Game,
  names: readonly string[], targetOffset: number, adapters: SoundSequenceAdapters = {}) {
  if (target === graph.game || !Number.isInteger(targetOffset) || targetOffset < 0 || targetOffset > 0xffff)
    throw new Error('Invalid foreign sound destination');
  if (source.length !== graph.size || (CRC32.buf(source) >>> 0) !== graph.crc32)
    throw new Error('Source SFX sequence revision mismatch');
  const nodes = [...graph.nodes].sort((a, b) => a.offset - b.offset);
  const byOffset = new Map<number, SoundSequenceNode>();
  let end = 0;
  for (const node of nodes) {
    if (!Number.isInteger(node.offset) || !Number.isInteger(node.size) || node.offset < end || node.size < 1 || node.offset + node.size > source.length)
      throw new Error('Invalid or overlapping source sound metadata');
    byOffset.set(node.offset, node);
    end = node.offset + node.size;
    for (const ref of node.references) {
      if (![1, 2].includes(ref.width) || !Number.isInteger(ref.offset) || ref.offset < 0 || ref.offset + ref.width > node.size ||
        !Number.isInteger(ref.target) || ref.target < 0 || ref.target >= source.length)
        throw new Error('Invalid source sound reference');
      let actual = ref.width === 1 ? source[node.offset + ref.offset] : bufReadU16BE(source, node.offset + ref.offset);
      if (ref.relative) {
        if (actual & (ref.width === 1 ? 0x80 : 0x8000)) actual -= ref.width === 1 ? 0x100 : 0x10000;
        actual += node.offset + ref.offset + ref.width;
      }
      if (actual !== ref.target) throw new Error('Source sound pointer fingerprint mismatch');
    }
    const fields = [...node.references.map(r => [r.offset, r.offset + r.width]), ...(node.fonts ?? []).map(f => [f, f + 1])];
    fields.sort((a, b) => a[0] - b[0]);
    for (let i = 0; i < fields.length; ++i) if (!Number.isInteger(fields[i][0]) || fields[i][0] < 0 || fields[i][1] > node.size || (i && fields[i][0] < fields[i - 1][1]))
      throw new Error('Overlapping source sound relocation fields');
  }
  const containing = (address: number) => {
    let low = 0, high = nodes.length;
    while (low < high) {
      const mid = (low + high) >>> 1;
      if (nodes[mid].offset <= address) low = mid + 1; else high = mid;
    }
    const node = nodes[low - 1];
    if (!node || address >= node.offset + node.size) throw new Error(`Unresolved source sound data at 0x${address.toString(16)}`);
    return node;
  };
  const externalCalls = adapters.channelCalls ?? new Map<number, number>();
  for (const [sourceOffset, targetOffset] of externalCalls) {
    if (!Number.isInteger(sourceOffset) || !Number.isInteger(targetOffset) || targetOffset < 0 || targetOffset > 0xffff ||
      byOffset.get(sourceOffset)?.section !== 'channel') throw new Error('Invalid native sound helper binding');
  }
  const external = (node: SoundSequenceNode, ref: SoundSequenceReference) => {
    if (!externalCalls.has(ref.target)) return undefined;
    if (node.section !== 'channel' || source[node.offset] !== 0xfc || node.size !== 3 || ref.offset !== 1 || ref.width !== 2 || ref.relative)
      throw new Error('Native sound helper binding requires a direct channel call');
    return externalCalls.get(ref.target)!;
  };
  const replacements = new Map<number, Uint8Array>();
  const selected = new Set<number>(), pending: SoundSequenceNode[] = [];
  const include = (node: SoundSequenceNode) => {
    if (!selected.has(node.offset)) {
      selected.add(node.offset);
      pending.push(node);
    }
  };
  for (const name of names) {
    const entry = graph.entries[name], node = byOffset.get(entry);
    if (!node || node.section !== 'channel') throw new Error(`Unknown foreign sound channel: ${name}`);
    include(node);
  }
  for (let i = 0; i < pending.length; ++i) {
    const node = pending[i];
    if (node.unsupportedOn?.includes(target)) {
      const opcode = source[node.offset];
      if (graph.game === 'mm' && target === 'oot' && node.section === 'layer' && opcode === 0xf0 && node.size === 3 &&
        adapters.mmLayerFlags && (bufReadU16BE(source, node.offset + 1) & ~0x1040) === 0) {
      } else if (graph.game === 'mm' && target === 'oot' && node.section === 'channel' && opcode === 0xbd &&
        node.size === 2 && source[node.offset + 1] === 0 && adapters.mmSampleStartZero) {
        replacements.set(node.offset, Uint8Array.of(0xc9, 0xff));
      } else throw new Error(`Foreign sound opcode needs a ${target} adapter at 0x${node.offset.toString(16)}`);
    }
    if (graph.game === 'oot' && target === 'mm' && node.offset === 0x4826) {
      replacements.set(node.offset, Uint8Array.of(...source.subarray(node.offset, node.offset + node.size), 0xff));
    } else if (node.next) {
      const next = byOffset.get(node.offset + node.size);
      if (!next) throw new Error('Source sound falls through unclassified data');
      include(next);
    }
    for (const ref of node.references) if (external(node, ref) === undefined) include(containing(ref.target));
  }
  const offsets = new Map<number, number>();
  let cursor = targetOffset;
  for (const node of nodes.filter(n => selected.has(n.offset))) {
    cursor += (node.offset - cursor) & 15;
    offsets.set(node.offset, cursor);
    cursor += replacements.get(node.offset)?.length ?? node.size;
    if (cursor > 0x10000) throw new Error('Hybrid SFX sequence exceeds 16-bit addressing');
  }
  const data = new Uint8Array(cursor - targetOffset);
  for (const node of nodes.filter(n => selected.has(n.offset))) {
    const destination = offsets.get(node.offset)!;
    data.set(replacements.get(node.offset) ?? source.subarray(node.offset, node.offset + node.size), destination - targetOffset);
    for (const ref of node.references) {
      const native = external(node, ref);
      const referencedNode = native === undefined ? containing(ref.target) : undefined;
      let translated = native ?? (offsets.get(referencedNode!.offset)! + ref.target - referencedNode!.offset);
      if (ref.relative) translated -= destination + ref.offset + ref.width;
      const minimum = ref.relative ? (ref.width === 1 ? -0x80 : -0x8000) : 0;
      const maximum = ref.relative ? (ref.width === 1 ? 0x7f : 0x7fff) : (ref.width === 1 ? 0xff : 0xffff);
      if (translated < minimum || translated > maximum) throw new Error('Imported sound pointer exceeds its encoded range');
      const field = destination - targetOffset + ref.offset;
      if (ref.width === 1) data[field] = translated & 0xff;
      else bufWriteU16BE(data, field, translated & 0xffff);
    }
    for (const field of node.fonts ?? []) {
      const original = source[node.offset + field];
      if (original > 1) throw new Error('Foreign sound uses an unregistered source font');
      data[destination - targetOffset + field] = original + 2;
    }
  }
  const entries = Object.fromEntries(names.map(name => [name, offsets.get(graph.entries[name])!]));
  return { data, entries, offsets };
}

type EnemySoundParameters = { importance: number; flags2: number; params: number };

function translateEnemySoundParameters(value: EnemySoundParameters, source: Game, target: Game, mmSpatial = false): Uint8Array {
  const { importance, flags2, params } = value;
  if (![importance, flags2, params].every(Number.isInteger) || importance < 0 || importance > 255 ||
    flags2 < 0 || flags2 > 255 || params < 0 || params > 0xffff)
    throw new Error('Invalid enemy sound parameters');
  const result = new Uint8Array(4);
  result[0] = importance;
  let translated = params;
  if (source === 'oot' && flags2 !== 0) throw new Error('OoT sound parameter padding must be zero');
  if (source !== target) {
    if (source === 'oot') {
      result[1] = (params >>> 2) & 1;
      translated &= ~4;
    } else {
      if (((params & 7) > 3 && !mmSpatial) || (flags2 & ~1)) throw new Error('MM sound needs a spatial parameter adapter');
      result[1] = (params & 4) >>> 1;
      translated = (params & ~4) | ((flags2 & 1) << 2);
    }
  } else result[1] = flags2;
  bufWriteU16BE(result, 2, translated);
  return result;
}

class SoundAssembler {
  private bytes: number[] = [];
  private labels = new Map<string, number>();
  private refs: { offset: number; target: string | number }[] = [];
  constructor(readonly base: number) { }
  get cursor() {
    return this.base + this.bytes.length;
  }
  label(name: string) {
    if (this.labels.has(name)) throw new Error('Duplicate sound label');
    this.labels.set(name, this.cursor);
  }
  emit(...bytes: number[]) {
    this.bytes.push(...bytes);
  }
  align(alignment: number) {
    while (this.cursor % alignment) this.emit(0);
  }
  ptr(target: string | number) {
    this.refs.push({ offset: this.bytes.length, target });
    this.emit(0, 0);
  }
  jump(opcode: number, target: string | number) {
    this.emit(opcode);
    this.ptr(target);
  }
  address(name: string) {
    const value = this.labels.get(name);
    if (value === undefined) throw new Error(`Missing sound label: ${name}`);
    return value;
  }
  finish() {
    if (this.cursor > 0x10000) throw new Error('Hybrid SFX sequence exceeds 16-bit addressing');
    const result = Uint8Array.from(this.bytes);
    for (const ref of this.refs) {
      const target = typeof ref.target === 'number' ? ref.target : this.address(ref.target);
      if (!Number.isInteger(target) || target < 0 || target > 0xffff) throw new Error('Invalid sound sequence pointer');
      bufWriteU16BE(result, ref.offset, target);
    }
    return result;
  }
}

const VOICE_ENTRIES = {
  oot: [
    ['NA_SE_VO_LI_SWORD_N', 0x6800, 0x615d], ['NA_SE_VO_LI_SWORD_L', 0x6801, 0x6175], ['NA_SE_VO_LI_LASH', 0x6802, 0x619b],
    ['NA_SE_VO_LI_DAMAGE_S', 0x6805, 0x61ea], ['NA_SE_VO_LI_TAKEN_AWAY', 0x680c, 0x626f],
    ['NA_SE_VO_LI_AUTO_JUMP', 0x6814, 0x62ce], ['NA_SE_VO_LI_DAMAGE_S_KID', 0x6825, 0x63af],
    ['NA_SE_VO_LI_TAKEN_AWAY_KID', 0x682c, 0x6434], ['NA_SE_VO_BL_DOWN', 0x681d, 0x622d],
  ],
  mm: [
    ['NA_SE_VO_LI_DAMAGE_S', 0x6805, 0xb72a], ['NA_SE_VO_DUMMY_37', 0x6825, 0xb927],
    ['NA_SE_VO_DUMMY_133', 0x6885, 0xbc83], ['NA_SE_VO_DUMMY_165', 0x68a5, 0xbd1c],
    ['NA_SE_VO_DUMMY_197', 0x68c5, 0xbda3],
  ],
} as const;

function mmComputedDamageVoices(overlay: string): readonly string[] {
  return ['En_Bigslime', 'En_Raf', 'En_Dragon'].includes(overlay) ? VOICE_ENTRIES.mm.map(([name]) => name) : [];
}

type HybridEnemySoundInput = {
  game: Game;
  sequence: Uint8Array;
  nativeSounds: readonly EnemySound[];
  nativeParameters: Uint8Array;
  nativeVoiceParameters?: Uint8Array;
  foreignSequence: Uint8Array;
  foreignGraph: SoundSequenceGraph;
  foreignSounds: readonly EnemySound[];
  foreignParameters: Uint8Array;
  foreignSharedParameters?: Readonly<Record<string, EnemySoundParameters>>;
  requested: readonly string[];
  mmSpatial?: boolean;
};

function emitVoiceDispatch(b: SoundAssembler, input: HybridEnemySoundInput, voices: readonly EnemySoundBinding[],
  linked: Readonly<Record<string, number>>) {
  const { game, foreignGraph } = input;
  const nativeCount = game === 'oot' ? 128 : 400;
  if (input.nativeVoiceParameters?.length !== nativeCount * 4) throw new Error('Native voice parameter table revision mismatch');
  const rows = voices.map(binding => {
    const { name, id, fixed } = binding.source;
    const ordinal = VOICE_ENTRIES[foreignGraph.game].findIndex(([n, i, entry]) =>
      name === n && id === i && foreignGraph.entries[name] === entry);
    const params = input.foreignSharedParameters?.[name];
    if (!fixed || ordinal < 0 || params?.importance !== (name === 'NA_SE_VO_BL_DOWN' ? 0x80 : 0x30) || params.flags2 !== 0 || params.params !== 0x402)
      throw new Error(`Unreviewed foreign voice dispatcher entry: ${name}`);
    const index = game === 'oot' ? nativeCount + ordinal : 0x200 + (id & 0xff);
    binding.target = { name, id: 0x6800 + index };
    return { name, index, id, params };
  });
  const parameters = new Uint8Array(4 * (Math.max(...rows.map(row => row.index)) + 1));
  parameters.set(input.nativeVoiceParameters);
  b.label('voice-dispatch');
  b.emit(0xcc, 0, 0x71);
  if (game === 'mm') {
    b.emit(0x65, 0xc8, 4);
    b.jump(0xfa, 'voice-page');
    b.jump(0xfb, 'voice-native');
    b.label('voice-page');
  } else {
    b.emit(0xd8, 0);
  }
  for (const { index } of rows) {
    b.emit(0x64, 0xc8, index & 0xff);
    b.jump(0xfa, `voice-${index}`);
  }
  b.label('voice-native');
  b.emit(0xc6, 0);
  b.jump(0xfb, game === 'oot' ? 0x5fc7 : 0xb2d8);
  for (const { name, index, id, params } of rows) {
    parameters.set(translateEnemySoundParameters(params, foreignGraph.game, game), index * 4);
    b.label(`voice-${index}`);
    b.emit(0xc6, 2, 0xc1, 0x7e, 0xcc, id & 0x7f);
    b.jump(0xfc, linked[name]);
    b.jump(0xfb, game === 'oot' ? 0x5fd6 : 0xb2e8);
  }
  return parameters;
}

function buildHybridEnemySoundBank(input: HybridEnemySoundInput) {
  const { game, sequence, nativeSounds, foreignSounds, foreignGraph } = input;
  if (game === foreignGraph.game) throw new Error('Sound import requires the other game');
  const nativeCount = game === 'oot' ? 499 : 736;
  if (input.nativeParameters.length !== nativeCount * 4 || input.foreignParameters.length !== (game === 'oot' ? 736 : 499) * 4)
    throw new Error('Enemy sound parameter table revision mismatch');
  for (const [group, sounds] of [nativeSounds, foreignSounds].entries()) {
    const ids = new Set<number>(), names = new Set<string>();
    for (const sound of sounds) {
      if (!Number.isInteger(sound.id) || sound.id < 0 || sound.id >= 0x7000 ||
        (group === 0 && (sound.id < 0x3800 || sound.id >= 0x3c00)) || names.has(sound.name) || ids.has(sound.id))
        throw new Error('Invalid enemy sound bank definition');
      ids.add(sound.id);
      names.add(sound.name);
    }
  }
  const requested = [...new Set(input.requested)].sort();
  const definitions = new Map(foreignSounds.map(s => [s.name, s]));
  const selected = requested.map(name => {
    const sound = definitions.get(name);
    if (!sound) throw new Error(`Unknown foreign enemy sound: ${name}`);
    return sound;
  });
  const bindings = bindEnemySounds(selected, nativeSounds);
  const imports = bindings.filter(b => b.mode === 'import');
  const voices = imports.filter(b => (b.source.id >>> 12) === 6);
  const enemies = imports.filter(b => (b.source.id >>> 12) !== 6);
  if (!imports.length) return { sequence: sequence.slice(), parameters: input.nativeParameters.slice(), bindings, imports: 0, enemyImports: 0, voiceParameters: undefined, programStart: 0, programEnd: 0 };
  if (nativeCount + enemies.length > 1024) throw new Error('Hybrid enemy sound bank exceeds its 10-bit index');

  const fingerprint = (offset: number, expected: readonly number[]) => {
    if (offset + expected.length > sequence.length || expected.some((v, i) => sequence[offset + i] !== v))
      throw new Error('Native enemy sound dispatcher revision mismatch');
  };
  const resetCall = game === 'oot' ? 0x310e : 0x5634;
  const resetRoutine = game === 'oot' ? 0x7c : 0x76;
  const voiceHook = game === 'oot' ? 0x5fc4 : 0xb2d5;
  if (voices.length) fingerprint(voiceHook, game === 'oot' ?
    [0xcc, 0, 0x71, 0x64, 0xf5, 0x5f, 0xd2] : [0xcc, 0, 0x71, 0x65, 0xc9, 0x7f, 0xb2, 0xb2, 0xfe]);
  fingerprint(resetCall, [0xfc, 0, resetRoutine]);
  if (game === 'oot') fingerprint(0x3119, [0x65, 0xc8, 1, 0xf3, 0x0b]);
  else fingerprint(0x5645, [0xb2, 0x56, 0x6e]);
  const originalTable = game === 'oot' ? 0x315e : 0x567e;
  const nativeProgramCount = game === 'oot' ? 499 : 733;
  if (originalTable + nativeProgramCount * 2 > sequence.length) throw new Error('Truncated native sound pointer table');

  const helpers = new SoundAssembler(sequence.length);
  const calls = new Map<number, number>();
  if (game === 'oot') {
    const helper = helpers.cursor;
    helpers.emit(0x63, 0xc7, 0);
    helpers.ptr(helper + 6);

    helpers.emit(0xb3, 0, 0xfb, 0, 0x84);
    if (foreignGraph.nodes.some(node => node.offset === 0x7e && node.section === 'channel')) calls.set(0x7e, helper);
  }
  const helperData = helpers.finish();
  const programStart = sequence.length + helperData.length;
  const linked = compileForeignSoundPrograms(input.foreignSequence, foreignGraph, game, imports.map(b => b.source.name), programStart,
    { mmLayerFlags: true, mmSampleStartZero: true, channelCalls: calls });
  const b = new SoundAssembler(programStart + linked.data.length);
  const voiceParameters = voices.length ? emitVoiceDispatch(b, input, voices, linked.entries) : undefined;
  const parameters = new Uint8Array((nativeCount + enemies.length) * 4);
  parameters.set(input.nativeParameters);
  if (enemies.length) {
    b.label('reset');
    b.emit(0xc6, 1);
    b.jump(0xfc, resetRoutine);
    b.emit(0xff);
    const entries: (string | number)[] = Array.from({ length: 1024 }, () => 'silent');
    for (let i = 0; i < nativeProgramCount; ++i) entries[i] = bufReadU16BE(sequence, originalTable + i * 2);
    enemies.forEach((binding, i) => {
      const index = nativeCount + i, sourceIndex = binding.source.id & 0x3ff, label = `foreign-${i}`;
      if ((binding.source.id >>> 12) !== 3 && !input.foreignSharedParameters?.[binding.source.name])
        throw new Error('Missing original shared-bank sound parameters');
      if (!input.foreignSharedParameters?.[binding.source.name] && sourceIndex * 4 + 4 > input.foreignParameters.length)
        throw new Error('Foreign sound parameter index exceeds its source table');
      binding.target = { name: binding.source.name, id: 0x3800 + index };
      entries[index] = label;
      const offset = sourceIndex * 4;
      const original = input.foreignSharedParameters?.[binding.source.name] ?? {
        importance: input.foreignParameters[offset], flags2: input.foreignParameters[offset + 1],
        params: bufReadU16BE(input.foreignParameters, offset + 2)
      };
      parameters.set(translateEnemySoundParameters(original, foreignGraph.game, game, input.mmSpatial), index * 4);
      b.label(label);
      const high = foreignGraph.game === 'oot' ? sourceIndex >>> 8 : sourceIndex >>> 7;
      const font = (binding.source.id >>> 12) === 3 ? 3 : 2;
      b.emit(0xc6, font, 0xcc, sourceIndex & 255, 0x74, 0xcc, high, 0x75);
      b.jump(0xfb, linked.entries[binding.source.name]);
    });
    b.label('silent');
    b.emit(0xff);
    b.align(2);
    b.label('entries');
    entries.forEach(entry => b.ptr(entry));
    const table = b.address('entries');
    if (game === 'mm') {
      b.label('pages');
      for (let page = 0; page < 8; ++page) b.ptr(table + page * 256);
    } else {
      b.label('dispatch');
      for (let chunk = 0; chunk < 3; ++chunk) {
        b.emit(0x65, 0xc8, chunk);
        b.jump(0xfa, `chunk-${chunk}`);
      }
      b.jump(0xfb, 'chunk-3');
      for (let chunk = 0; chunk < 4; ++chunk) {
        b.label(`chunk-${chunk}`);
        b.emit(0x64);
        b.jump(0xf5, `positive-${chunk}`);
        b.emit(0xc9, 0x7f);
        b.jump(0xc2, table + (chunk * 2 + 1) * 256);
        b.jump(0xfb, 'play');
        b.label(`positive-${chunk}`);
        b.jump(0xc2, table + chunk * 512);
        b.jump(0xfb, 'play');
      }
      b.label('play');
      b.jump(0xda, 0x6a7c);
      b.emit(0xd9, 0xfb, 0xe4);
      b.jump(0xfb, 0x3140);
    }
  }
  const tail = b.finish();
  const result = new Uint8Array(programStart + linked.data.length + tail.length);
  result.set(sequence);
  result.set(helperData, sequence.length);
  result.set(linked.data, programStart);
  result.set(tail, programStart + linked.data.length);
  if (voiceParameters) {
    result[voiceHook] = 0xfb;
    bufWriteU16BE(result, voiceHook + 1, b.address('voice-dispatch'));
  }
  if (enemies.length) {
    bufWriteU16BE(result, resetCall + 1, b.address('reset'));
    if (game === 'mm') bufWriteU16BE(result, 0x5646, b.address('pages'));
    else {
      result[0x3119] = 0xfb;
      bufWriteU16BE(result, 0x311a, b.address('dispatch'));
    }
  }
  return {
    sequence: result, parameters, voiceParameters, bindings: bindings as EnemySoundBinding[], imports: imports.length, enemyImports: enemies.length,
    programStart, programEnd: programStart + linked.data.length
  };
}
