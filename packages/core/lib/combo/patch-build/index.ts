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
import { FileResolver, Options } from '../options';
import { World } from '../logic/world';

export type BuildPatchfileIn = {
  opts: Options;
  patch: Patchfile;
  monitor: Monitor;
  roms: DecompressedRoms;
  addresses: GameAddresses;
  logic: LogicResult;
  settings: Settings;
  resolver: FileResolver;
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
    OOT_SHUFFLE_GRASS: settings.shuffleGrassOot,
    MM_SHUFFLE_GRASS: settings.shuffleGrassMm,
    MM_LENIENT_SPIKES: settings.lenientSpikes,
    OOT_CHEST_GAME_SHUFFLE: settings.smallKeyShuffleChestGame !== 'vanilla',
    MM_CLIMB_MOST_SURFACES: settings.climbMostSurfacesMm,
    NO_BROKEN_ACTORS: !settings.restoreBrokenActors,
    OOT_FISHING_POND_SHUFFLE: settings.pondFishShuffle,
    OOT_NOT_FISHING_POND_SHUFFLE: !settings.pondFishShuffle,
    MM_BOMBCHU_BAG: settings.bombchuBagMm,
    ER_WALLMASTERS: settings.erWallmasters !== 'none',
    OOT_SHUFFLE_FREE_RUPEES: settings.shuffleFreeRupeesOot,
    MM_SHUFFLE_FREE_RUPEES: settings.shuffleFreeRupeesMm,
    MM_OPEN_STT: world.resolvedFlags.openDungeonsMm.has('ST'),
    CRIT_WIGGLE_DISABLE: settings.critWiggleDisable,
    OOT_LOST_WOODS_EXITS: settings.alterLostWoodsExits,
    MM_VOID_WARP: settings.voidWarpMm,
    OOT_SWORDLESS_ADULT: settings.swordlessAdult,
    MM_FAST_MASKS: settings.fastMasks,
    OOT_FAST_BOAT: settings.shadowFastBoat,
  };
  const keys = Object.keys(groups) as PatchGroup[];
  return keys.filter((k) => groups[k]);
}

export async function buildPatchfiles(args: BuildPatchfileIn): Promise<Patchfile[]> {
  args.monitor.log("Building Patchfile");
  const patches: Patchfile[] = [];
  const patchedFiles = new Set<string>();
  let ovlAddr = 0xe0000000;

  for (let world = 0; world < args.settings.players; ++world) {
    const p = args.patch.dup();
    const groups = asmPatchGroups(args.logic.worlds[world], args.settings);
    const meta: any = {};

    for (const game of GAMES) {
      const gc = CONFIG[game];

      /* Apply ASM patches */
      const rom = args.roms[game].rom;
      const patches = await args.resolver.fetch(`${game}_patch.bin`);
      const patcher = new Patcher(args.opts, game, rom, groups, args.addresses, patches, p);
      const { files } = patcher.run();
      for (const f of files) {
        patchedFiles.add(f);
      }

      /* Pack the payload */
      const payload = await args.resolver.fetch(`${game}_payload.bin`);
      if (payload.length > (game === 'mm' ? 0x50000 : 0x80000)) {
        throw new Error(`Payload too large ${game}`);
      }
      p.addNewFile(`${game}/payload`, game === 'oot' ? 0xf0000000 : 0xf0100000, payload, false);

      /* Handle extra overlays */
      const allOverlays = await args.resolver.glob(/ovl\/(oot|mm)\/.+\.zovlx$/);
      const overlays = allOverlays.filter((f) => f.startsWith(`ovl/${game}`));
      for (const ov of overlays) {
        /* Resolve and inject the new overlay */
        const raw = await args.resolver.fetch(ov);
        const header = raw.subarray(0, 0x10);
        const data = raw.subarray(0x10);
        p.addNewFile(ov, ovlAddr, data, true);
        const vromStart = ovlAddr;
        const vromEnd = ovlAddr + data.length;
        ovlAddr = vromEnd;

        /* Parse the header */
        const actorId = header.readUInt32BE(0x00);
        const vramInit = header.readUInt32BE(0x04);
        const vramStart = header.readUInt32BE(0x08);
        const vramEnd = header.readUInt32BE(0x0c);

        const patch = Buffer.alloc(4 * 6);
        patch.writeUInt32BE(vromStart,  0x00);
        patch.writeUInt32BE(vromEnd,    0x04);
        patch.writeUInt32BE(vramStart,  0x08);
        patch.writeUInt32BE(vramEnd,    0x0c);
        patch.writeUInt32BE(0,          0x10);
        patch.writeUInt32BE(vramInit,   0x14);

        /* Delete the old overlay if possible */
        const oldHeader = rom.subarray(gc.actorsOvlAddr + actorId * 0x20, gc.actorsOvlAddr + actorId * 0x20 + 0x20);
        const oldVramStart = oldHeader.readUInt32BE(0x08);
        if (oldVramStart !== 0) {
          const oldFile = args.addresses[game].fileFromRAM(oldVramStart);
          p.removedFiles.push(oldFile.name);
        }

        const fileAddr = args.addresses[game].fileFromROM(gc.actorsOvlAddr + actorId * 0x20);
        p.addPatch(fileAddr.name, fileAddr.offset, patch);
      }

      /* Handle cosmetics */
      const gameCosmetics: {[k: string]: number[]} = {};
      meta.cosmetics = meta.cosmetics || {};
      meta.cosmetics[game] = gameCosmetics;
      const cosmetic_name = await args.resolver.fetch(`${game}_cosmetic_name.bin`);
      const cosmetic_addr = await args.resolver.fetch(`${game}_cosmetic_addr.bin`);
      const names = cosmetic_name.toString('utf-8').split(/\0+/);
      names.pop();

      for (let i = 0; i < names.length; i++) {
        const name = names[i];
        const addr = cosmetic_addr.readUint32BE(i * 4);
        if (gameCosmetics[name] === undefined) {
          gameCosmetics[name] = [];
        }
        gameCosmetics[name].push(addr);
      }
    }

    patchRandomizer(world, args.logic, args.settings, p);
    p.setMeta(meta);

    patches.push(p);
  }

  return patches;
};
