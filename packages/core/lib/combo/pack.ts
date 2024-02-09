
import { FILES } from '@ootmm/data';

import { CONFIG, GAMES, Game } from './config';
import { cosmetics } from './cosmetics';
import { DecompressedRoms } from './decompress';
import { DmaData } from './dma';
import { Monitor } from './monitor';
import { Patchfile } from './patch-build/patchfile';
import { RomBuilder } from './rom-builder';
import { Options } from './options';
import { GameAddresses } from './addresses';

function extractFiles(game: Game, roms: DecompressedRoms, romBuilder: RomBuilder) {
  const config = CONFIG[game];
  const rom = roms[game].rom;
  const compressedDma = new DmaData(roms[game].dma);
  const uncompressedDma = new DmaData(rom.subarray(config.dmaAddr, config.dmaAddr + config.dmaCount * 0x10));

  for (let i = 0; i < config.dmaCount; ++i) {
    const uncompressedEntry = uncompressedDma.read(i);
    const compressedEntry = compressedDma.read(i);
    const name = `${game}/${FILES[game][i]}`;
    if (compressedEntry.physEnd === 0xffffffff) {
      romBuilder.addFile({ type: 'dummy', data: Buffer.alloc(0), name, game, index: i, vaddr: uncompressedEntry.virtStart });
      continue;
    }
    const data = rom.subarray(uncompressedEntry.virtStart, uncompressedEntry.virtEnd);
    const type = compressedEntry.physEnd === 0 ? 'uncompressed' : 'compressed';
    romBuilder.addFile({ type, data, name, game, index: i, vaddr: uncompressedEntry.virtStart });
  }
}

async function injectFirst(game: Game, romBuilder: RomBuilder, count: number) {
  for (let i = 0; i < count; ++i) {
    const file = romBuilder.fileByIndex(game, i);
    await romBuilder.inject(file!);
  }
}

type PackArgs = {
  opts: Options;
  monitor: Monitor;
  roms: DecompressedRoms;
  patchfile: Patchfile;
  addresses: GameAddresses;
};
export async function pack(args: PackArgs) {
  const { monitor, roms, patchfile } = args;
  const romBuilder = new RomBuilder();

  monitor.log("Pack: Building ROM");
  for (const game of GAMES) {
    const dg = roms[game];
    const patches = patchfile.gamePatches[game];

    /* Apply patches */
    for (const patch of patches) {
      patch.data.copy(dg.rom, patch.addr);
    }

    /* Extract files */
    extractFiles(game, roms, romBuilder);
  }

  /* We need to pack a few static files before we can pack the rest */
  await injectFirst('oot', romBuilder, 27);
  await injectFirst('mm', romBuilder, 31);

  /* Add the extra files */
  for (const newFile of patchfile.newFiles) {
    const type = newFile.compressed ? 'compressed' : 'uncompressed';
    const { data } = newFile;
    const vaddr = newFile.vrom;
    let name = newFile.name;
    if (name === null) {
      name = `unk/${vaddr.toString(16)}`;
    }
    romBuilder.addFile({ type, data, name, game: 'custom', vaddr });
  }

  /* Apply cosmetics */
  await cosmetics(args.opts, args.addresses, romBuilder, (patchfile.meta || {}).cosmetics);

  /* Build the final ROM */
  monitor.log("Pack: Finishing up ROM");
  const out = await romBuilder.run();

  return out;
}
