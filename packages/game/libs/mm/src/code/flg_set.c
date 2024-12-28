/**
 * File: flg_set.c
 * Description: Event Editor, used to view and edit weekEventReg, eventInf and masksGivenOnMoon flags.
 * Controls:
 *  + Left and Right: select different flags/bits in array element
 *  + Up and Down: select array element (byte) 1 up/down
 *  C Up and Down: select array element (byte) 10 up/down
 *  A: toggle flag
 *  B: exit
 *  Hold Start and press B: clear all weekEventReg and eventInf flags
 */

#include "flg_set.h"

#include "gfxalloc.h"
#include "global.h"

#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

typedef struct FlagSetEntry {
    /* 0x0 */ u8* value;
    /* 0x4 */ const char* name;
} FlagSetEntry; // size = 0x8

static FlagSetEntry sFlagEntries[] = {
    { &gMmSave.saveInfo.weekEventReg[0], "week_event_reg[0]" },
    { &gMmSave.saveInfo.weekEventReg[1], "week_event_reg[1]" },
    { &gMmSave.saveInfo.weekEventReg[2], "week_event_reg[2]" },
    { &gMmSave.saveInfo.weekEventReg[3], "week_event_reg[3]" },
    { &gMmSave.saveInfo.weekEventReg[4], "week_event_reg[4]" },
    { &gMmSave.saveInfo.weekEventReg[5], "week_event_reg[5]" },
    { &gMmSave.saveInfo.weekEventReg[6], "week_event_reg[6]" },
    { &gMmSave.saveInfo.weekEventReg[7], "week_event_reg[7]" },
    { &gMmSave.saveInfo.weekEventReg[8], "week_event_reg[8]" },
    { &gMmSave.saveInfo.weekEventReg[9], "week_event_reg[9]" },
    { &gMmSave.saveInfo.weekEventReg[10], "week_event_reg[10]" },
    { &gMmSave.saveInfo.weekEventReg[11], "week_event_reg[11]" },
    { &gMmSave.saveInfo.weekEventReg[12], "week_event_reg[12]" },
    { &gMmSave.saveInfo.weekEventReg[13], "week_event_reg[13]" },
    { &gMmSave.saveInfo.weekEventReg[14], "week_event_reg[14]" },
    { &gMmSave.saveInfo.weekEventReg[15], "week_event_reg[15]" },
    { &gMmSave.saveInfo.weekEventReg[16], "week_event_reg[16]" },
    { &gMmSave.saveInfo.weekEventReg[17], "week_event_reg[17]" },
    { &gMmSave.saveInfo.weekEventReg[18], "week_event_reg[18]" },
    { &gMmSave.saveInfo.weekEventReg[19], "week_event_reg[19]" },
    { &gMmSave.saveInfo.weekEventReg[20], "week_event_reg[20]" },
    { &gMmSave.saveInfo.weekEventReg[21], "week_event_reg[21]" },
    { &gMmSave.saveInfo.weekEventReg[22], "week_event_reg[22]" },
    { &gMmSave.saveInfo.weekEventReg[23], "week_event_reg[23]" },
    { &gMmSave.saveInfo.weekEventReg[24], "week_event_reg[24]" },
    { &gMmSave.saveInfo.weekEventReg[25], "week_event_reg[25]" },
    { &gMmSave.saveInfo.weekEventReg[26], "week_event_reg[26]" },
    { &gMmSave.saveInfo.weekEventReg[27], "week_event_reg[27]" },
    { &gMmSave.saveInfo.weekEventReg[28], "week_event_reg[28]" },
    { &gMmSave.saveInfo.weekEventReg[29], "week_event_reg[29]" },
    { &gMmSave.saveInfo.weekEventReg[30], "week_event_reg[30]" },
    { &gMmSave.saveInfo.weekEventReg[31], "week_event_reg[31]" },
    { &gMmSave.saveInfo.weekEventReg[32], "week_event_reg[32]" },
    { &gMmSave.saveInfo.weekEventReg[33], "week_event_reg[33]" },
    { &gMmSave.saveInfo.weekEventReg[34], "week_event_reg[34]" },
    { &gMmSave.saveInfo.weekEventReg[35], "week_event_reg[35]" },
    { &gMmSave.saveInfo.weekEventReg[36], "week_event_reg[36]" },
    { &gMmSave.saveInfo.weekEventReg[37], "week_event_reg[37]" },
    { &gMmSave.saveInfo.weekEventReg[38], "week_event_reg[38]" },
    { &gMmSave.saveInfo.weekEventReg[39], "week_event_reg[39]" },
    { &gMmSave.saveInfo.weekEventReg[40], "week_event_reg[40]" },
    { &gMmSave.saveInfo.weekEventReg[41], "week_event_reg[41]" },
    { &gMmSave.saveInfo.weekEventReg[42], "week_event_reg[42]" },
    { &gMmSave.saveInfo.weekEventReg[43], "week_event_reg[43]" },
    { &gMmSave.saveInfo.weekEventReg[44], "week_event_reg[44]" },
    { &gMmSave.saveInfo.weekEventReg[45], "week_event_reg[45]" },
    { &gMmSave.saveInfo.weekEventReg[46], "week_event_reg[46]" },
    { &gMmSave.saveInfo.weekEventReg[47], "week_event_reg[47]" },
    { &gMmSave.saveInfo.weekEventReg[48], "week_event_reg[48]" },
    { &gMmSave.saveInfo.weekEventReg[49], "week_event_reg[49]" },
    { &gMmSave.saveInfo.weekEventReg[50], "week_event_reg[50]" },
    { &gMmSave.saveInfo.weekEventReg[51], "week_event_reg[51]" },
    { &gMmSave.saveInfo.weekEventReg[52], "week_event_reg[52]" },
    { &gMmSave.saveInfo.weekEventReg[53], "week_event_reg[53]" },
    { &gMmSave.saveInfo.weekEventReg[54], "week_event_reg[54]" },
    { &gMmSave.saveInfo.weekEventReg[55], "week_event_reg[55]" },
    { &gMmSave.saveInfo.weekEventReg[56], "week_event_reg[56]" },
    { &gMmSave.saveInfo.weekEventReg[57], "week_event_reg[57]" },
    { &gMmSave.saveInfo.weekEventReg[58], "week_event_reg[58]" },
    { &gMmSave.saveInfo.weekEventReg[59], "week_event_reg[59]" },
    { &gMmSave.saveInfo.weekEventReg[60], "week_event_reg[60]" },
    { &gMmSave.saveInfo.weekEventReg[61], "week_event_reg[61]" },
    { &gMmSave.saveInfo.weekEventReg[62], "week_event_reg[62]" },
    { &gMmSave.saveInfo.weekEventReg[63], "week_event_reg[63]" },
    { &gMmSave.saveInfo.weekEventReg[64], "week_event_reg[64]" },
    { &gMmSave.saveInfo.weekEventReg[65], "week_event_reg[65]" },
    { &gMmSave.saveInfo.weekEventReg[66], "week_event_reg[66]" },
    { &gMmSave.saveInfo.weekEventReg[67], "week_event_reg[67]" },
    { &gMmSave.saveInfo.weekEventReg[68], "week_event_reg[68]" },
    { &gMmSave.saveInfo.weekEventReg[69], "week_event_reg[69]" },
    { &gMmSave.saveInfo.weekEventReg[70], "week_event_reg[70]" },
    { &gMmSave.saveInfo.weekEventReg[71], "week_event_reg[71]" },
    { &gMmSave.saveInfo.weekEventReg[72], "week_event_reg[72]" },
    { &gMmSave.saveInfo.weekEventReg[73], "week_event_reg[73]" },
    { &gMmSave.saveInfo.weekEventReg[74], "week_event_reg[74]" },
    { &gMmSave.saveInfo.weekEventReg[75], "week_event_reg[75]" },
    { &gMmSave.saveInfo.weekEventReg[76], "week_event_reg[76]" },
    { &gMmSave.saveInfo.weekEventReg[77], "week_event_reg[77]" },
    { &gMmSave.saveInfo.weekEventReg[78], "week_event_reg[78]" },
    { &gMmSave.saveInfo.weekEventReg[79], "week_event_reg[79]" },
    { &gMmSave.saveInfo.weekEventReg[80], "week_event_reg[80]" },
    { &gMmSave.saveInfo.weekEventReg[81], "week_event_reg[81]" },
    { &gMmSave.saveInfo.weekEventReg[82], "week_event_reg[82]" },
    { &gMmSave.saveInfo.weekEventReg[83], "week_event_reg[83]" },
    { &gMmSave.saveInfo.weekEventReg[84], "week_event_reg[84]" },
    { &gMmSave.saveInfo.weekEventReg[85], "week_event_reg[85]" },
    { &gMmSave.saveInfo.weekEventReg[86], "week_event_reg[86]" },
    { &gMmSave.saveInfo.weekEventReg[87], "week_event_reg[87]" },
    { &gMmSave.saveInfo.weekEventReg[88], "week_event_reg[88]" },
    { &gMmSave.saveInfo.weekEventReg[89], "week_event_reg[89]" },
    { &gMmSave.saveInfo.weekEventReg[90], "week_event_reg[90]" },
    { &gMmSave.saveInfo.weekEventReg[91], "week_event_reg[91]" },
    { &gMmSave.saveInfo.weekEventReg[92], "week_event_reg[92]" },
    { &gMmSave.saveInfo.weekEventReg[93], "week_event_reg[93]" },
    { &gMmSave.saveInfo.weekEventReg[94], "week_event_reg[94]" },
    { &gMmSave.saveInfo.weekEventReg[95], "week_event_reg[95]" },
    { &gMmSave.saveInfo.weekEventReg[96], "week_event_reg[96]" },
    { &gMmSave.saveInfo.weekEventReg[97], "week_event_reg[97]" },
    { &gMmSave.saveInfo.weekEventReg[98], "week_event_reg[98]" },
    { &gMmSave.saveInfo.weekEventReg[99], "week_event_reg[99]" },

    { &gSaveContext.eventInf[0], "event_inf[0]" },
    { &gSaveContext.eventInf[1], "event_inf[1]" },
    { &gSaveContext.eventInf[2], "event_inf[2]" },
    { &gSaveContext.eventInf[3], "event_inf[3]" },
    { &gSaveContext.eventInf[4], "event_inf[4]" },
    { &gSaveContext.eventInf[5], "event_inf[5]" },
    { &gSaveContext.eventInf[6], "event_inf[6]" },
    { &gSaveContext.eventInf[7], "event_inf[7]" },

    { &gSaveContext.masksGivenOnMoon[0], "mask_mask_bit[0]" },
    { &gSaveContext.masksGivenOnMoon[1], "mask_mask_bit[1]" },
    { &gSaveContext.masksGivenOnMoon[2], "mask_mask_bit[2]" },

    { NULL, NULL }, // used in the code to detect array end
};

