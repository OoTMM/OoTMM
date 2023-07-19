import { GameAddresses } from "../addresses";
import { BuildOutput } from "../build";
import { CONFIG, CUSTOM_ADDR, GAMES } from "../config";
import { DecompressedRoms } from "../decompress";
import { LogicResult } from "../logic";
import { Monitor } from "../monitor";
import { Settings } from "../settings";
import { Patcher } from "./patcher";
import { Patchfile } from "./patchfile";
import { patchRandomizer } from "./randomizer";
import { PatchGroup } from "./group";
import { isEntranceShuffle } from "../logic/helpers";

export type BuildPatchfileIn = {
  monitor: Monitor;
  roms: DecompressedRoms;
  addresses: GameAddresses;
  build: BuildOutput;
  custom: Buffer;
  logic: LogicResult;
  settings: Settings;
};

function asmPatchGroups(settings: Settings) {
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
    OOT_AGELESS_CHILD_TRADE: settings.agelessChildTrade,
    OOT_BLUE_FIRE_ARROWS: settings.blueFireArrows,
    ER: isEntranceShuffle(settings),
    DEFAULT_HOLD: settings.defaultHoldTarget,
    OOT_SILVER_RUPEE_SHUFFLE: settings.silverRupeeShuffle !== 'vanilla',
    OOT_FREE_SCARECROW: settings.freeScarecrowOot,
  };
  const keys = Object.keys(groups) as PatchGroup[];
  return keys.filter((k) => groups[k]);
}

export function buildPatchfiles(args: BuildPatchfileIn): Patchfile[] {
  args.monitor.log("Building Patchfile");
  const file = new Patchfile(args.logic.hash);
  const groups = asmPatchGroups(args.settings);

  for (const game of GAMES) {
    /* Apply ASM patches */
    const rom = args.roms[game].rom;
    const patcher = new Patcher(game, rom, groups, args.addresses, args.build[game].patches, file);
    patcher.run();

    /* Pack the payload */
    const payload = args.build[game].payload;
    if (payload.length > 0x40000) {
      throw new Error("Payload too large");
    }
    file.addPatch('global', CONFIG[game].payloadAddr, payload);
  }

  /* Pack the custom data */
  if (args.custom.length > 0x40000) {
    throw new Error("Custom data too large");
  }
  file.addPatch('global', CUSTOM_ADDR, args.custom);

  /* Patch rom header */
  file.addPatch('global', 0x20, Buffer.from('OOT+MM COMBO       '));
  file.addPatch('global', 0x3c, Buffer.from('ZZE'));

  /* Patch the randomized data */
  const patches: Patchfile[] = [];
  for (let world = 0; world < args.settings.players; ++world) {
    const p = file.dup();
    patchRandomizer(world, args.logic, args.settings, p);
    patches.push(p);
  }

  return patches;
};
