import { DecompressedRoms } from "./decompress";
import { Patchfile } from "./patch-build/patchfile";

const validBlastMaskCooldowns: {[k: string]: number} = {
    "instant": 0x1,
    "veryshort": 0x20,
    "short": 0x80,
    "default": 0x136,
    "long": 0x200,
    "verylong": 0x400
}

function bufferFromShort(value: number) {
    return Buffer.from([value >>> 8, value & 0xFF]);
 }

const validClockSpeeds: {[k: string]: [speed: number, invertedModifier: number]} = {
    "veryslow":  [1, 0],
    "slow":      [2, 1],
    "default":   [3, -2],
    "fast":      [6, -4],
    "veryfast":  [9, -6],
    "superfast": [18, -12],
}

export function writeBlastMaskCooldown(value: string, patch: Patchfile) {
    const cooldown = Buffer.alloc(2);
    cooldown.writeUInt16BE(validBlastMaskCooldowns[value])
    const addr = 0x00CA7F00
    const offset = 0x002766
    patch.addPatch('mm', addr + offset, cooldown)
}

export function writeClockSpeed(value: string, patch: Patchfile) {
    const codeFileAddress = 0xB3C000;
    const hackAddressOffset = 0x8A674;
    const sp = validClockSpeeds[value][0]
    const im = validClockSpeeds[value][1]
    const invertedModifier = bufferFromShort(im)
    const hackFixClockSpeed = Buffer.from([
        0x3C, 0x01, 0x00, 0x01, 0x14, 0xE2, 0x00, 0x02, 0x00, 0x26, 0x08, 0x21, 0x24, 0x02, 0x00, 0x00, 
        0x10, 0x40, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x24, 0x02, 0x00,  sp , 0xA4, 0x22, 0x70, 0x06
    ])
    patch.addPatch('mm', codeFileAddress+hackAddressOffset, hackFixClockSpeed)
    const invertedModifierOffsets = [0xb1b8e, 0x7405e]
    invertedModifierOffsets.forEach(offset => {
     patch.addPatch('mm', codeFileAddress+offset, invertedModifier)   
    })
}

export function allowAnywhere(value: string[], patch: Patchfile, roms: DecompressedRoms) {
    if(value.includes('fd')) {
        patch.addPatch('mm', 0xB3C000 + 0x6B6D1, Buffer.from([0x00]))
        patch.addPatch('mm', 0xF4C720 + 0xC84, Buffer.from([0x54, 0x41, 0x00, 0x2B, 0x24, 0x06, 0x00, 0x01]))
        patch.addPatch('mm', 0xE765A0 + 0x42B2, Buffer.from([0xCA, 0x84]))
        patch.addPatch('mm', 0xE765A0 + 0x42B6, Buffer.from([0xCA, 0x84]))
        patch.addPatch('mm', 0x10630F0 + 0x1D0, Buffer.from([0x28, 0x61, 0x00, 0x02, 0x10, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xA4, 0x87, 0x02, 0xC8]))
        patch.addPatch('mm', 0xE379E0 + 0xAF8, Buffer.from([0x28, 0x41, 0x00, 0x02, 0x14, 0x20]))
        patch.addPatch('mm', 0x10698B0 + 0x2B0, Buffer.from([0x24, 0x08, 0x00, 0x02]))
        patch.addPatch('mm', 0x10698B0 + 0x2BC, Buffer.concat([ 
            Buffer.from([0x28, 0x41, 0x00, 0x03, 0x01, 0x01, 0x40, 0x22, 0xA4, 0xE8, 0x02, 0xC2, 0xA4, 0xF9, 0x02, 0xCE]), 
            Buffer.alloc(52)
        ]))
        patch.addPatch('mm', 0x10684B0 + 0x4B0, Buffer.alloc(4))
        patch.addPatch('mm', 0x26FC000 + 0x8FF6, Buffer.from([0x00, 0xA9]))
        patch.addPatch('mm', 0x26FC000 + 0x8FFC, Buffer.from([0x00, 0xA9]))
    }
    
    if(value.includes('hookshot')) {
        patch.addPatch('mm', 0xD3B220 + 0x810, Buffer.alloc(4))
        patch.addPatch('oot', 0xCADAF8, Buffer.alloc(4))
    }
    if(value.includes('oot-climb')) {
        for(let idx = 0; idx < 32; idx++) {
            if(idx===2) {
                continue
            }
            let wallFlags = roms['oot'].rom.readUint32BE(0xB61F80 + idx*4)
            let b = Buffer.alloc(4)
            b.writeUint32BE(wallFlags | 0x00000008)
            patch.addPatch('oot', 0xB61F80 + idx*4, b)
        }
    }
}