import { Patchfile } from "./patchfile";

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
