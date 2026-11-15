import { mat4, vec3 } from 'gl-matrix';
import { SOUL_PREVIEW_MODELS } from './soul-preview-descriptors';
import { CONFIG } from '../config';
import type { DecompressedRoms } from '../decompress';

export type SoulPreviewFragment = { offset: number; data: Uint8Array };

type Point = [number, number, number];

type Model = {
  game: string; id: number; name: string; object: number; segment: number; scale: number;
  skeletons: { offset: number; flex: boolean; name: string }[];
  animations: { offset: number; name: string }[];
  lists: { offset: number; name: string }[];
  textureSegments?: Record<string, number | undefined>;
  textures?: { offset: number; size: number }[];
  setup?: number;
};

const align = (n: number) => (n + 15) & ~15;

const TRANSPARENCY = 220;

export function translucentCombine(w0: number, w1: number): [number, number] {

  w0 = (w0 & ~0x1ff) | (15 << 5) | 31;
  w1 = (w1 & ~((15 << 24) | (7 << 21) | (7 << 18) | (7 << 6) | (7 << 3) | 7)) |
    (15 << 24) | (5 << 18) | (7 << 3) | 7;
  return [w0 >>> 0, w1 >>> 0];
}

export function bakeSoulPreview(object: Uint8Array, model: Model,
  addressBase = model.segment * 0x1000000, keep?: Uint8Array, fragments?: SoulPreviewFragment[]): Uint8Array {
  const data = new DataView(object.buffer, object.byteOffset, object.byteLength);
  const local = (p: number, size: number) => {
    if (!Number.isInteger(size) || size < 0 || p < addressBase || p - addressBase + size > object.length)
      throw Error('Preview pointer outside object');
    return p - addressBase;
  };
  const matrices: mat4[] = [], draws: { list: number; matrix: number }[] = [];
  const skeleton = model.skeletons[0];
  if (skeleton) {
    const header = skeleton.offset, count = data.getUint8(header + 4);
    const limbs = local(data.getUint32(header), count * 4);
    if (!count || count > 128) throw Error('Invalid preview skeleton');
    const pose: Point[] = Array.from({ length: count + 1 }, () => [0, 0, 0]);
    const animation = model.animations[0];
    if (animation) {
      const h = animation.offset, values = local(data.getUint32(h + 4), 2);
      const indices = local(data.getUint32(h + 8), (count + 1) * 6);
      for (let i = 0; i <= count; ++i) for (let axis = 0; axis < 3; ++axis) {
        const at = values + data.getUint16(indices + i * 6 + axis * 2) * 2;
        if (at + 2 > object.length) throw Error('Invalid preview pose');
        pose[i][axis] = data.getInt16(at);
      }
    }
    const visited = new Set<number>();
    function limb(i: number, parent: mat4) {
      if (i === 255) return;
      if (i >= count || visited.has(i)) throw Error('Invalid preview limb tree');
      visited.add(i);
      const at = local(data.getUint32(limbs + i * 4), 12), m = mat4.clone(parent);
      const pos: Point = i === 0 ? pose[0] : [data.getInt16(at), data.getInt16(at + 2), data.getInt16(at + 4)];
      mat4.translate(m, m, pos);
      mat4.rotateZ(m, m, pose[i + 1][2] * Math.PI / 32768);
      mat4.rotateY(m, m, pose[i + 1][1] * Math.PI / 32768);
      mat4.rotateX(m, m, pose[i + 1][0] * Math.PI / 32768);
      const dl = data.getUint32(at + 8);
      if (dl) {
        draws.push({ list: local(dl, 8), matrix: matrices.length });
        matrices.push(m);
      }
      limb(data.getUint8(at + 6), m);
      limb(data.getUint8(at + 7), parent);
    }
    limb(0, mat4.create());
  } else {
    const base = mat4.create();

    if (model.name === 'En_Tanron1') mat4.rotateX(base, base, Math.PI / 2);
    matrices.push(base);
    if (model.name === 'En_Rr') {

      for (let i = 1; i <= 4; ++i) matrices.push(mat4.fromTranslation(mat4.create(), [0, i * 1000, 0]));
    }
  }
  const embedded = model.object <= 3 || addressBase >= 0x80000000;
  type Program = { words: number[]; copies: { at: number; source: Uint8Array; offset: number; size: number }[]; triangles: number; minY: number };
  function program(listOffset: number, initialMatrix: number, setup = model.setup): Program {
    const result: Program = { words: [], copies: [], triangles: 0, minY: Infinity };
    let instructions = 0, currentMatrix = initialMatrix;
    const cache: (Point | undefined)[] = [];
    const emit = (w0: number, w1: number) => result.words.push(w0 >>> 0, w1 >>> 0);
    const reference = (op: number, source: Uint8Array, offset: number, size: number, copy: boolean) => {
      if (offset < 0 || offset + size > source.length) throw Error('Invalid preview data reference');
      emit(op, copy ? 0 : 0x06000000 + offset);
      if (copy) result.copies.push({ at: result.words.length - 1, source, offset, size });
    };
    function list(offset: number, depth = 0) {
      if (depth > 16) throw Error('Preview display list recursion');
      for (let at = offset; at + 8 <= object.length; at += 8) {
        if (++instructions > 100000) throw Error('Preview display list cycle');
        const w0 = data.getUint32(at), w1 = data.getUint32(at + 4), op = w0 >>> 24;
        if (op === 0xdf) return;
        if (op === 0xde) {
          if (w1 >= addressBase && w1 < addressBase + object.length) list(local(w1, 8), depth + 1);
          else if ((w1 >>> 24) < 7 || (w1 >>> 24) > 12 || (w1 & 0xffffff) > 0x20)
            throw Error('Unresolved preview display list');

          if (w0 & 0x10000) return;
        } else if (op === 0xda) {
          if ((w1 >>> 24) === 13) currentMatrix = (w1 & 0xffffff) / 64;
          else if (model.name === 'En_Rr' && (w1 >>> 24) === 12) currentMatrix = 1 + (w1 & 0xffffff) / 64;
          else if ((w1 >>> 24) === 1 || ['En_Ba', 'En_Bx'].includes(model.name)) continue;
          else throw Error('Unresolved preview matrix');
          if (!matrices[currentMatrix]) throw Error('Invalid preview matrix index');
          emit(0xda380003, 0x0d000000 + currentMatrix * 64);
        } else if (op === 1) {
          const count = (w0 >>> 12) & 255, first = ((w0 >>> 1) & 127) - count;
          const p = local(w1, count * 16);
          if (!count || first < 0 || first + count > 32) throw Error('Preview vertex cache overflow');
          reference(w0, object, p, count * 16, embedded);
          for (let i = 0; i < count; ++i) {
            const v = vec3.fromValues(data.getInt16(p + i * 16), data.getInt16(p + i * 16 + 2), data.getInt16(p + i * 16 + 4));
            vec3.transformMat4(v, v, matrices[currentMatrix]);
            cache[first + i] = [...v] as Point;
          }
        } else if (op === 5 || op === 6 || op === 7) {
          for (const word of op === 5 ? [w0] : [w0, w1]) {
            const tri = [(word >>> 16) & 255, (word >>> 8) & 255, word & 255].map(i => cache[i / 2]);
            if (tri.some(v => !v)) throw Error('Preview triangle references unset vertex');
            for (const v of tri) result.minY = Math.min(result.minY, v![1]);
            ++result.triangles;
          }
          emit(w0, w1);
        } else if (op === 0xfd) {
          let ptr = w1;
          const seg = ptr >>> 24;
          if (model.textureSegments?.[seg] !== undefined) ptr = addressBase + model.textureSegments[seg]! + (ptr & 0xffffff);
          if (ptr >= addressBase && ptr < addressBase + object.length) {
            const p = local(ptr, 8);
            let size = model.textures?.find(t => t.offset === p)?.size;

            if (!size) for (let next = at + 8; next < Math.min(at + 80, object.length - 7); next += 8) {
              const a = data.getUint32(next), b = data.getUint32(next + 4), cmd = a >>> 24;
              if (cmd === 0xf3) {
                size = (((b >>> 12) & 4095) + 1) * (1 << ((w0 >>> 19) & 3)) / 2;
                break;
              }
              if (cmd === 0xf0) {
                size = (((b >>> 14) & 1023) + 1) * 2;
                break;
              }
            }
            reference(w0, object, p, size ?? 8, embedded);
          } else if (seg === 4 && keep) {
            const p = ptr & 0xffffff;
            let size = 0;
            for (let next = at + 8; next < Math.min(at + 80, object.length - 7); next += 8) {
              const a = data.getUint32(next), b = data.getUint32(next + 4);
              if (a >>> 24 === 0xf3) {
                size = (((b >>> 12) & 4095) + 1) * (1 << ((w0 >>> 19) & 3)) / 2;
                break;
              }
            }
            if (!size) throw Error('Unbounded keep texture');
            reference(w0, keep, p, size, true);
          } else throw Error('Unresolved preview texture 0x' + w1.toString(16));
        } else if (op === 2) {
          const vertex = (w0 & 65535) / 2, field = (w0 >>> 16) & 255;
          if (!cache[vertex] || ![0x10, 0x14].includes(field)) throw Error('Invalid preview vertex modification');
          emit(w0, w1);
        }
        else if (op === 0xfc) emit(...translucentCombine(w0, w1));
        else if (op === 0xfb) emit(w0, (w1 & 0xffffff00) | TRANSPARENCY);
        else if (op === 0xe2) { }
        else if (op === 0xe3) {

          const length = (w0 & 255) + 1, shift = 32 - ((w0 >>> 8) & 255) - length;
          if (shift === 20 && length === 2) continue;
          emit(w0, w1);
        } else if (op === 0xd9) emit(w0, w1 & ~0x10000);
        else if ([0xd7, 0xe6, 0xe7, 0xe8, 0xe9, 0xf0, 0xf2, 0xf3, 0xf4, 0xf5, 0xf8, 0xfa].includes(op)) emit(w0, w1);
        else if ([0x00, 0x03].includes(op)) { }
        else throw Error('Unsafe preview opcode 0x' + op.toString(16));
      }
      throw Error('Unterminated preview display list');
    }
    emit(0xda380003, 0x0d000000 + initialMatrix * 64);
    if (setup !== undefined) list(setup);
    list(listOffset);
    return result;
  }
  let programs: Program[], alternate: Program[] = [];
  if (model.name === 'En_Mkk') {

    const part = (name: string) => {
      const entry = model.lists.find(list => list.name === name);
      if (!entry) throw Error('Missing Boe preview component ' + name);
      return entry.offset;
    };
    const boe = (color: string) => [
      program(part(`g${color}BoeBodyModelDL`), 0, part(`g${color}BoeBodyMaterialDL`)),
      program(part(`g${color}BoeEyesDL`), 0),
    ];
    programs = boe('Black');
    alternate = boe('White');
  }
  else if (draws.length) programs = draws.map(d => program(d.list, d.matrix));
  else {
    let best: Program | undefined;
    for (const dl of model.lists) {
      try {
        const p = program(dl.offset, 0);
        if (p.triangles > (best?.triangles ?? 0)) best = p;
      } catch (e) {
        if (['En_Rr', 'Arwing'].includes(model.name)) throw e;
      }
    }
    if (!best) throw Error('No valid preview geometry');
    programs = [best];
  }
  const allPrograms = [...programs, ...alternate];
  const triangles = programs.reduce((n, p) => n + p.triangles, 0), minY = Math.min(...allPrograms.map(p => p.minY));
  if (!triangles || !Number.isFinite(minY) || !model.scale) throw Error('Empty or zero-scale preview');
  const copies: { offset: number; data: Uint8Array }[] = [];
  let cursor = 32 + matrices.length * 64;
  const copied = new Map<string, number>();
  for (const p of allPrograms) for (const c of p.copies) {
    const key = (c.source === object ? 'o' : 'k') + '/' + c.offset + '/' + c.size;
    let offset = copied.get(key);
    if (offset === undefined) {
      offset = align(cursor);
      copies.push({ offset, data: c.source.subarray(c.offset, c.offset + c.size) });
      cursor = offset + c.size;
      copied.set(key, offset);
    }
    p.words[c.at] = 0x0b000000 + offset;
  }
  const dl = align(cursor);
  let words = programs.flatMap(p => p.words);
  words.push(0xdf000000, 0);
  let alternateDl = alternate.length ? dl + words.length * 4 : 0;
  if (alternate.length) words.push(...alternate.flatMap(p => p.words), 0xdf000000, 0);
  if (model.name === 'En_Mkk') {
    type Command = [number, number];
    const commands = (p: Program): Command[] => Array.from({ length: p.words.length / 2 }, (_, i) => [p.words[i * 2], p.words[i * 2 + 1]]);
    const same = (a: readonly Command[], b: readonly Command[]) => JSON.stringify(a) === JSON.stringify(b);
    const require = (condition: boolean) => {
      if (!condition) throw Error('Unexpected Boe material layout');
    };
    const body = (p: Program) => {
      const c = commands(p), texture = c.findIndex(c => c[0] >>> 24 === 0xfd), color = c.findIndex(c => c[0] >>> 24 === 0xfa);
      require(texture >= 0 && color >= 0 && c[c.length - 3][0] >>> 24 === 0xd9 && c[c.length - 2][0] >>> 24 === 1 && c[c.length - 1][0] >>> 24 === 5);
      const prefix = c.slice(0, texture + 1);

      if (color > texture) prefix.splice(prefix.length - 1, 0, c[color]);
      require(prefix.every(c => [0xda, 0xd7, 0xe7, 0xfc, 0xe3, 0xfa, 0xfd].includes(c[0] >>> 24)));
      let synced = false;
      const setup = prefix.filter(c => {
        if (c[0] >>> 24 !== 0xe7) return true;
        if (synced) return false;
        synced = true;
        return true;
      });

      require(synced);
      const load = c.slice(texture + 1, c.length - 3).filter(c => c[0] >>> 24 !== 0xfa);
      return { setup, load: [...load, c[c.length - 3]], vertex: c[c.length - 2], triangle: c[c.length - 1] };
    };
    const black = body(programs[0]), white = body(alternate[0]);
    require(same(black.load, white.load) && same([black.triangle], [white.triangle]));
    const eyes = commands(programs[1]), whiteEyes = commands(alternate[1]);
    require(eyes.length === whiteEyes.length);
    for (let i = 0; i < eyes.length; i++) {
      if (eyes[i][0] >>> 24 === 1) {
        require(eyes[i][0] === whiteEyes[i][0] && (eyes[i][1] >>> 24) === 6 && (whiteEyes[i][1] >>> 24) === 6);
        const size = ((eyes[i][0] >>> 12) & 255) * 16;
        const a = local(eyes[i][1], size), b = local(whiteEyes[i][1], size);
        require(object.subarray(a, a + size).every((value, j) => value === object[b + j]));
      } else require(same([eyes[i]], [whiteEyes[i]]));
    }

    const tail = eyes.filter(c => {
      if (![0xda, 0xd7, 0xe3].includes(c[0] >>> 24)) return true;
      require(black.setup.some(p => same([p], [c])) && white.setup.some(p => same([p], [c])));
      return false;
    });
    const entryBytes = (b: typeof black) => (b.setup.length + 3) * 8;
    alternateDl = dl + entryBytes(black);
    const loadAt = alternateDl + entryBytes(white), tailAt = loadAt + (black.load.length + 1) * 8;
    const call = (offset: number, branch = false): Command => [branch ? 0xde010000 : 0xde000000, 0x0b000000 + offset];
    const end: Command = [0xdf000000, 0];
    const entry = (b: typeof black) => [...b.setup, call(loadAt), b.vertex, call(tailAt, true)];
    words = [...entry(black), ...entry(white), ...black.load, end, black.triangle, ...tail, end].flat();
  }
  const out = new Uint8Array(dl + words.length * 4), view = new DataView(out.buffer);
  view.setUint32(0, 0x53504d32);
  view.setUint32(4, dl);
  view.setUint32(8, triangles);
  view.setUint16(12, embedded ? 0 : model.object);
  view.setUint16(14, matrices.length);
  view.setFloat32(16, model.scale);
  view.setFloat32(20, minY);
  view.setUint32(24, model.game === 'mm' ? 1 : 0);
  const billboard = ['En_Bubble', 'En_Mkk', 'En_Tanron1'].includes(model.name);
  view.setUint32(28, ((billboard ? 0x80000000 : 0) | alternateDl) >>> 0);
  matrices.forEach((m, i) => Array.from(m).forEach((n, j) => view.setFloat32(32 + i * 64 + j * 4, n)));
  for (const c of copies) out.set(c.data, c.offset);
  fragments?.push(...copies);
  words.forEach((word, i) => view.setUint32(dl + i * 4, word));
  return out;
}

