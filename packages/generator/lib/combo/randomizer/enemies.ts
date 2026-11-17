import { soulPreviewSources, storeSoulPreview } from '../enemies/soul-preview-storage';
import { ENEMY_SOUL_PREVIEW_MEMORY, ENEMY_UTILITY_MODULE_BYTES } from '../../../../core/src/enemy-soul-preview-memory-data';
import { bakeSoulPreview, buildSoulPreviewMeshes } from '../enemies/soul-preview';
import { verifyOotGameplayMemory } from '../enemies/oot-gameplay-memory';
import { NATIVE_LILY_FACTS } from '../enemies/extra-port-data';
import { align, toU32Buffer } from '../util';
import {
  OOT_FIELD_STALCHILD_SLOTS, OOT_DESERT_LEEVER_SLOTS, ENEMY_GOLD_SOURCES, nativeGoldTarget, verifyMmRoomOverlaps, mmRoomOverlapStates, enemySceneMemoryReports, enemyOotSceneMemoryReports, OOT_GAMEPLAY_POLICY, ENEMY_MEMORY_POLICY,
  enemyWeight, enemyFormWeights, ootNativeEnemyDefinitionKey, ARWING_ENEMIES,
  standaloneEnemyTransfer, ENEMIES, nativeResourceSource, nativeResourceTarget, NATIVE_ARENA_COMBAT_TARGETS,
  nativeFightRestorationSource, nativeFightRestorationRoomAllowed, nativeFightRestorationCombat, scriptedMmSource, scriptedMmTargetAllowed, BIG_OCTO_ARENA_COMBAT_TARGETS,
  nativeMinibossVariant, nativeArenaCombatTarget, enemyDefinition, assertForeignEnemyExclusive, enemyEncounterCompatible,
  enemyCanRespawn, enemyShuffleExcluded, nativeEnemyOnDemandObjects, mmAquaticTarget, tentacleSlot,
  mmAquaticSlot, enemyIsCarnivorousLily, ootWaterSurfaceSlot, mmWaterSurfaceSlot, tentacleSlotAt, MM_NATIVE_SCENES, isOotEnemizerNativeExtensionChoice, ootNativeChoiceAllows,
  OOT_ENEMIZER, ENEMY_SOURCE_ADDITIONS, ootEnemizerLocationKey,
  type EnemyDefinition, type EnemyPlan, type Settings, type Monitor, type BigOctoArenaCombatTargetKey, type NativeArenaCombatTargetKey, type NativeMinibossVariantKey, type OotEnemizerReplacement, type OotEnemizerChoice,
} from '@ootmm/core';
import { installDebugEnemyCatalog } from '../enemies/debug-catalog';
import {
  verifyOotEnemySourceCorrections, validateNativePeahatPlacement, reviewedNativeLayoutFile, installedEnemyDependency, readOverlayLayout, nativeCombatProfileCategory, verifyNativeEnemyPortSource, verifyEnemyRoomEntryHook, installOotEnemyNativeArena, nativeEnemyObjectDependencies, prepareForeignEnemies,
  type ForeignEnemyPlacement, type ForeignEnemyProgram, type NativeEnemyPort,
} from '../enemies/overlay';
import {
  oot_placement as oot,
  mm_placement as mm,
  oot_placement_adapters as adapters,
  oot_enemizer_runtime as ootRuntimeFacts,
  native_flying_pot as flyingPotFacts,
  native_iron_knuckle as nativeIronKnuckleFacts,
  native_skullwalltula as source,
  native_renewable_baba as nativeRenewableBabaFacts,
  native_guay as guayFacts,
  native_stalchild as stalchildFacts,
  native_poe as poeFacts,
  native_big_octo_activation as nativeBigOctoFacts,
  fighter_restoration as fighterFacts,
  scripted_mm_arenas as scriptedArenaFacts,
  keep_assets as keepFacts,
  native_extra_mm_sources as sources,
  native_extra_mm_facts as standaloneFacts,
  native_ports as nativePorts,
} from '../enemies/enemy-rom-data';
import { bufEqual, matchesRomBytes, bufReadI16BE, bufReadU16BE, bufReadU32BE, bufWriteU16BE, bufWriteU32BE } from '../util/buffer';
import type { GameAddresses } from '../addresses';
import { Patchfile } from '../patch-build/patchfile';
import type { DecompressedRoms } from '../decompress';
import CRC32 from 'crc-32';
import { enemy_placement_data as placementData, enemy_big_octo_slot as slot } from '../../../../core/src/enemy-data';
import { GI } from '../data';
import { CONFIG } from '../config';
import { ENGINE_ACTORS, type Game } from '@ootmm/data';

type Change = { offset: number; value: number };

type EnemyRoomLocation = { scene: number; room: number; setup: number };

type Actor = Pick<EnemyDefinition, 'game' | 'actorId' | 'spawnParams'>;

const ootRooms: Readonly<Record<string, Readonly<Record<number, readonly number[]>>>> = oot.rooms;

function ootPlacementSlot(mq: boolean, loc: EnemyRoomLocation, slot: number, source: number) {
  const row = ootRooms[`${mq ? 'mq' : 'base'}/${loc.scene}/${loc.room}/${loc.setup}`]?.[slot];
  return row?.[0] === source ? row : undefined;
}

type SceneRules = Readonly<Record<number, readonly number[]>>;

const mmSourceScenes: SceneRules = mm.sourceScenes;

const mmTargetScenes: SceneRules = mm.targetScenes;

const mmSensitive: SceneRules = mm.sensitiveSources;

const mmPairs: Readonly<Record<number, SceneRules>> = mm.blockedPairs;

const mmRoomCaps: Readonly<Record<number, number>> = {
  0x0b: 2, 0x0c: 3, 0x47: 1, 0x109: 2, 0x14b: 1,
};

function mmClearCombatant(id: number, params: number) {
  switch (id) {
    case 0x0b: return params === 0;
    case 0x0c: return params === 0 || params === 2 || params === 3 || params === 0x8002 || params === 0x8003;
    case 0x12: return params === 0xffff || params === 0xfffd;
    case 0x66: return params === 1 || params === 2;
    case 0x50: return params === 0xff00;
    default: return false;
  }
}

const mmKeepSource: Readonly<Record<number, readonly number[]>> = {
  0x08: [0x13, 0x49], 0x14: [0x07], 0x19: [0x1b, 0x21, 0x60, 0x66],
  0x1d: [0x69], 0x24: [0x1b, 0x27, 0x28, 0x49], 0x33: [0x1b], 0x3b: [0x46],
  0x4c: [0x4b, 0x13], 0xed: [0x43, 0x28], 0x12d: [0x49], 0x14a: [0x49],
  0x164: [0x1b], 0x1ba: [0x1b], 0x1f3: [0x16, 0x18], 0x204: [0x23],
};

const mmBlockTarget: Readonly<Record<number, readonly number[]>> = {
  0x19: [0x0c], 0x14a: [0x00],
};

const mmFairies: Readonly<Record<string, readonly number[]>> = {
  '22/1': [2], '24/1': [3, 1], '27/1': [4, 34], '27/3': [3], '27/5': [22],
  '33/11': [2, 3], '73/8': [7],
};

const mmClearRooms: Readonly<Record<number, readonly number[]>> = {
  0x0c: [1, 2, 4], 0x07: [7, 13, 8], 0x16: [4, 7, 10], 0x18: [4],
  0x1b: [4, 6, 7, 8, 9], 0x1d: [5], 0x21: [1, 2, 5, 6, 9, 12],
  0x23: [0, 1, 2], 0x42: [0, 1], 0x49: [3, 5, 7], 0x4b: [12], 0x4f: [1],
  0x60: [2, 3, 4, 5], 0x66: [1, 2, 3],
};

function enemySlotAllows(game: Game, mq: boolean, loc: EnemyRoomLocation, slot: number, source: number, target: Actor, params?: number, csId?: number): boolean {
  if (enemyCanRespawn(target.game, target.actorId, target.spawnParams)) return false;
  if (game !== target.game && !standaloneEnemyTransfer({ game, actorId: source }, target)) return false;
  if (game === 'mm') {
    if (!nativeMmSkullwalltulaSlotAllows(source, target, params, csId)) return false;
    if (!nativeMmSkulltulaSlotAllows(source, target, params, csId)) return false;
    const scene = loc.scene;
    if (mmKeepSource[source]?.includes(scene) || mmSourceScenes[source]?.includes(scene) ||
      mmSourceScenes[source]?.includes(-1) || mmFairies[`${scene}/${loc.room}`]?.includes(slot)) return false;
    if (mmSensitive[scene]?.includes(source)) return false;
    if (mmClearRooms[scene]?.includes(loc.room)) {
      if (![0x16, 0x18, 0x1b, 0x21, 0x49].includes(scene) || csId !== 0x7f ||
        params === undefined || target.game !== 'mm' || !mmClearCombatant(source, params) ||
        !mmClearCombatant(target.actorId, target.spawnParams)) return false;
    }
    if (target.game !== 'mm') return true;
    return !mmBlockTarget[target.actorId]?.includes(scene) && !mmTargetScenes[target.actorId]?.includes(scene) &&
      !mmPairs[scene]?.[source]?.includes(target.actorId);
  }
  if (!nativeOotSlotAllows(mq, loc, slot, source, target, params)) return false;
  if (target.game === 'mm' && target.actorId === 0x15b && target.spawnParams === 0x019f &&
    (source !== 0x13 || params !== 0)) return false;
  const row = ootPlacementSlot(mq, loc, slot, source);
  if (!row) return false;
  if ((source === 0xc5 || source === 0xec) && row[2] !== 1) return false;
  const policy = oot.policies[row[1]];
  return target.game === 'oot' ? policy.native.includes(`${target.actorId}/${target.spawnParams}`) || nativeOotStalchildPolicyAllows(policy, target) || nativeOotPoePolicyAllows(policy, target) || nativeOotRenewableBabaPolicyAllows(policy, target) : policy.foreignOrdinary;
}

const ootPositionAdapters = new Map<string, readonly number[]>();

if (adapters.version !== 1) throw new Error('Unknown pinned OoT position adapter version');

for (const row of adapters.rows) {
  if (row.length !== 11 || row.some(n => !Number.isInteger(n)) || row[0] !== 0 ||
    ![0, 1].includes(row[1]) || row[7] < 1 || row[7] & ~31 || (row[7] & 4 && row[7] & 16) ||
    row.slice(8).some(n => n < -32768 || n > 32767)) throw new Error('Invalid pinned OoT position adapter');
  const key = row.slice(1, 7).join('/');
  if (ootPositionAdapters.has(key)) throw new Error('Duplicate pinned OoT position adapter');
  ootPositionAdapters.set(key, row);
}

type OotPlacementAdapter = {
  floorOperation: 0 | 32;
  flags: number;
  position: readonly [number, number, number];
};

type OotPlacementAlias = { mq: boolean; loc: EnemyRoomLocation; slot: number; source: number };

function ootPlacementAdapter(mq: boolean, loc: EnemyRoomLocation, slot: number, source: number): OotPlacementAdapter | undefined {
  const owner = ootPlacementSlot(mq, loc, slot, source);
  if (!owner) return;
  const policy = oot.policies[owner[1]];
  if (!policy.native.length && !policy.foreignOrdinary) return;
  const row = ootPositionAdapters.get([Number(mq), loc.scene, loc.room, loc.setup, slot, source].join('/'));
  const flags = row?.[7] ?? 0;
  const position = (row?.slice(8) ?? [0, 0, 0]) as [number, number, number];
  return { floorOperation: flags & 1 ? 0 : 32, flags, position };
}

function ootPlacementAdaptersAgree(aliases: readonly OotPlacementAlias[]): boolean {
  const first = aliases[0];
  if (!first) return false;
  const reference = ootPlacementAdapter(first.mq, first.loc, first.slot, first.source);
  return !!reference && aliases.every(({ mq, loc, slot, source }, index) => {
    if (index === 0) return true;
    const adapter = ootPlacementAdapter(mq, loc, slot, source);
    return !!adapter && adapter.flags === reference.flags &&
      adapter.position.every((value, axis) => value === reference.position[axis]);
  });
}

function ootPlacementPositionChanges(rom: Uint8Array, offset: number, mq: boolean,
  loc: EnemyRoomLocation, slot: number, source: number): { changes: Change[]; floorOperation: 0 | 32 } | undefined {
  const fact = ootPlacementAdapter(mq, loc, slot, source);
  if (!fact) return;
  if (!Number.isSafeInteger(offset) || offset < 0 || offset + 16 > rom.length || bufReadU16BE(rom, offset) !== source)
    throw new Error('OoT numeric position source drift');
  const changes: Change[] = [];
  for (let axis = 0; axis < 3; ++axis) {
    const at = offset + 2 + axis * 2;
    const signed = bufReadI16BE(rom, at);
    const value = fact.flags & (2 << axis)
      ? fact.position[axis]
      : axis === 1 && (fact.flags & 16) ? signed + fact.position[1] : signed;
    if (value < -32768 || value > 32767) throw new Error('OoT numeric position exceeds actor coordinates');
    if ((value & 0xffff) !== (signed & 0xffff)) changes.push({ offset: at, value: value & 0xffff });
  }
  return { changes, floorOperation: fact.floorOperation };
}

function matchesMmActorEntry(rom: Uint8Array, actorOffset: number, raw: readonly number[]): boolean {
  return actorOffset >= 0 && actorOffset + 16 <= rom.length && raw.length === 16 &&
    raw.every((value, index) => rom[actorOffset + index] === value);
}

function verifyMmSkullwalltulaSlot(rom: Uint8Array, placement: NonNullable<EnemyPlan[number]['placement']>) {
  const { actorOffset, sourceActorId, sourceParams, targetParams, raw } = placement;
  if ((bufReadU16BE(rom, actorOffset) & 0xfff) !== sourceActorId ||
    bufReadU16BE(rom, actorOffset + 14) !== sourceParams ||
    !nativeMmSkullwalltulaSlotAllows(sourceActorId, { game: 'mm', actorId: 0x50, spawnParams: targetParams },
      sourceParams, ((raw[10] << 8) | raw[11]) & 0x7f))
    throw new Error('MM native Skullwalltula requires a certified authored wall slot without cutscene');
}

function planExactMmPlacements(rom: Uint8Array, plan: EnemyPlan, foreign: ReadonlyMap<string, ForeignEnemyPlacement>,
  addresses: GameAddresses, patch: Patchfile): Change[] {
  const writes = new Map<number, number>();
  for (const { placement, target } of plan) {
    if (!placement || placement.sourceActorId === 0x13a) continue;
    const { actorOffset, raw, targetGame, targetActorId, targetParams, dryAquatic } = placement;
    if (!matchesMmActorEntry(rom, actorOffset, raw))
      throw new Error('MM original actor entry mismatch at ' + actorOffset.toString(16));
    if (targetGame === 'mm' && targetActorId === 0x50) verifyMmSkullwalltulaSlot(rom, placement);

    let actorId = targetActorId;
    if (targetGame !== 'mm') {
      const imported = foreign.get(target);
      if (!imported || imported.targetGame !== 'mm' || imported.sourceGame !== targetGame ||
        imported.sourceActorId !== targetActorId || imported.actorId !== 0xc00 + targetActorId)
        throw new Error('Missing MM foreign target adapter: ' + target);
      actorId = imported.actorId;
    }
    writes.set(actorOffset, (bufReadU16BE(rom, actorOffset) & 0xf000) | actorId);
    writes.set(actorOffset + 14, targetParams);
    if (enemyIsCarnivorousLily(targetGame, targetActorId) && mmWaterSurfaceSlot(actorOffset))
      writes.set(actorOffset + 4, lilyWaterSurfaceY('mm', rom, actorOffset, mmWaterSurfaceSlot(actorOffset)!, addresses, patch) & 0xffff);
    if (targetGame === 'mm' && targetActorId === 0x1a8)
      writes.set(actorOffset + 4, nativeBigOctoPlacementY(rom, actorOffset, addresses, patch) & 0xffff);
    if (!dryAquatic && mmAquaticTarget(targetGame, targetActorId, targetParams) &&
      (targetActorId !== 0x12d || !!mmAquaticSlot(actorOffset)))
      writes.set(actorOffset + 4, mmAquaticPlacementY(rom, actorOffset, addresses, patch) & 0xffff);
    if (targetGame === 'oot' && targetActorId === 0xde)
      writes.set(actorOffset + 4, nativeTentaclePlacementY('mm', rom, actorOffset, addresses, patch) & 0xffff);
  }
  return [...writes].map(([offset, value]) => ({ offset, value }));
}

const MM_PLACEMENT_OPERATIONS: Readonly<Partial<Record<number, number>>> = {
  0x13a: 31, 0x3e: 30, 0xf1: 30, 0x14a: 30, 0x15d: 25, 0x23f: 24, 0x1a8: 21, 0x184: 19, 0x291: 18, 0x1e8: 13,
  0x8d: 15, 0x208: 11, 0x21d: 12, 0x113: 9, 0x216: 10,
};

const elevatedEnemySources = new Set(ENEMIES.filter(enemy => enemy.habitat === 'air')
  .map(enemy => `${enemy.game}/${enemy.actorId}`));

for (const [game, ids] of [['oot', [0x37, 0x95]], ['mm', [0x24, 0x50]]] as const)
  for (const id of ids) elevatedEnemySources.add(`${game}/${id}`);

function needsGroundPlacement(host: Game, source: number, target: EnemyDefinition | undefined): boolean {
  return !!target && target.habitat === 'ground' &&
    !mmAquaticTarget(target.game, target.actorId, target.spawnParams) &&
    !(target.game === 'oot' ? [0x37, 0x95, 0xde].includes(target.actorId) : [0x24, 0x50].includes(target.actorId)) &&
    elevatedEnemySources.has(`${host}/${source}`);
}

function exactMmActivationRecords(rom: Uint8Array, plan: EnemyPlan, patch: Patchfile): NativePlacementRecord[] {
  const relocations = patch.meta.enemyRoomRelocations ?? [];
  return plan.flatMap(row => {
    const placement = row.placement;
    if (!placement || placement.sourceActorId === 0x13a) return [];
    const { actorOffset, raw, targetGame, targetActorId, targetParams, sourceActorId, dryAquatic } = placement;
    const peahat = targetGame === 'mm' && targetActorId === 0x14 && targetParams === 0 && sourceActorId !== 0x14;
    const skullwall = targetGame === 'mm' && targetActorId === 0x50;
    const definition = ENEMIES.find(enemy => enemy.key === row.target);
    if (!definition) throw new Error('Missing selected MM native target definition');

    const imported = targetGame !== 'mm';
    const attached = skullwall || targetActorId === 0x24;
    const air = !attached && (definition.habitat === 'air' || dryAquatic);
    const base = imported ? 0 : dryAquatic ? 29 : skullwall ? 5 : peahat ? 4 : MM_PLACEMENT_OPERATIONS[targetActorId] ?? 0;
    const grounded = !dryAquatic && needsGroundPlacement('mm', sourceActorId, definition);
    const operation = base | (imported ? 0 : (air ? 256 : 0) | (attached ? 0 : 512)) | (grounded ? 1024 : 0);
    if (imported && !grounded) return [];
    const runtimeId = targetActorId + (imported ? 0xc00 : 0);
    if (!operation && !row.chaoticMmChoices?.length) return [];
    if (!matchesMmActorEntry(rom, actorOffset, raw))
      throw new Error('MM activation original actor entry mismatch');
    if (skullwall) verifyMmSkullwalltulaSlot(rom, placement);

    const changes = [
      { offset: actorOffset, value: (bufReadU16BE(rom, actorOffset) & 0xf000) | runtimeId },
      { offset: actorOffset + 14, value: targetParams },
    ];
    if (enemyIsCarnivorousLily(targetGame, targetActorId) && mmWaterSurfaceSlot(actorOffset))
      changes.push({ offset: actorOffset + 4, value: lilyWaterSurfaceY('mm', rom, actorOffset, mmWaterSurfaceSlot(actorOffset)!) & 0xffff });
    if (targetActorId === 0x1a8)
      changes.push({ offset: actorOffset + 4, value: nativeBigOctoPlacementY(rom, actorOffset) & 0xffff });
    else if (!dryAquatic && mmAquaticTarget(targetGame, targetActorId, targetParams) &&
      (targetActorId !== 0x12d || !!mmAquaticSlot(actorOffset)))
      changes.push({ offset: actorOffset + 4, value: mmAquaticPlacementY(rom, actorOffset) & 0xffff });

    const record = nativePlacementRecord(rom, actorOffset, operation, changes, runtimeId, targetParams);
    const remapped = remapRelocatedRoomKey(record.vrom, relocations);
    if (remapped !== record.vrom) {
      record.sourceVrom = record.vrom;
      record.vrom = remapped;
    }
    return [record];
  });
}

function applyExactMmPlacements(rom: Uint8Array, addresses: GameAddresses, patch: Patchfile, plan: EnemyPlan,
  foreign: ReadonlyMap<string, ForeignEnemyPlacement>): number {
  const staged = patch.dup();
  for (const { offset, value } of planExactMmPlacements(rom, plan, foreign, addresses, staged)) {
    const address = addresses.mm.fileFromROM(offset);
    staged.addPatch(address.name, address.offset, new Uint8Array([value >>> 8, value & 255]));
  }
  const count = plan.filter(row => row.placement).length;
  patch.patches = staged.patches;
  patch.newFiles = staged.newFiles;
  patch.removedFiles = staged.removedFiles;
  patch.meta = staged.meta;
  return count;
}

const ootActorKey = ({ mq, scene, room, setup, slot }: OotEnemizerPhysicalKey) => [+mq, scene, room, setup, slot].join('/');

const mqDungeonScene = (dungeon: number) => dungeon === 11 ? 13 : dungeon === 10 ? 11 : dungeon;

function mqActorVrom(scene: number, room: number, slot: number): number {
  return 0xe0000000 + scene * 0x100000 + room * 0x1000 + slot;
}

type Resolved = {
  offset: number;
  vrom: number;
  data: Uint8Array;
  mq?: NativePlacementRecord['mq'];
};

function applyExactOotEnemizer(roms: DecompressedRoms, addresses: GameAddresses, patch: Patchfile, plan: EnemyPlan,
  foreignTargets: ReadonlyMap<string, ForeignEnemyPlacement>) {
  const rows = plan.filter((row): row is OotEnemizerReplacement => !!row.ootr);
  const rom = roms.oot.rom;
  verifyOotEnemySourceCorrections(rom);
  if (rows.some(({ ootr: placement }) => !placement.foreign && placement.target.actorId === 0x0d) &&
    !verifyNativeOotPoe(rom, addresses, patch))
    throw Error('Native combined Poe source/profile/model revision mismatch');

  const actors = new Map<string, Resolved>();
  for (const room of collectEnemyRooms(rom, 'oot')) {
    for (const header of room.headers) {
      if (!header.actors) continue;
      for (let slot = 0; slot < header.actors.count; ++slot) {
        const { scene, room, setup } = header;
        const offset = header.actors.offset + slot * 16;
        const actorKey = ootActorKey({ scene, room, setup, slot, mq: false });
        const previous = actors.get(actorKey);
        if (previous && previous.offset !== offset) throw Error('Conflicting original OoT room aliases');
        actors.set(actorKey, { offset, vrom: offset, data: rom });
      }
    }
  }
  const mqIndex = patch.newFiles.findIndex(file => file.name === 'custom/mq_rooms');
  const mqSource = patch.newFiles[mqIndex]?.data;
  const mqCopy = mqSource && Uint8Array.from(mqSource);
  if (mqSource) {
    if (mqSource.length < 0x20) throw Error('Truncated MQ room directory');
    const count = bufReadU32BE(mqSource, 0);
    const directoryEnd = 0x20 + count * 0x20;
    if (directoryEnd > mqSource.length) throw Error('Truncated MQ room directory');
    for (let i = 0; i < count; ++i) {
      const header = 0x20 + i * 0x20;
      const start = bufReadU32BE(mqSource, header + 4);
      const end = start + bufReadU32BE(mqSource, header);
      const dungeon = mqSource[header + 8];
      const room = mqSource[header + 9];
      if (dungeon > 11 || start < directoryEnd || end > mqSource.length)
        throw Error('Invalid MQ room bounds');
      const scene = mqDungeonScene(dungeon);
      const offset = start + bufReadU16BE(mqSource, header + 12);
      const actorCount = bufReadU16BE(mqSource, header + 10);
      if (offset < start || offset + actorCount * 16 > end) throw Error('Invalid MQ actor bounds');
      for (let slot = 0; slot < actorCount; ++slot) {
        const at = offset + slot * 16;
        const actorKey = ootActorKey({ scene, room, setup: 0, slot, mq: true });
        if (actors.has(actorKey)) throw Error('Duplicate MQ authored actor key');
        actors.set(actorKey, {
          offset: at,
          vrom: mqActorVrom(scene, room, slot),
          data: mqSource,
          mq: { scene, room, slot, offset: at, source: mqSource },
        });
      }
    }
  }
  const records = new Map<number, NativePlacementRecord>();
  const switchRecords: OotEnemizerSwitchRecord[] = [];
  const missing: OotEnemizerPhysicalKey[] = [];
  const splitChoices = new Map<number, EnemySplitChoices>();
  const writes = new Map<number, { resolved: Resolved; replacement: Uint8Array }>();
  let replaced = 0;
  let mqReplaced = 0;
  for (const row of rows) {
    const choice = row.ootr;
    const keys: OotEnemizerPhysicalKey[] = [
      { ...choice.location },
      ...choice.aliases.map(([scene, room, setup, slot]) => ({ scene, room, setup, slot, mq: false })),
    ];
    let imported: { actorId: number; params: number } | undefined;
    if (choice.foreign) {
      const target = foreignTargets.get(row.target);
      if (!target || target.targetGame !== 'oot' || target.sourceGame !== 'mm' ||
        target.sourceActorId !== choice.target.actorId || target.actorId !== 0xc00 + target.sourceActorId)
        throw Error('Missing exact OoT foreign target adapter');
      imported = { actorId: target.actorId, params: choice.targetParams };
    }
    for (const key of keys) {
      const resolved = actors.get(ootActorKey(key));
      if (!resolved) {
        missing.push(key);
        continue;
      }
      const original = resolved.data.subarray(resolved.offset, resolved.offset + 16);
      const entry = prepareOotEnemizerActor(choice, original, key, imported);
      if (!entry) continue;
      const previous = writes.get(resolved.vrom);
      if (previous && !bufEqual(entry.replacement, previous.replacement))
        throw Error('Conflicting exact OoT alias choices');
      if (!previous) {
        writes.set(resolved.vrom, { resolved, replacement: entry.replacement });
        if (key.mq) ++mqReplaced;
        else ++replaced;
      }
      if (row.splitChildren)
        splitChoices.set(resolved.vrom, { seeded: row.splitChildren, pool: row.splitPool });
      if (entry.operation && !records.has(resolved.vrom)) {
        const changes = Array.from({ length: 8 }, (_, index) => {
          const offset = index * 2;
          return { offset, value: bufReadU16BE(entry.replacement, offset) };
        });
        const record = nativePlacementRecord(original, 0, entry.operation, changes, entry.targetId, entry.targetParams);
        record.vrom = resolved.vrom;
        if (resolved.mq) record.mq = resolved.mq;
        record.ootPlacement = { choice, key };
        records.set(record.vrom, record);
      }
      if (entry.switchFlag >= 0)
        switchRecords.push({ key: resolved.vrom, scene: key.scene, room: key.room, switchFlag: entry.switchFlag });
    }
  }
  if (missing.length) throw Error('Missing authored OoT enemy locations: ' + missing.map(ootActorKey).join(', '));
  const nativeWrites = [...writes.values()]
    .filter(({ resolved }) => !resolved.mq)
    .map(({ resolved, replacement }) => ({ address: addresses.oot.fileFromROM(resolved.offset), replacement }));
  for (const { resolved, replacement } of writes.values())
    if (resolved.mq) mqCopy!.set(replacement, resolved.offset);
  for (const { address, replacement } of nativeWrites)
    patch.addPatch(address.name, address.offset, replacement);
  if (mqIndex >= 0 && mqCopy)
    patch.newFiles[mqIndex] = { ...patch.newFiles[mqIndex], data: mqCopy };
  return { records: [...records.values()], replaced, mqReplaced, switchRecords, splitChoices };
}

type OotEnemizerEntryResult = {
  replacement: Uint8Array;
  operation: number;
  switchFlag: number;
  targetId: number;
  targetParams: number;
};

type OotEnemizerPhysicalKey = EnemyRoomLocation & { slot: number; mq: boolean };

const OOT_ACTOR_ADJUSTMENTS: Readonly<Partial<Record<number, number>>> = {
  0x91: 13,
  0x11d: 15,
  0x175: 11,
  0x197: 12,
  0x1c: 10,
  0x1b0: 3,
  0x1c0: 7,
  0x115: 16,
};

