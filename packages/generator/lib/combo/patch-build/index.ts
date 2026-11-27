import CRC32 from 'crc-32';
import type { Settings } from '@ootmm/core';
import type { LogicResult, LogicResultWorld } from '@ootmm/logic';
import type { GameAddresses } from '../addresses';
import type { DecompressedRoms } from '../decompress';
import type { PatchGroup } from './group';

import { GAMES } from '@ootmm/data';
import { Monitor, enemyMemoryBuildBounds } from '@ootmm/core';
import { isEntranceShuffle } from '@ootmm/logic';
import { CONFIG } from '../config';
import { Patcher } from './patcher';
import { Patchfile } from './patchfile';
import { patchRandomizer } from '../randomizer';
import { patchEnemies } from '../randomizer/enemies';
import { bufReadU16BE, bufReadU32BE, bufWriteU32BE } from '../util/buffer';
import { FileResolver } from '../file-resolver';
import { archiveEnemyPort } from '../enemies/overlay';

export type BuildPatchfileIn = {
  patch: Patchfile;
  monitor: Monitor;
  roms: DecompressedRoms;
  addresses: GameAddresses;
  logic: LogicResult;
  settings: Settings;
};

function asmPatchGroups(world: LogicResultWorld, settings: Settings) {
  const groups: {[k in PatchGroup]: boolean} = {
    OOT_HOOKSHOT_ANYWHERE: settings.hookshotAnywhereOot !== 'off',
    OOT_CLIMB_ANYWHERE: settings.climbMostSurfacesOot !== 'off',
    MM_FD_ANYWHERE: settings.fierceDeityAnywhere,
    MM_HOOKSHOT_ANYWHERE: settings.hookshotAnywhereMm !== 'off',
    MM_BLAST_MASK_DELAY_INSTANT: settings.blastMaskCooldown === 'instant',
    MM_BLAST_MASK_DELAY_VERYSHORT: settings.blastMaskCooldown === 'veryshort',
    MM_BLAST_MASK_DELAY_SHORT: settings.blastMaskCooldown === 'short',
    MM_BLAST_MASK_DELAY_LONG: settings.blastMaskCooldown === 'long',
    MM_BLAST_MASK_DELAY_VERYLONG: settings.blastMaskCooldown === 'verylong',
    MM_CLOCK_SPEED_VERYSLOW: settings.clockSpeed === 'veryslow',
    MM_CLOCK_SPEED_SLOW: settings.clockSpeed === 'slow',
    MM_CLOCK_SPEED_FAST: settings.clockSpeed === 'fast',
    MM_CLOCK_SPEED_VERYFAST: settings.clockSpeed === 'veryfast',
    MM_CLOCK_SPEED_SUPERFAST: settings.clockSpeed === 'superfast',
    MM_BOSS_WARP_PADS_BEATEN: settings.bossWarpPads === 'bossBeaten',
    OOT_FIRE_TEMPLE_CHILD: world.resolvedFlags.openDungeonsOot.has('fireChild'),
    OOT_FAST_BUNNY_HOOD: settings.fastBunnyHood,
    OOT_AGELESS_SWORDS: settings.agelessSwords,
    OOT_AGELESS_SHIELDS: settings.agelessShields,
    OOT_AGELESS_TUNICS: settings.agelessTunics,
    OOT_AGELESS_BOOTS: settings.agelessBoots,
    OOT_AGELESS_STICKS: settings.agelessSticks,
    OOT_AGELESS_BOOMERANG: settings.agelessBoomerang,
    OOT_AGELESS_HAMMER: settings.agelessHammer,
    OOT_AGELESS_GREAT_FAIRY_SWORD: settings.agelessGFS,
    OOT_AGELESS_HOOKSHOT: settings.agelessHookshot,
    OOT_AGELESS_SLINGSHOT: settings.agelessSlingshot,
    OOT_AGELESS_BOW: settings.agelessBow,
    OOT_AGELESS_CHILD_TRADE: settings.agelessChildTrade,
    OOT_BLUE_FIRE_ARROWS: settings.blueFireArrows,
    OOT_SUNLIGHT_ARROWS: settings.sunlightArrows,
    ER: isEntranceShuffle(settings),
    OOT_SILVER_RUPEE_SHUFFLE: settings.silverRupeeShuffle !== 'vanilla',
    OOT_FREE_SCARECROW: settings.freeScarecrowOot,
    MM_FREE_SCARECROW: settings.freeScarecrowMm,
    MM_LENIENT_SPIKES: settings.lenientSpikes,
    OOT_CHEST_GAME_SHUFFLE: settings.smallKeyShuffleChestGame !== 'vanilla',
    MM_CLIMB_MOST_SURFACES: settings.climbMostSurfacesMm,
    NO_BROKEN_ACTORS: !settings.restoreBrokenActors,
    OOT_FISHING_POND_SHUFFLE: settings.pondFishShuffle,
    OOT_NOT_FISHING_POND_SHUFFLE: !settings.pondFishShuffle,
    ER_WALLMASTERS: settings.erWallmasters !== 'none',
    OOT_SHUFFLE_FREE_RUPEES: settings.shuffleFreeRupeesOot !== 'none',
    MM_SHUFFLE_FREE_RUPEES: settings.shuffleFreeRupeesMm !== 'none',
    MM_OPEN_STT: world.resolvedFlags.openDungeonsMm.has('ST'),
    CRIT_WIGGLE_DISABLE: settings.critWiggleDisable,
    OOT_LOST_WOODS_EXITS: settings.alterLostWoodsExits,
    MM_VOID_WARP: settings.voidWarpMm,
    MM_FAST_MASKS: settings.fastMasks,
    OOT_FAST_BOAT: settings.shadowFastBoat,
    MM_WELL_OPEN_OR_REMORSELESS: settings.beneathWell !== 'vanilla',
    MM_JP_LAYOUT_DEKU_PALACE: world.resolvedFlags.jpLayouts.has('DekuPalace'),
    MM_JP_LAYOUT_STONE_TOWER: world.resolvedFlags.jpLayouts.has('StoneTower') && !world.resolvedFlags.openDungeonsMm.has('ST'),
    MM_JP_LAYOUT_STONE_TOWER_TEMPLE: world.resolvedFlags.jpLayouts.has('ST'),
    MM_JP_LAYOUT_GREAT_BAY_COAST: world.resolvedFlags.jpLayouts.has('GreatBayCoast'),
    MM_KEG_STRENGTH_3: settings.kegStrength3,
    ER_TELESCOPES: settings.erIndoorsTelescopes,
    OOT_SKIP_CUTSCENES: settings.cutsceneSkipOoT,
    MM_CS_GOHT: settings.cutsceneIntroGoht,
    MM_CS_WART: settings.cutsceneIntroWart,
    MM_CS_GYORG: settings.cutsceneIntroGyorg,
    MM_CS_IGOS: settings.cutsceneIntroIgos,
    MM_CS_GOMESS: settings.cutsceneIntroGomess,
    MM_CS_TWINMOLD: settings.cutsceneIntroTwinmold,
    MM_CS_MAJORA: settings.cutsceneIntroMajora
  };
  const keys = Object.keys(groups) as PatchGroup[];
  return keys.filter((k) => groups[k]);
}

