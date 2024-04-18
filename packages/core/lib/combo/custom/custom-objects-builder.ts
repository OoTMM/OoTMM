import { FILES } from '@ootmm/data';
import { Game } from '../config';
import { DecompressedRoms } from '../decompress';
import { DmaData } from '../dma';
import { arrayToIndexMap } from '../util';
import { ObjectEditor, ObjectEditorOut } from './object-editor';

const FILES_TO_INDEX = {
  oot: arrayToIndexMap(FILES.oot),
  mm: arrayToIndexMap(FILES.mm),
};

export type CustomObject = {
  name: string;
  data: Buffer;
  offsets: number[];
};

export class CustomObjectsBuilder {
  constructor(
    private readonly roms: DecompressedRoms,
  ){
  }

  private async getFile(game: Game, filename: string) {
    const rom = this.roms[game].rom;
    const dma = new DmaData(this.roms[game].dma);
    const index = FILES_TO_INDEX[game][filename];
    if (index === undefined) {
      throw new Error(`File ${filename} not found in game ${game}`);
    }
    const dmaEntry = dma.read(index);
    return Buffer.from(rom.subarray(dmaEntry.virtStart, dmaEntry.virtEnd));
  }

  private async simpleExtract(name: string, game: Game, file: string, offsets: number[], segIn: number, segOut: number) {
    const data = await this.getFile(game, file);
    const editor = new ObjectEditor(segOut);
    editor.loadSegment(segIn, data);
    for (const offset of offsets) {
      const list = editor.listData(offset)!;
      editor.submitList(list);
    }
    const out = editor.build();
    return { name, data: out.data, offsets: out.offsets };
  }

  private async makeEqMasterSword(): Promise<CustomObject> {
    const editor = new ObjectEditor(0xa);
    const object_link_boy = await this.getFile('oot', 'objects/object_link_boy');
    editor.loadSegment(0x06, object_link_boy);

    let ms = editor.listData(0x06021f78)!;
    ms = editor.stripList(ms, 0x2d8, 0x518);

    editor.submitList(ms);
    return { name: 'EQ_MASTER_SWORD', ...editor.build() };
  }

  private async makeEqHammer(): Promise<CustomObject> {
    const editor = new ObjectEditor(0xa);
    const object_link_boy = await this.getFile('oot', 'objects/object_link_boy');
    editor.loadSegment(0x06, object_link_boy);

    const b = 0x060233e0;
    let ms = editor.listData(b)!;
    ms = editor.stripList(ms, 0x06023660 - b, 0x060238C0 - b);

    editor.submitList(ms);
    return { name: 'EQ_HAMMER', ...editor.build() };
  }

  async build(): Promise<CustomObject[]> {
    return [
      await this.makeEqMasterSword(),
      await this.makeEqHammer(),
      //await this.simpleExtract('LIMB_OOT_CHILD_LHAND_CLOSED', 'oot', 'objects/object_link_child', [], 0x06, 0x0a),
    ];
  }
};
