
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

/* Files to alias (will use the OoT version) */
const ALIASES_OOT = [
  'icon_item_gameover_static',
  'message_texture_static',
  'objects/object_wallmaster',
  'objects/object_firefly',
  'objects/object_dodongo',
  'objects/object_niw',
  'objects/object_tite',
  'objects/object_peehat',
  'objects/object_zl1',
  'objects/object_bubble',
  'objects/object_bombf',
  'objects/object_dekubaba',
  'objects/object_b_heart',
  'objects/object_Bb',
  'objects/object_hata',
  'objects/object_vm',
  'objects/object_efc_star_field',
  'objects/object_efc_tw',
  'objects/object_ru2',
  'objects/object_gi_purse',
  'objects/object_rr',
  'objects/object_mastergolon',
  'objects/object_masterzoora',
  'objects/object_cne',
  'objects/object_bob',
  'objects/object_yabusame_point',
  'objects/object_d_hsblock',
  'objects/object_toryo',
  'objects/object_ms',
  'objects/object_hs',
  'objects/object_lightswitch',
  'objects/object_kusa',
  'objects/object_tsubo',
  'objects/object_fu',
  'objects/object_gi_soldout',
  'objects/object_zg',
  'objects/object_ds2',
  'objects/object_bigokuta',
  'objects/object_hintnuts',
  'objects/object_gs',
  'objects/object_crow',
  'objects/object_cow',
  'misc/elf_message_field',
  'misc/elf_message_ydan',
  'dummy/bump_texture_static',
];

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
  await injectFirst('oot', romBuilder, 6);
  await injectFirst('mm', romBuilder,  6);

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

  /* Alias everything */
  for (const a of ALIASES_OOT) {
    romBuilder.alias(`oot/${a}`, `mm/${a}`);
  }

  /* Apply cosmetics */
  await cosmetics(args.opts, args.addresses, romBuilder, (patchfile.meta || {}).cosmetics);

  /* Build the final ROM */
  monitor.log("Pack: Finishing up ROM");
  const { rom, size } = await romBuilder.run();
  const sizeMB = (size / 1024 / 1024).toFixed(2);
  monitor.debug(`Pack: ROM size: ${sizeMB}MiB`);

  return rom;
}