static s32 sEntryIndex = 0;
static u32 sCurrentBit = 0;
static s32 sTimer = 0;

void FlagSet_Update(GameState* gameState) {
    PlayState* play = (PlayState*)gameState;
    Input* input = CONTROLLER1(&play->state);

    /* Intra-byte navigation */

    if (CHECK_BTN_ALL(input->press.button, BTN_DLEFT)) {
        sCurrentBit++;
        sTimer = 10;
    }

    if (CHECK_BTN_ALL(input->press.button, BTN_DRIGHT)) {
        sCurrentBit--;
        sTimer = 10;
    }

    if (sTimer == 0) {
        if (CHECK_BTN_ALL(input->cur.button, BTN_DLEFT)) {
            sCurrentBit++;
            sTimer = 2;
        }
        if (CHECK_BTN_ALL(input->cur.button, BTN_DRIGHT)) {
            sCurrentBit--;
            sTimer = 2;
        }
    }

    // Wrap cursor position at beginning/end of line
    sCurrentBit %= 8;

    /* Navigation between bytes */

    // + Up/Down scroll 1 at a time
    if (CHECK_BTN_ALL(input->press.button, BTN_DUP)) {
        sEntryIndex--;
        if (sEntryIndex < 0) {
            sEntryIndex = 0;
        }
        sTimer = 10;
    }

    if (CHECK_BTN_ALL(input->press.button, BTN_DDOWN)) {
        sEntryIndex++;
        if (sFlagEntries[sEntryIndex].value == NULL) { // End of array
            sEntryIndex--;
        }
        sTimer = 10;
    }

    // C Up/Down scroll 10 at a time
    if (CHECK_BTN_ALL(input->press.button, BTN_CUP)) {
        sEntryIndex -= 10;
        if (sEntryIndex < 0) {
            sEntryIndex = 0;
        }
        sTimer = 10;
    }

    if (CHECK_BTN_ALL(input->press.button, BTN_CDOWN)) {
        sEntryIndex += 10;
        if (sEntryIndex > 100) {
            sEntryIndex = 100;
        }
        sTimer = 10;
    }

    if (sTimer == 0) {
        if (CHECK_BTN_ALL(input->cur.button, BTN_DUP)) {
            sEntryIndex--;
            if (sEntryIndex < 0) {
                sEntryIndex = 0;
            }
            sTimer = 2;

        } else if (CHECK_BTN_ALL(input->cur.button, BTN_DDOWN)) {
            sEntryIndex++;
            if (sFlagEntries[sEntryIndex].value == NULL) { // End of array
                sEntryIndex--;
            }
            sTimer = 2;

        } else if (CHECK_BTN_ALL(input->cur.button, BTN_CUP)) {
            sEntryIndex -= 10;
            if (sEntryIndex < 0) {
                sEntryIndex = 0;
            }
            sTimer = 2;

        } else if (CHECK_BTN_ALL(input->cur.button, BTN_CDOWN)) {
            sEntryIndex += 10;
            if (sEntryIndex > 100) {
                sEntryIndex = 100;
            }
            sTimer = 2;
        }
    }

    /* Other controls */

    // A toggles the selected flag
    if (CHECK_BTN_ALL(input->press.button, BTN_A)) {
        *sFlagEntries[sEntryIndex].value ^= (1 << sCurrentBit);
    }

    if (sTimer != 0) {
        sTimer--;
    }

    // Hold Start and press B will reset the first two flag arrays
    if (CHECK_BTN_ALL(input->cur.button, BTN_START)) {
        if (CHECK_BTN_ALL(input->press.button, BTN_B)) {
            s16 i;
            for (i = 0; i < ARRAY_COUNT(gMmSave.saveInfo.weekEventReg); i++) {
                gMmSave.saveInfo.weekEventReg[i] = 0;
            }
            for (i = 0; i < ARRAY_COUNT(gSaveContext.eventInf); i++) {
                gSaveContext.eventInf[i] = 0;
            }
        }

        // Pressing B will exit
    } else if (CHECK_BTN_ALL(input->press.button, BTN_B)) {
        play->pauseCtx.debugEditor = DEBUG_EDITOR_NONE;
    }
}

