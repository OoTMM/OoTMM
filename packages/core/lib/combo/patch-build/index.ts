import { GameAddresses } from '../addresses';
import { CONFIG, GAMES } from '../config';
import { DecompressedRoms } from '../decompress';
import { LogicResult } from '../logic';
import { Monitor } from '../monitor';
import { Settings } from '../settings';
import { Patcher } from './patcher';
import { Patchfile } from './patchfile';
import { patchRandomizer } from './randomizer';
import { PatchGroup } from './group';
import { isEntranceShuffle } from '../logic/helpers';
import { Options } from '../options';
import { World } from '../logic/world';
import { bufReadU32BE, bufWriteU32BE } from '../util/buffer';
import { FileResolver } from '../file-resolver';

export type BuildPatchfileIn = {
  opts: Options;
  patch: Patchfile;
  monitor: Monitor;
  roms: DecompressedRoms;
  addresses: GameAddresses;
  logic: LogicResult;
  settings: Settings;
};

function asmPatchGroups(world: World, settings: Settings) {
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
    MM_BOMBCHU_BAG: settings.bombchuBagMm,
    ER_WALLMASTERS: settings.erWallmasters !== 'none',
    OOT_SHUFFLE_FREE_RUPEES: settings.shuffleFreeRupeesOot !== 'none',
    MM_SHUFFLE_FREE_RUPEES: settings.shuffleFreeRupeesMm !== 'none',
    MM_OPEN_STT: world.resolvedFlags.openDungeonsMm.has('ST'),
    CRIT_WIGGLE_DISABLE: settings.critWiggleDisable,
    OOT_LOST_WOODS_EXITS: settings.alterLostWoodsExits,
    MM_VOID_WARP: settings.voidWarpMm,
    OOT_SWORDLESS_ADULT: settings.swordlessAdult,
    MM_FAST_MASKS: settings.fastMasks,
    OOT_FAST_BOAT: settings.shadowFastBoat,
    MM_WELL_OPEN_OR_REMORSELESS: settings.beneathWell !== 'vanilla',
    MM_JP_LAYOUT_DEKU_PALACE: world.resolvedFlags.jpLayouts.has('DekuPalace'),
    MM_JP_LAYOUT_STONE_TOWER: world.resolvedFlags.jpLayouts.has('StoneTower') && !world.resolvedFlags.openDungeonsMm.has('ST'),
    MM_JP_LAYOUT_STONE_TOWER_TEMPLE: world.resolvedFlags.jpLayouts.has('ST'),
  };
  const keys = Object.keys(groups) as PatchGroup[];
  return keys.filter((k) => groups[k]);
}

const REMOVED_FILES = [
  'mm/ovl_title',
  'mm/ovl_select',
  'mm/ovl_opening',
  'mm/ovl_file_choose',
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
      const patcher = new Patcher(args.opts, game, rom, groups, args.addresses, patches, p);
      const { files } = patcher.run();
      for (const f of files) {
        patchedFiles.add(f);
      }
      /* Pack the payload */
      const payload = await fileResolver.fetch(`${game}_payload.bin`);
      if (payload.length > (game === 'mm' ? 0x50000 : 0x80000)) {
        throw new Error(`Payload too large ${game}`);
      }
      const payloadVrom = game === 'oot' ? 0xf0000000 : 0xf0100000;
      const payloadVram = game === 'oot' ? 0x80400000 : 0x80730000; /* TODO: Codegen this */
      const payloadVramEnd = payloadVram + payload.length;
      p.addNewFile({ name: `${game}/payload`, vrom: payloadVrom, vram: { [game]: [payloadVram, payloadVramEnd] }, data: payload, compressed: false });

      /* Handle extra overlays */
      const allOverlays = await fileResolver.glob(/ovl\/(oot|mm)\/.+\.zovlx$/);
      const overlays = allOverlays.filter((f) => f.startsWith(`ovl/${game}`));
      for (const ov of overlays) {
        /* Resolve and inject the new overlay */
        const raw = await fileResolver.fetch(ov);
        const headerSize = raw.subarray(0, 8);
        const header = raw.subarray(8, 64);
        const data = raw.subarray(64);
        const vramStart = bufReadU32BE(headerSize, 0x00);
        const vramEnd = bufReadU32BE(headerSize, 0x04);
        p.addNewFile({ name: ov, vram: { [game]: [vramStart, vramEnd] }, vrom: ovlAddr, data, compressed: true });
        const vromStart = ovlAddr;
        const vromEnd = ovlAddr + data.length;
        ovlAddr = vromEnd;

        const type = bufReadU32BE(header, 0x00);

        if (type === 0x01) {
          /* Actor */
          const actorId = bufReadU32BE(header, 0x04);
          const vramInit = bufReadU32BE(header, 0x08);

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
          if (oldVramStart !== 0) {
            const oldFile = args.addresses[game].fileFromRAM(oldVramStart);
            p.removedFiles.push(oldFile.name);
          }

          const fileAddr = args.addresses[game].fileFromROM(gc.actorsOvlAddr + actorId * 0x20);
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
    }

    patchRandomizer(world, args.logic, args.settings, p);

    patches.push(p);
  }

  return patches;
};
