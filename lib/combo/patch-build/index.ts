import { BuildOutput } from "../build";
import { CONFIG, CUSTOM_ADDR, GAMES } from "../config";
import { Monitor } from "../monitor";
import { Patcher } from "./patcher";
import { Patchfile } from "./patchfile";

export type RomsDMA = {
  oot: Buffer;
  mm: Buffer;
};

export type BuildPatchfileIn = {
  monitor: Monitor;
  rom: Buffer;
  dma: RomsDMA;
  build: BuildOutput;
  custom: Buffer;
};

export function buildPatchfile(args: BuildPatchfileIn): Patchfile {
  args.monitor.log("Building Patchfile");
  const file = new Patchfile();

  for (const game of GAMES) {
    /* Apply ASM patches */
    const patcher = new Patcher(game, args.rom, args.build[game].patches, file);
    patcher.run();

    /* Pack the payload */
    const payload = args.build[game].payload;
    if (payload.length > 0x30000) {
      throw new Error("Payload too large");
    }
    file.addPatch('post-compress', CONFIG[game].payloadAddr, payload);
  }

  /* Pack the custom data */
  if (args.custom.length > 0x20000) {
    throw new Error("Custom data too large");
  }
  file.addPatch('post-compress', CUSTOM_ADDR, args.custom);

  /* Patch rom header */
  file.addPatch('post-compress', 0x20, Buffer.from('OOT+MM COMBO       '));
  file.addPatch('post-compress', 0x3c, Buffer.from('ZZE'));

  return file;
};