function prepareOotEnemizerActor(choice: OotEnemizerChoice, original: Uint8Array, key: OotEnemizerPhysicalKey,
  imported?: { actorId: number; params?: number }): OotEnemizerEntryResult | undefined {
  if (original.length !== 16) throw Error('Invalid OoT ActorEntry length');
  const locationKey = ootEnemizerLocationKey(key);
  const canonical = choice.location.mq === key.mq && ootEnemizerLocationKey(choice.location) === locationKey;
  const alias = !key.mq && choice.aliases.some(parts => parts.join('/') === locationKey);
  if (!canonical && !alias) throw Error('Unrelated OoT actor alias');
  if (bufReadU16BE(original, 0) === 0xffff) return undefined;
  const metadata = canonical ? choice.location : OOT_ENEMIZER.locations.find(row =>
    row.mq === key.mq && ootEnemizerLocationKey(row) === locationKey);
  const added = ENEMY_SOURCE_ADDITIONS.originals.find(row => row.mq === key.mq &&
    row.scene === key.scene && row.room === key.room && row.setup === key.setup && row.slot === key.slot);
  if (added && !matchesRomBytes(original, 0, added.raw))
    throw Error('Reviewed OoT enemy source entry changed');
  const out = Uint8Array.from(original);
  const overrides = metadata?.varOverrides as Readonly<Record<string, number>> | undefined;
  const id = imported?.actorId ?? choice.target.actorId;
  const params = imported?.params ?? overrides?.[choice.target.name] ?? choice.target.params;
  if (enemyCanRespawn(choice.foreign || imported ? 'mm' : 'oot', choice.target.actorId, params))
    throw Error('Respawning enemy cannot be used as a shuffle replacement');
  bufWriteU16BE(out, 0, id);
  bufWriteU16BE(out, 8, 0);
  bufWriteU16BE(out, 12, 0);
  bufWriteU16BE(out, 14, params);
  for (const edit of metadata?.position ?? []) {
    const [axis, mode, amount] = edit as [string, string, number];
    const offset = axis === 'x' ? 2 : axis === 'y' ? 4 : 6;
    const signed = bufReadI16BE(out, offset);
    bufWriteU16BE(out, offset, (mode === 'add' ? signed + amount : amount) & 0xffff);
  }
  const target = ENEMIES.find(enemy => enemy.game === (imported ? 'mm' : 'oot') &&
    enemy.actorId === choice.target.actorId && enemy.spawnParams === (imported ? choice.target.params : params));
  const ceiling = !imported && id === 0xde ? tentacleSlot('oot', key) : undefined;
  if (!imported && id === 0xde) {
    if (!ceiling || ceiling.original.length !== original.length * 2 || !matchesRomBytes(original, 0, ceiling.original))
      throw new Error('Uncertified native Tentacle ceiling slot');
    bufWriteU16BE(out, 4, Math.round(ceiling.ceilingY - 100) & 0xffff);
  }
  const lily = !!imported && enemyIsCarnivorousLily('mm', choice.target.actorId);
  const lilySurface = lily ? ootWaterSurfaceSlot(key) : undefined;
  if (lily && !choice.ignoreRestrictions && !lilySurface)
    throw Error('Carnivorous Lily requires a certified OoT water-surface slot');
  if (lilySurface) {
    if (!matchesRomBytes(original, 0, lilySurface.original)) throw Error('OoT water-surface actor entry changed');
    bufWriteU16BE(out, 4, lilySurface.surfaceY & 0xffff);
  }
  const attached = id === 0x37 || id === 0x95 || !!ceiling;
  const grounded = needsGroundPlacement('oot', bufReadU16BE(original, 0), target);
  const traits = (imported ? 0 : (attached ? 0 : 512) | (!attached && target?.habitat === 'air' ? 256 : 0)) |
    (grounded ? 1024 : 0);
  const dryAquatic = !imported && choice.ignoreRestrictions && !choice.location.choices.includes(choice.targetIndex) &&
    ['Octorok', 'Stinger', 'Stingray'].includes(choice.target.name) &&
    !(metadata?.restrictions as readonly string[] | undefined)?.some(rule => rule === 'UNDERWATER' || rule === 'ABOVE_WATER') &&
    !(metadata?.meets as readonly string[] | undefined)?.includes('ABOVE_WATER');
  const raycast = ceiling || lilySurface || grounded || metadata?.skipRaycast || (!imported && id === 0x95) ? 0 : 32;
  let adjustment = 0;
  if (!imported) {
    if (dryAquatic) adjustment = 28;
    else if (ceiling) adjustment = 20;
    else if (id === 0x69 && [0xfd, 0x2fc, 0x2fb].includes(params)) adjustment = 17;
    else if (id === 0x113 && [0xff81, 0xff82, 0xff83].includes(params)) adjustment = 64;
    else if (id === 0xc7 && params === 1) adjustment = 6;
    else adjustment = OOT_ACTOR_ADJUSTMENTS[id] ?? 0;
  }
  const operation = traits | raycast | adjustment;
  return { replacement: out, operation, switchFlag: metadata?.switchFlag ?? -1, targetId: id, targetParams: params };
}

function applyOverlappingEdits(bytes: Uint8Array, offset: number, edits: readonly { addr: number; data: Uint8Array }[], size = bytes.length) {
  for (const { addr, data } of edits) {
    const start = Math.max(offset, addr);
    const end = Math.min(offset + size, addr + data.length);
    if (start < end) bytes.set(data.subarray(start - addr, end - addr), start - offset);
  }
}

function installOotEnemizerRuntime(rom: Uint8Array, addresses: GameAddresses, patch: Patchfile): void {
  const payloads = patch.newFiles.filter(file => file.name === 'oot/payload');
  const span = payloads.length === 1 ? payloads[0].vram?.oot : undefined;
  const writes: { name: string; offset: number; data: Uint8Array }[] = [];
  for (const actor of ootRuntimeFacts.actors) {
    if (actor.end > rom.length || (CRC32.buf(rom.subarray(actor.start, actor.end)) >>> 0) !== actor.crc32)
      throw Error('OoT native actor revision mismatch');
    const signature = actor.tableBytes;
    if (signature.length % 2 || /[^0-9a-f]/i.test(signature))
      throw Error('Invalid OoT native actor table bytes');
    if (!matchesRomBytes(rom, actor.table, signature)) throw Error('OoT native actor table mismatch');
    const file = addresses.oot.fileFromROM(actor.start);
    const last = addresses.oot.fileFromROM(actor.end - 1);
    if (file.offset || last.name !== file.name || last.offset !== actor.end - actor.start - 1 ||
      patch.removedFiles.includes(file.name) || patch.newFiles.some(entry => entry.name === file.name))
      throw Error('OoT native actor replacement conflict');
    for (const change of actor.changes) {
      if (bufReadU32BE(rom, change.offset) !== change.original)
        throw Error('OoT native instruction revision mismatch');
      let word: number;
      if (typeof change.replacement === 'number') {
        word = change.replacement;
      } else {
        const symbols = patch.symbols.oot.get(change.replacement);
        if (symbols?.length !== 1 || !span || span[1] - span[0] !== payloads[0].data.length ||
          symbols[0] % 4 || symbols[0] < span[0] || symbols[0] >= span[1])
          throw Error('Missing OoT native runtime callback: ' + change.replacement);
        const target = symbols[0];
        word = change.address === 0x80957638 ? target : (0x0c000000 | ((target >>> 2) & 0x3ffffff)) >>> 0;
      }
      const offset = change.offset - actor.start;
      const staged = Uint8Array.from(rom.subarray(change.offset, change.offset + 4));
      applyOverlappingEdits(staged, offset, patch.patches[file.name] ?? []);
      const previous = bufReadU32BE(staged, 0);
      if (previous !== change.original && previous !== word)
        throw Error('Conflicting OoT native overlay instruction');
      writes.push({ name: file.name, offset, data: toU32Buffer([word]) });
    }
  }
  for (const { name, offset, data } of writes) patch.addPatch(name, offset, data);
}

type OotEnemizerSwitchRecord = {
  key: number;
  scene: number;
  room: number;
  switchFlag: number;
};

function installOotEnemizerSwitches(records: readonly OotEnemizerSwitchRecord[], patch: Patchfile): void {
  const rows = new Map<number, OotEnemizerSwitchRecord>();
  for (const row of records) {
    const { key, scene, room, switchFlag } = row;
    if (!Number.isInteger(key) || key < 0 || key > 0xffffffff ||
      !Number.isInteger(scene) || scene < 0 || scene > 100 ||
      !Number.isInteger(room) || room < 0 || room > 255 ||
      !Number.isInteger(switchFlag) || switchFlag < 0 || switchFlag >= 64)
      throw Error('Invalid OoT kill switch record');
    const previous = rows.get(key);
    if (previous && (previous.scene !== scene || previous.room !== room || previous.switchFlag !== switchFlag))
      throw Error('Conflicting OoT kill switch aliases');
    rows.set(key, row);
  }
  if (!rows.size) return;
  if (rows.size > 2048) throw Error('OoT kill switch table capacity');
  const pointerSymbol = 'ENEMY_OOT_ENEMIZER_SWITCHES';
  if (patch.symPatches.has(pointerSymbol) || patch.symbols.oot.get(pointerSymbol)?.length !== 1 ||
    patch.symbols.oot.get('ENEMY_OOT_ENEMIZER_SWITCHES_V1')?.length !== 1)
    throw Error('Missing/conflicting OoT kill switch capability');

  const recordSize = 12;
  const header = toU32Buffer([0x454e4f53, 1, recordSize, rows.size]);
  const data = new Uint8Array(header.length + rows.size * recordSize);
  data.set(header);
  [...rows.values()].sort((a, b) => a.key - b.key).forEach((row, i) => {
    const offset = header.length + i * recordSize;
    bufWriteU32BE(data, offset, row.key);
    bufWriteU32BE(data, offset + 4, (row.scene << 16) | row.room);
    bufWriteU32BE(data, offset + 8, row.switchFlag);
  });
  const vrom = allocatePlacementMetadata(patch, 'oot', data.length, 'OoT kill switch metadata space');
  patch.addNewFile({ name: 'enemy/native/oot/switches', vrom, data, compressed: false });
  patch.addSymbolPatch(pointerSymbol, toU32Buffer([vrom]));
}

const MM_STANDALONE_MINIBOSSES = new Map<number, readonly [number, string, number]>([
  [0x182, [0x189, 'GaroMaster', 0x155]],
  [0x43, [0x18a, 'Gomess', 0x52]],
  [0x12c, [0x19d, 'Wart', 0x15d]],
  [0x7, [0x1b2, 'Snapper', 0x128]],
  [0x65, [0x1bf, 'Slime', 0x128]],
  [0x115, [0x1c6, 'Igos', 0x156]],
  [0x28f, [0x1cb, 'Keeta', 0x268]],
]);

type NativePlacementRecord = {
  vrom: number;
  operation: number;
  weight?: number;
  ootPlacement?: { choice: OotEnemizerChoice; key: OotEnemizerPhysicalKey };
  sourceVrom?: number;
  placementAliases?: readonly OotPlacementAlias[];
  mq?: Pick<EnemyRoomLocation, 'scene' | 'room'> & { slot: number; offset: number; source: Uint8Array };
  original: Uint8Array;
  replacement: Uint8Array;
  targetId: number;
  targetParams: number;
};

function nativePlacementRecord(rom: Uint8Array, offset: number, operation: number,
  changes: readonly Change[], targetId?: number, targetParams?: number): NativePlacementRecord {
  if (!Number.isSafeInteger(offset) || offset < 0 || offset % 4 || offset + 16 > rom.length)
    throw new Error('Invalid original native enemy placement');
  const original = Uint8Array.from(rom.subarray(offset, offset + 16));
  const replacement = original.slice();
  for (const change of changes) {
    if (!(change.offset >= offset && change.offset < offset + 16)) continue;
    if (change.offset % 2 || !Number.isInteger(change.value) || change.value < 0 || change.value > 0xffff)
      throw new Error('Invalid native placement edit');
    bufWriteU16BE(replacement, change.offset - offset, change.value);
  }
  return {
    vrom: offset, operation, original, replacement,
    targetId: targetId ?? (bufReadU16BE(replacement, 0) & 0x1fff), targetParams: targetParams ?? bufReadU16BE(replacement, 14)
  };
}

const NATIVE_PLACEMENT_RECORD_SIZE = 44;

function writeNativePlacement(data: Uint8Array, offset: number, row: NativePlacementRecord) {
  bufWriteU32BE(data, offset, row.vrom);
  bufWriteU32BE(data, offset + 4, row.operation);
  data.set(row.original, offset + 8);
  data.set(row.replacement, offset + 24);
  bufWriteU16BE(data, offset + 40, row.targetId);
  bufWriteU16BE(data, offset + 42, row.targetParams);
}

function enemyWeightThresholds(weights: readonly number[]): number[] {
  const total = weights.reduce((sum, weight) => sum + weight, 0);
  let cumulative = 0;
  return weights.map(weight => {
    cumulative += weight;
    return Math.round(cumulative / total * 0x1000000);
  });
}

function allocatePlacementMetadata(patch: Patchfile, game: Game, size: number, error: string) {
  const base = game === 'oot' ? 0xd6000000 : 0xd6800000;
  const end = base + 0x800000;
  let vrom = base;
  for (const file of [...patch.newFiles].sort((a, b) => a.vrom - b.vrom)) {
    const fileEnd = file.vrom + file.data.length;
    if (fileEnd <= vrom) continue;
    if (file.vrom >= vrom + size) break;
    vrom = align(fileEnd, 16);
  }
  if (vrom + size > end) throw new Error(error);
  return vrom;
}

function installChaoticPlacements(game: Game, patch: Patchfile, pools: ReadonlyMap<number, readonly NativePlacementRecord[]>) {
  const rows = [...pools].filter(([, choices]) => choices.length > 0).sort(([a], [b]) => a - b);
  if (!rows.length) return;
  const symbol = `ENEMY_CHAOTIC_PLACEMENTS_${game.toUpperCase()}`;
  if (patch.symbols[game].get(symbol)?.length !== 1) throw new Error('Missing chaotic placement runtime');
  const header = toU32Buffer([0x454e5043, 3, game === 'oot' ? 0 : 1, rows.length]);
  const indexSize = 12;
  const choiceSize = 24;
  const indexEnd = header.length + rows.length * indexSize;
  const choiceCount = rows.reduce((sum, [, choices]) => sum + choices.length, 0);
  const data = new Uint8Array(indexEnd + choiceCount * choiceSize);
  data.set(header);
  let cursor = indexEnd;
  rows.forEach(([key, choices], i) => {
    const offset = header.length + i * indexSize;
    bufWriteU32BE(data, offset, key);
    bufWriteU32BE(data, offset + 4, cursor);
    bufWriteU32BE(data, offset + 8, choices.length);
    const thresholds = enemyWeightThresholds(choices.map(choice => choice.weight ?? 1));
    for (const [index, choice] of choices.entries()) {
      bufWriteU32BE(data, cursor, choice.operation);
      data.set(choice.replacement, cursor + 4);
      bufWriteU32BE(data, cursor + 20, thresholds[index]);
      cursor += choiceSize;
    }
  });
  const vrom = allocatePlacementMetadata(patch, game, data.length, 'Chaotic placement metadata capacity exceeded');
  patch.addNewFile({ name: `enemy/native/${game}/chaotic`, vrom, data, compressed: false });
  patch.addSymbolPatch(symbol, toU32Buffer([vrom]));
}

