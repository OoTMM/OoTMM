import { GameAddresses } from "../addresses";
import { BuildOutput } from "../build";
import { GAMES } from "../config";
import { DecompressedRoms } from "../decompress";
import { LogicResult } from "../logic";
import { Monitor } from "../monitor";
import { Settings } from "../settings";
import { Patcher } from "./patcher";
import { Patchfile } from "./patchfile";
import { patchRandomizer } from "./randomizer";
import { PatchGroup } from "./group";
import { isEntranceShuffle } from "../logic/helpers";
import { Options } from "../options";
import { World } from "../logic/world";

export type BuildPatchfileIn = {
  opts: Options;
  patch: Patchfile;
  monitor: Monitor;
  roms: DecompressedRoms;
  addresses: GameAddresses;
  build: BuildOutput;
  logic: LogicResult;
  settings: Settings;
};

function asmPatchGroups(world: World, settings: Settings) {
  const groups: {[k in PatchGroup]: boolean} = {
    OOT_HOOKSHOT_ANYWHERE: settings.hookshotAnywhereOot,
    OOT_CLIMB_ANYWHERE: settings.climbMostSurfacesOot,
    MM_FD_ANYWHERE: settings.fierceDeityAnywhere,
    MM_HOOKSHOT_ANYWHERE: settings.hookshotAnywhereMm,
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
    OOT_ADULT_DEKU_TREE: settings.dekuTreeAdult,
    OOT_FIRE_TEMPLE_CHILD: settings.fireChild,
    OOT_FAST_BUNNY_HOOD: settings.fastBunnyHood,
    OOT_AGELESS_SWORDS: settings.agelessSwords,
    OOT_AGELESS_SHIELDS: settings.agelessShields,
    OOT_AGELESS_TUNICS: settings.agelessTunics,
    OOT_AGELESS_BOOTS: settings.agelessBoots,
    OOT_AGELESS_STICKS: settings.agelessSticks,
    OOT_AGELESS_BOOMERANG: settings.agelessBoomerang,
    OOT_AGELESS_HAMMER: settings.agelessHammer,
    OOT_AGELESS_HOOKSHOT: settings.agelessHookshot,
    OOT_AGELESS_CHILD_TRADE: settings.agelessChildTrade,
    OOT_BLUE_FIRE_ARROWS: settings.blueFireArrows,
    OOT_SUNLIGHT_ARROWS: settings.sunlightArrows,
    ER: isEntranceShuffle(settings),
    DEFAULT_HOLD: settings.defaultHoldTarget,
    OOT_SILVER_RUPEE_SHUFFLE: settings.silverRupeeShuffle !== 'vanilla',
    OOT_FREE_SCARECROW: settings.freeScarecrowOot,
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
  };
  const keys = Object.keys(groups) as PatchGroup[];
  return keys.filter((k) => groups[k]);
}

export function buildPatchfiles(args: BuildPatchfileIn): Patchfile[] {
  args.monitor.log("Building Patchfile");
  const patches: Patchfile[] = [];

  for (let world = 0; world < args.settings.players; ++world) {
    const p = args.patch.dup();
    const groups = asmPatchGroups(args.logic.worlds[world], args.settings);
    const meta: any = {};

    for (const game of GAMES) {
      /* Apply ASM patches */
      const rom = args.roms[game].rom;
      const patcher = new Patcher(args.opts, game, rom, groups, args.addresses, args.build[game].patches, p);
      patcher.run();

      /* Pack the payload */
      const payload = args.build[game].payload;
      if (payload.length > (game === 'mm' ? 0x40000 : 0x40000)) {
        throw new Error(`Payload too large ${game}`);
      }
      p.addNewFile(`${game}/payload`, game === 'oot' ? 0xf0000000 : 0xf0100000, payload, false);

      /* Handle cosmetics */
      const gameCosmetics: {[k: string]: number[]} = {};
      meta.cosmetics = meta.cosmetics || {};
      meta.cosmetics[game] = gameCosmetics;
      const { cosmetic_name, cosmetic_addr } = args.build[game];
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
