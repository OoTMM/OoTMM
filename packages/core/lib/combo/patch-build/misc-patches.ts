import { DecompressedRoms } from "../decompress";
import { Patchfile } from "./patchfile";

const BLAST_MASK_COOLDOWN = {
  instant: 0x1,
  veryshort: 0x20,
  short: 0x80,
  default: 0x136,
  long: 0x200,
  verylong: 0x400,
};

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

export function writeBlastMaskCooldown(value: keyof typeof BLAST_MASK_COOLDOWN, patch: Patchfile) {
  const cooldown = Buffer.alloc(2);
  cooldown.writeUInt16BE(BLAST_MASK_COOLDOWN[value]);
  const addr = 0x00CA7F00;
  const offset = 0x002766;
  patch.addPatch('mm', addr + offset, cooldown);
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
     patch.addPatch('mm', codeFileAddress + offset, invertedModifier)
    })
}

export function allowAnywhere(value: string[], patch: Patchfile, roms: DecompressedRoms) {
  if(value.includes('hookshot')) {
    patch.addPatch('mm', 0xD3B220 + 0x810, Buffer.alloc(4));
    patch.addPatch('oot', 0xCADAF8, Buffer.alloc(4));
  }
  if(value.includes('oot-climb')) {
    for (let idx = 0; idx < 32; ++idx) {
      if (idx === 2) {
        continue;
      }
      let wallFlags = roms['oot'].rom.readUint32BE(0xB61F80 + idx*4);
      let b = Buffer.alloc(4);
      b.writeUint32BE(wallFlags | 0x00000008);
      patch.addPatch('oot', 0xB61F80 + idx * 4, b);
    }
  }
}

export function arrowEquipSpeedUp(patch: Patchfile) {
  patch.addPatch('oot', 0xBB84CE, Buffer.alloc(2))
  patch.addPatch('oot', 0xBB84B7, Buffer.from([0xFF]))
  patch.addPatch('oot', 0xBB84CB, Buffer.from([0x01]))
  patch.addPatch('oot', 0xBB7E67, Buffer.from([0x04]))
  patch.addPatch('oot', 0xBB8957, Buffer.from([0x01]))
  patch.addPatch('oot', 0xBB854B, Buffer.from([0x05]))
}

export function bossCutscenesSpeedups(patch: Patchfile) {
    /* Phantom Ganon */
    patch.addPatch('oot', 0xC944D8, Buffer.alloc(4))
    patch.addPatch('oot', 0xC94548, Buffer.alloc(4))
    patch.addPatch('oot', 0xC94730, Buffer.alloc(4))
    patch.addPatch('oot', 0xC945A8, Buffer.alloc(4))
    patch.addPatch('oot', 0xC94594, Buffer.alloc(4))

    /* Twinrova */
    patch.addPatch('oot', 0xD678CC, Buffer.from([0x24, 0x01, 0x03, 0xA2, 0xA6, 0x01, 0x01, 0x42]))
    patch.addPatch('oot', 0xD67BA4, Buffer.from([0x10, 0x00]))

    /* Ganondorf */
    patch.addPatch('oot', 0xD82047, Buffer.from([0x09]))

    /* Ganon battle intro */
    patch.addPatch('oot', 0xE83D28, Buffer.alloc(4))
    patch.addPatch('oot', 0xE83B5C, Buffer.alloc(4))
    patch.addPatch('oot', 0xE84C80, Buffer.from([0x10, 0x00]))
}

// Surprisingly both commented sections set all the fish to weigh 0 pounds.
// Unsure what's going on, but they're both in OOTR for fishing speedup
export function fishingSpeedups(patch: Patchfile) {
    const blockCode = [
        0x3c, 0x0A, 0x80, 0x12, 0x8D, 0x4A, 0xA5, 0xD4, 0x14, 0x0a, 0x00, 0x06,
        0x31, 0x78, 0x00, 0x01, 0x14, 0x18, 0x00, 0x02, 0x3c, 0x18, 0x42, 0x30,
        0x3c, 0x18, 0x42, 0x50, 0x03, 0xe0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
        0x14, 0x18, 0x00, 0x02, 0x3c, 0x18, 0x42, 0x10, 0x3c, 0x18, 0x42, 0x38,
        0x03, 0xe0, 0x00, 0x08
    ]
    // patch.addPatch('oot', 0xDBF434, Buffer.from([0x44, 0x98, 0x90, 0x00, 0xe6, 0x52, 0x01, 0x9C]))
    patch.addPatch('oot', 0x3480C00, Buffer.from(blockCode))
    patch.addPatch('oot', 0xDBF484, Buffer.alloc(4))
    patch.addPatch('oot', 0xDBF4A8, Buffer.alloc(4))
    patch.addPatch('oot', 0xDCBEAA, Buffer.from([0x42, 0x48])) // set adult fish size requirement
    patch.addPatch('oot', 0xDCBF26, Buffer.from([0x42, 0x48])) // set adult fish size requirement
    patch.addPatch('oot', 0xDCBF32, Buffer.from([0x42, 0x30])) // set child fish size requirement
    patch.addPatch('oot', 0xDCBF9E, Buffer.from([0x42, 0x30])) // set child fish size requirement
    // patch.addPatch('oot', 0xDBF428, Buffer.from([0x0C, 0x10, 0x03, 0x00])) // Set Fishing Hook
}