export function buildSoulPreviewMeshes(roms: DecompressedRoms) {
  const meshes = [];
  const failures = [];
  const objectBytes = (game: 'oot' | 'mm', id: number) => {
    const rom = roms[game].rom, view = new DataView(rom.buffer, rom.byteOffset, rom.byteLength), at = CONFIG[game].objectTableAddr + id * 8;
    return rom.subarray(view.getUint32(at), view.getUint32(at + 4));
  };
  for (const model of SOUL_PREVIEW_MODELS) {
    const game = model.game as 'oot' | 'mm';
    try {
      const fragments: SoulPreviewFragment[] = [];
      const data = bakeSoulPreview(objectBytes(game, model.object), model, undefined, objectBytes(game, 1), fragments);
      meshes.push({ game, id: model.id, name: model.name, data, fragments });
    }
    catch (e) {
      failures.push(`${game}/${model.name}: ${e}`);
    }
  }
  const rom = roms.mm.rom, view = new DataView(rom.buffer, rom.byteOffset, rom.byteLength);
  const at = CONFIG.mm.actorsOvlAddr + 0x23d * 32, start = view.getUint32(at), end = view.getUint32(at + 4), base = view.getUint32(at + 8);
  try {
    const fragments: SoulPreviewFragment[] = [];
    const data = bakeSoulPreview(rom.subarray(start, end), { game: 'mm', id: 0x23d, name: 'En_Tanron1', object: 1, segment: 128, scale: 1.2, skeletons: [], animations: [], lists: [{ offset: 0x1900, name: 'moth wings' }], setup: 0x1888 }, base, undefined, fragments);
    meshes.push({ game: 'mm', id: 0x23d, name: 'En_Tanron1', data, fragments });
  } catch (e) {
    failures.push('mm/En_Tanron1: ' + e);
  }
  return { meshes, failures };
}
