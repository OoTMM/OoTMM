import CRC32 from 'crc-32';
import { FILES } from '@ootmm/data';
import { CONFIG } from '../config';
import type { DecompressedRoms } from '../decompress';
import type { Patchfile } from '../patch-build/patchfile';
import type { RomBuilder } from '../rom-builder';
import type { SoulPreviewFragment } from './soul-preview';

type Source = { name: string; vrom: number; data: Uint8Array };

type SourceProof = { name: string; vrom: number; size: number; ranges: { offset: number; size: number; crc32: number }[] };

const MOTH_SOURCE = 'ovl/mm/mm_ovl_En_Tanron1.zovlx';

export function soulPreviewSources(roms: DecompressedRoms, patch: Patchfile): Source[] {
  const rom = roms.oot.rom, view = new DataView(rom.buffer, rom.byteOffset, rom.byteLength);
  const sources = ['objects/gameplay_keep', 'objects/gameplay_dangeon_keep'].map(name => {
    const index = FILES.oot.indexOf(name);
    if (index < 0) throw Error('Missing soul preview keep');
    const at = CONFIG.oot.dmaAddr + index * 16, vrom = view.getUint32(at), end = view.getUint32(at + 4);
    return { name: 'oot/' + name, vrom, data: rom.subarray(vrom, end) };
  });
  const moth = patch.newFiles.find(file => file.name === MOTH_SOURCE);
  if (!moth) throw Error('Missing retained moth source for soul previews');
  sources.push({ name: MOTH_SOURCE, vrom: moth.vrom, data: moth.data });
  return sources;
}

export function storeSoulPreview(data: Uint8Array, fragments: readonly SoulPreviewFragment[],
  sources: readonly Source[], vrom: number, proofs: Map<string, SourceProof>): Uint8Array {
  if (!fragments.length) return data;
  const chunks: { vrom?: number; data?: Uint8Array; size: number }[] = [];
  let cursor = 0;
  for (const fragment of fragments) {
    if (fragment.offset < cursor || !fragment.data.length || fragment.offset + fragment.data.length > data.length ||
      !fragment.data.every((value, i) => value === data[fragment.offset + i])) throw Error('Invalid soul preview fragment');
    if (fragment.offset > cursor) chunks.push({ data: data.subarray(cursor, fragment.offset), size: fragment.offset - cursor });
    let found = false;
    for (const source of sources) {
      const bytes = fragment.data;
      let at = source.data.indexOf(bytes[0]);
      while (at >= 0 && at + bytes.length <= source.data.length) {
        if (bytes.every((value, i) => value === source.data[at + i])) {
          chunks.push({ vrom: source.vrom + at, size: bytes.length });
          const proof = proofs.get(source.name) ?? { name: source.name, vrom: source.vrom, size: source.data.length, ranges: [] };
          if (!proof.ranges.some(r => r.offset === at && r.size === bytes.length))
            proof.ranges.push({ offset: at, size: bytes.length, crc32: CRC32.buf(bytes) >>> 0 });
          proofs.set(source.name, proof);
          found = true;
          break;
        }
        at = source.data.indexOf(bytes[0], at + 1);
      }
      if (found) break;
    }
    if (!found) throw Error('No original ROM source for soul preview fragment');
    cursor = fragment.offset + fragment.data.length;
  }
  if (cursor < data.length) chunks.push({ data: data.subarray(cursor), size: data.length - cursor });
  const headerSize = chunks.length * 8;
  const out = new Uint8Array(headerSize + chunks.reduce((n, chunk) => n + (chunk.data?.length ?? 0), 0));
  const view = new DataView(out.buffer);
  cursor = headerSize;
  chunks.forEach((chunk, i) => {
    view.setUint32(i * 8, chunk.vrom ?? vrom + cursor);
    view.setUint32(i * 8 + 4, chunk.size);
    if (chunk.data) {
      out.set(chunk.data, cursor);
      cursor += chunk.size;
    }
  });
  return out;
}

export function prepareSoulPreviewSources(builder: Pick<RomBuilder, 'fileByNameRequired'>, patch: Patchfile): void {
  const sources: SourceProof[] = patch.meta.enemySoulPreviewSources ?? [];
  const allowed = new Set(['oot/objects/gameplay_keep', 'oot/objects/gameplay_dangeon_keep', MOTH_SOURCE]);
  if (!Array.isArray(sources) || sources.length > allowed.size) throw Error('Invalid soul preview sources');
  const seen = new Set<string>();
  for (const source of sources) {
    if (!source || !allowed.has(source.name) || seen.has(source.name)) throw Error('Invalid soul preview source identity');
    seen.add(source.name);
    const file = builder.fileByNameRequired(source.name);
    if (file.type === 'dummy' || file.alias || file.injected || file.vaddr !== source.vrom || file.data.length !== source.size ||
      !Array.isArray(source.ranges) || !source.ranges.length) throw Error('Soul preview source changed: ' + source.name);
    for (const range of source.ranges) {
      if (!Number.isInteger(range.offset) || !Number.isInteger(range.size) || range.offset < 0 || range.size <= 0 ||
        range.offset + range.size > file.data.length ||
        (CRC32.buf(file.data.subarray(range.offset, range.offset + range.size)) >>> 0) !== range.crc32)
        throw Error('Soul preview source fragment changed: ' + source.name);
    }
    file.type = 'uncompressed';
  }
}
