import { Patchfile } from "./patch-build/patchfile";

const validBlastMaskCooldowns: {[k: string]: number} = {
    "instant": 0x1,
    "veryshort": 0x20,
    "short": 0x80,
    "default": 0x136,
    "long": 0x200,
    "verylong": 0x400
}

export function writeBlastMaskCooldown(value: string, patch: Patchfile) {
    const cooldown = Buffer.alloc(2);
    cooldown.writeInt16BE(validBlastMaskCooldowns[value])
    const addr = 0x00CA7F00
    const offset = 0x002766
    patch.addPatch('mm', addr + offset, cooldown)
}