s32 sFlagBitIndex;

void FlagSet_Draw(GameState* gameState) {
    GraphicsContext* gfxCtx = gameState->gfxCtx;
    Gfx* gfx;
    Gfx* polyOpa;

    OPEN_DISPS(gfxCtx);
    GfxPrint printer;
    s32 pad;

    polyOpa = POLY_OPA_DISP;
    gfx = Gfx_Open(polyOpa);
    gSPDisplayList(OVERLAY_DISP++, gfx);

    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer, gfx);
    GfxPrint_SetColor(&printer, 250, 50, 50, 255);
    GfxPrint_SetPos(&printer, 8, 13);
    GfxPrint_Printf(&printer, sFlagEntries[sEntryIndex].name);
    GfxPrint_SetPos(&printer, 12, 15);

    // Print the flag bits in the current byte, largest to smallest
    for (sFlagBitIndex = 7; sFlagBitIndex >= 0; sFlagBitIndex--) {
        // Highlight current flag bit in white, rest in grey
        if (sFlagBitIndex == sCurrentBit) {
            GfxPrint_SetColor(&printer, 200, 200, 200, 255);
        } else {
            GfxPrint_SetColor(&printer, 100, 100, 100, 255);
        }

        // Display 1 if flag set and 0 if not
        if (*sFlagEntries[sEntryIndex].value & (1 << sFlagBitIndex)) {
            GfxPrint_Printf(&printer, "1");
        } else {
            GfxPrint_Printf(&printer, "0");
        }

        // Add a space after each group of 4
        if ((sFlagBitIndex % 4) == 0) {
            GfxPrint_Printf(&printer, " ");
        }
    }

    gfx = GfxPrint_Close(&printer);
    GfxPrint_Destroy(&printer);

    gSPEndDisplayList(gfx++);
    Gfx_Close(polyOpa, gfx);
    POLY_OPA_DISP = gfx;

    CLOSE_DISPS(gfxCtx);
}

static const s32 sPad[5] = { 0 };