const REMOVED_FILES = [
  'mm/ovl_title',
  'mm/ovl_select',
  'mm/ovl_opening',
  'mm/ovl_file_choose',
  'oot/icon_item_jpn_static',
  'oot/n64dd',
]

export async function buildPatchfiles(args: BuildPatchfileIn): Promise<Patchfile[]> {
  args.monitor.log("Building Patchfile");
  const patches: Patchfile[] = [];
  const patchedFiles = new Set<string>();
  let ovlAddr = 0xe0000000;

  const fileResolver = new FileResolver();

  for (let world = 0; world < args.settings.players; ++world) {
    const p = args.patch.dup();
    const groups = asmPatchGroups(args.logic.worlds[world], args.settings);

    for (const game of GAMES) {
      const gc = CONFIG[game];

      /* Apply ASM patches */
      const rom = args.roms[game].rom;
      const patches = await fileResolver.fetch(`${game}_patch.bin`);
      const patcher = new Patcher(game, rom, groups, args.addresses, patches, p);
      const { files } = patcher.run();
      for (const f of files) {
        patchedFiles.add(f);
      }
      /* Pack the payload */
      const payload = await fileResolver.fetch(`${game}_payload.bin`);
      if (payload.length > 0x80000) {
        throw new Error(`Payload too large ${game}`);
      }
      const payloadVrom = game === 'oot' ? 0xf0000000 : 0xf0100000;
      const payloadVram = game === 'oot' ? 0x80400000 : 0x80700000; /* TODO: Codegen this */
      const payloadVramEnd = payloadVram + payload.length;
      p.addNewFile({ name: `${game}/payload`, vrom: payloadVrom, vram: { [game]: [payloadVram, payloadVramEnd] }, data: payload, compressed: false });

      /* Handle extra overlays */
      const allOverlays = await fileResolver.glob(/ovl\/(oot|mm)\/.+\.zovlx$/);
      const overlays = allOverlays.filter((f) => f.startsWith(`ovl/${game}`));
      const actorSlots = new Map<number, string>();
      for (const ov of overlays) {
        /* Resolve and inject the new overlay */
        const raw = await fileResolver.fetch(ov);
        const headerSize = raw.subarray(0, 8);
        const header = raw.subarray(8, 64);
        const data = raw.subarray(64);
        const vramStart = bufReadU32BE(headerSize, 0x00);
        const vramEnd = bufReadU32BE(headerSize, 0x04);
        const type = bufReadU32BE(header, 0x00);
        const bound = enemyMemoryBuildBounds[ov];
        if (bound) {
          const profile = bufReadU32BE(header, 0x08) - vramStart;
          if (vramEnd <= vramStart || vramEnd - vramStart > bound.memory || data.length > bound.memory ||
            profile < 0 || profile % 4 || profile + 32 > data.length || bufReadU32BE(data, profile + 12) > bound.instance)
            throw Error(`Enemy memory facts need refreshing after overlay growth: ${ov}`);
        }
        let vromStart: number;
        if (type === 0x04 || type === 0x05) {
          vromStart = archiveEnemyPort(p, game, bufReadU32BE(header, 0x04), data);
          const installed = p.newFiles.find(file => file.vrom === vromStart)!;
          installed.name = ov;
          installed.vram = { [game]: [vramStart, vramEnd] };
        } else {
          vromStart = ovlAddr;
          p.addNewFile({ name: ov, vram: { [game]: [vramStart, vramEnd] }, vrom: vromStart, data, compressed: true });
          ovlAddr += data.length;
        }
        const vromEnd = vromStart + data.length;
        if (type === 0x05) {
          const profile = bufReadU32BE(header, 8) - vramStart;
          if (!ov.endsWith('_ovl_Enemy_Soul_Preview.zovlx') || profile < 0 || profile + 32 > data.length ||
            bufReadU16BE(data, profile) !== 0x3ff) throw Error('Invalid soul preview module');
          p.meta.enemySoulPreviewModules ??= {};
          p.meta.enemySoulPreviewModules[game] = [vromStart, data.length, vramStart, vramEnd - vramStart, profile, 0, CRC32.buf(data) >>> 0];
        }
        const nativePort = /\/[^/]+_ovl_Native_Enemy_(oot|mm)_[a-z0-9_]+\.zovlx$/.exec(ov);
        if (type === 0x04 || nativePort) {
          const sourceId = bufReadU32BE(header, 0x04), profile = bufReadU32BE(header, 0x08) - vramStart;
          if (type !== 0x04 || !nativePort || nativePort[1] === game || sourceId >= 0x400 ||
            profile % 4 || profile < 0 || profile + 32 > data.length || data.length % 16 ||
            bufReadU16BE(data, profile) !== 0xc00 + sourceId)
            throw new Error('Invalid receiving-native enemy overlay metadata: ' + ov);
        }

        /* TODO: Refactor this mess */
        if (type === 0x01) {
          /* Actor */
          const actorId = bufReadU32BE(header, 0x04);
          const vramInit = bufReadU32BE(header, 0x08);
          const previous = actorSlots.get(actorId);
          if (previous) throw new Error('Duplicate actor overlay slot: ' + previous + ' and ' + ov);
          actorSlots.set(actorId, ov);
          const gomessBat = ov.endsWith('_ovl_Enemy_GomessBat.zovlx');
          const profileOffset = vramInit - vramStart;
          if (gomessBat && (profileOffset < 0 || profileOffset + 32 > data.length ||
            bufReadU16BE(data, profileOffset) !== actorId))
            throw new Error('Invalid Gomess bat actor profile: ' + ov);

          const patch = new Uint8Array(4 * 6);
          bufWriteU32BE(patch, 0x00, vromStart);
          bufWriteU32BE(patch, 0x04, vromEnd);
          bufWriteU32BE(patch, 0x08, vramStart);
          bufWriteU32BE(patch, 0x0c, vramEnd);
          bufWriteU32BE(patch, 0x10, 0);
          bufWriteU32BE(patch, 0x14, vramInit);

          /* Delete the old overlay if possible */
          const oldHeader = rom.subarray(gc.actorsOvlAddr + actorId * 0x20, gc.actorsOvlAddr + actorId * 0x20 + 0x20);
          const oldVramStart = bufReadU32BE(oldHeader, 0x08);
          if ((gomessBat || ov.endsWith('_ovl_Enemy_Poe_Encounter.zovlx') || ov.startsWith('ovl/mm/mm_ovl_Standalone_')) && (oldVramStart || bufReadU32BE(oldHeader, 0x14)))
            throw new Error('Standalone enemy overlay requires an unused actor slot: ' + ov);
          if (oldVramStart !== 0) {
            const oldFile = args.addresses[game].fileFromRAM(oldVramStart);
            p.removedFiles.push(oldFile.name);
          }

          const fileAddr = args.addresses[game].fileFromROM(gc.actorsOvlAddr + actorId * 0x20);
          p.addPatch(fileAddr.name, fileAddr.offset, patch);
        }

        if (type === 0x03) {
          /* Effect_Ss */
          const effectSsId = bufReadU32BE(header, 0x04);
          const vramInit = bufReadU32BE(header, 0x08);

          const patch = new Uint8Array(4 * 6);
          bufWriteU32BE(patch, 0x00, vromStart);
          bufWriteU32BE(patch, 0x04, vromEnd);
          bufWriteU32BE(patch, 0x08, vramStart);
          bufWriteU32BE(patch, 0x0c, vramEnd);
          bufWriteU32BE(patch, 0x10, 0);
          bufWriteU32BE(patch, 0x14, vramInit);

          /* Delete the old overlay if possible */
          const oldHeader = rom.subarray(gc.effectsSsOvlAddr + effectSsId * 0x1c, gc.effectsSsOvlAddr + effectSsId * 0x1c + 0x1c);
          const oldVramStart = bufReadU32BE(oldHeader, 0x08);
          if (oldVramStart !== 0) {
            const oldFile = args.addresses[game].fileFromRAM(oldVramStart);
            p.removedFiles.push(oldFile.name);
          }

          const fileAddr = args.addresses[game].fileFromROM(gc.effectsSsOvlAddr + effectSsId * 0x1c);
          p.addPatch(fileAddr.name, fileAddr.offset, patch);
        }

        if (type === 0x02) {
          /* Gamemode */
          const id = bufReadU32BE(header, 0x04);
          const ctor = bufReadU32BE(header, 0x08);
          const dtor = bufReadU32BE(header, 0x0c);
          const size = bufReadU32BE(header, 0x10);

          const patch = new Uint8Array(0x30);
          bufWriteU32BE(patch, 0x00, 0);
          bufWriteU32BE(patch, 0x04, vromStart);
          bufWriteU32BE(patch, 0x08, vromEnd);
          bufWriteU32BE(patch, 0x0c, vramStart);
          bufWriteU32BE(patch, 0x10, vramEnd);
          bufWriteU32BE(patch, 0x18, ctor);
          bufWriteU32BE(patch, 0x1c, dtor);
          bufWriteU32BE(patch, 0x2c, size);

          /* Delete the old overlay if possible */
          const base = gc.gamestatesOvlAddr + id * 0x30;
          const oldHeader = rom.subarray(base, base + 0x30);
          const oldVramStart = bufReadU32BE(oldHeader, 0x0c);
          if (oldVramStart !== 0) {
            const oldFile = args.addresses[game].fileFromRAM(oldVramStart);
            p.removedFiles.push(oldFile.name);
          }

          const fileAddr = args.addresses[game].fileFromROM(base);
          p.addPatch(fileAddr.name, fileAddr.offset, patch);
        }
      }

      /* Remove some useless files */
      for (const f of REMOVED_FILES) {
        p.removedFiles.push(f);
      }

      /* Handle symbols */
      const symbols_name = await fileResolver.fetch(`${game}_symbols_name.bin`);
      const symbols_addr = await fileResolver.fetch(`${game}_symbols_addr.bin`);
      const names = new TextDecoder().decode(symbols_name).split(/\0+/);
      names.pop();

      for (let i = 0; i < names.length; i++) {
        const name = names[i];
        const addr = bufReadU32BE(symbols_addr, i * 4);
        const addrs = p.symbols[game].get(name) || [];
        addrs.push(addr);
        p.symbols[game].set(name, addrs);
      }
      const drawHook = p.symbols[game].get('ENEMY_DEBRIS_DRAW_ACTOR_V2');
      if (!drawHook || drawHook.length !== 1) {
        throw new Error(`Missing/ambiguous debris draw hook (${game}); rebuild the payload`);
      }
      const [draw, start, end] = game === 'oot'
        ? [0x80024280, 0x80024784, 0x80024cc4]
        : [0x800b9a04, 0x800b9ef4, 0x800ba660];
      const oldJal = (0x0c000000 | ((draw >>> 2) & 0x03ffffff)) >>> 0;
      const newJal = (0x0c000000 | ((drawHook[0] >>> 2) & 0x03ffffff)) >>> 0;
      const sites = [];
      for (let addr = start; addr < end; addr += 4) {
        const file = args.addresses[game].fileFromRAM(addr);
        if ((bufReadU32BE(rom, file.addr) >>> 0) === oldJal) sites.push(file);
      }
      if (sites.length !== 2) {
        throw new Error(`Expected 2 native Actor_Draw calls in ${game}, got ${sites.length}`);
      }
      for (const site of sites) {
        const instr = new Uint8Array(4);
        bufWriteU32BE(instr, 0, newJal);
        p.addPatch(site.name, site.offset, instr);
        patchedFiles.add(site.name);
      }
    }

    patchRandomizer(world, args.logic, args.settings, p);
    patchEnemies(args.logic.worlds[world].enemyPlan, args.roms, args.addresses, p, args.settings, args.monitor);
    patches.push(p);
  }

  return patches;
};
