import { OOT_GAMEPLAY_MEMORY } from '../../../../core/src/oot-gameplay-memory-data';
import type { GameAddresses } from '../addresses';
import type { Patchfile } from '../patch-build/patchfile';
import { CONFIG } from '../config';
import { bufReadU16BE, bufReadU32BE } from '../util/buffer';

export function verifyOotGameplayMemory(rom: Uint8Array, addresses: GameAddresses, patch: Patchfile) {
  const policy = OOT_GAMEPLAY_MEMORY.policy;
  const align = (n: number) => Math.ceil(n / 16) * 16;
  const read = (at: number, size: number): Uint8Array => {
    const fresh = patch.newFiles.find(f => at >= f.vrom && at + size <= f.vrom + f.data.length);
    if (fresh)
      return fresh.data.slice(at - fresh.vrom, at - fresh.vrom + size);
    const start = addresses.oot.fileFromROM(at), end = addresses.oot.fileFromROM(at + size - 1);
    if (start.name !== end.name || patch.removedFiles.includes(start.name))
      throw Error('Missing gameplay allocation file');
    const replacement = patch.newFiles.find(f => f.name === start.name);
    const data = replacement ? replacement.data.slice(start.offset, start.offset + size) : rom.slice(at, at + size);
    for (const edit of patch.patches[start.name] ?? []) {
      const lo = Math.max(start.offset, edit.addr), hi = Math.min(start.offset + size, edit.addr + edit.data.length);
      if (lo < hi)
        data.set(edit.data.subarray(lo - edit.addr, hi - edit.addr), lo - start.offset);
    }
    return data;
  };
  const resident = (at: number, size: number) => {
    const file = patch.newFiles.find(f => f.vram?.oot && at >= f.vram.oot[0] && at + size <= f.vram.oot[0] + f.data.length);
    if (!file)
      throw Error('Missing resident gameplay actor profile');
    return file.data.slice(at - file.vram!.oot![0], at - file.vram!.oot![0] + size);
  };
  if (bufReadU32BE(read(0xa87000 + 0x800251a8 - 0x800110a0, 4), 0) !== 0x240424e0)
    throw Error('Unexpected OoT absolute magic-overlay allocation');
  const keepStart = bufReadU32BE(read(CONFIG.oot.objectTableAddr + 8, 8), 0);
  const arrowJoints = read(keepStart + 0x6014, 1)[0] + 1;
  const arrowScratch = 2 * (align(arrowJoints * 6) + 48);
  if (arrowScratch > OOT_GAMEPLAY_MEMORY.actors.find(a => a.id === 0x16)!.extraNativeBytes)
    throw Error('OoT arrow animation tables exceed their gameplay allowance');
  const actors = OOT_GAMEPLAY_MEMORY.actors.map(bound => {
    const entry = read(CONFIG.oot.actorsOvlAddr + bound.id * 32, 32);
    const vrom = bufReadU32BE(entry, 0), lo = bufReadU32BE(entry, 8), hi = bufReadU32BE(entry, 12), profile = bufReadU32BE(entry, 20);
    const data = lo ? read(vrom + profile - lo, 32) : resident(profile, 32);
    const instance = bufReadU32BE(data, 12), object = bufReadU16BE(data, 8), overlay = hi - lo;
    if (instance !== bound.instance || object !== bound.object || overlay > bound.overlay || bufReadU16BE(entry, 28) !== bound.allocationType)
      throw Error(`Refresh OoT gameplay memory bounds after changing ${bound.name}`);
    return { ...bound, overlay, instance, codeCharge: lo ? align(overlay) + 48 : 0, instanceCharge: align(instance) + 48 };
  });
  let effects = 0, largestEffect = 0;
  for (let i = 0; i < CONFIG.oot.effectsSsOvlCount; ++i) {
    const entry = read(CONFIG.oot.effectsSsOvlAddr + i * 28, 28), lo = bufReadU32BE(entry, 8), hi = bufReadU32BE(entry, 12);
    if (lo) {
      const size = align(hi - lo) + 48;
      effects += size;
      largestEffect = Math.max(largestEffect, size);
    }
  }
  const customModels = OOT_GAMEPLAY_MEMORY.customModels.map(bound => {
    const file = patch.newFiles.find(f => f.name === bound.name);
    if (!file || file.data.length > bound.size)
      throw Error(`Refresh OoT gameplay model bound: ${bound.name}`);
    return { ...bound, size: file.data.length, charge: align(file.data.length) + 16 };
  });
  const native = actors.reduce((n, a) => n + a.codeCharge + a.count * (a.instanceCharge + a.extraNativeBytes), effects);
  const custom = customModels.reduce((n, m) => n + m.charge, 0);
  if (native + policy.fragmentationReserve > policy.nativeReserve ||
    custom * policy.customModelCopies + policy.contiguousReserve > policy.customReserve ||
    Math.max(largestEffect, ...actors.map(a => Math.max(a.codeCharge, a.instanceCharge))) > policy.contiguousReserve ||
    actors.reduce((n, a) => n + a.count, 0) > policy.actorSlots ||
    CONFIG.oot.effectsSsOvlCount !== 37)
    throw Error('OoT temporary gameplay allocations exceed protected headroom');
  return { actors, customModels, native, custom, effects, policy };
}
