import { GameAddresses } from "../addresses";
import { BuildOutput } from "../build";
import { CONFIG, CUSTOM_ADDR, GAMES } from "../config";
import { DecompressedRoms } from "../decompress";
import { LogicResult } from "../logic";
import * as miscPatches from "../misc-patches";
import { Monitor } from "../monitor";
import { Settings } from "../settings";
import { Patcher } from "./patcher";
import { Patchfile } from "./patchfile";
import { patchRandomizer } from "./randomizer";

export type BuildPatchfileIn = {
  monitor: Monitor;
  roms: DecompressedRoms;
  addresses: GameAddresses;
  build: BuildOutput;
  custom: Buffer;
  logic: LogicResult;
  settings: Settings;
};

export function buildPatchfile(args: BuildPatchfileIn): Patchfile {
  args.monitor.log("Building Patchfile");
  const file = new Patchfile(args.logic.hash);

  for (const game of GAMES) {
    /* Apply ASM patches */
    const rom = args.roms[game].rom;
    const patcher = new Patcher(game, rom, args.addresses, args.build[game].patches, file);
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

  /* OOT patches */
  miscPatches.arrowEquipSpeedUp(file) // In pause menu, shortens the animation of equipping an elemental arrow
  miscPatches.bossCutscenesSpeedups(file) // Speeds up the intro cutscene of Ganon and the death cutscenes of Phantom Ganon, Twinrova and Ganondorf 
  miscPatches.easyFishingWithSinkingLure(file) // Guarantees the sinking lure and the Hylian Loach to spawn, and allows Link to receive reward despite using sinking lure
  miscPatches.fishingSpeedups(file) // Mostly working... at 2 exceptions. See related function

  miscPatches.makeGCCheckForFireCompletion(file) // Forces Goron to check for Fire Temple completion instead of Fire Medallion
  miscPatches.removeOotSceneRestrictions(file) // Allow Farore's Wind, Warp songs in Gerudo Training Grounds & Inside Ganon's Castle + allow Ocarina usage in a few places  
  miscPatches.spawnFortressGateGuard(file) // Spawns a Gerudo Guard by the gate on Haunted Wasteland side 
  miscPatches.moveSwitches(file) // Lowers Forest Temple Basement switches and Fire Temple Hammer Chest switch by 1 unit
  miscPatches.fixIceCavernAlcoveCamera(file)
  miscPatches.makeOotCursedSkulltulasPeopleComeDownInstantly(file)
  
  // Only missing the proper ASM for the arrows, and we're good to add it as a setting
  // miscPatches.blueFireArrows(file)

  /* MM patches */
  miscPatches.writeBlastMaskCooldown(args.settings.blastMaskCooldown, file) // Blast Mask Cooldown settings
  miscPatches.writeClockSpeed(args.settings.clockSpeed, file) // Clock Speed modifier
  miscPatches.speedupDogRace(file) // Dog Race RNG mostly removed
  miscPatches.speedupDampeMM(file)

  /* Fierce Deity + Hookshot + Climb Anywhere changes */
  let anywhere: string[] = []
  if(args.settings.hookshotAnywhere) {
    anywhere.push('hookshot')
  }
  if(args.settings.fierceDeityAnywhere) {
    anywhere.push('fd')
  }
  if(args.settings.climbMostSurfaces) {
    anywhere.push('oot-climb')
  }
  miscPatches.allowAnywhere(anywhere, file, args.roms)

  /* Patch the randomized data */
  patchRandomizer(args.logic, args.settings, file);

  return file;
};