function installNativePlacements(game: Game, rom: Uint8Array, patch: Patchfile, records: readonly NativePlacementRecord[], addresses: GameAddresses): void {
  if (!records.length) return;
  verifyEnemyRoomEntryHook(game, rom);
  for (const symbol of ['ENEMY_NATIVE_PLACEMENTS_V1', `ENEMY_NATIVE_PLACEMENTS_${game.toUpperCase()}`]) {
    const value = patch.symbols[game].get(symbol);
    if (value?.length !== 1 || value[0] % 4 || value[0] < 0x80000000 || value[0] >= 0x80800000)
      throw new Error('Rebuild native payload for enemy placement lifecycle: ' + symbol);
  }
  if (game === 'oot' && records.some(row => row.targetId === 0x90 && [5, 6, 7].includes(row.targetParams & 0xff))) {
    if (patch.symbols.oot.get('ENEMY_NATIVE_DANCING_REDEAD_OOT_V1')?.length !== 1 ||
      !matchesRomBytes(rom, 0xcd9678, '8093b9a0') || !matchesRomBytes(rom, 0xcd99d8, '82000018'))
      throw new Error('Missing native Dancing ReDead update/relocation adapter');
  }
  const checkedSources = new Set<string>();
  const verifySource = (key: string, check: () => void) => {
    if (checkedSources.has(key)) return;
    check();
    checkedSources.add(key);
  };
  const sorted = new Map<number, NativePlacementRecord>();
  for (const row of records) {
    const floor = !!(row.operation & 32), traits = row.operation & (256 | 512 | 1024), operation = row.operation & ~(32 | 256 | 512 | 1024);
    if (!Number.isInteger(row.operation) || row.operation < 0 || (operation && ![1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 24, 25, 28, 29, 30, 31, 64, 128, 129].includes(operation)))
      throw new Error('Unsupported native placement operation');
    if (traits & 1024) {
      if (patch.symbols[game].get('ENEMY_GROUND_PLACEMENT_V1')?.length !== 1)
        throw Error('Missing safe enemy floor placement runtime');
      const imported = row.targetId >= 0xc00;
      const target = ENEMIES.find(enemy => enemy.game === (imported ? (game === 'oot' ? 'mm' : 'oot') : game) &&
        enemy.actorId === row.targetId - (imported ? 0xc00 : 0) &&
        (enemy.spawnParams === row.targetParams || enemy.params.includes(row.targetParams)));
      if (!needsGroundPlacement(game, bufReadU16BE(row.original, 0) & 0x1fff, target))
        throw Error(`Invalid grounded enemy source/target policy: ${game} source ${bufReadU16BE(row.original, 0).toString(16)} target ${row.targetId.toString(16)}/${row.targetParams.toString(16)} (${target?.key})`);
    }
    if (traits & (256 | 512)) {
      const capability = patch.symbols[game].get('ENEMY_NATIVE_PLACEMENT_TRAITS_V1');
      if (capability?.length !== 1 || capability[0] % 4 || capability[0] < 0x80000000 || capability[0] >= 0x80800000 ||
        row.targetId >= 0xc00 || (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId ||
        bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        !ENEMIES.some(enemy => enemy.game === game && enemy.actorId === row.targetId &&
          (enemy.spawnParams === row.targetParams || enemy.params.includes(row.targetParams))))
        throw new Error('Missing certified native enemy movement policy');
    }
    const standalone = game === 'mm' && MM_STANDALONE_MINIBOSSES.get(row.targetId);
    if (standalone && row.targetParams === 0x807f) {
      const hook = addresses.mm.fileFromRAM(0x800bae14);
      const symbol = patch.symbols.mm.get('ENEMY_NATIVE_DUEL_SPAWN_V2');
      const jump = symbol?.length === 1 ? (0x08000000 | ((symbol[0] >>> 2) & 0x03ffffff)) : undefined;
      const edit = hook && patch.patches[hook.name]?.find(row =>
        row.addr <= hook.offset && hook.offset + 4 <= row.addr + row.data.length);
      if ((!traits && operation !== 128) || (traits & 256) || !hook || !edit ||
        bufReadU32BE(edit.data, hook.offset - edit.addr) !== jump)
        throw Error('Missing standalone native miniboss spawn dispatch');
      const [slot, name, object] = standalone;
      const { overlay, profile } = installedEnemyDependency(patch, 'mm', addresses.mm, slot, 5,
        'ovl/mm/mm_ovl_Standalone_' + name + '.zovlx', row.targetId);
      if (bufReadU16BE(overlay.data, profile + 8) !== object)
        throw Error('Wrong standalone native miniboss object');
    }
    if (game === 'mm' && row.targetId === 0x23d) verifySource(`standalone/${row.targetId}`, () => verifyNativeStandaloneMm(row.targetId, rom, addresses, patch));
    const sourceData = row.mq?.source ?? rom, sourceOffset = row.mq?.offset ?? row.sourceVrom ?? row.vrom;
    if (row.sourceVrom !== undefined) {
      const relocations = patch.meta.enemyRoomRelocations;
      const matches = Array.isArray(relocations) ? relocations.filter((r: any) => r.game === game &&
        row.sourceVrom! >= r.oldStart && row.sourceVrom! + 16 <= r.oldEnd &&
        row.vrom === r.newStart + row.sourceVrom! - r.oldStart) : [];
      const file = matches.length === 1 ? patch.newFiles.find(f => f.name === matches[0].name && f.vrom === matches[0].newStart) : undefined;
      const at = file ? row.vrom - file.vrom : -1;
      if (row.mq || !file || at < 0 || at + 16 > file.data.length ||
        !row.replacement.every((byte, i) => byte === file.data[at + i]))
        throw new Error('Invalid relocated native enemy placement');
    }
    if (row.mq) {
      const mq = row.mq, key = mqActorVrom(mq.scene, mq.room, mq.slot);
      const packed = patch.newFiles.find(file => file.name === 'custom/mq_rooms');
      if (game !== 'oot' || ![0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13].includes(mq.scene) ||
        mq.room < 0 || mq.room > 255 || mq.slot < 0 || mq.slot >= 64 || row.vrom !== key ||
        patch.symbols.oot.get('ENEMY_NATIVE_MQ_PLACEMENTS_V1')?.length !== 1 ||
        !packed || sourceOffset + 16 > packed.data.length ||
        !row.replacement.every((byte, i) => byte === packed.data[sourceOffset + i]))
        throw new Error('Invalid packed MQ native placement');
    }
    if (row.ootPlacement) {
      if (game !== 'oot') throw new Error('OoT record in another game');
      const imported = row.targetId >= 0xc00, choice = row.ootPlacement.choice;
      const canonical = OOT_ENEMIZER.locations.find(loc => loc.mq === choice.location.mq &&
        loc.scene === choice.location.scene && loc.room === choice.location.room && loc.setup === choice.location.setup && loc.slot === choice.location.slot);
      const target = OOT_ENEMIZER.targets[choice.targetIndex];
      if (!canonical || (imported ? choice.targetIndex !== -1 || row.targetParams !== choice.targetParams :
        !isOotEnemizerNativeExtensionChoice(choice) && (!ootNativeChoiceAllows(canonical, choice.targetIndex, choice.ignoreRestrictions) || !target || target.actorId !== choice.target.actorId || target.params !== choice.target.params || target.name !== choice.target.name)))
        throw new Error('OoT selected target is outside the upstream location pool');
      if (row.targetId !== row.ootPlacement.choice.target.actorId + (imported ? 0xc00 : 0))
        throw new Error('OoT selected actor identity mismatch');
      const expected = prepareOotEnemizerActor(row.ootPlacement.choice, row.original, row.ootPlacement.key,
        imported ? { actorId: row.targetId, params: row.targetParams } : undefined);
      if (imported && enemyIsCarnivorousLily('mm', row.ootPlacement.choice.target.actorId)) {
        const surface = ootWaterSurfaceSlot(row.ootPlacement.key);
        if (surface) verifyPlacementGeometry('oot', rom, surface.collisionRanges, {
          changed: 'OoT water-surface geometry changed', missingAddresses: 'Missing OoT water-surface addresses',
          modified: 'Staged patch modifies certified OoT water-surface geometry',
        }, addresses, patch);
      }
      if (!expected || expected.operation !== row.operation || expected.targetParams !== row.targetParams ||
        !bufEqual(expected.replacement, row.replacement))
        throw new Error('OoT selected placement bytes mismatch');
    }
    if (floor) {
      if (game !== 'oot' || patch.symbols.oot.get('ENEMY_NATIVE_FLOOR_V1')?.length !== 1 ||
        (!row.ootPlacement && (!row.placementAliases || !ootPlacementAdaptersAgree(row.placementAliases) ||
          row.placementAliases.some(alias => alias.source !== bufReadU16BE(row.original, 0) ||
            ootPlacementAdapter(alias.mq, alias.loc, alias.slot, alias.source)?.floorOperation !== 32))))
        throw new Error('Missing certified native floor placement adapter');
    }
    if (!Number.isInteger(row.vrom) || row.vrom < 0 || (!row.mq && row.vrom % 4) || sourceOffset < 0 || sourceOffset + 16 > sourceData.length ||
      row.original.length !== 16 || row.replacement.length !== 16 ||
      !row.original.every((byte, i) => byte === sourceData[sourceOffset + i])) throw new Error('Native placement source drift');
    const source = bufReadU16BE(row.original, 0), params = bufReadU16BE(row.original, 14);
    if (operation === 3) {
      if (game !== 'oot' || patch.symbols.oot.get('ENEMY_NATIVE_STALCHILD_V1')?.length !== 1) throw new Error('Missing native Stalchild night gate');
      validateNativeStalchildPlacement(game, row);
      verifySource('stalchild', () => {
        if (!verifyNativeOotStalchild(rom, addresses, patch)) throw new Error('Native Stalchild source/profile/model revision mismatch');
        verifyNativeOotCallback(patch, 'ENEMY_NATIVE_STALCHILD_V1', 'Stalchild');
      });
    } else if (operation === 5) {
      if (game !== 'mm' || (patch.symbols.mm.get('ENEMY_NATIVE_SKULLWALLTULA_V1')?.length !== 1 || patch.symbols.mm.get('ENEMY_NATIVE_SKULLWALLTULA_WALL_V2')?.length !== 1)) throw new Error('Missing native Skullwalltula attachment adapter');
      validateNativeSkullwalltulaPlacement(game, row);
      verifySource('skullwalltula', () => verifyNativeSkullwalltulaSource(game, rom, addresses, patch));
    } else if (operation === 7) {
      if (game !== 'oot' || patch.symbols.oot.get('ENEMY_NATIVE_GUAY_V1')?.length !== 1) throw new Error('Missing native Guay victory adapter');
      validateNativeGuayPlacement(game, row);
      verifySource('guay', () => {
        if (!verifyNativeOotGuay(rom, addresses, patch)) throw new Error('Native Guay source/profile/model revision mismatch');
        verifyNativeOotCallback(patch, 'ENEMY_NATIVE_GUAY_V1', 'Guay');
      });
    } else if (operation === 31) {
      if (game !== 'mm' || row.targetId !== 0x13a || row.targetParams !== 0x0fff ||
        patch.symbols.mm.get('ENEMY_NATIVE_LILY_V1')?.length !== 1)
        throw Error('Missing native Carnivorous Lily adapter');
      verifySource('Carnivorous Lily', () => verifyNativeActivationSource('Carnivorous Lily', 'mm', NATIVE_LILY_FACTS, rom, addresses, patch));
    } else if (operation === 30) {
      if (game !== 'mm' || ![0x3e, 0xf1, 0x14a].includes(row.targetId) ||
        (row.targetId === 0xf1 && ![0, 1].includes(row.targetParams)) ||
        (row.targetId === 0x14a && (row.targetParams & 0xff) > 3) ||
        patch.symbols.mm.get('ENEMY_NATIVE_RENEWABLE_V1')?.length !== 1)
        throw new Error('Missing native nonrespawning enemy adapter');
    } else if (operation === 6) {
      if (patch.symbols[game].get('ENEMY_NATIVE_RENEWABLE_BABA_V1')?.length !== 1) throw new Error('Missing native renewable Baba adapter');
      validateNativeRenewableBabaPlacement(game, row);
      verifySource('renewable-baba', () => verifyNativeRenewableBabaSource(game, rom, addresses, patch));
    } else if (operation === 4) {
      if (patch.symbols[game].get('ENEMY_NATIVE_PEAHAT_V1')?.length !== 1) throw new Error('Missing native Peahat day gate');
      validateNativePeahatPlacement(game, rom, row, addresses, patch);
    } else if (operation === 9) {
      if (game !== 'mm' || row.targetId !== 0x113 || row.targetParams !== 0 ||
        (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId ||
        bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        patch.symbols.mm.get('ENEMY_NATIVE_GARO_V1')?.length !== 1)
        throw new Error('Missing native Garo combat activation adapter');
      verifySource('Garo', () => verifyNativeActivationSource('Garo', game, nativeGaroFacts[game], rom, addresses, patch, 'ENEMY_NATIVE_GARO_INTRO_V1'));
    } else if (operation === 10) {
      if (row.targetId !== (game === 'oot' ? 0x1c : 0x216) || ![0, game === 'oot' ? 1 : 0x8000].includes(row.targetParams) ||
        (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId ||
        bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        patch.symbols[game].get('ENEMY_NATIVE_LEEVER_V1')?.length !== 1)
        throw new Error('Missing native Leever terrain activation adapter');
      verifySource('Leever', () => verifyNativeActivationSource('Leever', game, nativeLeeverFacts[game], rom, addresses, patch, 'ENEMY_NATIVE_LEEVER_FLOOR_V1'));
    } else if (operation === 11) {
      if (row.targetId !== (game === 'oot' ? 0x175 : 0x208) || row.targetParams !== (game === 'oot' ? 1 : 0xff00) ||
        (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId ||
        bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        patch.symbols[game].get('ENEMY_NATIVE_BIG_POE_V1')?.length !== 1)
        throw new Error('Missing native Big Poe combat activation adapter');
      verifySource('Big Poe', () => verifyNativeActivationSource('Big Poe', game, nativeBigPoeFacts[game], rom, addresses, patch, 'ENEMY_NATIVE_BIG_POE_SWITCH_V1'));
    } else if (operation === 15) {
      validateNativeFlyingPotPlacement(game, row);
      verifySource('flying-pot', () => verifyNativeFlyingPotSource(game, rom, addresses, patch));
    } else if (operation === 17) {
      if (game !== 'oot' || row.targetId !== 0x69 || ![0x00fd, 0x02fc, 0x02fb].includes(row.targetParams) ||
        bufReadU16BE(row.replacement, 0) !== row.targetId || bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        patch.symbols.oot.get('ENEMY_NATIVE_BUBBLE_PATROL_V1')?.length !== 1)
        throw new Error('Missing native Bubble patrol adapter');
      verifySource('Bubble patrol', () => verifyNativeActivationSource('Bubble patrol', 'oot', nativeBubbleFacts, rom, addresses, patch));
    } else if (operation === 18) {
      if (game !== 'mm' || row.targetId !== 0x291 || row.targetParams !== 0 ||
        (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId || bufReadU16BE(row.replacement, 14) !== 0 ||
        patch.symbols.mm.get('ENEMY_NATIVE_TAKKURI_V1')?.length !== 1)
        throw new Error('Missing native Takkuri combat adapter');
      verifySource('Takkuri', () => verifyNativeActivationSource('Takkuri', 'mm', nativeTakkuriFacts, rom, addresses, patch, 'ENEMY_NATIVE_TAKKURI_ITEMS_V1'));
    } else if (operation === 12) {
      if (row.targetId !== (game === 'oot' ? 0x197 : 0x21d) || row.targetParams !== (game === 'oot' ? 0 : 0x1fc0) ||
        (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId || bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        patch.symbols[game].get('ENEMY_NATIVE_FIGHTER_V1')?.length !== 1)
        throw new Error('Missing native fighter combat adapter');
      verifySource('Fighter', () => verifyNativeActivationSource('Fighter', game, nativeFighterFacts[game], rom, addresses, patch));
    } else if (operation === 13) {
      if (row.targetId !== (game === 'oot' ? 0x91 : 0x1e8) || ![0, 0x100, 0x200, 0x300].includes(row.targetParams) ||
        (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId || bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        patch.symbols[game].get('ENEMY_NATIVE_POE_SISTER_V1')?.length !== 1)
        throw new Error('Missing native Poe Sister combat adapter');
      verifySource('Poe Sister', () => verifyNativeActivationSource('Poe Sister', game, nativePoeSisterFacts[game], rom, addresses, patch));
    } else if (operation === 19) {
      if (game !== 'mm' || row.targetId !== 0x184 || row.targetParams !== 0x1fc0 ||
        (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId || bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        patch.symbols.mm.get('ENEMY_NATIVE_EYEGORE_V1')?.length !== 1)
        throw new Error('Missing native Eyegore chase adapter');
      verifySource('Eyegore', () => verifyNativeActivationSource('Eyegore', game, nativeEyegoreFacts[game], rom, addresses, patch));
    } else if (operation === 20) {
      if (game !== 'oot' || row.mq || floor || row.targetId !== 0xde || ![0xff00, 0xff01, 0xff02].includes(row.targetParams) ||
        bufReadU16BE(row.replacement, 0) !== row.targetId || bufReadU16BE(row.replacement, 14) !== row.targetParams ||
        (bufReadU16BE(row.replacement, 4) << 16 >> 16) !== nativeTentaclePlacementY(game, rom, sourceOffset, addresses, patch) ||
        patch.symbols.oot.get('ENEMY_NATIVE_TENTACLE_V1')?.length !== 1)
        throw Error('Missing certified native Tentacle ceiling placement');
      verifySource('Parasitic Tentacle', () => verifyNativeActivationSource('Parasitic Tentacle', game, nativeTentacleFacts[game], rom, addresses, patch));
    } else if (operation === 21) {
      if (game !== 'mm' || row.targetId !== 0x1a8 || row.targetParams !== 0xff || traits & 256)
        throw Error('Invalid native MM Big Octo placement');
      verifyNativeBigOctoRecord(rom, sourceOffset, row.original, row.replacement);
      verifySource('big-octo', () => verifyNativeBigOctoSource(rom, addresses, patch));
    } else if (operation === 25) {
      if (game !== 'mm' || row.targetId !== 0x15d || ![0x807f, 0x817f].includes(row.targetParams) ||
        !traits || (traits & 256))
        throw new Error('Invalid standalone Wizzrobe activation record');
    } else if (operation === 28) {
      if (game !== 'oot' || !row.ootPlacement?.choice.ignoreRestrictions || !floor || !(traits & 512) ||
        !['Octorok', 'Stinger', 'Stingray'].includes(row.ootPlacement.choice.target.name))
        throw new Error('Invalid relaxed OoT aquatic placement');
    } else if (operation === 29) {
      if (game !== 'mm' || !mmAquaticTarget(game, row.targetId, row.targetParams) || mmAquaticSlot(sourceOffset) || !(traits & 256) || !(traits & 512))
        throw new Error('Invalid relaxed MM aquatic placement');
      if (row.targetId === 0x23f) verifySource(`standalone/${row.targetId}`, () => verifyNativeStandaloneMm(row.targetId, rom, addresses, patch));
    } else if (operation === 24) {
      if (game !== 'mm' || row.targetId !== 0x23f || row.targetParams !== 0 || !traits || (traits & 256))
        throw Error('Invalid native Garahna activation record');
      verifySource(`standalone/${row.targetId}`, () => verifyNativeStandaloneMm(row.targetId, rom, addresses, patch));
    } else if (operation === 129) {
      if (game !== 'oot' || source !== 0xa4 || row.targetId !== bufReadU16BE(row.replacement, 0) ||
        row.targetParams !== bufReadU16BE(row.replacement, 14))
        throw Error('Invalid Dead Hand encounter replacement');
    } else if (operation === 128) {
      if (game !== 'mm' || floor || !bufEqual(row.original, row.replacement))
        throw new Error('Invalid scripted MM source entry');
      verifyScriptedMmRecord(rom, sourceOffset, row.original, row.targetId, row.targetParams, addresses, patch);
    } else if (operation === 1) {
      if (!nativeResourceSource(game, source & 0x1fff, params) || !(row.targetId >= 0xc00 || nativeResourceTarget(game, row.targetId, row.targetParams)) ||
        (bufReadU16BE(row.replacement, 0) & 0x1fff) !== row.targetId || bufReadU16BE(row.replacement, 14) !== row.targetParams)
        throw new Error('Unreviewed native resource replacement');
      if ((game === 'oot' && (source & 0x1fff) === 0x197 || game === 'mm' && [0x208, 0x21d].includes(source & 0x1fff)) &&
        (patch.symbols[game].get('ENEMY_NATIVE_FIGHT_RESTORE_V1')?.length !== 1 ||
          !verifiedNativeFightRestorationRecord(game, sourceOffset, row.original)))
        throw new Error('Missing verified native fight restoration adapter');
    } else if (operation === 2) {
      if (game !== 'oot' || source !== 0xa5 || !bufEqual(row.original, row.replacement))
        throw new Error('Unreviewed suppressed native actor');
    } else if (operation === 8) {
      if (game !== 'oot' || ![0x136, 0xc8].includes(source) ||
        (source === 0x136 ? params !== 0xffff : sourceOffset !== BIG_OCTO_ARENA.start + BIG_OCTO_ARENA.actorOffset + BIG_OCTO_ARENA.controllerSlot * 16 ||
          !row.original.every((byte, i) => byte === Number.parseInt(BIG_OCTO_ARENA.controller.slice(i * 2, i * 2 + 2), 16))) ||
        !bufEqual(row.original, row.replacement) ||
        (source === 0xc8 ? row.targetId !== 0x34 || row.targetParams !== 0 :
          !(row.targetId >= 0xc00 || nativeResourceTarget(game, row.targetId, row.targetParams))) ||
        (patch.symbols.oot.get(source === 0xc8 ? 'FOREIGN_ENEMY_BIG_OCTO_ARENA_V1' : 'FOREIGN_ENEMY_DARK_LINK_ARENA_V1')?.length !== 1 ||
          patch.symbols.oot.get('ENEMY_NATIVE_ARENA_LIFETIME_V1')?.length !== 1))
        throw new Error('Missing native arena controller adapter');
      if (row.targetId === 0x113) {
        if (patch.symbols.oot.get('ENEMY_NATIVE_IRON_KNUCKLE_V1')?.length !== 1) throw new Error('Missing native Iron Knuckle spawn-mode adapter');
        verifySource('iron-knuckle', () => verifyNativeIronKnuckleSource(rom));
      }
    } else if (operation === 16) {
      if (game !== 'oot' || patch.symbols.oot.get('ENEMY_NATIVE_SKULL_KID_V1')?.length !== 1)
        throw new Error('Missing native Skull Kid constructor adapter');
      validateNativeSkullKidPlacement({ ...row, operation });
      verifySource('skull-kid', () => verifyNativeSkullKidSource(rom));
    } else if (operation === 64) {
      if (game !== 'oot' || patch.symbols.oot.get('ENEMY_NATIVE_IRON_KNUCKLE_V1')?.length !== 1)
        throw new Error('Missing native Iron Knuckle spawn-mode adapter');
      validateNativeIronKnucklePlacement({ ...row, operation });
      verifySource('iron-knuckle', () => verifyNativeIronKnuckleSource(rom));
    } else if (operation !== 0) throw new Error('Unsupported native placement operation');
    const previous = sorted.get(row.vrom);
    if (previous && (previous.operation !== row.operation || previous.targetId !== row.targetId || previous.targetParams !== row.targetParams ||
      !bufEqual(previous.replacement, row.replacement))) throw new Error('Conflicting native placement aliases');
    sorted.set(row.vrom, row);
  }
  const rows = [...sorted.values()].sort((a, b) => a.vrom - b.vrom);
  const header = toU32Buffer([0x454e5031, 1, game === 'oot' ? 0 : 1, rows.length]);
  const data = new Uint8Array(header.length + rows.length * NATIVE_PLACEMENT_RECORD_SIZE);
  data.set(header);
  rows.forEach((row, i) => writeNativePlacement(data, header.length + i * NATIVE_PLACEMENT_RECORD_SIZE, row));
  const vrom = allocatePlacementMetadata(patch, game, data.length, 'Native enemy placement metadata capacity exceeded');
  const symbol = `ENEMY_NATIVE_PLACEMENTS_${game.toUpperCase()}`;
  if (patch.symPatches.has(symbol)) throw new Error('Native placements already installed');
  patch.addNewFile({ name: `enemy/native/${game}/placements`, vrom, data, compressed: false });
  patch.addSymbolPatch(symbol, toU32Buffer([vrom]));
}

function modifiedRomRange(game: Game, start: number, size: number, addresses: GameAddresses, patch: Patchfile, requireContiguous = false) {
  const first = addresses[game].fileFromROM(start);
  const last = addresses[game].fileFromROM(start + size - 1);
  return first.name !== last.name || (requireContiguous && last.offset !== first.offset + size - 1) ||
    patch.removedFiles.includes(first.name) ||
    patch.newFiles.some(file => file.name === first.name) ||
    (patch.patches[first.name] ?? []).some(edit =>
      edit.addr < last.offset + 1 && edit.addr + edit.data.length > first.offset);
}

function verifyPlacementGeometry(game: Game, rom: Uint8Array, ranges: readonly (readonly number[])[],
  errors: { changed: string; missingAddresses: string; modified: string; checkReplacement?: boolean }, addresses?: GameAddresses, patch?: Patchfile) {
  for (const [start, size, crc] of ranges) {
    if (start + size > rom.length || (CRC32.buf(rom.subarray(start, start + size)) >>> 0) !== crc)
      throw new Error(errors.changed);
    if (patch) {
      if (!addresses) throw new Error(errors.missingAddresses);
      if (modifiedRomRange(game, start, size, addresses, patch, errors.checkReplacement))
        throw new Error(errors.modified);
    }
  }
}

function lilyWaterSurfaceY(game: Game, rom: Uint8Array, offset: number,
  slot: NonNullable<ReturnType<typeof mmWaterSurfaceSlot>>, addresses?: GameAddresses, patch?: Patchfile): number {
  if (!matchesRomBytes(rom, offset, slot.original)) throw Error('Water-surface source actor changed');
  verifyPlacementGeometry(game, rom, slot.collisionRanges, {
    changed: 'Water-surface collision geometry changed', missingAddresses: 'Missing water-surface geometry addresses',
    modified: 'Staged patch modifies certified water-surface geometry',
  }, addresses, patch);
  return slot.surfaceY;
}

function mmAquaticPlacementY(rom: Uint8Array, offset: number, addresses?: GameAddresses, patch?: Patchfile): number {
  const slot = mmAquaticSlot(offset);
  if (!slot || slot.original.length !== 32 || !matchesRomBytes(rom, offset, slot.original))
    throw new Error('Uncertified MM aquatic placement slot');
  verifyPlacementGeometry('mm', rom, slot.collisionRanges, {
    changed: 'MM aquatic placement geometry changed',
    missingAddresses: 'Missing MM aquatic placement geometry addresses',
    modified: 'Staged patch modifies certified MM aquatic geometry',
  }, addresses, patch);
  return slot.spawnY;
}

function nativeTentaclePlacementY(game: Game, rom: Uint8Array, offset: number, addresses?: GameAddresses, patch?: Patchfile): number {
  const slot = tentacleSlotAt(game, offset);
  if (!slot || slot.original.length !== 32 || !matchesRomBytes(rom, offset, slot.original))
    throw new Error('Uncertified native Tentacle ceiling slot');
  verifyPlacementGeometry(game, rom, slot.collisionRanges, {
    changed: 'Native Tentacle ceiling geometry changed',
    missingAddresses: 'Missing Tentacle geometry addresses',
    modified: 'Staged patch modifies certified Tentacle geometry',
  }, addresses, patch);
  return Math.round(slot.ceilingY - 100);
}

const sceneKeeps = placementData.sceneKeeps;

function validateNativeFlyingPotPlacement(game: Game, row: NativePlacementRecord) {
  const offset = row.sourceVrom ?? row.vrom;
  const scene = row.mq?.scene ?? row.ootPlacement?.key.scene ??
    sceneKeeps.rooms[game].find(room => offset >= room.start && offset + 16 <= room.end)?.scene;
  if (scene === undefined || (sceneKeeps.scenes[game] as Record<string, number>)[String(scene)] !== 3) throw new Error('Native flying pot requires receiving dungeon keep segment5');
  const f = flyingPotFacts[game];
  const params = game === 'oot' ? 0xffff : 0x3fff;
  if (row.targetId !== f.actorId || row.targetParams !== params || row.replacement.length !== 16 ||
    (bufReadU16BE(row.replacement, 0) & 0x1fff) !== f.actorId || bufReadU16BE(row.replacement, 14) !== params)
    throw new Error('Invalid selected native flying pot');
}

function verifyNativeFlyingPotSource(game: Game, rom: Uint8Array, addresses: GameAddresses, patch: Patchfile) {
  const f = flyingPotFacts[game];
  if (!matchesRomBytes(rom, f.actorTable, f.actorTableBytes) ||
    !matchesRomBytes(rom, f.profileOffset, f.profileBytes) ||
    !matchesRomBytes(rom, f.objectTable, f.objectTableBytes) ||
    f.vromEnd > rom.length || f.objectEnd > rom.length ||
    (CRC32.buf(rom.subarray(f.vromStart, f.vromEnd)) >>> 0) !== f.crc32 ||
    (CRC32.buf(rom.subarray(f.objectStart, f.objectEnd)) >>> 0) !== f.objectCrc32)
    throw new Error('Native flying pot source/model revision mismatch');
  if (modifiedRomRange(game, f.objectTable, 8, addresses, patch))
    throw new Error('Modified selected native flying pot dependency');
  const name = `ovl/${game}/${game}_ovl_En_Tubo_Trap.zovlx`;
  const { overlay, vram, profile, layout } = installedEnemyDependency(patch, game, addresses[game], f.actorId, 6, name);
  const cap = patch.symbols[game].get(`ENEMY_NATIVE_FLYING_POT_${game.toUpperCase()}_V1`);
  if (cap?.length !== 1 || cap[0] % 4 || cap[0] < vram[0] || cap[0] + 4 > vram[0] + layout.textSize || bufReadU32BE(overlay.data, profile + 16) !== cap[0] || bufReadU16BE(overlay.data, profile + 8) !== 3) throw new Error('Missing verified native flying pot constructor');
}

function verifyNativeStandaloneMm(actorId: number, rom: Uint8Array, addresses: GameAddresses | undefined, patch: Patchfile) {
  const target = actorId === 0x23f ? { name: 'En_Tanron3', cap: 'ENEMY_NATIVE_GARAHNA_MM_V2', category: 9 } : actorId === 0x23d ? { name: 'En_Tanron1', cap: 'ENEMY_NATIVE_MOTH_MM_V1', category: 5 } : undefined;
  if (!target) throw Error('Unknown native standalone enemy');
  const row = standaloneFacts.actors.find(row => row.sourceActorId === actorId);
  const program = row && sources.programs[row.program as keyof typeof sources.programs];
  if (!row || !program || program.sourceActorId !== actorId || !addresses) throw Error('Missing native standalone source facts');
  verifyNativeEnemyPortSource(program as unknown as ForeignEnemyProgram, row.nativePort as NativeEnemyPort, rom);
  const { overlay, vram, profile, layout } = installedEnemyDependency(patch, 'mm', addresses.mm, actorId, target.category, `ovl/mm/mm_ovl_${target.name}.zovlx`);
  const cap = patch.symbols.mm.get(target.cap);
  if (cap?.length !== 1 || cap[0] % 4 || cap[0] < vram[0] || cap[0] + 4 > vram[0] + layout.textSize ||
    bufReadU32BE(overlay.data, profile + 16) !== cap[0] || bufReadU16BE(overlay.data, profile + 8) !== program.sourceObjectId)
    throw Error('Missing verified native standalone combat overlay');
}

function verifyNativeIronKnuckleSource(rom: Uint8Array) {
  verifyNativeEnemyPortSource(nativeIronKnuckleFacts.program as unknown as ForeignEnemyProgram,
    nativeIronKnuckleFacts.nativePort as NativeEnemyPort, rom);
  const { manifest } = nativeIronKnuckleFacts.program, helper = nativeIronKnuckleFacts.helper;
  const offset = manifest.sourceVrom + helper.address - manifest.vramStart;
  if (helper.address !== 0x80a676e4 || helper.size !== 0xf8 || offset < manifest.sourceVrom ||
    offset + helper.size > manifest.sourceVrom + manifest.fileSize ||
    CRC32.buf(rom.subarray(offset, offset + helper.size)) >>> 0 !== helper.crc32)
    throw new Error('Native Iron Knuckle walk helper revision mismatch');
}

function validateNativeIronKnucklePlacement(row: {
  operation: number; replacement: Uint8Array; targetId: number; targetParams: number;
}) {
  if (row.operation !== 64 || row.targetId !== 0x113 || row.replacement.length !== 16 ||
    ![0xff81, 0xff82, 0xff83].includes(row.targetParams) || bufReadU16BE(row.replacement, 0) !== 0x113 ||
    bufReadU16BE(row.replacement, 14) !== row.targetParams)
    throw new Error('Invalid native Iron Knuckle combat placement');
}

const NATIVE_SKULL_KID = { operation: 16, actorId: 0x115, params: 0xffff, objectId: 0x10a } as const;

function verifyNativeSkullKidSource(rom: Uint8Array) {
  const donor = nativePorts.SKULL_KID;
  verifyNativeEnemyPortSource(donor.program as unknown as ForeignEnemyProgram,
    donor.nativePort as NativeEnemyPort, rom);
}

function validateNativeSkullKidPlacement(record: {
  operation: number; replacement: Uint8Array; targetId: number; targetParams: number;
}) {
  const expected = NATIVE_SKULL_KID;
  if (record.operation !== expected.operation || record.replacement.length !== 16 ||
    record.targetId !== expected.actorId || record.targetParams !== expected.params ||
    bufReadU16BE(record.replacement, 0) !== expected.actorId ||
    bufReadU16BE(record.replacement, 14) !== expected.params)
    throw new Error('Invalid native Skull Kid combat placement');
}

const facts = { mm: source };

function verifyNativeSkullwalltulaSource(game: 'mm', rom: Uint8Array, addresses?: GameAddresses, patch?: Patchfile) {
  const f = facts[game];
  if (!matchesRomBytes(rom, f.actorTable, f.actorTableBytes) || !matchesRomBytes(rom, f.objectTable, f.objectTableBytes) || !matchesRomBytes(rom, f.profileOffset, f.profileBytes) || f.vromEnd > rom.length || f.objectEnd > rom.length ||
    CRC32.buf(rom.subarray(f.vromStart, f.vromEnd)) >>> 0 !== f.crc32 || CRC32.buf(rom.subarray(f.objectStart, f.objectEnd)) >>> 0 !== f.objectCrc32)
    throw new Error('Native Skullwalltula source/profile/model revision mismatch');
  if (patch) {
    if (!addresses) throw new Error('Missing native Skullwalltula staged source addresses');
    const ranges = [[f.vromStart, f.vromEnd], [f.actorTable, f.actorTable + 32], [f.objectStart, f.objectEnd], [f.objectTable, f.objectTable + 8]];
    const hookVrom = f.vromStart + 0x808db928 - f.vramStart;
    const sites = [{ vrom: hookVrom, size: 4, original: '0c02e014' },
    { vrom: f.objectStart + 0x42e8, size: 16, original: 'fa000000f0dc1efffb000000141400ff' },
    { vrom: f.objectStart + 0x4580, size: 16, original: 'fa000000f0dc1efffb000000141400ff' }];
    const edits: { site: number; offset: number; data: Uint8Array }[] = [];
    for (const [start, end] of ranges) {
      const a = addresses[game].fileFromROM(start), b = addresses[game].fileFromROM(end - 1);
      if (a.name !== b.name || b.offset - a.offset !== end - start - 1 || patch.removedFiles.includes(a.name) || patch.newFiles.some(n => n.name === a.name)) throw new Error('Modified native Skullwalltula source/profile/model');
      for (const edit of patch.patches[a.name] ?? []) {
        if (edit.addr >= b.offset + 1 || edit.addr + edit.data.length <= a.offset) continue;
        const vrom = start + edit.addr - a.offset;
        const site = sites.findIndex(s => vrom >= s.vrom && vrom + edit.data.length <= s.vrom + s.size);
        if (site < 0) throw new Error('Modified native Skullwalltula source/profile/model');
        edits.push({ site, offset: vrom - sites[site].vrom, data: edit.data });
      }
    }
    if (!edits.length) return;
    const symbols = patch.symbols.mm.get('ENEMY_NATIVE_SKULLWALLTULA_TOKEN_DRAW_V1');
    const files = patch.newFiles.filter(n => n.name === 'mm/payload'), payload = files.length === 1 ? files[0] : undefined, span = payload?.vram?.mm;
    const destination = symbols?.[0];
    if (symbols?.length !== 1 || !Number.isInteger(destination) || destination! % 4 || !span || span[0] !== 0x80700000 || span[1] > 0x80780000 ||
      span[1] - span[0] !== payload!.data.length || destination! < span[0] || destination! + 4 > span[1]) throw new Error('Rebuild MM payload for verified native Skullwalltula token draw support');
    if (!sites.every(s => matchesRomBytes(rom, s.vrom, s.original))) throw new Error('Native Skullwalltula token draw original instruction/model mismatch');
    const expected = [new Uint8Array(4), Uint8Array.from([0xde, 0, 0, 0, 0x0a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), Uint8Array.from([0xde, 0, 0, 0, 0x0a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])];
    bufWriteU32BE(expected[0], 0, (0x0c000000 | ((destination! >>> 2) & 0x03ffffff)) >>> 0);
    for (let index = 0; index < sites.length; index++) {
      const site = sites[index], staged = rom.slice(site.vrom, site.vrom + site.size), changes = edits.filter(e => e.site === index);
      if (!changes.length) throw new Error('Incomplete native Skullwalltula token draw patch bundle');
      for (const edit of changes) staged.set(edit.data, edit.offset);
      if (!bufEqual(staged, expected[index])) throw new Error('Unverified native Skullwalltula token draw patch');
    }
  }

}

function validateNativeSkullwalltulaPlacement(game: 'mm', row: NativePlacementRecord) {
  const f = facts[game], params = [0xff00];
  if (row.targetId !== f.actorId || !params.includes(row.targetParams) || row.replacement.length !== 16 ||
    (bufReadU16BE(row.replacement, 0) & 0x1fff) !== f.actorId || bufReadU16BE(row.replacement, 14) !== row.targetParams)
    throw new Error('Invalid selected native Skullwalltula placement');
}

function nativeMmSkullwalltulaSlotAllows(source: number, target: { game: Game; actorId: number; spawnParams: number }, params?: number, csId?: number): boolean {
  if (target.game !== 'mm' || target.actorId !== 0x50) return true;
  return target.spawnParams === 0xff00 && csId === 0x7f && source === 0x50 && params !== undefined && params === 0xfffc && ((params >>> 8) & 0xff) === 0xff;
}

function verifyNativeRenewableBabaSource(game: Game, rom: Uint8Array, addresses?: GameAddresses, patch?: Patchfile) {
  const f = nativeRenewableBabaFacts[game];
  if (!matchesRomBytes(rom, f.actorTable, f.actorTableBytes) || !matchesRomBytes(rom, f.objectTable, f.objectTableBytes) || !matchesRomBytes(rom, f.profileOffset, f.profileBytes) || f.vromEnd > rom.length || f.objectEnd > rom.length ||
    CRC32.buf(rom.subarray(f.vromStart, f.vromEnd)) >>> 0 !== f.crc32 || CRC32.buf(rom.subarray(f.objectStart, f.objectEnd)) >>> 0 !== f.objectCrc32)
    throw new Error('Native renewable Baba source/profile/model revision mismatch');
  for (const fn of f.functions) {
    const at = f.vromStart + fn.address - f.vramStart;
    if (at < f.vromStart || at + fn.size > f.vromEnd || CRC32.buf(rom.subarray(at, at + fn.size)) >>> 0 !== fn.crc32)
      throw new Error('Native renewable Baba victory action revision mismatch');
  }
  if (patch) {
    if (!addresses) throw new Error('Missing native renewable Baba staged source addresses');
    for (const [start, end] of [[f.vromStart, f.vromEnd], [f.actorTable, f.actorTable + 32], [f.objectStart, f.objectEnd], [f.objectTable, f.objectTable + 8]]) {
      const a = addresses[game].fileFromROM(start), b = addresses[game].fileFromROM(end - 1);
      if (a.name !== b.name || b.offset - a.offset !== end - start - 1 || patch.removedFiles.includes(a.name) || patch.newFiles.some(n => n.name === a.name))
        throw new Error('Modified native renewable Baba source/profile/model');
      const edits = (patch.patches[a.name] ?? []).filter(p => p.addr < b.offset + 1 && p.addr + p.data.length > a.offset);
      if (!edits.length) continue;
      if (game === 'oot') {
        const hookVrom = f.vromStart + 0x809baebc - f.vramStart, hook = addresses.oot.fileFromROM(hookVrom);
        const caps = patch.symbols.oot.get('ENEMY_NATIVE_RENEWABLE_BABA_ITEM_OOT_V1');
        const payloads = patch.newFiles.filter(file => file.name === 'oot/payload'), payload = payloads.length === 1 ? payloads[0] : undefined, span = payload?.vram?.oot;
        if (start !== f.vromStart || hook.name !== a.name || caps?.length !== 1 || !span || span[1] - span[0] !== payload!.data.length || caps[0] % 4 || caps[0] < span[0] || caps[0] >= span[1] || bufReadU16BE(rom, hookVrom) !== 0x0c00 || bufReadU16BE(rom, hookVrom + 2) !== 0x8b3d) throw new Error('Modified native renewable Baba source/profile/model');
        const expected = new Uint8Array(4), staged = Uint8Array.from(rom.subarray(hookVrom, hookVrom + 4));
        bufWriteU32BE(expected, 0, (0x0c000000 | ((caps[0] >>> 2) & 0x03ffffff)) >>> 0);
        for (const edit of edits) {
          if (edit.addr < hook.offset || edit.addr + edit.data.length > hook.offset + 4) throw new Error('Modified native renewable Baba source/profile/model');
          staged.set(edit.data, edit.addr - hook.offset);
        }
        if (!bufEqual(staged, expected)) throw new Error('Unverified native renewable Baba item hook');
        continue;
      }
      const hookVrom = f.vromStart + 0x808f21f4 - f.vramStart, hook = addresses[game].fileFromROM(hookVrom);
      const caps = patch.symbols.mm.get('ENEMY_NATIVE_RENEWABLE_BABA_ITEM_V1');
      const payloads = patch.newFiles.filter(file => file.name === 'mm/payload'), payload = payloads.length === 1 ? payloads[0] : undefined, span = payload?.vram?.mm;
      if (game !== 'mm' || start !== f.vromStart || hook.name !== a.name || caps?.length !== 1 || !span || span[1] - span[0] !== payload!.data.length ||
        caps[0] % 4 || caps[0] < span[0] || caps[0] >= span[1] || !GI.MM_STICK || GI.MM_STICK.index > 0x7fff)
        throw new Error('Modified native renewable Baba source/profile/model');
      const expected = new Uint8Array(8), staged = Uint8Array.from(rom.subarray(hookVrom, hookVrom + 8));
      bufWriteU32BE(expected, 0, (0x0c000000 | ((caps[0] >>> 2) & 0x03ffffff)) >>> 0);
      bufWriteU32BE(expected, 4, 0x24060000 | GI.MM_STICK.index);
      for (const edit of edits) {
        if (edit.addr < hook.offset || edit.addr + edit.data.length > hook.offset + 8) throw new Error('Modified native renewable Baba source/profile/model');
        staged.set(edit.data, edit.addr - hook.offset);
      }
      if (!bufEqual(staged, expected)) throw new Error('Unverified native renewable Baba item hook');
    }
  }
}

function validateNativeRenewableBabaPlacement(game: Game, row: NativePlacementRecord) {
  const f = nativeRenewableBabaFacts[game], params = game === 'oot' ? [1] : [1, 2];
  if (row.targetId !== f.actorId || !params.includes(row.targetParams) || row.replacement.length !== 16 ||
    (bufReadU16BE(row.replacement, 0) & (game === 'mm' ? 0x1fff : 0xffff)) !== f.actorId || bufReadU16BE(row.replacement, 14) !== row.targetParams)
    throw new Error('Invalid selected native renewable Baba placement');
}

function nativeOotRenewableBabaPolicyAllows(policy: { foreignOrdinary: boolean }, target: { game: Game; actorId: number; spawnParams: number }): boolean {
  return target.game === 'oot' && target.actorId === 0xc7 && target.spawnParams === 1 && policy.foreignOrdinary;
}

function verifyOotCounterpart(facts: Pick<typeof guayFacts, 'vromStart' | 'vromEnd' | 'crc32' | 'objectStart' | 'objectEnd' | 'objectCrc32' | 'table' | 'tableBytes' | 'objectEntry' | 'objectEntryBytes'>, rom: Uint8Array, addresses?: GameAddresses, patch?: Patchfile): boolean {
  try {
    for (const [start, end, crc] of [[facts.vromStart, facts.vromEnd, facts.crc32], [facts.objectStart, facts.objectEnd, facts.objectCrc32]]) {
      if (start < 0 || end <= start || end > rom.length || CRC32.buf(rom.subarray(start, end)) >>> 0 !== crc) return false;
      if (patch) {
        if (!addresses) return false;
        const f = addresses.oot.fileFromROM(start), last = addresses.oot.fileFromROM(end - 1);
        if (f.offset || f.name !== last.name || last.offset !== end - start - 1 || patch.removedFiles.includes(f.name) || patch.newFiles.some(r => r.name === f.name) || patch.patches[f.name]?.some(p => p.data.length)) return false;
      }
    }
    for (const [at, hex] of [[facts.table, facts.tableBytes], [facts.objectEntry, facts.objectEntryBytes]] as const) {
      if (!matchesRomBytes(rom, at, hex)) return false;
      if (patch) {
        if (!addresses) return false;
        const f = addresses.oot.fileFromROM(at);
        if (patch.patches[f.name]?.some(p => p.addr < f.offset + hex.length / 2 && p.addr + p.data.length > f.offset)) return false;
      }
    }
    return true;
  } catch {
    return false;
  }
}

const verifyNativeOotGuay = (rom: Uint8Array, addresses?: GameAddresses, patch?: Patchfile) => verifyOotCounterpart(guayFacts, rom, addresses, patch);

function verifyNativeOotCallback(patch: Patchfile, symbol: string, enemy: string) {
  const symbols = patch.symbols.oot.get(symbol);
  const files = patch.newFiles.filter(file => file.name === 'oot/payload');
  const span = files.length === 1 ? files[0].vram?.oot : undefined;
  if (symbols?.length !== 1 || !span || span[1] - span[0] !== files[0].data.length ||
    symbols[0] % 4 || symbols[0] < span[0] || symbols[0] >= span[1])
    throw new Error('Missing selected native ' + enemy + ' callback capability');
}

function validateNativeGuayPlacement(game: Game, row: NativePlacementRecord): void {
  if (game !== 'oot' || row.targetId !== guayFacts.actorId || ![0, 1].includes(row.targetParams) || row.replacement.length !== 16 ||
    row.replacement[0] * 256 + row.replacement[1] !== guayFacts.actorId || bufReadU16BE(row.replacement, 14) !== row.targetParams) throw new Error('Invalid selected native Guay placement');
}

const verifyNativeOotStalchild = (rom: Uint8Array, addresses?: GameAddresses, patch?: Patchfile) => verifyOotCounterpart(stalchildFacts, rom, addresses, patch);

function validateNativeStalchildPlacement(game: Game, row: NativePlacementRecord): void {
  if (game !== 'oot' || row.targetId !== stalchildFacts.actorId || ![0, 5, 10].includes(row.targetParams) || row.replacement.length !== 16 ||
    row.replacement[0] * 256 + row.replacement[1] !== stalchildFacts.actorId || bufReadU16BE(row.replacement, 14) !== row.targetParams) throw new Error('Invalid selected native Stalchild placement');
}

function nativeOotStalchildPolicyAllows(policy: { foreignOrdinary: boolean }, target: { game: string; actorId: number; spawnParams: number }): boolean {
  return target.game === 'oot' && target.actorId === stalchildFacts.actorId && [0, 5, 10].includes(target.spawnParams) && policy.foreignOrdinary;
}

function nativeOotPoeTarget(target: { game: Game; actorId: number; spawnParams: number }): boolean {
  return target.game === 'oot' && target.actorId === poeFacts.actorId && target.spawnParams === poeFacts.params;
}

function nativeOotPoePolicyAllows(policy: { foreignOrdinary: boolean }, target: { game: Game; actorId: number; spawnParams: number }): boolean {
  return nativeOotPoeTarget(target) && policy.foreignOrdinary;
}

function nativeOotPoeTargetObjects(target: { game: Game; actorId: number; spawnParams: number }): ReadonlySet<number> | undefined {
  return nativeOotPoeTarget(target) ? new Set([poeFacts.modelObject]) : undefined;
}

const verifyNativeOotPoe = (rom: Uint8Array, addresses: GameAddresses, patch: Patchfile) => !!addresses && !!patch && verifyOotCounterpart(poeFacts, rom, addresses, patch);

function nativeMmSkulltulaSlotAllows(source: number, target: { game: Game; actorId: number; spawnParams: number },
  params?: number, csId?: number): boolean {
  if (target.game !== 'mm' || target.actorId !== 0x24) return true;
  return source === 0x24 && (params === 0x3f || params === 0x7f) && (target.spawnParams === 0x3f || target.spawnParams === 0x7f) && csId === 0x7f;
}

function verifyNativeBigOctoSource(rom: Uint8Array, addresses?: GameAddresses, patch?: Patchfile) {
  verifyNativeActivationSource('Big Octo', 'mm', nativeBigOctoFacts, rom);
  if (!patch) return;
  if (!addresses || modifiedRomRange('mm', nativeBigOctoFacts.objectTable, 8, addresses, patch, true) ||
    modifiedRomRange('mm', nativeBigOctoFacts.objectStart, nativeBigOctoFacts.objectEnd - nativeBigOctoFacts.objectStart, addresses, patch, true))
    throw new Error('Modified native MM Big Octo model');

  const entry = addresses.mm.fileFromROM(nativeBigOctoFacts.table);
  const tableEnd = addresses.mm.fileFromROM(nativeBigOctoFacts.table + 31);
  if (entry.name !== tableEnd.name || tableEnd.offset !== entry.offset + 31 ||
    patch.removedFiles.includes(entry.name) || patch.newFiles.some(file => file.name === entry.name))
    throw new Error('Modified native MM Big Octo actor table file');
  const table = rom.slice(nativeBigOctoFacts.table, nativeBigOctoFacts.table + 32);
  for (const edit of patch.patches[entry.name] ?? []) {
    const start = Math.max(edit.addr, entry.offset);
    const end = Math.min(edit.addr + edit.data.length, entry.offset + table.length);
    if (start < end) table.set(edit.data.subarray(start - edit.addr, end - edit.addr), start - entry.offset);
  }
  const vrom = bufReadU32BE(table, 0);
  const vram = bufReadU32BE(table, 8);
  const profile = bufReadU32BE(table, 20) - vram;
  const files = patch.newFiles.filter(file => file.name?.endsWith('/mm_ovl_En_Bigokuta.zovlx') && file.vrom === vrom);
  const file = files.length === 1 ? files[0] : undefined;
  const marker = patch.symbols.mm.get('ENEMY_NATIVE_BIG_OCTO_MM_V1');
  const markerOffset = marker?.length === 1 ? marker[0] - vram : -1;
  const expected = [0x424f4354, 1, 0x49, 12, 150, 285, -1350 >>> 0];
  if (!file || !file.vram?.mm || file.vram.mm[0] !== vram || file.vram.mm[1] !== bufReadU32BE(table, 12) ||
    bufReadU32BE(table, 4) !== vrom + file.data.length || bufReadU32BE(table, 16) !== 0 ||
    profile < 0 || profile % 4 || profile + 32 > file.data.length ||
    bufReadU16BE(file.data, profile) !== 0x1a8 || file.data[profile + 2] !== 9 ||
    bufReadU16BE(file.data, profile + 8) !== 0x19e || bufReadU32BE(file.data, profile + 12) !== 0x3f8 ||
    !Number.isInteger(markerOffset) || markerOffset < 0 || markerOffset % 4 ||
    markerOffset + expected.length * 4 > file.data.length ||
    expected.some((word, i) => bufReadU32BE(file.data, markerOffset + i * 4) !== word) ||
    patch.removedFiles.includes(file.name!) || patch.patches[file.name!]?.length)
    throw new Error('Missing certified native MM Big Octo combat overlay');
  for (const offset of [16, 20, 24, 28]) {
    const callback = bufReadU32BE(file.data, profile + offset);
    if (callback < vram || callback >= vram + file.data.length || callback % 4)
      throw new Error('Invalid native MM Big Octo callback');
  }
}

function nativeBigOctoPlacementY(rom: Uint8Array, offset: number, addresses?: GameAddresses, patch?: Patchfile): number {
  if (offset !== slot.offset || slot.original.length !== 32 || !matchesRomBytes(rom, offset, slot.original))
    throw new Error('Uncertified MM Big Octo water slot');
  const modified = 'Staged patch modifies certified MM Big Octo water geometry';
  verifyPlacementGeometry('mm', rom, slot.collisionRanges, {
    changed: 'MM Big Octo water geometry changed',
    missingAddresses: modified,
    modified,
    checkReplacement: true,
  }, addresses, patch);
  return slot.targetY;
}

function verifyNativeBigOctoRecord(rom: Uint8Array, offset: number, original: Uint8Array, replacement: Uint8Array) {
  const y = nativeBigOctoPlacementY(rom, offset);
  const raw = rom.subarray(offset, offset + 16);
  if (original.length !== 16 || replacement.length !== 16 || !bufEqual(original, raw))
    throw new Error('Invalid MM Big Octo original actor record');
  const expected = original.slice();
  bufWriteU16BE(expected, 0, (bufReadU16BE(original, 0) & 0xf000) | slot.targetActorId);
  bufWriteU16BE(expected, 4, y);
  bufWriteU16BE(expected, 14, slot.targetParams);
  if (!bufEqual(replacement, expected))
    throw new Error('Invalid MM Big Octo replacement actor record');
}

type NativeTarget = { game: Game; actorId: number; spawnParams: number };

function nativeOotSlotAllows(mq: boolean, loc: EnemyRoomLocation, slot: number,
  source: number, target: NativeTarget, params?: number): boolean {
  if (source === 0x6b && (mq || loc.scene !== 4 || ![18, 19].includes(loc.room) || loc.setup !== 0 ||
    slot < 1 || slot > (loc.room === 18 ? 5 : 8))) return false;
  if (source === 0xc7 && ![0, 1, 0xffff].includes(params ?? -1)) return false;
  if (source === 0x192 && (mq || loc.scene !== 0 || loc.room !== 1 || loc.setup !== 0 ||
    slot !== 0 || params !== 0x1a00)) return false;
  if (source === 0x37 && params !== 0 && params !== 1) return false;
  if (source === 0x95 && params !== 0) return false;
  if (source === 0x60 && (params === undefined || !Number.isInteger(params) || params < 0 || params > 0xff00 || (params & 0xff))) return false;
  if (source === 0x99 && params !== 0) return false;
  if (source === 0x2d && params !== 0) return false;
  if (target.game !== 'oot') return true;
  if (target.actorId === 0x95) return false;
  if (target.actorId === 0x1c0) return [0, 1].includes(target.spawnParams);
  if (target.actorId === 0x115) return target.spawnParams === 0xffff;
  if (target.actorId === 0xf6) return target.spawnParams === 3;
  if ([0x2d, 0x37, 0x60, 0x99].includes(target.actorId)) return target.spawnParams === 0;
  return true;
}

const nativeBigPoeFacts = { 'oot': { 'actorId': 373, 'objectId': 109, 'table': 11932464, 'tableBytes': '00e7504000e78a2080af7d6080afb7d00000000080afb2300000000000000000', 'vromStart': 15159360, 'vromEnd': 15174176, 'vramStart': 2158984544, 'crc32': 3702485317, 'profileOffset': 15172880, 'profileBytes': '0175050000001035006d0000000002cc80af7d6080af7f6880afa6ec80afab30', 'objectTable': 11989696, 'objectBytes': '018c8000018cea40', 'objectStart': 25985024, 'objectEnd': 26012224, 'objectCrc32': 1935558971, 'hooks': [{ 'vrom': 15167848, 'original': '0c008134' }, { 'vrom': 15167800, 'original': '0c01bf73', 'symbol': 'EnPoField_Item_Give' }, { 'vrom': 15167824, 'original': '0c01bf73', 'symbol': 'EnPoField_Item_Give' }] }, 'mm': { 'actorId': 520, 'objectId': 497, 'table': 12883472, 'tableBytes': '00fc676000fca64080b615e080b654c00000000080b64ff00000000000000000', 'vromStart': 16541536, 'vromEnd': 16557632, 'vramStart': 2159416800, 'crc32': 2202291957, 'profileOffset': 16556400, 'profileBytes': '020805000000121501f100000000039880b615e080b6186c80b63ed400000000', 'objectTable': 12950536, 'objectBytes': '01aff00001b04c20', 'objectStart': 28307456, 'objectEnd': 28331040, 'objectCrc32': 215781870, 'hooks': [], 'patches': [{ 'vrom': 16549580, 'original': '0c05e565468053204600040d4408800000000000', 'replacement': '0000000000000000000000000000000024080008' }, { 'vrom': 16549092, 'original': '240600ba', 'replacement': (0x24060000 | GI.MM_SCOOP.index).toString(16) }] } };

const nativeBubbleFacts = { 'actorId': 105, 'objectId': 93, 'table': 11923888, 'tableBytes': '00cb162000cb52f080913ea080917b7000000000809176700000000000000000', 'vromStart': 13309472, 'vromEnd': 13325040, 'vramStart': 2157002400, 'crc32': 3267939156, 'profileOffset': 13323760, 'profileBytes': '0069050001000015005d00000000031880914128809144d080916e5880917128', 'objectTable': 11989568, 'objectBytes': '012590000125aa40', 'objectStart': 19238912, 'objectEnd': 19245632, 'objectCrc32': 2931281406, 'hooks': [], 'patches': [{ 'vrom': 13321111, 'original': '30', 'replacement': '00' }] };

const nativeEyegoreFacts = { 'mm': { 'actorId': 388, 'objectId': 397, 'table': 12879248, 'tableBytes': '00ee43e000ee8c2080a7c99080a811d00000000080a80dec0000000000000000', 'vromStart': 15614944, 'vromEnd': 15633440, 'vramStart': 2158479760, 'crc32': 3067335304, 'profileOffset': 15632444, 'profileBytes': '0184050080000035018d000000001e7080a7cd6080a7d0f080a7f35480a7ffb8', 'objectTable': 12949736, 'objectBytes': '017ad000017c01f0', 'objectStart': 24825856, 'objectEnd': 24904176, 'objectCrc32': 3039087373, 'hooks': [{ 'vrom': 15616364, 'original': '0c04f592', 'symbol': 'ENEMY_NATIVE_EYEGORE_PATH_V1' }, { 'vrom': 15615880, 'original': '0c04f5a3', 'symbol': 'ENEMY_NATIVE_EYEGORE_POINT_V1' }] } };

const nativeFighterFacts = { 'oot': { 'actorId': 407, 'objectId': 361, 'table': 11933552, 'tableBytes': '00ebc84000ec1bf080b4111080b464c00000000080b45d900000000000000000', 'vromStart': 15452224, 'vromEnd': 15473648, 'vramStart': 2159284496, 'crc32': 3251889401, 'profileOffset': 15471808, 'profileBytes': '019705000000001501690000000004d480b4111c80b4134080b452e080b4588c', 'objectTable': 11991712, 'objectBytes': '0188f0000189a6f0', 'objectStart': 25751552, 'objectEnd': 25798384, 'objectCrc32': 2663731861, 'hooks': [{ 'vrom': 15462336, 'original': '0c00863b', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 15452800, 'original': '0c031a25', 'symbol': 'ENEMY_NATIVE_FIGHTER_RESTORE_V1' }, { 'vrom': 15468328, 'original': '0c031a25', 'symbol': 'ENEMY_NATIVE_FIGHTER_RESTORE_V1' }, { 'vrom': 15470724, 'original': '908e000a', 'symbol': 'ENEMY_NATIVE_FIGHTER_CAUGHT_V1' }, { 'vrom': 15462352, 'original': '0c037385', 'symbol': 'ENEMY_NATIVE_FIGHTER_TEXT_V1' }], 'patches': [{ 'vrom': 15470728, 'original': '3c028012', 'replacement': '00000000' }, { 'vrom': 15470796, 'original': 'a42b1e1a', 'replacement': '00000000' }, { 'vrom': 15470840, 'original': 'a42e1e1a', 'replacement': '00000000' }, { 'vrom': 15470856, 'original': 'a4381e1a', 'replacement': '00000000' }] }, 'mm': { 'actorId': 541, 'objectId': 516, 'table': 12884144, 'tableBytes': '00fea70000ff044080b8559080b8b2d00000000080b8ab5c0000000000000000', 'vromStart': 16688896, 'vromEnd': 16712768, 'vramStart': 2159564176, 'crc32': 3994638374, 'profileOffset': 16710860, 'profileBytes': '021d05000010001502040000000005f080b8559080b8580080b8a0a000000000', 'objectTable': 12950688, 'objectBytes': '01b7a00001b89900', 'objectStart': 28811264, 'objectEnd': 28875008, 'objectCrc32': 123701028, 'hooks': [{ 'vrom': 16707560, 'original': '0c2e2583', 'symbol': 'ENEMY_NATIVE_FIGHTER_DEATH_V1' }, { 'vrom': 16707772, 'original': '0c2e2583', 'symbol': 'ENEMY_NATIVE_FIGHTER_DEATH_V1' }, { 'vrom': 16691632, 'original': '0c02dca6', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 16692272, 'original': '0c02dca6', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 16692984, 'original': '0c02dca6', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 16693700, 'original': '0c02dca6', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 16694284, 'original': '0c02dca6', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 16695064, 'original': '0c02dca6', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 16696068, 'original': '0c02dca6', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 16705556, 'original': '0c02dca6', 'symbol': 'ENEMY_NATIVE_FIGHTER_CS_V1' }, { 'vrom': 16689584, 'original': '0c068bb6', 'symbol': 'ENEMY_NATIVE_FIGHTER_RESTORE_V1' }, { 'vrom': 16694352, 'original': '0c04c1a9', 'symbol': 'ENEMY_NATIVE_FIGHTER_THROWOUT_V1' }, { 'vrom': 16691584, 'original': '0c03c6f9', 'symbol': 'ENEMY_NATIVE_FIGHTER_NEXT_V1' }, { 'vrom': 16693652, 'original': '0c03c6f9', 'symbol': 'ENEMY_NATIVE_FIGHTER_NEXT_V1' }, { 'vrom': 16705912, 'original': '0c03c6f9', 'symbol': 'ENEMY_NATIVE_FIGHTER_NEXT_V1' }, { 'vrom': 16706572, 'original': '0c03c6f9', 'symbol': 'ENEMY_NATIVE_FIGHTER_NEXT_V1' }, { 'vrom': 16706800, 'original': '0c03c6f9', 'symbol': 'ENEMY_NATIVE_FIGHTER_NEXT_V1' }, { 'vrom': 16691600, 'original': '0c03c6e9', 'symbol': 'ENEMY_NATIVE_FIGHTER_QUEUE_V1' }, { 'vrom': 16693668, 'original': '0c03c6e9', 'symbol': 'ENEMY_NATIVE_FIGHTER_QUEUE_V1' }, { 'vrom': 16705928, 'original': '0c03c6e9', 'symbol': 'ENEMY_NATIVE_FIGHTER_QUEUE_V1' }, { 'vrom': 16706588, 'original': '0c03c6e9', 'symbol': 'ENEMY_NATIVE_FIGHTER_QUEUE_V1' }, { 'vrom': 16706816, 'original': '0c03c6e9', 'symbol': 'ENEMY_NATIVE_FIGHTER_QUEUE_V1' }, { 'vrom': 16691008, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16697860, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16698496, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16699260, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16700520, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16701248, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16704168, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16705360, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16706976, 'original': '0c2e1fdc', 'symbol': 'ENEMY_NATIVE_FIGHTER_SLASH_V1' }, { 'vrom': 16696796, 'original': '0c2e2244', 'symbol': 'ENEMY_NATIVE_FIGHTER_SPIN_V1' }, { 'vrom': 16699540, 'original': '0c2e2244', 'symbol': 'ENEMY_NATIVE_FIGHTER_SPIN_V1' }, { 'vrom': 16701500, 'original': '0c2e2244', 'symbol': 'ENEMY_NATIVE_FIGHTER_SPIN_V1' }], 'patches': [{ 'vrom': 16712492, 'original': '440048e8', 'replacement': '00000000', 'required': true }, { 'vrom': 16712500, 'original': '440049bc', 'replacement': '00000000', 'required': true }, { 'vrom': 16711580, 'original': '44000840', 'replacement': '00000000', 'required': true }, { 'vrom': 16711860, 'original': '44002304', 'replacement': '00000000', 'required': true }, { 'vrom': 16711892, 'original': '44002580', 'replacement': '00000000', 'required': true }, { 'vrom': 16711984, 'original': '4400287c', 'replacement': '00000000', 'required': true }, { 'vrom': 16712096, 'original': '44002d68', 'replacement': '00000000', 'required': true }, { 'vrom': 16712144, 'original': '44003040', 'replacement': '00000000', 'required': true }, { 'vrom': 16712308, 'original': '44003ba8', 'replacement': '00000000', 'required': true }, { 'vrom': 16712400, 'original': '44004050', 'replacement': '00000000', 'required': true }, { 'vrom': 16712472, 'original': '440046a0', 'replacement': '00000000', 'required': true }, { 'vrom': 16711808, 'original': '44001edc', 'replacement': '00000000', 'required': true }, { 'vrom': 16712008, 'original': '44002994', 'replacement': '00000000', 'required': true }, { 'vrom': 16712164, 'original': '4400313c', 'replacement': '00000000', 'required': true }] } };

const nativeGaroFacts = { 'mm': { 'actorId': 275, 'objectId': 341, 'table': 12875632, 'tableBytes': '00e2059000e24200809ad8e0809b155000000000809b0f680000000000000000', 'vromStart': 14812560, 'vromEnd': 14828032, 'vramStart': 2157631712, 'crc32': 3369156211, 'profileOffset': 14826520, 'profileBytes': '01130500000000150155000000000fec809ad8e0809adb24809b02cc00000000', 'objectTable': 12949288, 'objectBytes': '0160500001610600', 'objectStart': 23089152, 'objectEnd': 23135744, 'objectCrc32': 3492899932, 'hooks': [{ 'vrom': 14813112, 'original': '0c26b71f' }], 'patches': [{ 'vrom': 14827180, 'original': '44000228', 'replacement': '00000000', 'required': true }] } };

const nativeLeeverFacts = { 'oot': { 'actorId': 28, 'objectId': 23, 'table': 11921424, 'tableBytes': '00c2de6000c2f8d080890590808920000000000080891d100000000000000000', 'vromStart': 12770912, 'vromEnd': 12777680, 'vramStart': 2156463504, 'crc32': 104559866, 'profileOffset': 12776928, 'profileBytes': '001c08000800001500170000000002cc808905908089076c8089199880891c48', 'objectTable': 11989008, 'objectBytes': '0105000001051ef0', 'objectStart': 17104896, 'objectEnd': 17112816, 'objectCrc32': 1335972952, 'hooks': [{ 'vrom': 12771312, 'original': '0c00d0f5' }, { 'vrom': 12772416, 'original': '0c00d0f5' }, { 'vrom': 12772736, 'original': '0c00d0f5' }] }, 'mm': { 'actorId': 534, 'objectId': 513, 'table': 12883920, 'tableBytes': '00fe1a1000fe3ab080b7c89080b7e9300000000080b7e4e00000000000000000', 'vromStart': 16652816, 'vromEnd': 16661168, 'vramStart': 2159528080, 'crc32': 2594600561, 'profileOffset': 16660064, 'profileBytes': '0216050000000205020100000000031480b7c89080b7ca0880b7e2c480b7e424', 'objectTable': 12950664, 'objectBytes': '01b6000001b61ef0', 'objectStart': 28704768, 'objectEnd': 28712688, 'objectCrc32': 1335972952, 'hooks': [] } };

const nativePoeSisterFacts = { 'oot': { 'actorId': 145, 'objectId': 153, 'table': 11925168, 'tableBytes': '00cd9a6000cde7508093c340809410300000000080940a900000000000000000', 'vromStart': 13474400, 'vromEnd': 13494096, 'vramStart': 2157167424, 'crc32': 3284649197, 'profileOffset': 13492656, 'profileBytes': '009105000000521500990000000003288093c3408093c58c8093f6988094040c', 'objectTable': 11990048, 'objectBytes': '013a5000013b33c0', 'objectStart': 20598784, 'objectEnd': 20657088, 'objectCrc32': 106976798, 'hooks': [{ 'vrom': 13479432, 'original': '0c008134', 'symbol': 'ENEMY_NATIVE_POE_SISTER_SWITCH_V1' }, { 'vrom': 13483152, 'original': '0c008134', 'symbol': 'ENEMY_NATIVE_POE_SISTER_SWITCH_V1' }, { 'vrom': 13483196, 'original': '0c008144', 'symbol': 'ENEMY_NATIVE_POE_SISTER_UNSET_V1' }, { 'vrom': 13484328, 'original': '0c031a08', 'symbol': 'ENEMY_NATIVE_POE_SISTER_BGM_V1' }, { 'vrom': 13475052, 'original': '0c031a25', 'symbol': 'ENEMY_NATIVE_POE_SISTER_RESTORE_V1' }, { 'vrom': 13477144, 'original': '0c01adbf', 'symbol': 'ENEMY_NATIVE_POE_SISTER_CS_V1' }, { 'vrom': 13478180, 'original': '0c01adbf', 'symbol': 'ENEMY_NATIVE_POE_SISTER_CS_V1' }, { 'vrom': 13486036, 'original': '0c01adbf', 'symbol': 'ENEMY_NATIVE_POE_SISTER_CS_V1' }, { 'vrom': 13487676, 'original': '0c24f5c7', 'symbol': 'ENEMY_NATIVE_POE_SISTER_HOVER_V1' }], 'patches': [{ 'vrom': 13493732, 'original': '440033dc', 'replacement': '00000000', 'required': true }, { 'vrom': 13477667, 'original': '64', 'replacement': '1e' }] }, 'mm': { 'actorId': 488, 'objectId': 453, 'table': 12882448, 'tableBytes': '00f7f6b000f831b080b1a3b080b1deb00000000080b1da500000000000000000', 'vromStart': 16250544, 'vromEnd': 16265648, 'vramStart': 2159125424, 'crc32': 2145189594, 'profileOffset': 16264528, 'profileBytes': '01e805000000501501c500000000039880b1a3b080b1a60080b1c61c80b1d484', 'objectTable': 12950184, 'objectBytes': '019f6000019fc5d0', 'objectStart': 27222016, 'objectEnd': 27248080, 'objectCrc32': 45628473, 'hooks': [], 'patches': [] } };

const nativeTakkuriFacts = { 'actorId': 657, 'objectId': 34, 'table': 12887856, 'tableBytes': '010756f0010788a080c1077080c139300000000080c135a00000000000000000', 'vromStart': 17258224, 'vromEnd': 17270944, 'vramStart': 2160134000, 'crc32': 1889822134, 'profileOffset': 17270048, 'profileBytes': '0291050080001205002200000000099080c1077080c1095880c12ed480c134d0', 'objectTable': 12946832, 'objectBytes': '0126c000012723e0', 'objectStart': 19316736, 'objectEnd': 19342304, 'objectCrc32': 319473385, 'hooks': [{ 'vrom': 17263500, 'original': '0c3044ce' }, { 'vrom': 17266736, 'original': '0c3044ce' }], 'patches': [{ 'vrom': 17259148, 'original': '27bdffd0afbf0014', 'replacement': '03e0000824020000' }, { 'vrom': 17270584, 'original': '4400149c', 'replacement': '00000000', 'required': true }, { 'vrom': 17270788, 'original': '44002140', 'replacement': '00000000', 'required': true }] };

const nativeTentacleFacts = { 'oot': { 'actorId': 222, 'objectId': 213, 'table': 11927632, 'tableBytes': '00d7688000d78750809e9180809eb05000000000809eae500000000000000000', 'vromStart': 14116992, 'vromEnd': 14124880, 'vramStart': 2157875584, 'crc32': 2485555925, 'profileOffset': 14124368, 'profileBytes': '00de05000000001500d50000000003b0809e918c809e9378809ea990809eaa70', 'objectTable': 11990528, 'objectBytes': '015aa000015acbf0', 'objectStart': 22716416, 'objectEnd': 22727664, 'objectCrc32': 2953453030, 'hooks': [{ 'vrom': 14117304, 'original': '0c008127', 'symbol': 'ENEMY_NATIVE_TENTACLE_GET_V1' }, { 'vrom': 14123088, 'original': '0c008134', 'symbol': 'ENEMY_NATIVE_TENTACLE_SET_V1' }] } };

type FixedEdit = { vrom: number; original: string; replacement: string; required?: boolean };

type NativeActivationFacts = {
  table: number; tableBytes: string; vromStart: number; vromEnd: number;
  crc32: number; profileOffset: number; profileBytes: string; objectTable: number;
  objectBytes: string; objectStart: number; objectEnd: number; objectCrc32: number;
  hooks: { vrom: number; original: string; symbol?: string }[]; patches?: FixedEdit[];
};

const hex = (text: string) => Uint8Array.from(text.match(/../g)?.map(byte => Number.parseInt(byte, 16)) ?? []);

function verifyNativeActivationSource(name: string, game: Game, f: NativeActivationFacts, rom: Uint8Array,
  addresses?: GameAddresses, patch?: Patchfile, hookSymbol?: string) {
  const same = (offset: number, signature: string) =>
    signature.length % 2 === 0 && matchesRomBytes(rom, offset, signature);
  const overlay = rom.subarray(f.vromStart, f.vromEnd), model = rom.subarray(f.objectStart, f.objectEnd);
  if (!same(f.table, f.tableBytes) || !same(f.profileOffset, f.profileBytes) || !same(f.objectTable, f.objectBytes) ||
    f.vromEnd > rom.length || f.objectEnd > rom.length || (CRC32.buf(overlay) >>> 0) !== f.crc32 || (CRC32.buf(model) >>> 0) !== f.objectCrc32 ||
    f.hooks.some(h => h.original.length !== 8 || !same(h.vrom, h.original)) ||
    (f.patches ?? []).some(h => !same(h.vrom, h.original) || h.original.length !== h.replacement.length))
    throw Error(`Native ${name} source/model revision mismatch`);
  if (f.hooks.length) {
    const layout = readOverlayLayout(overlay);
    layout.relocations.forEach((r, index) => {
      if (!f.hooks.some(h => h.vrom === f.vromStart + r.offset)) return;
      const vrom = f.vromStart + layout.relocationOffset + 20 + index * 4;
      if (r.type !== 4 || !f.patches?.some(h => h.vrom === vrom && h.required && h.replacement === '00000000'))
        throw Error(`Native ${name} hook retains an original overlay relocation`);
    });
  }
  if (!patch) return;
  if (!addresses) throw Error(`Missing native ${name} staged addresses`);
  const edits = new Map<number, { expected: Uint8Array; actual: Uint8Array; required: boolean; touched: boolean }>();
  const expect = (vrom: number, expected: Uint8Array, required: boolean) => {
    if (edits.has(vrom)) throw Error(`Conflicting native ${name} activation proof`);
    edits.set(vrom, { expected, actual: rom.slice(vrom, vrom + expected.length), required, touched: false });
  };
  const files = patch.newFiles.filter(n => n.name === `${game}/payload`), payload = files.length === 1 ? files[0] : undefined;
  const span = payload?.vram?.[game], base = game === 'oot' ? 0x80400000 : 0x80700000;
  for (const hook of f.hooks) {
    const key = hook.symbol ?? hookSymbol, symbols = key ? patch.symbols[game].get(key) : undefined, destination = symbols?.[0];
    if (symbols?.length !== 1 || destination === undefined || !Number.isInteger(destination) || destination % 4 ||
      !span || span[0] !== base || span[1] > base + 0x80000 || span[1] - span[0] !== payload!.data.length || destination < span[0] || destination + 4 > span[1])
      throw Error(`Rebuild ${game} payload for native ${name} activation`);
    const expected = new Uint8Array(4);
    bufWriteU32BE(expected, 0, (0x0c000000 | ((destination >>> 2) & 0x03ffffff)) >>> 0);
    expect(hook.vrom, expected, true);
  }
  for (const edit of f.patches ?? []) expect(edit.vrom, hex(edit.replacement), !!edit.required);
  const expectedEdits = [...edits];
  for (const [start, end] of [[f.table, f.table + 32], [f.vromStart, f.vromEnd], [f.objectTable, f.objectTable + 8], [f.objectStart, f.objectEnd]]) {
    const a = addresses[game].fileFromROM(start), b = addresses[game].fileFromROM(end - 1);
    if (a.name !== b.name || b.offset - a.offset !== end - start - 1 || patch.removedFiles.includes(a.name) || patch.newFiles.some(n => n.name === a.name))
      throw Error(`Modified native ${name} source/model`);
    for (const edit of patch.patches[a.name] ?? []) {
      if (edit.addr >= b.offset + 1 || edit.addr + edit.data.length <= a.offset) continue;
      const at = start + edit.addr - a.offset;
      for (let i = 0; i < edit.data.length; i++) {
        const byte = at + i;
        const allowed = expectedEdits.find(([vrom, row]) => byte >= vrom && byte < vrom + row.expected.length);
        if (!allowed) throw Error(`Modified native ${name} ${game} source/model at ${byte.toString(16)}`);
        allowed[1].actual[byte - allowed[0]] = edit.data[i];
        allowed[1].touched = true;
      }
    }
  }
  for (const [vrom, row] of edits) if ((row.required || row.touched) && (!row.touched || !bufEqual(row.actual, row.expected)))
    throw Error(`Incomplete native ${name} activation hooks or relocation edits at ${vrom.toString(16)}`);
}

type Target = { key: string; game: Game; actorId: number; objectId: number; spawnParams: number; killMacro: string };

type NativeFightRestorationLocation = EnemyRoomLocation & { start: number; end: number; mq?: boolean };

type NativeFightRestorationPlan = { actorOffset: number; changes: Change[] };

function planNativeFightRestoration(game: Game, rom: Uint8Array, loc: NativeFightRestorationLocation,
  sourceKey: string, target: Target): NativeFightRestorationPlan | undefined {
  const source = nativeFightRestorationSource(sourceKey);
  const arena = fighterFacts.find(arena => arena.game === game && arena.source === sourceKey && !loc.mq && arena.scene === loc.scene &&
    arena.room === loc.room && arena.setup === loc.setup && arena.start === loc.start && arena.end === loc.end);
  if (!source || !arena || source.game !== game || arena.actorOffset + 16 > rom.length) return;
  const original = rom.subarray(arena.actorOffset, arena.actorOffset + 16);
  if (!matchesRestorationRecord(original, arena.original)) return;
  const mm = game === 'mm';
  const fields = [
    [0, (mm ? bufReadU16BE(original, 0) & 0xe000 : 0) | target.actorId],
    [8, mm ? bufReadU16BE(original, 8) & 7 : 0],
    [10, mm ? (bufReadU16BE(original, 10) & 0xff80) | 0x7f : bufReadU16BE(original, 10)],
    [12, mm ? bufReadU16BE(original, 12) & 0x7f : 0],
    [14, target.spawnParams],
  ];
  const changes: Change[] = [];
  for (const [field, replacement] of fields) {
    const value = replacement & 0xffff;
    if (value !== bufReadU16BE(original, field)) changes.push({ offset: arena.actorOffset + field, value });
  }
  return { actorOffset: arena.actorOffset, changes };
}

function matchesRestorationRecord(original: Uint8Array, expected: string): boolean {
  return original.length === 16 && expected.length === 32 &&
    original.every((byte, i) => byte === Number.parseInt(expected.slice(i * 2, i * 2 + 2), 16));
}

function verifiedNativeFightRestorationRecord(game: Game, vrom: number, original: Uint8Array): boolean {
  return fighterFacts.some(fact => fact.game === game && fact.actorOffset === vrom && matchesRestorationRecord(original, fact.original));
}

const SCRIPTED_MM_CONTROLLER_ID = 0x37;

function verifyScriptedMmController(rom: Uint8Array, addresses: GameAddresses, patch: Patchfile): void {
  for (const capability of ['ENEMY_SCRIPTED_MM_V1', 'ENEMY_SCRIPTED_MM_LEASE_V1']) {
    const value = patch.symbols.mm.get(capability);
    if (value?.length !== 1 || !Number.isInteger(value[0]) || value[0] % 4 || value[0] < 0x80000000 || value[0] >= 0x80800000)
      throw new Error('Rebuild native scripted MM controller lease');
  }
  const at = CONFIG.mm.actorsOvlAddr + SCRIPTED_MM_CONTROLLER_ID * 32;
  if (rom.subarray(at, at + 32).some(byte => byte !== 0)) throw new Error('Scripted MM controller slot is not unused');
  const address = addresses.mm.fileFromROM(at), table = Uint8Array.from(rom.subarray(at, at + 32));
  applyOverlappingEdits(table, address.offset, patch.patches[address.name] ?? []);
  const vrom = bufReadU32BE(table, 0), vram = bufReadU32BE(table, 8), init = bufReadU32BE(table, 20);
  const files = patch.newFiles.filter(file => file.name?.endsWith('/mm_ovl_Custom_Enemy_Arena.zovlx') && file.vrom === vrom);
  const file = files.length === 1 ? files[0] : undefined, profile = init - vram;
  if (!file || !file.vram?.mm || file.vram.mm[0] !== vram || file.vram.mm[1] !== bufReadU32BE(table, 12) ||
    bufReadU32BE(table, 4) !== vrom + file.data.length || profile < 0 || profile + 32 > file.data.length ||
    bufReadU16BE(file.data, profile) !== SCRIPTED_MM_CONTROLLER_ID || file.data[profile + 2] !== 8 ||
    bufReadU16BE(file.data, profile + 8) !== 1 || bufReadU32BE(file.data, profile + 12) < 0x160)
    throw new Error('Missing installed native scripted MM arena overlay');
  const marker = patch.symbols.mm.get('ENEMY_SCRIPTED_MM_DRIVER_LEASE_ABI_V1');
  const markerOffset = marker?.length === 1 ? marker[0] - vram : -1;
  const expected = [0x45534c41, 1, 0x16c, 0x160, 0x164, 0x16c, bufReadU32BE(file.data, profile + 12), 0x4c534531];
  if (!Number.isInteger(markerOffset) || markerOffset < 0 || markerOffset % 4 || markerOffset + 32 > file.data.length ||
    expected.some((word, i) => bufReadU32BE(file.data, markerOffset + i * 4) !== word))
    throw new Error('Stale scripted MM controller lease ABI');
  for (const offset of [16, 20, 24]) {
    const callback = bufReadU32BE(file.data, profile + offset);
    if (callback < vram || callback >= file.vram.mm[1] || callback % 4) throw new Error('Invalid scripted MM controller callback');
  }
}

function verifyScriptedMmRecord(rom: Uint8Array, sourceVrom: number, original: Uint8Array,
  targetId: number, targetParams: number, addresses: GameAddresses, patch: Patchfile): void {
  const sourceId = bufReadU16BE(original, 0) & 0x1fff;
  const rows = SCRIPTED_MM_ARENAS.filter(row => row.roots.some(root => root.offset === sourceVrom && root.actorId === sourceId &&
    original.every((byte, i) => byte === Number.parseInt(root.raw.slice(i * 2, i * 2 + 2), 16))));
  if (!rows.length || rows.some(row => CRC32.buf(rom.subarray(row.roomStart, row.roomEnd)) >>> 0 !== row.roomCrc32 ||
    CRC32.buf(rom.subarray(row.sceneStart, row.sceneEnd)) >>> 0 !== row.sceneCrc32) ||
    !verifyScriptedMmNativeSource(rom, sourceId, addresses, patch)) throw new Error('Unverified scripted MM source or arena');
  const source = ENEMIES.find(row => scriptedMmSource(row.key)?.actorId === sourceId);
  const target = ENEMIES.find(row => row.game === (targetId >= 0xc00 ? 'oot' : 'mm') &&
    row.actorId === (targetId >= 0xc00 ? targetId - 0xc00 : targetId) && row.spawnParams === targetParams && scriptedMmTargetAllowed(row, true, source));
  if (!source || !(target || targetId < 0xc00 && nativeResourceTarget('mm', targetId, targetParams)))
    throw new Error('Unreviewed scripted MM target');
  if (targetId === 0x16f && !verifyScriptedMmNativeTarget(rom, targetId, targetParams, addresses, patch))
    throw new Error('Unverified dungeon Bombchu terminal update');
  if (targetId >= 0xc00 && !patch.symPatches.has('FOREIGN_ENEMY_REGISTRY_MM')) throw new Error('Missing scripted MM donor registry');
  verifyScriptedMmController(rom, addresses, patch);
}

type ScriptedMmArena = typeof scriptedArenaFacts.arenas[number];

type ScriptedMmArenaLocation = {
  scene: number;
  room: number;
  start: number;
  end: number;
  actorOffset?: number;
  roomHeader?: number;
  sceneHeader?: number;
};

const SCRIPTED_MM_ARENAS: readonly ScriptedMmArena[] = scriptedArenaFacts.arenas;

function verifiedScriptedMmArenas(rom: Uint8Array, loc: ScriptedMmArenaLocation,
  addresses?: GameAddresses, patch?: Patchfile): readonly ScriptedMmArena[] {
  const rows = SCRIPTED_MM_ARENAS.filter(arena =>
    arena.scene === loc.scene && arena.room === loc.room && arena.roomStart === loc.start && arena.roomEnd === loc.end &&
    (loc.actorOffset === undefined || arena.roots.some(root => root.offset === loc.actorOffset)) &&
    (loc.roomHeader === undefined || arena.roomHeader === loc.roomHeader) &&
    (loc.sceneHeader === undefined || arena.sceneHeader === loc.sceneHeader));
  try {
    for (const row of rows) {
      for (const [start, end, crc] of [[row.sceneStart, row.sceneEnd, row.sceneCrc32], [row.roomStart, row.roomEnd, row.roomCrc32]]) {
        if (start < 0 || end <= start || end > rom.length || CRC32.buf(rom.subarray(start, end)) >>> 0 !== crc) return [];
        if (patch && (!addresses || !reviewedNativeLayoutFile(rom, 'mm', start, end, addresses, patch))) return [];
      }
    }
  } catch {
    return [];
  }
  return rows;
}

function verifyScriptedMmNativeSource(rom: Uint8Array, actorId: number, addresses?: GameAddresses, patch?: Patchfile): boolean {
  const fact = [...scriptedArenaFacts.nativeSources, ...scriptedArenaFacts.nativeSupports, ...scriptedArenaFacts.nativeTargets].find(f => f.actorId === actorId);
  if (!fact) return false;
  try {
    if (fact.vromStart < 0 || fact.vromEnd > rom.length || CRC32.buf(rom.subarray(fact.vromStart, fact.vromEnd)) >>> 0 !== fact.crc32 ||
      !rom.subarray(fact.table, fact.table + 32).every((b, i) => b === parseInt(fact.tableBytes.slice(i * 2, i * 2 + 2), 16))) return false;
    if (patch && actorId === 0x184) {
      verifyNativeActivationSource('Eyegore', 'mm', nativeEyegoreFacts.mm, rom, addresses, patch);
    }
    else if (patch) {
      if (!addresses) return false;
      for (const [start, end] of [[fact.vromStart, fact.vromEnd], [fact.table, fact.table + 32]]) {
        const file = addresses.mm.fileFromROM(start), last = addresses.mm.fileFromROM(end - 1);
        if (file.name !== last.name || last.offset - file.offset !== end - start - 1 || patch.removedFiles.includes(file.name) || patch.newFiles.some(f => f.name === file.name)) return false;
        const rows = (patch.patches[file.name] ?? []).filter(p => p.addr < last.offset + 1 && p.addr + p.data.length > file.offset);
        if (!rows.length) continue;
        if (start !== fact.vromStart || !fact.patches.length) return false;
        const expected = fact.patches.map(rule => {
          let data: Uint8Array;
          if (rule.symbol) {
            const syms = patch.symbols.mm.get(rule.symbol), files = patch.newFiles.filter(f => f.name === 'mm/payload');
            if (syms?.length !== 1 || files.length !== 1) throw Error('Missing native callback capability');
            const target = syms[0], span = files[0].vram?.mm;
            if (!span || span[1] - span[0] !== files[0].data.length || target % 4 || target < span[0] || target >= span[1]) throw Error('Invalid native callback target');
            data = new Uint8Array(8);
            bufWriteU32BE(data, 0, (0x08000000 | ((target >>> 2) & 0x3ffffff)) >>> 0);
          } else data = Uint8Array.from(rule.replacement.match(/../g)!.map(b => parseInt(b, 16)));
          return { offset: file.offset + rule.vrom - start, data };
        });
        if (rows.some(row => !expected.some(e => row.addr === e.offset && bufEqual(row.data, e.data))) ||
          expected.some(e => !rows.some(row => row.addr === e.offset && bufEqual(row.data, e.data)))) return false;
      }
    }
    return scriptedArenaFacts.supportDependencies.filter(d => d.sourceId === actorId).every(d => verifyScriptedMmNativeSource(rom, d.supportId, addresses, patch));
  } catch {
    return false;
  }
}

function verifyScriptedMmNativeTarget(rom: Uint8Array, actorId: number, params: number, addresses?: GameAddresses, patch?: Patchfile): boolean {
  return scriptedArenaFacts.nativeTargets.some(f => f.actorId === actorId && f.spawnParams === params) && verifyScriptedMmNativeSource(rom, actorId, addresses, patch);
}

type Setup = { actorOffsets: readonly number[]; complete: boolean };

type NativeMinibossSlot = {
  actorOffset: number;
  setups: readonly Setup[];
};

const WIZ = 0x15d;

const WIZZROBE_PLATFORM_ACTOR = ENGINE_ACTORS.find(actor => actor.game === 'mm' && actor.overlay === 'En_Wiz_Brock')!.actorId;

function planNativeMinibossVariant(rom: Uint8Array, game: Game, slot: NativeMinibossSlot,
  targetKey: NativeMinibossVariantKey): Change[] {
  const target = nativeMinibossVariant(targetKey), offset = slot.actorOffset;
  const validOffset = (value: number) => Number.isSafeInteger(value) && value >= 0 && value + 16 <= rom.length && value % 2 === 0;
  if (!target || game !== 'mm' || !validOffset(offset) || !slot.setups.length ||
    (bufReadU16BE(rom, offset) & 0x1fff) !== WIZ) return [];
  const params = bufReadU16BE(rom, offset + 14), type = params >>> 8;
  if (type > 1 || type === target.type || (bufReadU16BE(rom, offset + 10) & 0x7f) === 0x7f) return [];
  for (const setup of slot.setups) {
    if (!setup.complete || !setup.actorOffsets.includes(offset) || new Set(setup.actorOffsets).size !== setup.actorOffsets.length ||
      setup.actorOffsets.some(value => !validOffset(value))) return [];
    let platforms = 0;
    let wizzrobes = 0;
    for (const actorOffset of setup.actorOffsets) {
      const id = bufReadU16BE(rom, actorOffset) & 0x1fff;
      if (id === WIZZROBE_PLATFORM_ACTOR) ++platforms;
      if (id === WIZ) ++wizzrobes;
    }
    if (wizzrobes !== 1 || platforms < 2 || platforms > 10) return [];
  }
  return [{ offset: offset + 14, value: (params & 0xff) | target.type << 8 }];
}

const DEAD_HAND_ARENAS = [
  { scene: 7, room: 4, setup: 0, start: 0x27dc000, end: 0x27ddc40, actorCount: 8, arms: 6, actorCrc32: 0xc3e9644a, objectCrc32: 0x97a75f13 },
  { scene: 8, room: 4, setup: 0, start: 0x32fb000, end: 0x32fd450, actorCount: 7, arms: 4, actorCrc32: 0x749bb126, objectCrc32: 0x881d5bc0 },
] as const;

type DeadHandArenaLocation = { scene: number; room: number; setup: number; start: number; end: number; mq: boolean };

type NativeArenaCombatPlan = {
  changes: Change[];
  suppressedOriginalSlots: readonly number[];
  controllerSlot?: number;
  replacementSlot?: number;
};

function planDeadHandArenaCombat(rom: Uint8Array, loc: DeadHandArenaLocation,
  target: { actorId: number; spawnParams: number }): NativeArenaCombatPlan | undefined {
  const arena = DEAD_HAND_ARENAS.find(arena => !loc.mq && arena.scene === loc.scene && arena.room === loc.room &&
    arena.setup === loc.setup && arena.start === loc.start && arena.end === loc.end);
  if (!arena || !target || arena.end > rom.length) return;
  const actors = arena.start + 0x4c, objects = arena.start + 0x40;
  if (bufReadU32BE(rom, arena.start + 0x28) !== 0x0b050000 || bufReadU32BE(rom, arena.start + 0x2c) !== 0x03000040 ||
    bufReadU32BE(rom, arena.start + 0x30) !== (0x01000000 | arena.actorCount << 16) ||
    bufReadU32BE(rom, arena.start + 0x34) !== 0x0300004c ||
    CRC32.buf(rom.subarray(actors, actors + arena.actorCount * 16)) >>> 0 !== arena.actorCrc32 ||
    CRC32.buf(rom.subarray(objects, objects + 10)) >>> 0 !== arena.objectCrc32) return;
  const retained = Array.from({ length: arena.actorCount }, (_, index) => index)
    .filter(index => bufReadU16BE(rom, actors + index * 16) !== 0xa5);
  if (retained.length !== arena.actorCount - arena.arms ||
    retained.filter(index => bufReadU16BE(rom, actors + index * 16) === 0xa4).length !== 1) return;
  const changes: Change[] = [];
  const body = retained.find(original => bufReadU16BE(rom, actors + original * 16) === 0xa4)!;
  changes.push({ offset: actors + body * 16, value: target.actorId },
    { offset: actors + body * 16 + 14, value: target.spawnParams });
  return {
    changes, replacementSlot: body,
    suppressedOriginalSlots: Array.from({ length: arena.actorCount }, (_, slot) => slot)
      .filter(slot => bufReadU16BE(rom, actors + slot * 16) === 0xa5)
  };
}

const DARK_LINK_ARENA = {
  scene: 5, room: 13, setup: 0, start: 0x2647000, end: 0x2647a00,
  actorCrc32: 0x2775c650, objectCrc32: 0xdbd7f427,
} as const;

function planDarkLinkArenaCombat(rom: Uint8Array, loc: DeadHandArenaLocation,
  target: { actorId: number; spawnParams: number }): NativeArenaCombatPlan | undefined {
  const arena = DARK_LINK_ARENA;
  if (loc.mq || !target || loc.scene !== arena.scene || loc.room !== arena.room || loc.setup !== 0 ||
    loc.start !== arena.start || loc.end !== arena.end || arena.end > rom.length) return;
  if (bufReadU32BE(rom, arena.start + 0x28) !== 0x0b050000 || bufReadU32BE(rom, arena.start + 0x2c) !== 0x03000040 ||
    bufReadU32BE(rom, arena.start + 0x30) !== 0x01010000 || bufReadU32BE(rom, arena.start + 0x34) !== 0x0300004c ||
    CRC32.buf(rom.subarray(arena.start + 0x4c, arena.start + 0x5c)) >>> 0 !== arena.actorCrc32 ||
    CRC32.buf(rom.subarray(arena.start + 0x40, arena.start + 0x4a)) >>> 0 !== arena.objectCrc32) return;
  return { controllerSlot: 0, suppressedOriginalSlots: [], changes: [] };
}

const BIG_OCTO_ARENA = {
  scene: 2, room: 6, setup: 0, start: 41406464, end: 41423424,
  roomCrc32: 125093238, controllerSlot: 3, actorOffset: 108, actorCount: 9,
  objectOffset: 88, objectCount: 10, controller: '00c8fab0fbfff2f10000c00000001f00',
  overlayStart: 13941712, overlayEnd: 13946528, overlayCrc32: 3938908422
} as const;

function planBigOctoArenaCombat(rom: Uint8Array, loc: DeadHandArenaLocation,
  targetKey: BigOctoArenaCombatTargetKey): NativeArenaCombatPlan | undefined {
  const a = BIG_OCTO_ARENA, target = BIG_OCTO_ARENA_COMBAT_TARGETS.find(t => t.key === targetKey);
  if (!target || loc.mq || loc.scene !== a.scene || loc.room !== a.room || loc.setup !== 0 ||
    loc.start !== a.start || loc.end !== a.end || a.end > rom.length ||
    (CRC32.buf(rom.subarray(a.start, a.end)) >>> 0) !== a.roomCrc32 ||
    !rom.subarray(a.start + a.actorOffset + a.controllerSlot * 16,
      a.start + a.actorOffset + (a.controllerSlot + 1) * 16).every((value, i) =>
        value === Number.parseInt(a.controller.slice(i * 2, i * 2 + 2), 16))) return;
  const row = CONFIG.oot.actorsOvlAddr + 0xc8 * 32;
  if (bufReadU32BE(rom, row) !== a.overlayStart || bufReadU32BE(rom, row + 4) !== a.overlayEnd ||
    (CRC32.buf(rom.subarray(a.overlayStart, a.overlayEnd)) >>> 0) !== a.overlayCrc32) return;
  const profileRow = CONFIG.oot.actorsOvlAddr + target.actorId * 32;
  const begin = bufReadU32BE(rom, profileRow), end = bufReadU32BE(rom, profileRow + 4);
  const profile = begin + bufReadU32BE(rom, profileRow + 20) - bufReadU32BE(rom, profileRow + 8);
  if (!begin || end > rom.length || profile < begin || profile + 32 > end ||
    bufReadU16BE(rom, profile) !== target.actorId || rom[profile + 2] !== 5 ||
    bufReadU16BE(rom, profile + 8) !== target.objectId) return;
  const resident = Array.from({ length: a.objectCount }, (_, i) => bufReadU16BE(rom, a.start + a.objectOffset + i * 2));
  if (!resident.includes(target.objectId) || !resident.includes(0x15f)) return;
  return { controllerSlot: a.controllerSlot, suppressedOriginalSlots: [], changes: [] };
}

type RoomRelocation = { game: Game; oldStart: number; oldEnd: number; newStart: number; newEnd: number; name: string };

function remapRelocatedRoomKey(key: number, relocations: readonly RoomRelocation[]): number {
  const matches = relocations.filter(r => key >= r.oldStart && key < r.oldEnd);
  if (matches.length > 1) throw new Error('Conflicting room relocation key mappings');
  return matches.length ? matches[0].newStart + key - matches[0].oldStart : key;
}

type EnemySupplementalRoom = { game: Game; source: string; target: string; scene: number; room: number };

type EnemyRoomModelRoster = { scene: number; room: number; modelIds: number[]; halfDays: number[] };

function validRoomModelId(id: number): boolean {
  return Number.isInteger(id) && (id > 3 && id < 0x1000 || id >= 0x1004 && id < 0x2080);
}

function enemyRoomModelRosters(game: Game, plan: EnemyPlan, foreign: ReadonlyMap<string, ForeignEnemyPlacement>,
  supplemental: readonly EnemySupplementalRoom[]): EnemyRoomModelRoster[] {
  const rooms = new Map<string, { scene: number; room: number; ids: Map<number, number> }>();
  const add = (scene: number, room: number, ids: readonly number[], halfDays = 0x3ff) => {
    if (!Number.isInteger(scene) || scene < 0 || scene > 112 || !Number.isInteger(room) || room < 0 || room > 31)
      throw Error('Invalid enemy room model placement');
    const key = scene + '/' + room;
    const row = rooms.get(key) ?? { scene, room, ids: new Map<number, number>() };
    ids.forEach(id => row.ids.set(id, (row.ids.get(id) ?? 0) | halfDays));
    rooms.set(key, row);
  };
  for (const row of plan.flatMap(row => [row, ...[...new Set([...(row.splitChildren ?? []), ...(row.splitPool ?? [])])]
    .map(target => ({ ...row, target }))])) {
    const source = enemyDefinition(row.source), target = enemyDefinition(row.target);
    if (source.game !== game) continue;
    let ids: readonly number[];
    if (target.game === game) {
      const nativeIds = nativeEnemyOnDemandObjects(game, target.actorId, target.spawnParams);
      if (!nativeIds) continue;
      if (target.objectId > 3 && !nativeIds.includes(target.objectId)) throw Error('Missing native enemy profile model');
      ids = nativeIds;
    } else {
      const imported = foreign.get(row.target);
      if (!imported || imported.targetGame !== game || imported.sourceGame !== target.game ||
        imported.sourceActorId !== target.actorId || !imported.objects.pinned.has(imported.objectId))
        throw Error('Missing foreign room model closure');
      ids = [...imported.objects.pinned.keys()].filter(id => id < 0x1001 || id > 0x1003);
    }
    if (ids.some(id => !validRoomModelId(id))) throw Error('Invalid room model identity');
    if (row.ootr) {
      const location = row.ootr.location;
      add(location.scene, location.room, ids);
      for (const alias of row.ootr.aliases) {
        if (alias.length !== 4) throw Error('Invalid OoT room model alias');
        add(alias[0], alias[1], ids);
      }
    } else if (row.placement) {
      const placement = row.placement;
      const raw = placement.raw;
      if (raw.length !== 16) throw Error('Missing MM model schedule source');
      const halfDays = (((raw[9] & 7) << 7) | (raw[13] & 0x7f)) || 0x3ff;
      if (placement.sourceActorId === 0x12d && placement.sourceParams === 0)
        ids = [...ids, 0x15e];
      add(placement.scene, placement.room, ids, halfDays);
      const scene = MM_NATIVE_SCENES.find(candidate => candidate.scene === placement.scene);
      for (const map of scene?.maps ?? []) {
        if (map.room !== placement.room && map.actors.some(actor => actor.offset === placement.actorOffset))
          add(scene!.scene, map.room, ids, halfDays);
      }
    } else if (row.kind || target.game === game) {
      for (const binding of supplemental) {
        if (binding.game === game && binding.source === row.source && binding.target === row.target)
          add(binding.scene, binding.room, ids);
      }
    } else throw Error('Foreign room preload requires authored physical placements');
  }
  return [...rooms.values()].sort((a, b) => a.scene - b.scene || a.room - b.room)
    .map(row => {
      const modelIds = [...row.ids.keys()].sort((a, b) => a - b);
      return { scene: row.scene, room: row.room, modelIds, halfDays: modelIds.map(id => row.ids.get(id)!) };
    });
}

function encodeEnemyRoomModels(game: Game, rows: readonly EnemyRoomModelRoster[]): Uint8Array {
  const modelDataOffset = 16 + rows.length * 16;
  const size = modelDataOffset + rows.reduce((count, row) => count + row.modelIds.length, 0) * 4;
  if (rows.length > 4096 || size > 0x10000) throw Error('Enemy room model metadata capacity exceeded');

  const data = new Uint8Array(size);
  data.set(toU32Buffer([0x45524d31, game === 'mm' ? 2 : 1, game === 'oot' ? 0 : 1, rows.length]));
  const maxModels = 128 - (game === 'oot' ? 19 : 35);
  let cursor = 0;
  rows.forEach((row, index) => {
    if (!Number.isInteger(row.scene) || row.scene < 0 || row.scene > 112 ||
      !Number.isInteger(row.room) || row.room < 0 || row.room > 31 ||
      row.modelIds.length > maxModels || row.halfDays.length !== row.modelIds.length ||
      row.halfDays.some(days => !Number.isInteger(days) || days < 1 || days > 0x3ff) ||
      row.modelIds.some((id, j) => !validRoomModelId(id) || (j > 0 && row.modelIds[j - 1] >= id)))
      throw Error('Invalid enemy room model roster');

    const previous = rows[index - 1];
    if (previous && (previous.scene > row.scene || (previous.scene === row.scene && previous.room >= row.room)))
      throw Error('Unsorted enemy room model roster');

    [row.scene, row.room, cursor, row.modelIds.length].forEach((word, j) => bufWriteU32BE(data, 16 + index * 16 + j * 4, word));
    row.modelIds.forEach((id, j) => bufWriteU32BE(data, modelDataOffset + cursor++ * 4,
      game === 'mm' ? id | (row.halfDays[j] << 16) : id));
  });
  return data;
}

export function verifyMmEnemyRoomModelSlots(rows: readonly EnemyRoomModelRoster[]): void {
  for (const scene of new Set(rows.map(row => row.scene))) {
    const rooms = rows.filter(row => row.scene === scene);

    const maps = rooms.map(row => ({ room: row.room, setup: 0 }));
    for (const state of mmRoomOverlapStates(scene, maps)) {
      const ids = new Set(state.maps.flatMap(index => rooms[index].modelIds));
      if (ids.size > 128 - 35)
        throw Error(`MM scene ${scene} overlapping rooms exceed enemy model slots`);
    }
  }
}

function installEnemyRoomModels(patch: Patchfile, plan: EnemyPlan, foreign: ReadonlyMap<string, ForeignEnemyPlacement>,
  supplemental: readonly EnemySupplementalRoom[]): void {
  const staged: { game: Game; symbol: string; vrom: number; data: Uint8Array }[] = [];
  for (const game of ['oot', 'mm'] as const) {
    const rows = enemyRoomModelRosters(game, plan, foreign, supplemental);
    if (!rows.length) continue;
    const symbol = 'ENEMY_ROOM_MODELS_' + game.toUpperCase();
    const refs = patch.symbols[game].get(symbol);
    const payloads = patch.newFiles.filter(file => file.name === game + '/payload');
    if (refs?.length !== 1 || payloads.length !== 1 || patch.symPatches.has(symbol))
      throw Error('Missing enemy room model capability');
    const payload = payloads[0], span = payload.vram?.[game];
    const validSymbol = (address: number) => Number.isInteger(address) && address % 4 === 0 &&
      span && span[1] - span[0] === payload.data.length && address >= span[0] && address + 4 <= span[1];
    if (!validSymbol(refs[0])) throw Error('Invalid enemy room model capability');
    if (game === 'mm') {
      const schedule = patch.symbols.mm.get('ENEMY_ROOM_MODEL_SCHEDULE_MM_V2');
      if (schedule?.length !== 1 || !validSymbol(schedule[0]))
        throw Error('Missing scheduled MM model residency support');
    }
    if (rows.some(row => row.modelIds.some(id => id < 0x1000))) {
      const native = patch.symbols[game].get('ENEMY_NATIVE_ROOM_MODELS_' + game.toUpperCase());
      if (native?.length !== 1 || !validSymbol(native[0]))
        throw Error('Missing on-demand native room model capability');
    }
    if (game === 'mm') verifyMmEnemyRoomModelSlots(rows);
    const data = encodeEnemyRoomModels(game, rows);
    const vrom = game === 'oot' ? 0xd9b00000 : 0xd9b10000;
    if (patch.newFiles.some(file => vrom < file.vrom + file.data.length && file.vrom < vrom + data.length))
      throw Error('Enemy room model metadata VROM collision');
    staged.push({ game, symbol, vrom, data });
  }
  for (const row of staged) {
    patch.addNewFile({ name: row.game + '/enemy-room-models', vrom: row.vrom, data: row.data, compressed: false });
    patch.addSymbolPatch(row.symbol, toU32Buffer([row.vrom]));
  }
}

function installMmObjectBufferBudgets(roms: DecompressedRoms, addresses: GameAddresses, patch: Patchfile,
  plan: EnemyPlan, supplemental: readonly EnemySupplementalRoom[]): void {
  const candidates = new Set<number>(supplemental.filter(row => row.game === 'mm').map(row => row.scene));
  plan.forEach(row => {
    if (row.placement) candidates.add(row.placement.scene);
  });
  if (!candidates.size) return;
  const symbol = 'MM_OBJECT_BUFFER_BUDGET_V1';
  if (patch.symbols.mm.get(symbol)?.length !== 1) throw Error('Missing MM object-buffer sizing support');
  const rom = roms.mm.rom;
  const relocations: readonly RoomRelocation[] = patch.meta.enemyRoomRelocations ?? [];
  const read = (at: number, size: number): Uint8Array => {
    const relocation = relocations.find(row => row.game === 'mm' && at >= row.oldStart && at + size <= row.oldEnd);
    const address = relocation ? { name: relocation.name, offset: at - relocation.oldStart } : addresses.mm.fileFromROM(at);
    const replacement = patch.newFiles.find(file => file.name === address.name);
    if (patch.removedFiles.includes(address.name) && !replacement) throw Error('Removed object-budget source');
    const bytes = replacement ? replacement.data.slice(address.offset, address.offset + size) : rom.slice(at, at + size);
    if (bytes.length !== size) throw Error('Truncated object-budget source');
    applyOverlappingEdits(bytes, address.offset, patch.patches[address.name] ?? [], size);
    return bytes;
  };
  const objectSize = (id: number) => {
    if (!Number.isInteger(id) || id <= 0 || id >= CONFIG.mm.objectCount) throw Error('Unknown native budget object');
    const entry = read(CONFIG.mm.objectTableAddr + id * 8, 8);
    const size = bufReadU32BE(entry, 4) - bufReadU32BE(entry, 0);
    if (size <= 0 || size > 0x200000) throw Error('Invalid native budget object size');
    return align(size, 16);
  };
  const objectList = (start: number, command: Uint8Array, segment: number) => {
    const count = command[1], ptr = bufReadU32BE(command, 4);
    if (!count) return [];
    if (count > 35) throw Error('Invalid object-budget list');
    let bytes: Uint8Array;
    if ((ptr >>> 24) === segment) bytes = read(start + (ptr & 0xffffff), count * 2);
    else {
      const address = ((ptr & 0x1fffffff) + 0x80000000) >>> 0;
      const file = patch.newFiles.find(file => file.vram?.mm && address >= file.vram.mm[0] &&
        address + count * 2 <= file.vram.mm[1]);
      if (!file) throw Error('Unverified object-budget list pointer');
      const offset = address - file.vram!.mm![0];
      bytes = file.data.slice(offset, offset + count * 2);
      applyOverlappingEdits(bytes, offset, patch.patches[file.name!] ?? [], bytes.length);
    }
    return Array.from({ length: count }, (_, i) => bufReadU16BE(bytes, i * 2));
  };
  const headers = (start: number, end: number, segment: number, extraCount: number) => {
    const base = readHeader(rom, start, start, end, segment);
    const offsets = [{ offset: start, setup: 0 }];
    if (base.alternates !== undefined) {
      if (!extraCount) throw Error('Unknown alternate-header count');
      const before = rom.subarray(base.alternates, base.alternates + extraCount * 4);
      const after = read(base.alternates, before.length);
      if (before.some((b, i) => b !== after[i])) throw Error('Changed alternate-header table');
      for (let i = 0; i < extraCount; ++i) {
        if (bufReadU32BE(before, i * 4)) offsets.push({ offset: pointer(rom, base.alternates + i * 4, start, segment, end), setup: i + 1 });
      }
    }
    return offsets.map(({ offset, setup }) => {
      let subKeep = 0, spawnLists = 0;
      const objects: number[] = [], roomStarts: number[] = [];
      for (let i = 0; i < 256; ++i, offset += 8) {
        range(rom, offset, 8, start, end);
        const original = rom.subarray(offset, offset + 8), cmd = read(offset, 8), code = cmd[0];
        if (code !== original[0]) throw Error('Changed object-budget header layout');
        if ([0, 4, 7, 0x18].includes(code) && original.some((b, j) => b !== cmd[j]))
          throw Error('Changed object-budget persistent layout');
        if (code === 0x14) return { subKeep, objects, roomStarts, setup };
        if (segment === 3 && (code === 0 || code === 7)) throw Error('Room changes persistent object layout');
        if (code === 0 && ++spawnLists > 1) throw Error('Multiple player-object reservations');
        if (code === 7) {
          if (subKeep) throw Error('Multiple sub-keep reservations');
          subKeep = bufReadU32BE(cmd, 4);
        }
        if (code === 0x0b) {
          if (segment === 2 && cmd[1]) throw Error('Scene object list may become persistent');
          objects.push(...objectList(start, cmd, segment));
        }
        if (code === 4) {
          const at = pointer(rom, offset + 4, start, segment, end);
          for (let j = 0; j < cmd[1]; ++j) {
            const oldStart = bufReadU32BE(rom, at + j * 8), oldEnd = bufReadU32BE(rom, at + j * 8 + 4);
            roomStarts.push(oldStart);
            const row = relocations.find(row => row.game === 'mm' && row.oldStart === oldStart && row.oldEnd === oldEnd);
            const current = read(at + j * 8, 8);
            if (bufReadU32BE(current, 0) !== (row?.newStart ?? oldStart) ||
              bufReadU32BE(current, 4) !== (row?.newEnd ?? oldEnd)) throw Error('Changed scene room list');
          }
        }
      }
      throw Error('Unterminated object-budget header');
    });
  };
  const rooms = collectEnemyRooms(rom, 'mm');
  const data = new Uint8Array(16 + 113 * 16);
  data.set(toU32Buffer([0x4f424d31, 1, 113, 16]));
  const verified: { scene: number; baseBytes: number; playerSlots: number; horseSlots: number }[] = [];
  const skipped: { scene: number; reason: string }[] = [];
  for (const scene of [...candidates].sort((a, b) => a - b)) {
    try {
      if (scene < 0 || scene >= 113) throw Error('Unknown scene');
      const at = SCENE_TABLES.mm.address + scene * SCENE_TABLES.mm.stride;
      const start = bufReadU32BE(rom, at), end = bufReadU32BE(rom, at + 4);
      const extraCount = scene === 0x2d ? 9 : MM_EXTRA_SETUPS[start] || 0;
      const sceneHeaders = headers(start, end, 2, extraCount);
      const sceneRooms = rooms.filter(room => room.headers.some(header => header.scene === scene));
      if (!sceneRooms.length) throw Error('No verified rooms');
      if (sceneHeaders.some(header => header.roomStarts.some(start => !sceneRooms.some(room => room.start === start))))
        throw Error('Uncatalogued cutscene room');
      const all = [...sceneHeaders, ...sceneRooms.flatMap(room => headers(room.start, room.end, 3, extraCount))]
        .filter(header => header.setup === 0);
      let roomBytes = 0, playerSlots = 1, horseSlots = 1;
      for (const header of all) {
        let bytes = 0, players = 1, horses = 1;
        for (const id of header.objects) {
          if ([0x10, 0x11, 0x14c, 0x14d, 0x154].includes(id)) ++players;
          else if (id === 0x7d) ++horses;
          else bytes += objectSize(id);
        }
        roomBytes = Math.max(roomBytes, bytes);
        playerSlots = Math.max(playerSlots, players);
        horseSlots = Math.max(horseSlots, horses);
      }
      const baseBytes = objectSize(1) + roomBytes +
        Math.max(0, ...all.map(header => header.subKeep ? objectSize(header.subKeep) : 0));
      if (baseBytes > 0x200000 || playerSlots > 35 || horseSlots > 35) throw Error('Object budget outside native range');
      [baseBytes, playerSlots, horseSlots, 1].forEach((word, i) => bufWriteU32BE(data, 16 + scene * 16 + i * 4, word));
      verified.push({ scene, baseBytes, playerSlots, horseSlots });
    } catch (error) {
      skipped.push({ scene, reason: error instanceof Error ? error.message : String(error) });
    }
  }
  const vrom = 0xd9b20000;
  if (patch.newFiles.some(file => vrom < file.vrom + file.data.length && file.vrom < vrom + data.length))
    throw Error('MM object budget VROM collision');
  patch.addNewFile({ name: 'mm/object-buffer-budgets', vrom, data, compressed: false });
  patch.addSymbolPatch(symbol, toU32Buffer([vrom]));
  patch.meta.enemyObjectBufferBudgets = { setup: 0, alternateSetups: 'vanilla', verified, skipped };
}

function foreignKeepFootprint(source: Game) {
  return keepFacts.games[source].entries.reduce((bytes, entry) =>
    bytes + align(entry.size, 16) + 48 + entry.fixups.length * 12, 16);
}

const FOREIGN_KEEP_BYTES = { oot: foreignKeepFootprint('mm'), mm: foreignKeepFootprint('oot') };

function fitsEnemyObjectBudget(game: Game, objects: ReadonlyMap<number, number>): boolean {
  const nativeSlots = game === 'oot' ? 19 : 35;
  const heapSize = game === 'oot' ? 0x100000 : 0x40000;
  let bytes = 0;
  let slots = 0;
  for (const [id, size] of objects) {
    if (!Number.isInteger(id) || !(id > 0x1000 && id < 0x2000 || game === 'oot' && id >= 0x2000 && id < 0x2080) ||
      !Number.isSafeInteger(size) || size <= 0 || size % 16)
      throw new Error('Invalid extended enemy object footprint');
    if (id >= 0x1001 && id <= 0x1003) {
      if (size !== 16) throw new Error('Whole foreign keep residency is forbidden');
    } else {
      ++slots;
      bytes += size + 16;
    }
  }
  const keepBytes = objects.size ? FOREIGN_KEEP_BYTES[game] : 0;
  return slots <= 128 - nativeSlots && bytes + keepBytes <= heapSize;
}

const SCENE_TABLES = {
  oot: { address: 0xb71440, count: 101, stride: 0x14 },
  mm: { address: 0xc5a1e0, count: 113, stride: 0x10 },
};

const MM_EXTRA_SETUPS: Readonly<Record<number, number>> = {
  0x2036000: 3, 0x2249000: 1, 0x263b000: 1, 0x265e000: 1,
  0x26bf000: 1, 0x26fc000: 1, 0x2778000: 1, 0x2879000: 2,
  0x2bfe000: 1, 0x2c09000: 1, 0x2da7000: 1, 0x2e1d000: 1,
};

const MM_EXCLUDED_SCENES = new Set([0x08, 0x20, 0x24, 0x43, 0x69]);

type List = { offset: number; count: number };

type Header = { actors?: List; objects?: List; objectCommand?: number; rooms?: List; alternates?: number };

type Room = {
  start: number; end: number; headers: (Header & EnemyRoomLocation)[]; mq?: boolean;
  scriptedPeers?: readonly ReadonlySet<number>[];
  sceneActors?: Set<number>;
  excluded?: boolean;
  residentObjects?: ReadonlySet<number>;
  protectedEntries?: ReadonlySet<number>;
};

type SceneObjects = Map<number, ReadonlyMap<number, number>>;

function range(rom: Uint8Array, offset: number, length: number, start = 0, end = rom.length) {
  if (!Number.isSafeInteger(offset) || offset < start || length < 0 || offset + length > end || end > rom.length)
    throw new Error(`Invalid enemy scene range: 0x${offset.toString(16)} + 0x${length.toString(16)}`);
}

function pointer(rom: Uint8Array, field: number, base: number, segment: number, end: number) {
  const value = bufReadU32BE(rom, field);
  if ((value >>> 24) !== segment) throw new Error(`Invalid room pointer segment at 0x${field.toString(16)}`);
  const result = base + (value & 0xffffff);
  range(rom, result, 0, base, end);
  return result;
}

function readHeader(rom: Uint8Array, offset: number, base: number, end: number, segment: number): Header {
  const header: Header = {};
  for (let i = 0; i < 256; ++i, offset += 8) {
    range(rom, offset, 8, base, end);
    const code = rom[offset];
    if (code === 0x14) return header;
    if (code !== 0x01 && code !== 0x04 && code !== 0x0b && code !== 0x18) continue;
    if (code === 0x18 && bufReadU32BE(rom, offset + 4) === 0) continue;
    const address = pointer(rom, offset + 4, base, segment, end);
    const count = rom[offset + 1];
    if (code === 0x18) {
      header.alternates = address;
      continue;
    }
    range(rom, address, count * (code === 0x01 ? 16 : code === 0x04 ? 8 : 2), base, end);
    const list = { offset: address, count };
    if (code === 0x01) header.actors = list;
    if (code === 0x04) header.rooms = list;
    if (code === 0x0b) {
      header.objects = list;
      header.objectCommand = offset;
    }
  }
  throw new Error('Unterminated scene header while randomizing enemies');
}

function readHeaders(rom: Uint8Array, start: number, end: number, segment: number, extraCount: number) {
  const base = readHeader(rom, start, start, end, segment);
  const headers = [{ ...base, setup: 0 }];
  if (base.alternates !== undefined) {
    range(rom, base.alternates, extraCount * 4, start, end);
    for (let i = 0; i < extraCount; ++i) {
      const field = base.alternates + i * 4;
      if (!bufReadU32BE(rom, field)) continue;
      const offset = pointer(rom, field, start, segment, end);
      headers.push({ ...readHeader(rom, offset, start, end, segment), setup: i + 1 });
    }
  }
  return headers;
}

function profileObject(rom: Uint8Array, game: Game, actorId: number): number | undefined {
  const config = CONFIG[game];
  if (actorId >= config.actorsOvlCount) return undefined;
  const entry = config.actorsOvlAddr + actorId * 0x20;
  const vrom = bufReadU32BE(rom, entry);
  const vram = bufReadU32BE(rom, entry + 8);
  const profile = bufReadU32BE(rom, entry + 0x14);
  if (!vrom || !vram || profile < vram) return undefined;
  const address = vrom + profile - vram;
  range(rom, address, 0x20, vrom, bufReadU32BE(rom, entry + 4));
  return bufReadU16BE(rom, address + 8);
}

function planRoomEnemies(rom: Uint8Array, game: Game, room: Room, plan: EnemyPlan, metadataRom: Uint8Array, foreignTargets: ReadonlyMap<string, ForeignEnemyPlacement>, sceneObjects: SceneObjects): Change[] {
  const pairs = plan.filter(p => !p.kind).map(p => ({ source: enemyDefinition(p.source), target: enemyDefinition(p.target) }))
    .filter(p => p.source.game === game && p.source.sourceAllowed !== false && p.target.targetAllowed !== false && p.source.key !== p.target.key &&
      enemyEncounterCompatible(p.source.game, p.source.actorId, p.target.game, p.target.actorId));
  const foreign = (target: EnemyDefinition) => target.game === game ? undefined : foreignTargets.get(target.key);
  for (const { source, target } of pairs) {
    if (target.game === game) continue;
    if (!standaloneEnemyTransfer(source, target))
      throw new Error('Cross-game replacement is outside the standalone enemy shuffle pools');
    assertForeignEnemyExclusive(target.game, target.actorId, game, target.spawnParams);
    if (target.nativeOnly) throw new Error('Enemy variant is verified for native placements only');
    const imported = foreign(target);
    if (!imported || imported.sourceGame !== target.game || imported.targetGame !== game || imported.sourceActorId !== target.actorId ||
      imported.sourceObjectId !== target.objectId || imported.actorId !== 0xc00 + target.actorId || imported.objectId !== (0x1000 | target.objectId) ||
      imported.objects.pinned.get(imported.objectId) !== imported.objectSize || !Number.isInteger(imported.objectSize) || imported.objectSize <= 0 || imported.objectSize % 16)
      throw new Error('Foreign overlays require a matching ABI adapter before scene patching');
  }
  const mask = game === 'mm' ? 0x1fff : 0xffff;
  const actorOffsets = new Set<number>();
  const setups = room.headers.map(header => {
    const actors = new Set<number>(), entries: number[] = [], objects: number[] = [];
    if (header.actors) for (let i = 0; i < header.actors.count; ++i) {
      const offset = header.actors.offset + i * 16;
      actorOffsets.add(offset);
      entries.push(offset);
      actors.add(bufReadU16BE(rom, offset) & mask);
    }
    if (header.objects) for (let i = 0; i < header.objects.count; ++i) {
      const offset = header.objects.offset + i * 2;
      objects.push(offset);
    }
    return { header, actors, entries, objects };
  });
  const accepted = new Map<number, { source: EnemyDefinition; target: EnemyDefinition; entries: Set<number> }>();
  if (room.excluded) return [];
  for (const pair of pairs) {
    const { source, target } = pair;
    if (source.habitat !== target.habitat) continue;
    if (source.preserveNativeResource && (target.game !== game || !nativeResourceTarget(game, target.actorId, target.spawnParams))) continue;
    if (enemyCanRespawn(target.game, target.actorId, target.spawnParams)) continue;
    const grouped = [source, target].filter(enemy => enemy.game === game && enemy.requiresNoScriptedPeers);
    if (grouped.length && (!room.scriptedPeers?.length || grouped.some(enemy =>
      room.scriptedPeers!.some(scene => scene.has(enemy.actorId))))) continue;
    const matching = [...actorOffsets].filter(a => (bufReadU16BE(rom, a) & mask) === source.actorId);
    const rotationMask = game === 'mm' ? 0xff80 : 0xffff;
    if (!matching.length) continue;
    const entries = new Set(matching.filter(a => {
      if (room.protectedEntries?.has(a)) return false;
      if ((bufReadU16BE(rom, a + 8) & rotationMask) || (bufReadU16BE(rom, a + 12) & rotationMask) ||
        !source.params.includes(bufReadU16BE(rom, a + 14))) return false;
      if (game !== 'mm') return true;
      if ((source.requiresNoCutscene || target.requiresNoCutscene || foreign(target)) && (bufReadU16BE(rom, a + 10) & 0x7f) !== 0x7f) return false;
      if (!foreign(target) && !source.preserveNativeResource) return true;
      const days = ((bufReadU16BE(rom, a + 8) & 7) << 7) | (bufReadU16BE(rom, a + 12) & 0x7f);
      return days === 0 || days === 0x3ff;
    }));
    if (game === 'oot' && !source.sourceFormsIndependent && entries.size !== matching.length) continue;
    if (!entries.size) continue;
    if (profileObject(metadataRom, game, source.actorId) !== source.objectId || (!foreign(target) && profileObject(metadataRom, game, target.actorId) !== target.objectId))
      throw new Error(`Enemy profile/object mismatch: ${source.key} -> ${target.key}`);
    if (game === 'oot' && source.objectId !== 1 && !room.residentObjects?.has(source.objectId) && setups.some(setup => setup.actors.has(source.actorId) &&
      !setup.objects.some(off => bufReadU16BE(rom, off) === source.objectId))) continue;
    for (const setup of setups) {
      const ownsModel = source.objectId === 1 || !!room.residentObjects?.has(source.objectId) || setup.objects.some(off => bufReadU16BE(rom, off) === source.objectId);
      for (const [slot, offset] of setup.entries.entries()) {
        if (entries.has(offset) && (!ownsModel || !enemySlotAllows(game, !!room.mq, setup.header, slot,
          source.actorId, target, bufReadU16BE(rom, offset + 14), bufReadU16BE(rom, offset + 10) & 0x7f)))
          entries.delete(offset);
      }
    }
    if (game === 'oot') for (const offset of entries) {
      const aliases = setups.filter(setup => setup.entries.includes(offset)).map(setup => ({
        mq: !!room.mq,
        loc: setup.header, slot: setup.entries.indexOf(offset), source: source.actorId
      }));
      if (!ootPlacementAdaptersAgree(aliases)) entries.delete(offset);
    }
    if (entries.size) accepted.set(source.actorId, { ...pair, entries });
  }
  if (!accepted.size) return [];
  const dependencies = new Map<number, ReadonlySet<number> | undefined>();
  const requiredObjects = (actorId: number) => {
    if (!dependencies.has(actorId)) dependencies.set(actorId,
      nativeEnemyObjectDependencies(game, actorId, id => profileObject(metadataRom, game, id)));
    return dependencies.get(actorId);
  };
  while (accepted.size) {
    const additionsByScene = new Map<number, Map<number, number>>();
    let reject: number | undefined;
    for (const setup of setups) {
      const counts = new Map<number, number>(), additions = new Map<number, number>();
      for (const offset of setup.entries) {
        const original = bufReadU16BE(rom, offset) & mask, pair = accepted.get(original);
        const target = pair?.entries.has(offset) ? pair.target : undefined;
        if ((target?.game ?? game) !== 'mm') continue;
        const id = target?.actorId ?? original;
        if (mmRoomCaps[id] === undefined) continue;
        counts.set(id, (counts.get(id) ?? 0) + 1);
        if (pair && target && (target.game !== game || id !== original))
          additions.set(id, pair.source.objectId);
      }
      for (const [id, incoming] of additions) {
        if (counts.get(id)! > mmRoomCaps[id]) {
          reject = incoming;
          break;
        }
      }
      if (reject !== undefined) break;
      const available = new Set<number>(room.residentObjects ?? [1]);
      for (const offset of setup.objects) available.add(bufReadU16BE(rom, offset));
      for (const offset of setup.entries) {
        const actor = bufReadU16BE(rom, offset) & mask, pair = accepted.get(actor);
        if (pair?.entries.has(offset)) {
          const imported = foreign(pair.target);
          if (imported) {
            const scene = setup.header.scene;
            if (!Number.isInteger(scene) || scene < 0 || scene >= SCENE_TABLES[game].count)
              throw new Error('Foreign enemy placement requires a receiving scene');
            let held = additionsByScene.get(scene);
            if (!held) {
              held = new Map(sceneObjects.get(scene));
              additionsByScene.set(scene, held);
            }
            for (const [id, size] of imported.objects.pinned) {
              if (held.has(id) && held.get(id) !== size) throw new Error('Conflicting enemy object sizes');
              held.set(id, size);
            }
            const missingChild = [...imported.objects.nativeChildren].some(id => {
              const needed = requiredObjects(id);
              return !needed || [...needed].some(object => !available.has(object));
            });
            if (missingChild || !fitsEnemyObjectBudget(game, held)) {
              reject = pair.source.objectId;
              break;
            }
            continue;
          }
          const onDemand = !room.mq && nativeEnemyOnDemandObjects(game, pair.target.actorId);
          const needed = nativeOotPoeTargetObjects(pair.target) ?? requiredObjects(pair.target.actorId);
          if (!needed || [...needed].some(id => !available.has(id) && !(onDemand && onDemand.includes(id))))
            reject = pair.source.objectId;
        }
        if (reject !== undefined) break;
      }
      if (reject !== undefined) break;
    }
    if (reject === undefined) {
      for (const [scene, held] of additionsByScene) sceneObjects.set(scene, held);
      break;
    }
    for (const [actor, pair] of accepted) {
      if (pair.source.objectId === reject) accepted.delete(actor);
    }
  }
  const changes: Change[] = [];
  for (const a of actorOffsets) {
    const raw = bufReadU16BE(rom, a);
    const pair = [...accepted.values()].find(p => p.entries.has(a));
    if (!pair) continue;
    changes.push({ offset: a, value: (raw & ~mask) | (foreign(pair.target)?.actorId ?? pair.target.actorId) });
    changes.push({ offset: a + 14, value: pair.target.spawnParams });
    if (game === 'oot') {
      const owner = setups.find(setup => setup.entries.includes(a))!;
      const position = ootPlacementPositionChanges(rom, a, !!room.mq, owner.header, owner.entries.indexOf(a), pair.source.actorId);
      if (position) changes.push(...position.changes);
    }
  }
  return changes;
}

function patchMqEnemies(data: Uint8Array, metadataRom: Uint8Array, plan: EnemyPlan, foreignTargets: ReadonlyMap<string, ForeignEnemyPlacement>, sceneObjects: SceneObjects, nativeRecords: NativePlacementRecord[]) {
  const copy = new Uint8Array(data);
  range(copy, 0, 0x20);
  const count = bufReadU32BE(copy, 0);
  range(copy, 0x20, count * 0x20);
  let replaced = 0;
  for (let i = 0; i < count; ++i) {
    const header = 0x20 + i * 0x20;
    const size = bufReadU32BE(copy, header);
    const start = bufReadU32BE(copy, header + 4);
    const end = start + size;
    range(copy, start, size, 0x20 + count * 0x20);
    const actors = { offset: start + bufReadU16BE(copy, header + 0x0c), count: bufReadU16BE(copy, header + 0x0a) };
    const objects = { offset: start + bufReadU16BE(copy, header + 0x10), count: bufReadU16BE(copy, header + 0x0e) };
    range(copy, actors.offset, actors.count * 16, start, end);
    range(copy, objects.offset, objects.count * 2, start, end);
    const dungeon = copy[header + 8];
    if (dungeon > 11) throw new Error('Unknown MQ enemy dungeon');
    const scene = dungeon === 11 ? 0x0d : dungeon === 10 ? 0x0b : dungeon;
    const owner = { actors, objects, scene, room: copy[header + 9], setup: 0 };
    const changes = planRoomEnemies(copy, 'oot', { start, end, mq: true, headers: [owner] }, plan, metadataRom, foreignTargets, sceneObjects);
    for (let slot = 0; slot < actors.count; ++slot) {
      const offset = actors.offset + slot * 16, actorChange = changes.find(change => change.offset === offset);
      if (!actorChange) continue;
      const source = bufReadU16BE(data, offset), target = actorChange.value;
      const params = changes.find(change => change.offset === offset + 14)!.value;
      let operation = nativeResourceSource('oot', source, bufReadU16BE(data, offset + 14)) ? 1 :
        target === 0x1b0 && [0, 5, 10].includes(params) ? 3 : target === 0x1c0 && [0, 1].includes(params) ? 7 : target === 0xc7 && params === 1 ? 6 : target === 0x1d && [0, 0xffff].includes(params) ? 4 : target === 0x115 ? 16 : target === 0x113 && [0xff82, 0xff83].includes(params) ? 64 : 0;
      const aliases: OotPlacementAlias[] = [{ mq: true, loc: owner, slot, source }];
      if (ootPlacementAdaptersAgree(aliases) && ootPlacementAdapter(true, owner, slot, source)?.floorOperation) operation |= 32;
      if (!operation) continue;
      const entryChanges = changes.filter(change => change.offset >= offset && change.offset < offset + 16)
        .map(change => ({ ...change, offset: change.offset - offset }));
      const record = nativePlacementRecord(data.subarray(offset, offset + 16), 0, operation, entryChanges, target, params);
      record.vrom = mqActorVrom(scene, owner.room, slot);
      record.mq = { scene, room: owner.room, slot, offset, source: data };
      record.placementAliases = aliases;
      nativeRecords.push(record);
    }
    for (const c of changes) {
      bufWriteU16BE(copy, c.offset, c.value);
      if (c.offset >= actors.offset && c.offset < actors.offset + actors.count * 16 && (c.offset - actors.offset) % 16 === 0) ++replaced;
    }
  }
  return { data: copy, replaced, nativePlacements: nativeRecords };
}

function collectEnemyRooms(rom: Uint8Array, game: Game): Room[] {
  const table = SCENE_TABLES[game], rooms = new Map<number, Room>();
  for (let scene = 0; scene < table.count; ++scene) {
    const excluded = game === 'mm' ? MM_EXCLUDED_SCENES.has(scene) : scene >= 0x11 && scene <= 0x1a;
    const entry = table.address + scene * table.stride;
    const sceneStart = bufReadU32BE(rom, entry), sceneEnd = bufReadU32BE(rom, entry + 4);
    if (!sceneStart) continue;
    const extraCount = game === 'oot' ? 3 : MM_EXTRA_SETUPS[sceneStart] || 0;
    const sceneRooms = new Set<Room>(), scripted = new Set<number>(), sceneActors = new Set<number>();
    const addScripted = (header: Header) => {
      if (game !== 'mm' || !header.actors) return;
      for (let i = 0; i < header.actors.count; i++) {
        const actor = header.actors.offset + i * 16;
        if ((bufReadU16BE(rom, actor + 10) & 0x7f) !== 0x7f) scripted.add(bufReadU16BE(rom, actor) & 0x1fff);
      }
    };
    for (const header of readHeaders(rom, sceneStart, sceneEnd, 2, extraCount)) {
      addScripted(header);
      if (header.actors) for (let i = 0; i < header.actors.count; ++i)
        sceneActors.add(bufReadU16BE(rom, header.actors.offset + i * 16) & (game === 'mm' ? 0x1fff : 0xffff));
      if (!header.rooms) continue;
      for (let index = 0; index < header.rooms.count; index++) {
        const field = header.rooms.offset + index * 8, start = bufReadU32BE(rom, field), end = bufReadU32BE(rom, field + 4);
        if (!start) continue;
        let room = rooms.get(start);
        if (!room) {
          room = { start, end, headers: [], scriptedPeers: [] };
          rooms.set(start, room);
        }
        if (room.end !== end) throw new Error('Shared enemy room has conflicting file bounds');
        const setups = readHeaders(rom, start, end, 3, extraCount);
        for (const setup of setups) {
          if (!room.headers.some(h => h.scene === scene && h.room === index && h.setup === setup.setup))
            room.headers.push({ ...setup, scene, room: index });
          addScripted(setup);
        }
        room.excluded ||= excluded;
        sceneRooms.add(room);
      }
    }
    for (const room of sceneRooms) {
      room.scriptedPeers = [...room.scriptedPeers!, scripted];
      if (sceneActors.size) {
        room.sceneActors ??= new Set();
        for (const actor of sceneActors) room.sceneActors.add(actor);
      }
    }
  }
  return [...rooms.values()];
}

function nativeEntryProtected(game: Game, offset: number, addresses: GameAddresses, patch: Patchfile): boolean {
  try {
    const file = addresses[game].fileFromROM(offset);
    return patch.removedFiles.includes(file.name) || patch.newFiles.some(row => row.name === file.name) ||
      !!patch.patches[file.name]?.some(row => row.addr < file.offset + 16 && row.addr + row.data.length > file.offset);
  } catch {
    return true;
  }
}

function nativeActorPreservation(plan: EnemyPlan, roms: DecompressedRoms, addresses: GameAddresses, patch: Patchfile,
  supplementalEdits: ReadonlySet<string>) {
  if (plan.some(row => !row.ootr && !row.placement && !row.kind))
    throw Error('Unclassified enemy preservation plan');
  const allowed = new Set<string>();
  const allowedMq = new Set<string>();
  for (const row of plan) {
    if (row.placement) allowed.add('mm/' + row.placement.actorOffset);
    if (row.ootr) {
      const { scene, room, setup, slot, mq } = row.ootr.location;
      if (mq) allowedMq.add([scene, room, slot].join('/'));
      else allowed.add(['oot', scene, room, setup, slot].join('/'));
      for (const alias of row.ootr.aliases) allowed.add('oot/' + alias.join('/'));
    }
  }
  const relocations: readonly RoomRelocation[] = patch.meta.enemyRoomRelocations ?? [];
  const read = (game: Game, at: number, size = 16): Uint8Array => {
    const relocation = relocations.find(row =>
      row.game === game && at >= row.oldStart && at + size <= row.oldEnd);
    if (relocation) {
      const file = patch.newFiles.find(file => file.name === relocation.name);
      if (!file) throw Error('Missing relocated native actor file');
      const offset = at - relocation.oldStart;
      return file.data.slice(offset, offset + size);
    }
    let address;
    try {
      address = addresses[game].fileFromROM(at);
    } catch (error) {
      throw Error(`Unmapped authored actor/model read ${game}/${at.toString(16)} size ${size}`, { cause: error });
    }
    const file = patch.newFiles.find(file => file.name === address.name);
    if (file) return file.data.slice(address.offset, address.offset + size);
    const bytes = Uint8Array.from(roms[game].rom.subarray(at, at + size));
    applyOverlappingEdits(bytes, address.offset, patch.patches[address.name] ?? [], size);
    return bytes;
  };
  const readProfileObject = (game: Game, actorId: number) => {
    const rom = roms[game].rom;
    const config = CONFIG[game];
    if (actorId >= config.actorsOvlCount) return undefined;
    const table = config.actorsOvlAddr + actorId * 32;
    const start = bufReadU32BE(rom, table);
    const vram = bufReadU32BE(rom, table + 8);
    const profile = bufReadU32BE(rom, table + 20);
    const address = start + profile - vram;
    if (!start || !vram || profile < vram || address + 10 > rom.length) return undefined;
    return bufReadU16BE(rom, address + 8);
  };
  const entries = new Map<string, { game: Game; at: number; bytes: Uint8Array }>();
  const dependencies = new Map<string, {
    game: Game; start: number; command: number; objectId: number; actorId: number; actorOffsets: number[];
  }>();
  const objectIds = (game: Game, start: number, command: number): number[] => {
    const data = read(game, command, 8);
    const count = data[1];
    if (!count) return [];
    const pointer = bufReadU32BE(data, 4);
    let list: Uint8Array;
    if ((pointer >>> 24) !== 3) {
      const address = (pointer & 0x1fffffff) + 0x80000000;
      const file = patch.newFiles.find(file => {
        const span = file.vram?.[game];
        return span && address >= span[0] && address + count * 2 <= span[1];
      });
      if (!file) throw Error('Unverified native object-list pointer');
      const offset = address - file.vram![game]![0];
      list = Uint8Array.from(file.data.subarray(offset, offset + count * 2));
      applyOverlappingEdits(list, offset, patch.patches[file.name!] ?? []);
    } else {
      const offset = pointer & 0xffffff;
      const relocation = relocations.find(row => row.game === game && row.oldStart === start);
      const file = relocation ? patch.newFiles.find(file => file.name === relocation.name) : undefined;
      list = file ? file.data.subarray(offset, offset + count * 2) : read(game, start + offset, count * 2);
      if (list.length !== count * 2) throw Error('Truncated native room object list');
    }
    return Array.from({ length: count }, (_, index) => bufReadU16BE(list, index * 2));
  };
  for (const game of ['oot', 'mm'] as const) {
    if (!plan.some(row => enemyDefinition(row.source).game === game)) continue;
    const rooms = collectEnemyRooms(roms[game].rom, game);
    const headers = rooms.flatMap(room => room.headers.map(header => ({ room, header })));
    for (const { header } of headers) {
      if (!header.actors) continue;
      const prefix = [game, header.scene, header.room, header.setup].join('/') + '/';
      for (let slot = 0; slot < header.actors.count; ++slot) {
        if (allowed.has(prefix + slot))
          allowed.add(game + '/' + (header.actors.offset + slot * 16));
      }
    }
    for (const { room, header } of headers) {
      if (!header.actors) continue;
      const command = header.objectCommand;
      const objects = command === undefined ? [] : objectIds(game, room.start, command);
      for (let slot = 0; slot < header.actors.count; ++slot) {
        const at = header.actors.offset + slot * 16;
        const key = `${game}/${at}`;
        if (allowed.has(key)) continue;
        const bytes = read(game, at);
        entries.set(key, { game, at, bytes });
        const actorId = bufReadU16BE(bytes, 0) & (game === 'mm' ? 0x1fff : 0xffff);
        const objectId = readProfileObject(game, actorId);
        if (objectId === undefined) continue;
        if (command !== undefined && objects.includes(objectId)) {
          const dependencyKey = `${game}/${room.start}/${command}/${objectId}`;
          const previous = dependencies.get(dependencyKey);
          const actorOffsets = previous?.actorOffsets ?? [];
          actorOffsets.push(at);
          dependencies.set(dependencyKey, {
            game, start: room.start, command, objectId, actorId, actorOffsets,
          });
        }
      }
    }
  }
  const mqHasObject = (data: Uint8Array, header: number, objectId: number) => {
    const list = bufReadU32BE(data, header + 4) + bufReadU16BE(data, header + 16);
    const count = bufReadU16BE(data, header + 14);
    return Array.from({ length: count }, (_, i) => bufReadU16BE(data, list + i * 2)).includes(objectId);
  };
  const mq = patch.newFiles.find(file => file.name === 'custom/mq_rooms');
  const mqEntries: { at: number; bytes: Uint8Array }[] = [];
  const mqDependencies: { header: number; objectId: number }[] = [];
  if (mq) {
    const data = mq.data;
    for (let i = 0; i < bufReadU32BE(data, 0); ++i) {
      const header = 32 + i * 32;
      const dungeon = data[header + 8];
      const scene = mqDungeonScene(dungeon);
      const room = data[header + 9];
      const start = bufReadU32BE(data, header + 4) + bufReadU16BE(data, header + 12);
      for (let slot = 0; slot < bufReadU16BE(data, header + 10); ++slot) {
        if (allowedMq.has([scene, room, slot].join('/'))) continue;
        const at = start + slot * 16;
        const bytes = data.slice(at, at + 16);
        mqEntries.push({ at, bytes });
        const objectId = readProfileObject('oot', bufReadU16BE(bytes, 0));
        if (objectId !== undefined && mqHasObject(data, header, objectId))
          mqDependencies.push({ header, objectId });
      }
    }
  }
  return () => {
    for (const [key, row] of entries) {
      if (supplementalEdits.has(key)) continue;
      const current = read(row.game, row.at);
      if (current.length !== 16 || row.bytes.some((byte, i) => current[i] !== byte))
        throw Error('Enemy shuffle modified an unshuffled authored actor: ' + key);
    }
    for (const row of dependencies.values()) {
      if (row.actorOffsets.every(at => supplementalEdits.has(row.game + '/' + at))) continue;
      if (!objectIds(row.game, row.start, row.command).includes(row.objectId))
        throw Error(`Enemy shuffle removed object ${row.objectId.toString(16)} required by native actor ${row.game}/${row.actorId.toString(16)}`);
    }
    const after = patch.newFiles.find(file => file.name === 'custom/mq_rooms');
    for (const row of mqEntries) {
      if (!after || row.bytes.some((byte, i) => after.data[row.at + i] !== byte))
        throw Error('Enemy shuffle modified an unshuffled MQ actor');
    }
    for (const row of mqDependencies) {
      if (!mqHasObject(after!.data, row.header, row.objectId))
        throw Error('Enemy shuffle removed an unshuffled MQ actor model');
    }
  };
}

type EnemySplitChoices = { seeded: readonly string[]; pool?: readonly string[] };

function installEnemySplits(game: Game, patch: Patchfile, choices: ReadonlyMap<number, EnemySplitChoices>,
  foreign: ReadonlyMap<string, ForeignEnemyPlacement>, settings: Settings): void {
  if (!choices.size) return;
  const symbol = `ENEMY_SPLIT_CHOICES_${game.toUpperCase()}`;
  if (choices.size > 2048 || patch.symbols[game].get(symbol)?.length !== 1)
    throw new Error('Missing enemy split payload support');
  const count = [...choices.values()].reduce((sum, choice) => sum + (choice.pool?.length ?? 0), 0);
  if (count > 0xffff) throw new Error('Enemy split candidate capacity exceeded');
  const rowSize = 20;
  const poolOffset = 16 + choices.size * rowSize;
  const data = new Uint8Array(poolOffset + count * 8);
  data.set(toU32Buffer([0x454e4253, 3, rowSize, choices.size]));
  const writeChild = (name: string, offset: number) => {
    const enemy = enemyDefinition(name);
    const actorId = enemy.game === game ? enemy.actorId : foreign.get(name)?.actorId;
    if (!actorId) throw new Error('Invalid enemy split target');
    bufWriteU16BE(data, offset, actorId | (enemy.habitat === 'ground' ? 0x8000 : 0));
    bufWriteU16BE(data, offset + 2, enemy.spawnParams);
  };
  let cursor = 0;
  [...choices].sort(([a], [b]) => a - b).forEach(([key, { seeded, pool }], index) => {
    const offset = 16 + index * rowSize;
    if (seeded.length !== 3) throw new Error('Invalid enemy split child count');
    bufWriteU32BE(data, offset, key);
    seeded.forEach((name, slot) => writeChild(name, offset + 4 + slot * 4));
    bufWriteU16BE(data, offset + 16, cursor);
    bufWriteU16BE(data, offset + 18, pool?.length ?? 0);
    const candidates = pool ?? [];
    const weights = enemyFormWeights(candidates, enemyDefinition, enemy => enemyWeight(settings, game, enemy));
    const thresholds = enemyWeightThresholds(weights);
    candidates.forEach((name, i) => {
      const at = poolOffset + cursor++ * 8;
      writeChild(name, at);
      bufWriteU32BE(data, at + 4, thresholds[i]);
    });
  });
  const vrom = allocatePlacementMetadata(patch, 'mm', data.length, 'Enemy split metadata space exhausted');
  patch.addNewFile({ name: `enemy/native/${game}/split-choices`, vrom, data, compressed: false });
  patch.addSymbolPatch(symbol, toU32Buffer([vrom]));
}

function dynamicEnemyChoice(host: Game, target: EnemyDefinition, foreign: ReadonlyMap<string, ForeignEnemyPlacement>) {
  if (target.game !== host) {
    const imported = foreign.get(target.key);
    if (!imported) throw Error('Missing dynamic foreign enemy: ' + target.key);
    return { id: imported.actorId, params: target.spawnParams, operation: 0 };
  }
  const id = target.actorId, params = target.spawnParams;
  let operation = (host === 'oot' ? OOT_ACTOR_ADJUSTMENTS : MM_PLACEMENT_OPERATIONS)[id] ?? 0;
  if (host === 'oot') {
    if (id === 0x113) operation = 64;
    else if (id === 0xc7 && params === 1) operation = 6;
    else if (id === 0x69) operation = 17;
    else if (id === 0x1d) operation = 4;
  } else {
    if (id === 0x50) operation = 5;
    else if (id === 0x14) operation = 4;
    else if (id === 0x66 && [1, 2].includes(params)) operation = 6;
  }
  if (host === 'oot' && ['Octorok', 'Stinger', 'Stingray'].includes(target.name)) operation = 28;
  if (host === 'mm' && mmAquaticTarget(host, id, params)) operation = 29;
  const attached = host === 'oot' ? [0x37, 0x95].includes(id) : [0x24, 0x50].includes(id);
  if (!attached) operation |= 512 | (target.habitat === 'air' ? 256 : 0);
  return { id, params, operation };
}

function installDynamicChoices(patch: Patchfile, plan: EnemyPlan, foreign: ReadonlyMap<string, ForeignEnemyPlacement>) {
  for (const game of ['oot', 'mm'] as const) {
    const choices = new Map<string, ReturnType<typeof dynamicEnemyChoice>>();
    for (const row of plan) {
      if (enemyDefinition(row.source).game !== game) continue;
      for (const key of [row.target, ...(row.splitChildren ?? []), ...(row.splitPool ?? [])]) {
        const choice = dynamicEnemyChoice(game, enemyDefinition(key), foreign);
        choices.set(choice.id + '/' + choice.params, choice);
      }
    }
    if (!choices.size) continue;
    const sorted = [...choices.values()].sort((a, b) => a.id - b.id || a.params - b.params);
    const data = new Uint8Array(4 + sorted.length * 8);
    bufWriteU32BE(data, 0, sorted.length);
    sorted.forEach((choice, i) => [choice.id, choice.params, choice.operation, 0]
      .forEach((value, j) => bufWriteU16BE(data, 4 + i * 8 + j * 2, value)));
    const vrom = allocatePlacementMetadata(patch, game, data.length, 'Dynamic enemy metadata capacity exceeded');
    patch.addNewFile({ name: `enemy/native/${game}/dynamic-choices`, vrom, data, compressed: false });
    patch.addSymbolPatch('ENEMY_DYNAMIC_CHOICES_' + game.toUpperCase(), toU32Buffer([vrom]));
  }
}

function installDesertLeeverShuffle(patch: Patchfile, plan: EnemyPlan, rooms: EnemySupplementalRoom[], foreign: ReadonlyMap<string, ForeignEnemyPlacement>): void {
  const data = new Uint8Array(OOT_DESERT_LEEVER_SLOTS.length * 8);
  OOT_DESERT_LEEVER_SLOTS.forEach((slot, index) => {
    const row = plan.find(row => row.kind === 'desert-spawner' && row.source === slot.key);
    if (!row) return;
    const target = enemyDefinition(row.target);
    const choice = dynamicEnemyChoice('oot', target, foreign);
    [choice.id, choice.params, choice.operation, 1].forEach((value, field) =>
      bufWriteU16BE(data, index * 8 + field * 2, value));
    rooms.push({ game: 'oot', source: row.source, target: row.target, scene: slot.scene, room: slot.room });
  });
  patch.addSymbolPatch('ENEMY_DESERT_LEEVER_CHOICES', data);
}

function installFieldStalchildShuffle(patch: Patchfile, settings: Settings, plan: EnemyPlan,
  foreign: ReadonlyMap<string, ForeignEnemyPlacement>, rooms: EnemySupplementalRoom[]): void {
  if (!settings.enemyShuffleOot || settings.games === 'mm') return;
  const header = new Uint8Array(OOT_FIELD_STALCHILD_SLOTS.length * 8);
  OOT_FIELD_STALCHILD_SLOTS.forEach((slot, index) => {
    const row = plan.find(row => row.kind === 'field-spawner' && row.source === slot.key);
    if (!row?.field) throw Error('Missing budgeted Hyrule Field spawner slot');
    const original = new Uint8Array(16);
    bufWriteU16BE(original, 0, 0x1b0);
    const unique = new Map([row.field, ...(row.chaoticOotChoices ?? [])].map(choice =>
      [`${choice.foreign ? 'mm' : 'oot'}/${choice.target.actorId}/${choice.targetParams}`, choice]));
    const choices = [...unique.values()];
    const enemies = choices.map(choice => choice === row.field ? enemyDefinition(row.target) :
      enemyDefinition(ootNativeEnemyDefinitionKey(choice.target.actorId, choice.targetParams)));
    const weights = enemyFormWeights(enemies, enemy => enemy, enemy => enemyWeight(settings, 'oot', enemy));
    const total = weights.reduce((a, b) => a + b, 0);
    const data = new Uint8Array(choices.length * 8);
    let cumulative = 0;
    choices.forEach((choice, i) => {
      const imported = choice.foreign ? foreign.get(row.target) : undefined;
      if (choice.foreign && !imported) throw Error('Missing Hyrule Field foreign enemy');
      const entry = prepareOotEnemizerActor(choice, original, choice.location, imported);
      if (!entry) throw Error('Missing Hyrule Field spawn adapter');
      cumulative += Math.max(1, Math.floor(weights[i] * 32768 / total));
      [entry.targetId, entry.targetParams, entry.operation, cumulative].forEach((value, field) =>
        bufWriteU16BE(data, i * 8 + field * 2, value));
    });
    const vrom = allocatePlacementMetadata(patch, 'oot', data.length, 'Field enemy metadata space exhausted');
    patch.addNewFile({ name: `enemy/native/oot/field-${slot.slot}`, vrom, data, compressed: false });
    bufWriteU32BE(header, index * 8, vrom);
    bufWriteU16BE(header, index * 8 + 4, choices.length);
    bufWriteU16BE(header, index * 8 + 6, cumulative);
    rooms.push({ game: 'oot', source: row.source, target: row.target, scene: slot.scene, room: slot.room });
  });
  patch.addSymbolPatch('ENEMY_FIELD_STALCHILD_CHOICES', header);
}

function installMmEnemyDependencies(rom: Uint8Array, plan: EnemyPlan, patch: Patchfile) {
  const replacements = new Map(plan.filter(row => row.placement).map(row => [row.placement!.actorOffset, row.placement!]));
  const rows = new Map<number, Uint8Array>();
  const relocations = patch.meta.enemyRoomRelocations ?? [];
  for (const room of collectEnemyRooms(rom, 'mm')) for (const header of room.headers) {
    const list = header.actors;
    if (!list) continue;
    for (let slot = 0; slot < list.count; ++slot) {
      const at = list.offset + slot * 16;
      const id = bufReadU16BE(rom, at) & 0x1fff, params = bufReadU16BE(rom, at + 14);
      let type = 0, source = at, hostSlot = 0, target = 0, yaw = 0;
      if (id === 0x1b0 && (params & 15) === 4) {
        const hosts: number[] = [];
        for (let i = 0; i < list.count; ++i) {
          const candidate = list.offset + i * 16;
          if (replacements.has(candidate) && bufEqual(rom.subarray(at + 2, at + 8), rom.subarray(candidate + 2, candidate + 8))) hosts.push(i);
        }
        if (hosts.length > 1) throw Error('Ambiguous shuffled stray fairy host');
        if (!hosts.length) continue;
        hostSlot = hosts[0];
        source = list.offset + hostSlot * 16;
        const replacement = replacements.get(source)!;
        target = replacement.targetActorId + (replacement.targetGame === 'oot' ? 0xc00 : 0);
        type = 1;
      } else if (id === 0x12d && params === 0 && replacements.has(at)) {
        const replacement = replacements.get(at)!;
        if (replacement.targetGame === 'mm' && replacement.targetActorId === 0x12d && replacement.targetParams === 0) continue;
        target = 2;
        hostSlot = slot;
        yaw = (bufReadU16BE(rom, at + 10) >>> 7) * 0x10000 / 360;
        type = 3;
      } else if (id === 0xec && (params & 0x80) && replacements.has(at)) {
        const replacement = replacements.get(at)!;
        if (replacement.targetGame === 'mm' && replacement.targetActorId === 0xec && (replacement.targetParams & 0x80)) continue;
        const rotZ = bufReadU16BE(rom, at + 12) >>> 7;
        const signedRotZ = (bufReadU16BE(rom, at) & 0x2000) ? (rotZ > 180 ? rotZ - 360 : rotZ) : ((Math.trunc(rotZ * 0x10000 / 360) << 16) >> 16);
        target = 0xff00 | Math.max(1, Math.min(255, signedRotZ + 100));
        yaw = (bufReadU16BE(rom, at + 10) >>> 7) * 0x10000 / 360;
        type = 2;
      } else continue;
      const key = remapRelocatedRoomKey(at, relocations);
      const data = new Uint8Array(16);
      bufWriteU32BE(data, 0, key);
      data.set(rom.subarray(at + 2, at + 8), 4);
      bufWriteU16BE(data, 10, target);
      bufWriteU16BE(data, 12, yaw & 0xffff);
      data[14] = hostSlot;
      data[15] = type;
      if (rows.has(key) && !bufEqual(rows.get(key)!, data)) throw Error('Conflicting MM enemy puzzle binding');
      rows.set(key, data);
    }
  }
  if (!rows.size) return;
  if (rows.size > 64) throw Error('MM enemy puzzle binding capacity exceeded');
  const data = new Uint8Array(8 + rows.size * 16);
  bufWriteU32BE(data, 0, 0x45445031);
  bufWriteU32BE(data, 4, rows.size);
  [...rows].sort((a, b) => a[0] - b[0]).forEach(([, row], index) => data.set(row, 8 + index * 16));
  const vrom = allocatePlacementMetadata(patch, 'mm', data.length, 'MM enemy puzzle metadata capacity exceeded');
  patch.addNewFile({ name: 'enemy/mm-room-dependencies', vrom, data, compressed: false });
  patch.addSymbolPatch('ENEMY_MM_ROOM_DEPENDENCIES_V1', new Uint8Array([vrom >>> 24, vrom >>> 16, vrom >>> 8, vrom]));
}

function installLilySourceChoices(plan: EnemyPlan, rom: Uint8Array, patch: Patchfile, rooms: EnemySupplementalRoom[], foreign: ReadonlyMap<string, ForeignEnemyPlacement>) {
  const selected = plan.filter(row => row.placement?.sourceActorId === 0x13a)
    .sort((a, b) => a.placement!.scene - b.placement!.scene || a.placement!.sourceParams - b.placement!.sourceParams);
  if (!selected.length) return;
  const data = new Uint8Array(8 + selected.length * 12);
  bufWriteU32BE(data, 0, 0x454c5331);
  bufWriteU32BE(data, 4, selected.length);
  const keys = new Set<string>();
  selected.forEach((row, i) => {
    const source = row.placement!, target = enemyDefinition(row.target), key = `${source.scene}/${source.sourceParams}`;
    if (keys.has(key) || source.sourceParams >> 12 ||
      !matchesMmActorEntry(rom, source.actorOffset, source.raw)) throw Error(`Unreviewed lily platform replacement: ${key} -> ${target.key}/${source.targetParams.toString(16)}`);
    keys.add(key);
    rooms.push({ game: 'mm', source: row.source, target: row.target, scene: source.scene, room: source.room });
    const choice = dynamicEnemyChoice('mm', { ...target, spawnParams: source.targetParams }, foreign);
    [source.scene, source.sourceParams, choice.id, choice.params, +(target.habitat === 'air'), choice.operation]
      .forEach((value, j) => bufWriteU16BE(data, 8 + i * 12 + j * 2, value));
  });
  const vrom = allocatePlacementMetadata(patch, 'mm', data.length, 'Lily source metadata capacity exceeded');
  patch.addNewFile({ name: 'enemy/mm-lily-sources', vrom, data, compressed: false });
  if (patch.symbols.mm.get('ENEMY_LILY_CHOICES_MM_V1')?.length !== 1) throw Error('Missing lily source runtime');
  patch.addSymbolPatch('ENEMY_LILY_CHOICES_MM_V1', toU32Buffer([vrom]));
}

function installGoldSkulltulaChoices(plan: EnemyPlan, roms: DecompressedRoms, patch: Patchfile, rooms: EnemySupplementalRoom[], foreign: ReadonlyMap<string, ForeignEnemyPlacement>) {
  for (const game of ['oot', 'mm'] as const) {
    const selected = plan.filter(row => row.kind === 'gold' && enemyDefinition(row.source).game === game).sort((a, b) => {
      const x = ENEMY_GOLD_SOURCES.find(s => s.key === a.source)!;
      const y = ENEMY_GOLD_SOURCES.find(s => s.key === b.source)!;
      return x.scene - y.scene || x.reward - y.reward;
    });
    if (!selected.length) continue;
    const data = new Uint8Array(8 + selected.length * 12);
    bufWriteU32BE(data, 0, 0x45475331);
    bufWriteU32BE(data, 4, selected.length);
    for (let i = 0; i < selected.length; i++) {
      const row = selected[i], source = ENEMY_GOLD_SOURCES.find(s => s.key === row.source)!;
      const target = enemyDefinition(row.target);
      for (const slot of source.slots) {
        const rom = slot.mq ? patch.newFiles.find(file => file.name === 'custom/mq_rooms')!.data : roms[game].rom;
        if (!matchesRomBytes(rom, slot.offset, slot.raw)) throw Error('Gold Skulltula source entry changed');
        rooms.push({ game, source: row.source, target: row.target, scene: source.scene, room: slot.room });
      }
      const choice = dynamicEnemyChoice(game, target, foreign);
      const at = 8 + i * 12;
      const flags = +(target.habitat === 'air') | ('groundReward' in source && source.groundReward ? 2 : 0);
      [source.scene, source.reward, choice.id, choice.params, flags, choice.operation]
        .forEach((v, j) => bufWriteU16BE(data, at + j * 2, v));
    }
    const vrom = allocatePlacementMetadata(patch, game, data.length, 'Gold Skulltula choice metadata capacity exceeded');
    patch.addNewFile({ name: `enemy/${game}-gold-skulltulas`, vrom, data, compressed: false });
    const symbols = patch.symbols[game].get(`ENEMY_GOLD_CHOICES_${game.toUpperCase()}_V1`);
    if (symbols?.length !== 1) throw Error('Missing Gold Skulltula reward runtime');
    patch.addSymbolPatch(`ENEMY_GOLD_CHOICES_${game.toUpperCase()}_V1`, toU32Buffer([vrom]));
  }
}

export function installSoulPreviews(roms: DecompressedRoms, patch: Patchfile, settings: Settings) {
  const enabled = settings.soulsEnemyLens !== 'none' && (settings.soulsEnemyOot || settings.soulsEnemyMm);
  const { meshes, failures } = enabled ? buildSoulPreviewMeshes(roms) : { meshes: [], failures: [] };
  if (failures.length) throw Error('Invalid soul preview meshes: ' + failures.join('; '));
  if (enabled) {
    const arwing = patch.newFiles.find(f => f.name === 'custom/arwing');
    if (!arwing) throw Error('Missing Arwing preview source');
    const data = bakeSoulPreview(arwing.data, { game: 'oot', id: 0x73, name: 'Arwing', object: 0x2026, segment: 6, scale: 0.02, skeletons: [], animations: [], lists: [{ offset: 0x39c0, name: 'body' }] });
    meshes.push({ game: 'oot', id: 0x73, name: 'Arwing', data, fragments: [] }, { game: 'mm', id: 0x56, name: 'Arwing', data, fragments: [] });
  }
  const records: { game: number; id: number; vrom: number; size: number }[] = [];
  const sources = enabled ? soulPreviewSources(roms, patch) : [];
  const proofs = new Map();
  let fragmentBytes = 0;
  for (const mesh of meshes) {
    if (mesh.data.length + 80 > (ENEMY_SOUL_PREVIEW_MEMORY[`${mesh.game}/${mesh.id}`] ?? 0)) throw Error('Soul preview exceeds memory bound: ' + mesh.name);
    const vrom = allocatePlacementMetadata(patch, 'oot', mesh.data.length, 'Soul preview model capacity exceeded');
    const data = storeSoulPreview(mesh.data, mesh.fragments, sources, vrom, proofs);
    fragmentBytes += mesh.fragments.reduce((n, f) => n + f.data.length, 0);
    patch.addNewFile({ name: `enemy/soul-preview/${mesh.game}-${mesh.id}`, vrom, data, compressed: false });
    records.push({ game: mesh.game === 'oot' ? 0 : 1, id: mesh.id, vrom: vrom + (mesh.fragments.length ? 1 : 0), size: mesh.data.length });
  }

  for (const [game, id, fromGame, fromId] of [[0, 0x69, 1, 0x3e], [0, 0xf6, 0, 0xe0], [0, 0xa5, 0, 0xa4], [0, 0x86, 0, 2], [1, 0x235, 1, 0x4c], [1, 0x181, 1, 0x180], [1, 0x15e, 1, 0x15d], [1, 0x144, 1, 7], [1, 0x212, 1, 0xed]]) {
    const source = records.find(r => r.game === fromGame && r.id === fromId);
    if (source && !records.some(r => r.game === game && r.id === id)) records.push({ ...source, game, id });
  }
  records.sort((a, b) => a.game - b.game || a.id - b.id);
  const data = new Uint8Array(8 + records.length * 12);
  bufWriteU32BE(data, 0, 0x53505431);
  bufWriteU32BE(data, 4, records.length);
  records.forEach((r, i) => {
    const at = 8 + i * 12;
    bufWriteU16BE(data, at, r.game);
    bufWriteU16BE(data, at + 2, r.id);
    bufWriteU32BE(data, at + 4, r.vrom);
    bufWriteU32BE(data, at + 8, r.size);
  });
  const vrom = allocatePlacementMetadata(patch, 'oot', data.length, 'Soul preview table capacity exceeded');
  patch.addNewFile({ name: 'enemy/soul-preview-table', vrom, data, compressed: false });
  for (const game of ['oot', 'mm'] as const) {
    const symbol = `ENEMY_SOUL_PREVIEW_${game.toUpperCase()}_V1`;
    if (patch.symbols[game].get(symbol)?.length !== 1) throw Error('Missing soul preview runtime');
    const module = patch.meta.enemySoulPreviewModules?.[game];
    if (!module || module.length !== 7 || Math.max(module[1], module[3]) + 48 > ENEMY_UTILITY_MODULE_BYTES) throw Error('Missing soul preview module manifest');
    patch.addSymbolPatch(symbol, toU32Buffer([...module, vrom]));
  }
  patch.meta.enemySoulPreviews = { models: records.length, failures, fragmentBytes };
  patch.meta.enemySoulPreviewSources = [...proofs.values()];
}

export function patchEnemies(plan: EnemyPlan, roms: DecompressedRoms, addresses: GameAddresses, patch: Patchfile, settings: Settings, monitor?: Monitor) {
  for (const game of ['oot', 'mm'] as const) {
    if (!(game === 'oot' ? settings.enemyShuffleOot : settings.enemyShuffleMm)) continue;
    const timers = game === 'oot' ? [[0x80b25350, 0x241803ff]] :
      [[0x80973c60, 0x241801ff], [0x80b10310, 0], [0x80b10508, 0x5000001b]];
    for (const [vram, instruction] of timers) {
      const address = addresses[game].fileFromRAM(vram);
      patch.addPatch(address.name, address.offset, toU32Buffer([instruction]));
    }
  }
  monitor?.log('Preparing enemy models and effects');
  verifyMmRoomOverlaps(roms.mm.rom);
  for (const game of ['oot', 'mm'] as const) {
    const arwing = ARWING_ENEMIES[game];
    if (!plan.some(row => row.target === arwing.key)) continue;
    const { overlay, profile } = installedEnemyDependency(patch, game, addresses[game],
      arwing.actorId, 5, `ovl/${game}/${game}_ovl_Enemy_Arwing.zovlx`);
    if (bufReadU16BE(overlay.data, profile + 8) !== arwing.objectId ||
      !patch.newFiles.some(file => file.name === 'custom/arwing'))
      throw new Error('Missing Arwing model or incorrect native profile');
  }
  installDebugEnemyCatalog(patch);
  const supplementalRooms: EnemySupplementalRoom[] = [];
  const supplementalEdits = new Set<string>();
  const verifyNativeActors = nativeActorPreservation(plan, roms, addresses, patch, supplementalEdits);
  const foreignTargets = prepareForeignEnemies(plan, roms, addresses, patch);
  installDynamicChoices(patch, plan, foreignTargets);
  installGoldSkulltulaChoices(plan, roms, patch, supplementalRooms, foreignTargets);
  installLilySourceChoices(plan, roms.mm.rom, patch, supplementalRooms, foreignTargets);
  installDesertLeeverShuffle(patch, plan, supplementalRooms, foreignTargets);
  const guards = plan.find(row => row.kind === 'keeta-guards');
  if (guards) {
    const target = enemyDefinition(guards.target);
    const choice = dynamicEnemyChoice('mm', target, foreignTargets);
    const data = new Uint8Array(8);
    [choice.id, choice.params, choice.operation, 1].forEach((value, i) => bufWriteU16BE(data, i * 2, value));
    patch.addSymbolPatch('ENEMY_MM_KEETA_GUARDS_V1', data);
    for (const room of collectEnemyRooms(roms.mm.rom, 'mm')) for (const header of room.headers) {
      if (header.scene !== 0x0c || !header.actors) continue;
      if (Array.from({ length: header.actors.count }, (_, slot) => header.actors!.offset + slot * 16)
        .some(at => (bufReadU16BE(roms.mm.rom, at) & 0x1fff) === 0x11b))
        supplementalRooms.push({ game: 'mm', source: guards.source, target: guards.target, scene: header.scene, room: header.room });
    }
  }
  installFieldStalchildShuffle(patch, settings, plan, foreignTargets, supplementalRooms);
  monitor?.log('Preparing enemy soul previews');
  installSoulPreviews(roms, patch, settings);
  monitor?.log('Writing enemy replacements and rewards');
  const chaoticOotPools = new Map<number, NativePlacementRecord[]>();
  const chaoticMmPools = new Map<number, NativePlacementRecord[]>();
  const chaoticOotChoices = new Map(plan.filter(row => row.ootr && row.chaoticOotChoices)
    .map(row => [row.ootr!, row.chaoticOotChoices!]));
  const setPoolWeights = (host: Game, records: NativePlacementRecord[], enemies: readonly EnemyDefinition[]) => {
    const weights = enemyFormWeights(enemies, enemy => enemy, enemy => enemyWeight(settings, host, enemy));
    records.forEach((record, index) => {
      record.weight = weights[index];
    });
  };
  const splitChoices = { oot: new Map<number, EnemySplitChoices>(), mm: new Map<number, EnemySplitChoices>() };
  for (const game of ['oot', 'mm'] as const) {
    if (!plan.some(p => enemyDefinition(p.source).game === game)) continue;
    const exactRecords: NativePlacementRecord[] = [];
    const sourceLayoutProof = patch.dup();
    let exactApplied = false;
    if (game === 'oot' && plan.some(row => row.ootr)) {
      installOotEnemyNativeArena(roms.oot.rom, addresses, patch);
      installOotEnemizerRuntime(roms.oot.rom, addresses, patch);
      const lifecycle = patch.symbols.oot.get('ENEMY_NATIVE_OBJECT_LIFECYCLE_OOT_V1');
      const payloads = patch.newFiles.filter(file => file.name === 'oot/payload'), span = payloads.length === 1 ? payloads[0].vram?.oot : undefined;
      if (lifecycle?.length !== 1 || !span || lifecycle[0] < span[0] || lifecycle[0] >= span[1]) throw new Error('Missing exact OoT native object lifecycle');
      patch.addSymbolPatch('ENEMY_NATIVE_OBJECT_LIFECYCLE_OOT_V1', Uint8Array.of(1));
      const exact = applyExactOotEnemizer(roms, addresses, patch, plan, foreignTargets);
      exactRecords.push(...exact.records);
      exactApplied = true;
      for (const seed of exact.records) {
        if (!seed.ootPlacement) continue;
        const { choice, key } = seed.ootPlacement;
        const options = chaoticOotChoices.get(choice);
        if (!options?.length) continue;
        const alternatives = options.flatMap(option => {
          const entry = prepareOotEnemizerActor(option, seed.original, key);
          return entry?.operation ? [{ ...seed, ...entry }] : [];
        });
        if (!alternatives.length) continue;
        const pool = [{ ...seed }, ...alternatives];
        const enemies = [enemyDefinition(plan.find(row => row.ootr === choice)!.target),
        ...alternatives.map(record => enemyDefinition(ootNativeEnemyDefinitionKey(record.targetId, record.targetParams)))];
        setPoolWeights('oot', pool, enemies);
        chaoticOotPools.set(seed.vrom, pool);
      }
      for (const [key, choices] of exact.splitChoices) splitChoices.oot.set(key, choices);
      installOotEnemizerSwitches(exact.switchRecords, patch);
      patch.meta.enemyShuffle ??= {};
      patch.meta.enemyShuffle.oot = {
        replaced: exact.replaced,
        mqReplaced: exact.mqReplaced,
        arenaReplaced: 0,
        algorithm: 'OoT weighted actor locations',
        placementMode: settings.enemyShuffleOotMode ?? 'individual',
        plan: plan.filter(row => row.ootr || row.field),
      };
    }
    if (game === 'mm' && plan.some(row => row.placement)) {
      const replaced = applyExactMmPlacements(roms.mm.rom, addresses, patch, plan, foreignTargets);
      const activationRecords = exactMmActivationRecords(roms.mm.rom, plan, patch);
      exactRecords.push(...activationRecords);
      exactApplied = true;
      const byOffset = new Map(activationRecords.map(record => [record.sourceVrom ?? record.vrom, record]));
      for (const row of plan) {
        if (!row.placement || !row.chaoticMmChoices?.length) continue;
        const seed = byOffset.get(row.placement.actorOffset);
        if (!seed) continue;
        const alternatives = exactMmActivationRecords(roms.mm.rom,
          row.chaoticMmChoices.map(placement => ({ ...row, target: placement.targetKey, placement })), patch);
        if (alternatives.length) {
          const pool = [{ ...seed }, ...alternatives];
          const placements = [row.placement, ...row.chaoticMmChoices];
          const enemies = pool.map(record => {
            const placement = placements.find(candidate => candidate.targetParams === record.targetParams &&
              (candidate.targetActorId === record.targetId || candidate.targetActorId + 0xc00 === record.targetId));
            if (!placement) throw Error('Missing chaotic MM enemy definition');
            const enemy = enemyDefinition(placement.targetKey);
            return { ...enemy, spawnParams: placement.targetParams };
          });
          setPoolWeights('mm', pool, enemies);
          chaoticMmPools.set(seed.vrom, pool);
        }
      }
      patch.meta.enemyShuffle ??= {};
      patch.meta.enemyShuffle.mm = {
        replaced,
        arenaReplaced: 0,
        algorithm: 'MM scene object groups',
        placementMode: settings.enemyShuffleMmMode ?? 'grouped',
        plan: plan.filter(row => row.placement),
      };
    }
    const supplementalPlan = exactApplied ? plan.filter(row => row.kind && enemyDefinition(row.source).game === game) : plan;
    if (game === 'oot') {
      const relocations = patch.meta.enemyRoomRelocations ?? [];
      for (const row of exactRecords) {
        const key = remapRelocatedRoomKey(row.vrom, relocations);
        if (key === row.vrom) continue;
        row.sourceVrom = row.vrom;
        row.vrom = key;
        const pool = chaoticOotPools.get(row.sourceVrom);
        if (pool) {
          chaoticOotPools.delete(row.sourceVrom);
          chaoticOotPools.set(key, pool.map(choice => ({ ...choice, vrom: key })));
        }
      }
    }
    if (exactApplied && !supplementalPlan.length) {
      installNativePlacements(game, roms[game].rom, patch, exactRecords, addresses);
      continue;
    }
    const rom = roms[game].rom;
    const records: NativePlacementRecord[] = [];
    const scriptedLayoutProof = sourceLayoutProof;
    const sceneObjects: SceneObjects = new Map();
    if (exactApplied) for (const row of plan) {
      const imported = foreignTargets.get(row.target), location = row.placement ?? row.ootr?.location;
      if (!imported || imported.targetGame !== game || !location) continue;
      for (const scene of new Set([location.scene, ...(row.ootr?.aliases.map(alias => alias[0]) ?? [])])) {
        const held = new Map(sceneObjects.get(scene));
        for (const [id, size] of imported.objects.pinned) held.set(id, size);
        sceneObjects.set(scene, held);
      }
    }
    let replaced = 0, arenaReplaced = 0;
    for (const room of collectEnemyRooms(rom, game)) {
      const hasScriptedSource = game === 'mm' && supplementalPlan.some(pair => pair.kind === 'scripted' && room.headers.some(header =>
        verifiedScriptedMmArenas(rom, { scene: header.scene, room: header.room, start: room.start, end: room.end }).length));
      if (room.excluded && !hasScriptedSource) continue;
      const actorOffsets = new Set<number>();
      for (const header of room.headers) if (header.actors)
        for (let i = 0; i < header.actors.count; ++i) actorOffsets.add(header.actors.offset + i * 16);
      room.protectedEntries = new Set([...actorOffsets].filter(offset => nativeEntryProtected(game, offset, addresses, patch)));
      const changes = planRoomEnemies(rom, game, room, supplementalPlan, rom, foreignTargets, sceneObjects);
      const stageRoomEdits = (candidate: typeof patch, edits: readonly Change[]) => {
        for (const { offset, value } of edits) {
          const address = addresses[game].fileFromROM(offset), data = new Uint8Array(2);
          bufWriteU16BE(data, 0, value);
          candidate.addPatch(address.name, address.offset, data);
        }
      };
      if (game === 'mm') for (const pair of supplementalPlan.filter(pair => pair.kind === 'scripted')) {
        const source = scriptedMmSource(pair.source), target = enemyDefinition(pair.target);
        if (!source || !scriptedMmTargetAllowed(target, true, source) || !verifyScriptedMmNativeSource(rom, source.actorId, addresses, patch)) continue;

        if (source.actorId === 0x15d && target.game === 'mm' && target.actorId === 0x15d) continue;
        const imported = target.game === 'mm' ? undefined : foreignTargets.get(target.key);
        if (target.game !== 'mm' && !imported) throw new Error('Missing scripted MM foreign target');
        const arenaFacts = SCRIPTED_MM_ARENAS.filter(row => row.roomStart === room.start && row.roomEnd === room.end &&
          room.headers.some(header => header.scene === row.scene && header.room === row.room));
        const scriptedOffsets = new Set(arenaFacts.flatMap(row => row.roots.filter(root => root.actorId === source.actorId).map(root => root.offset)));
        for (const offset of scriptedOffsets) {
          if (nativeEntryProtected(game, offset, addresses, patch)) continue;
          const aliases = arenaFacts.filter(row => row.roots.some(root => root.offset === offset));
          if (!aliases.length || aliases.some(row => !verifiedScriptedMmArenas(rom, {
            scene: row.scene, room: row.room,
            start: room.start, end: room.end, actorOffset: offset, roomHeader: row.roomHeader, sceneHeader: row.sceneHeader
          }, addresses, scriptedLayoutProof).length)) continue;
          verifyScriptedMmController(rom, addresses, patch);

          if (!imported && !nativeEnemyOnDemandObjects(game, target.actorId)) continue;
          records.push(nativePlacementRecord(rom, offset, 128, [], imported?.actorId ?? target.actorId, target.spawnParams));
          if (imported || nativeEnemyOnDemandObjects(game, target.actorId)) for (const alias of aliases) supplementalRooms.push({ game, source: pair.source, target: pair.target, scene: alias.scene, room: alias.room });
          ++arenaReplaced;
        }
      }
      let roomUnmodified = false;
      try {
        const file = addresses[game].fileFromROM(room.start);
        roomUnmodified = !room.excluded && !file.offset && !sourceLayoutProof.removedFiles.includes(file.name) &&
          !sourceLayoutProof.newFiles.some(row => row.name === file.name) && !(sourceLayoutProof.patches[file.name]?.length);
      } catch { }
      for (const pair of supplementalPlan.filter(pair => pair.kind === 'restore' && enemyDefinition(pair.source).game === game)) {
        if (!roomUnmodified || room.mq || !nativeFightRestorationSource(pair.source)) continue;
        const target = enemyDefinition(pair.target);
        for (const header of room.headers) {
          if (!nativeFightRestorationRoomAllowed(pair.source, header.room, settings)) continue;
          const restored = planNativeFightRestoration(game, rom, { ...header, start: room.start, end: room.end, mq: !!room.mq },
            pair.source, { ...target, actorId: foreignTargets.get(pair.target)?.actorId ?? target.actorId });
          if (!restored) continue;
          changes.push(...restored.changes);
          records.push(nativePlacementRecord(rom, restored.actorOffset, 1, changes, foreignTargets.get(pair.target)?.actorId ?? target.actorId, target.spawnParams));
          if (foreignTargets.has(pair.target) || nativeEnemyOnDemandObjects(game, target.actorId))
            for (const alias of room.headers) supplementalRooms.push({ game, source: pair.source, target: pair.target, scene: alias.scene, room: alias.room });
          ++arenaReplaced;
          break;
        }
      }
      for (const pair of supplementalPlan.filter(pair => pair.kind && enemyDefinition(pair.source).game === game)) {
        if (pair.kind === 'variant' && nativeMinibossVariant(pair.target)) {
          for (const offset of actorOffsets) {
            const setups = room.headers.filter(header => header.actors && offset >= header.actors.offset && offset < header.actors.offset + header.actors.count * 16)
              .map(header => ({ complete: true, actorOffsets: Array.from({ length: header.actors!.count }, (_, i) => header.actors!.offset + i * 16) }));
            if (room.protectedEntries.has(offset) || !roomUnmodified) continue;
            const variant = planNativeMinibossVariant(rom, game, { actorOffset: offset, setups }, pair.target as 'mm-wizzrobe-fire' | 'mm-wizzrobe-ice');
            changes.push(...variant);
            arenaReplaced += Number(variant.length > 0);
          }
          continue;
        }
        if (game !== 'oot' || pair.kind !== 'arena' || room.mq ||
          room.headers.length !== 1 || room.protectedEntries.size || !roomUnmodified) continue;
        const header = room.headers[0], loc = { ...header, start: room.start, end: room.end, mq: !!room.mq };
        const target = enemyDefinition(pair.target);
        const selected = { actorId: foreignTargets.get(pair.target)?.actorId ?? target.actorId, spawnParams: target.spawnParams };
        const arena = pair.source === 'oot-dead-hand-encounter' ? planDeadHandArenaCombat(rom, loc, selected) :
          pair.source === 'oot-dark-link-encounter' ? planDarkLinkArenaCombat(rom, loc, selected) :
            pair.source === 'oot-big-octo-encounter' && pair.target === 'oot-big-octo-arena-biri' ? planBigOctoArenaCombat(rom, loc, pair.target) : undefined;
        if (!arena || !header.actors) continue;
        if (foreignTargets.has(pair.target) || nativeEnemyOnDemandObjects(game, target.actorId))
          supplementalRooms.push({ game, source: pair.source, target: pair.target, scene: header.scene, room: header.room });
        if (arena.controllerSlot !== undefined) {
          records.push(nativePlacementRecord(rom, header.actors.offset + arena.controllerSlot * 16, 8, [], selected.actorId, selected.spawnParams));
        }
        if (arena.replacementSlot !== undefined)
          records.push(nativePlacementRecord(rom, header.actors.offset + arena.replacementSlot * 16, 129, arena.changes, selected.actorId, selected.spawnParams));
        for (const slot of arena.suppressedOriginalSlots)
          records.push(nativePlacementRecord(rom, header.actors.offset + slot * 16, 2, []));
        changes.push(...arena.changes);
        ++arenaReplaced;
      }
      for (const offset of actorOffsets) {
        if (!changes.some(change => change.offset === offset)) continue;
        const source = bufReadU16BE(rom, offset) & (game === 'mm' ? 0x1fff : 0xffff);
        const target = changes.find(change => change.offset === offset)!.value & (game === 'mm' ? 0x1fff : 0xffff);
        const params = changes.find(change => change.offset === offset + 14)?.value ?? bufReadU16BE(rom, offset + 14);
        const previous = records.find(row => row.vrom === offset);
        let operation = previous?.operation ?? (nativeResourceSource(game, source, bufReadU16BE(rom, offset + 14)) ? 1 :
          (game === 'oot' && target === 0x1b0 && [0, 5, 10].includes(params)) ? 3 : (game === 'mm' && target === 0x50 && params === 0xff00) ? 5 : (game === 'oot' && target === 0x1c0 && [0, 1].includes(params)) ? 7 : ((game === 'oot' && target === 0xc7 && params === 1) || (game === 'mm' && target === 0x66 && [1, 2].includes(params))) ? 6 : ((game === 'oot' && target === 0x1d && [0, 0xffff].includes(params)) || (game === 'mm' && target === 0x14 && params === 0)) ? 4 : game === 'oot' && target === 0x115 ? 16 : game === 'oot' && target === 0x113 && [0xff82, 0xff83].includes(params) ? 64 : 0);
        let aliases: OotPlacementAlias[] | undefined;
        if (game === 'oot') {
          aliases = room.headers.filter(header => header.actors && offset >= header.actors.offset && offset < header.actors.offset + header.actors.count * 16)
            .map(header => ({ mq: false, loc: header, slot: (offset - header.actors!.offset) / 16, source }));
          if (ootPlacementAdaptersAgree(aliases) && ootPlacementAdapter(false, aliases[0].loc, aliases[0].slot, source)?.floorOperation)
            operation |= 32;
        }
        if (!operation) continue;
        const record = nativePlacementRecord(rom, offset, operation, changes, target, params);
        record.placementAliases = aliases;
        if (previous) records[records.indexOf(previous)] = record; else records.push(record);
      }
      for (const record of records) if (record.operation === 2 && actorOffsets.has(record.sourceVrom ?? record.vrom))
        supplementalEdits.add(`${game}/${record.sourceVrom ?? record.vrom}`);
      const roomExact = exactApplied ? plan.filter(row => {
        if (row.placement) return game === 'mm' && actorOffsets.has(row.placement.actorOffset);
        const location = row.ootr?.location;
        return game === 'oot' && location && !location.mq && room.headers.some(h =>
          h.scene === location.scene && h.room === location.room && h.setup === location.setup ||
          row.ootr!.aliases.some(alias => h.scene === alias[0] && h.room === alias[1] && h.setup === alias[2]));
      }) : [];
      for (const row of roomExact) {
        const offsets = row.placement ? [row.placement.actorOffset] : room.headers.flatMap(h => {
          if (!h.actors) return [];
          const location = row.ootr!.location;
          const slots = [...(h.scene === location.scene && h.room === location.room && h.setup === location.setup ? [location.slot] : []),
          ...row.ootr!.aliases.filter(a => h.scene === a[0] && h.room === a[1] && h.setup === a[2]).map(a => a[3])];
          return slots.map(slot => h.actors!.offset + slot * 16);
        });
        if (changes.some(change => offsets.some(offset => change.offset >= offset && change.offset < offset + 16)))
          throw Error('Supplemental encounter overlaps an exact standalone placement');

      }
      for (const pair of supplementalPlan) {
        if (pair.kind || room.mq) continue;
        const source = enemyDefinition(pair.source), target = enemyDefinition(pair.target);
        if (source.game !== game || target.game !== game || !nativeEnemyOnDemandObjects(game, target.actorId))
          continue;
        const mask = game === 'mm' ? 0x1fff : 0xffff;
        const offsets = changes.filter(change => actorOffsets.has(change.offset) &&
          (bufReadU16BE(rom, change.offset) & mask) === source.actorId &&
          (change.value & mask) === target.actorId).map(change => change.offset);
        for (const header of room.headers) {
          const list = header.actors;
          if (list && offsets.some(offset => offset >= list.offset && offset < list.offset + list.count * 16))
            supplementalRooms.push({ game, source: pair.source, target: pair.target, scene: header.scene, room: header.room });
        }
      }
      const edits = new Map<number, number>();
      for (const change of changes) {
        if (edits.has(change.offset) && edits.get(change.offset) !== change.value) throw new Error('Conflicting native enemy arena edits');
        edits.set(change.offset, change.value);
      }
      stageRoomEdits(patch, [...edits].map(([offset, value]) => ({ offset, value })));
      for (const offset of actorOffsets) if (changes.some(change => change.offset >= offset && change.offset < offset + 16))
        supplementalEdits.add(`${game}/${offset}`);
      replaced += [...edits.keys()].filter(offset => actorOffsets.has(offset)).length;
    }
    patch.meta.enemyShuffle ??= {};
    const exactMeta = exactApplied ? patch.meta.enemyShuffle[game] : undefined;
    patch.meta.enemyShuffle[game] = { ...exactMeta, replaced: (exactMeta?.replaced ?? 0) + replaced, arenaReplaced, plan: plan.filter(p => enemyDefinition(p.source).game === game) };
    if (game === 'oot' && !exactApplied) {
      const index = patch.newFiles.findIndex(f => f.name === 'custom/mq_rooms');
      if (index >= 0) {
        const file = patch.newFiles[index], mq = patchMqEnemies(file.data, rom, supplementalPlan, foreignTargets, sceneObjects, records);
        patch.newFiles[index] = { ...file, data: mq.data };
        patch.meta.enemyShuffle.oot.mqReplaced = mq.replaced;
      }
    }
    installNativePlacements(game, rom, patch, [...exactRecords, ...records], addresses);
  }
  installChaoticPlacements('oot', patch, chaoticOotPools);
  installChaoticPlacements('mm', patch, chaoticMmPools);
  for (const row of plan) {
    if (!row.splitChildren || !row.placement) continue;
    const key = remapRelocatedRoomKey(row.placement.actorOffset, patch.meta.enemyRoomRelocations ?? []);
    splitChoices.mm.set(key, { seeded: row.splitChildren, pool: row.splitPool });
  }
  installEnemySplits('oot', patch, splitChoices.oot, foreignTargets, settings);
  installEnemySplits('mm', patch, splitChoices.mm, foreignTargets, settings);
  installEnemyRoomModels(patch, plan, foreignTargets, supplementalRooms);
  installMmObjectBufferBudgets(roms, addresses, patch, plan, supplementalRooms);
  monitor?.log('Validating enemy placements and memory limits');
  const memoryReports = enemySceneMemoryReports(plan, settings);
  for (const report of memoryReports) {
    if (report.objectSavings) {
      const row = patch.meta.enemyObjectBufferBudgets?.verified?.find((r: {
        scene: number;
      }) => r.scene === report.scene);
      const original = (report.scene === 0x2d ? 1470 : 1380) * 1024;
      const used = row ? row.baseBytes + row.playerSlots * ENEMY_MEMORY_POLICY.playerObjectBound +
        row.horseSlots * ENEMY_MEMORY_POLICY.horseObjectBound + 0x1000 : original;
      const verifiedSavings = Math.max(0, original - used);
      if (verifiedSavings < report.objectSavings) {
        report.budget -= report.objectSavings - verifiedSavings;
        report.objectSavings = verifiedSavings;
      }
    }
    if (report.resourceExcess > 0 || report.bytes > report.budget)
      throw Error(`Final enemy plan exceeds MM scene ${report.scene} memory/actor budget`);
  }
  if (memoryReports.length) {
    for (const [name, limit] of [['custom/mm_adult_link', ENEMY_MEMORY_POLICY.playerObjectBound],
    ['custom/mm_adult_epona', ENEMY_MEMORY_POLICY.horseObjectBound]] as const) {
      const model = patch.newFiles.find(file => file.name === name);
      if (!model || model.data.length > limit)
        throw Error(`Player model exceeds enemy memory profile: ${name}`);
    }
  }
  const gameplay = settings.enemyShuffleOot && settings.games !== 'mm' ? verifyOotGameplayMemory(roms.oot.rom, addresses, patch) : undefined;
  patch.meta.enemyMemoryBudget = { gameplay, version: 2, policy: ENEMY_MEMORY_POLICY, mm: memoryReports, ootPolicy: OOT_GAMEPLAY_POLICY, oot: enemyOotSceneMemoryReports(plan, settings) };
  installMmEnemyDependencies(roms.mm.rom, plan.flatMap(row => [row, ...(row.splitChildren ?? []).map(target => ({ ...row, target }))]), patch);
  verifyNativeActors();
}
