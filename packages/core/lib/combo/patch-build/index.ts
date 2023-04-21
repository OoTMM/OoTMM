import { GameAddresses } from "../addresses";
import { BuildOutput } from "../build";
import { CONFIG, CUSTOM_ADDR, GAMES } from "../config";
import { DecompressedRoms } from "../decompress";
import { LogicResult } from "../logic";
import { writeBlastMaskCooldown } from "../misc-patches";
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

  /* Blast Mask Cooldown*/
  writeBlastMaskCooldown(args.settings.blastMaskCooldown, file)

  /* Patch the randomized data */
  patchRandomizer(args.logic, args.settings, file);

  return file;
};