export function easyFishingWithSinkingLure(patch: Patchfile) {
    /* Make sinking lure available immediately */
    patch.addPatch('oot', 0xDC2F00, Buffer.alloc(4))
    patch.addPatch('oot', 0xDC2F10, Buffer.alloc(4))
    /* Don't set sinking lure position after recieving child/adult fishing prizes */
    patch.addPatch('oot', 0xDCC064, Buffer.alloc(4))
    patch.addPatch('oot', 0xDCC06C, Buffer.alloc(4))
    patch.addPatch('oot', 0xDCC12C, Buffer.alloc(4))
    patch.addPatch('oot', 0xDCC134, Buffer.alloc(4))
    /* Give the child/adult fishing prizes even if using the sinking lure */
    patch.addPatch('oot', 0xDCBEBC, Buffer.alloc(4))
    patch.addPatch('oot', 0xDCBEC0, Buffer.alloc(4))
    patch.addPatch('oot', 0xDCBF1C, Buffer.alloc(4))
    patch.addPatch('oot', 0xDCBF20, Buffer.alloc(4))
    /* # Display the normal text when getting the prize, instead of text saying the sinking lure is in violation of the rules */
    patch.addPatch('oot', 0xDCBBDB, Buffer.from([0x86]))

    /* In case 1: of Fishing_UpdateFish, set unk_1A2 = 200 instead of 20000 */
    let b: Buffer = Buffer.alloc(4)
    b.writeUInt32BE(0x240100c8)
    patch.addPatch('oot', 0xDC652C, b) // replace 'mtc1 zero, f10' with 'addiu at, zero, 0x00c8'
    b.writeUInt32BE(0xa6010192)
    patch.addPatch('oot', 0xDC6540, b) // replace 'sh v0, 0x0192(s0)' with 'sh at, 0x0192(s0)'
    b.writeUInt32BE(0xE60601AC)
    patch.addPatch('oot', 0xDC6550, b) // replace 'swc1 f10, 0x01ac(s0)' with 'swc1 f6, 0x01ac(s0)'
}

export function makeGCCheckForFireCompletion(patch: Patchfile) {
    patch.addPatch('oot', 0xED59DC, Buffer.from([0x80, 0xC9, 0x0E, 0xDC]))
}

export function removeOotSceneRestrictions(patch: Patchfile) {
    // Warp Songs
    patch.addPatch('oot', 0xB6D3D2, Buffer.from([0x00])) // Gerudo Training Grounds
    patch.addPatch('oot', 0xB6D42A, Buffer.from([0x00])) // Inside Ganon's Castle

    // Farore's Wind
    patch.addPatch('oot', 0xB6D3D3, Buffer.from([0x00])) // Gerudo Training Grounds
    patch.addPatch('oot', 0xB6D42B, Buffer.from([0x00])) // Inside Ganon's Castle

    // Ocarina
    patch.addPatch('oot', 0xB6D346, Buffer.from([0x11])) // Granny's Shop
    patch.addPatch('oot', 0xB6D33A, Buffer.from([0x51])) // Bombchu Bowling
    patch.addPatch('oot', 0xB6D30A, Buffer.from([0x51])) // Archer
}


export function spawnFortressGateGuard(patch: Patchfile) {
    let new_gate_opening_guard: number[] = [0x0138, 0xFAC8, 0x005D, 0xF448, 0x0000, 0x95B0, 0x0000, 0x0301]
    let guard: Buffer = Buffer.alloc(new_gate_opening_guard.length * 2)
    for (let i = 0; i < new_gate_opening_guard.length; i++) {
        guard.writeUInt16BE(new_gate_opening_guard[i], i*2)
    }
    patch.addPatch('oot', 0x21BD3EC, guard) // Adult Day
    patch.addPatch('oot', 0x21BD62C, guard) // Adult Night
}

export function blueFireArrows(patch: Patchfile) {
    patch.addPatch('oot', 0xC230C1, Buffer.from([0x29])) // Adds AT_TYPE_OTHER to arrows to allow collision with red ice
    // patch.addPatch('oot', 0xDB38FE, Buffer.from([0xEF])) // disables ice arrow collision on secondary cylinder for red ice crystals
    patch.addPatch('oot', 0xC9F036, Buffer.from([0x10])) // enable ice arrow collision on mud walls
    //increase cylinder radius/height for red ice sheets
    patch.addPatch('oot', 0xDB391B, Buffer.from([0x50]))
    patch.addPatch('oot', 0xDB3927, Buffer.from([0x5A]))
}

export function moveSwitches(patch: Patchfile) {
    patch.addPatch('oot', 0x24860A8, Buffer.from([0xFC, 0xF4])) // forest basement 1
    patch.addPatch('oot', 0x24860C8, Buffer.from([0xFC, 0xF4])) // forest basement 2
    patch.addPatch('oot', 0x24860E8, Buffer.from([0xFC, 0xF4])) // forest basement 3
    patch.addPatch('oot', 0x236C148, Buffer.from([0x11, 0x93])) // fire hammer room
}

export function fixIceCavernAlcoveCamera(patch: Patchfile) {
    patch.addPatch('oot', 0x2BECA25, Buffer.from([0x01]))
    patch.addPatch('oot', 0x2BECA2D, Buffer.from([0x01]))
}

export function makeOotCursedSkulltulasPeopleComeDownInstantly(patch: Patchfile) {
    let buf: Buffer = Buffer.alloc(2)
    buf.writeUInt16BE(0x44C8)
    patch.addPatch('oot', 0xEA185A, buf)
}

export function speedupDampeMM(patch: Patchfile) {
    patch.addPatch('mm', 0xFC6760 + 0x1F6C, Buffer.from([
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x24, 0x08, 0x00, 0x08
    ]))
}
