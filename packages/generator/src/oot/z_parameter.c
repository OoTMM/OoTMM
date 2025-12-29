#include <combo.h>
#include <combo/magic.h>
#include <combo/inventory.h>

void Magic_FlashMeterBorder(void)
{
    s16* sMagicBorderR = (s16*)0x800F7508;
    s16* sMagicBorderG = (s16*)0x800F750C;
    s16* sMagicBorderB = (s16*)0x800F7510;
    s16* sMagicBorderRatio = (s16*)0x800F75B0;
    s16* sMagicBorderStep = (s16*)0x800F75B4;
    s16* sMagicBorderIndices = (s16*)0x800F75A8;
    s16 (*sMagicBorderColors)[3] = (s16(*)[3])0x800F7590;

    s16 borderChangeR;
    s16 borderChangeG;
    s16 borderChangeB;
    s16 index = sMagicBorderIndices[*sMagicBorderStep];

    borderChangeR = ABS_ALT(*sMagicBorderR - sMagicBorderColors[index][0]) / *sMagicBorderRatio;
    borderChangeG = ABS_ALT(*sMagicBorderG - sMagicBorderColors[index][1]) / *sMagicBorderRatio;
    borderChangeB = ABS_ALT(*sMagicBorderB - sMagicBorderColors[index][2]) / *sMagicBorderRatio;

    if (*sMagicBorderR >= sMagicBorderColors[index][0])
    {
        *sMagicBorderR -= borderChangeR;
    }
    else
    {
        *sMagicBorderR += borderChangeR;
    }

    if (*sMagicBorderG >= sMagicBorderColors[index][1])
    {
        *sMagicBorderG -= borderChangeG;
    }
    else
    {
        *sMagicBorderG += borderChangeG;
    }

    if (*sMagicBorderB >= sMagicBorderColors[index][2])
    {
        *sMagicBorderB -= borderChangeB;
    }
    else
    {
        *sMagicBorderB += borderChangeB;
    }

    *sMagicBorderRatio = *sMagicBorderRatio - 1;
    if (*sMagicBorderRatio == 0)
    {
        *sMagicBorderR = sMagicBorderColors[index][0];
        *sMagicBorderG = sMagicBorderColors[index][1];
        *sMagicBorderB = sMagicBorderColors[index][2];

        *sMagicBorderRatio = YREG(40 + *sMagicBorderStep);

        *sMagicBorderStep = *sMagicBorderStep + 1;
        if (*sMagicBorderStep >= 4)
        {
            *sMagicBorderStep = 0;
        }
    }
}

s32 Magic_Decrement(void)
{
    if (comboIsChateauActive())
    {
        return 0;
    }

    gSaveContext.save.info.playerData.magic--;
    return 1;
}

s32 Magic_RequestChangeCustom(PlayState* play, s16 amount, s16 type)
{
    if (type != MAGIC_CONSUME_WAIT_PREVIEW && type != MAGIC_ADD && comboIsChateauActive())
    {
        amount = 0;
    }
    return Magic_RequestChange(play, amount, type);
}

void Magic_UpdateCustom(PlayState* play)
{
    if (comboIsChateauActive())
    {
        /* R_MAGIC_FILL_COLOR(0) = 0; // Red */
        R_MAGIC_FILL_COLOR(1) = 0; /* Green */
        R_MAGIC_FILL_COLOR(2) = 200; /* Blue */
        Magic_FlashMeterBorder();
        switch (gSaveContext.magicState) {
            case MAGIC_STATE_CONSUME:
            case MAGIC_STATE_METER_FLASH_1:
            case MAGIC_STATE_METER_FLASH_2:
            case MAGIC_STATE_METER_FLASH_3:
                return;
        }
    }
    else
    {
        /* R_MAGIC_FILL_COLOR(0) = 0; // Red */
        R_MAGIC_FILL_COLOR(1) = 200; /* Green */
        R_MAGIC_FILL_COLOR(2) = 0; /* Blue */
    }
    Magic_Update(play);
}

PATCH_CALL(0x80832578, Magic_RequestChangeCustom) /* Player - Magic Arrows */
// PATCH_CALL(0x808FF258, Magic_RequestChangeCustom) /* Spin Attack - Weird Check in Initializer */

PATCH_CALL(0x80078B78, Magic_UpdateCustom)

s32 Interface_CustomIsRestrictionCategoryAll(u8 item)
{
    if (item != ITEM_OOT_OCARINA_FAIRY &&
        item != ITEM_OOT_OCARINA_TIME &&
        !comboIsTradeBottleOot(item) &&
        !(item >= ITEM_OOT_WEIRD_EGG && item <= ITEM_OOT_CLAIM_CHECK))
    {
        return 1;
    }
    return 0;
}

s32 Interface_CustomIsRestrictionCategoryAll2(u8 item)
{
    if (item != ITEM_OOT_SPELL_FIRE &&
        item != ITEM_OOT_HOOKSHOT &&
        item != ITEM_OOT_LONGSHOT &&
        item != ITEM_OOT_SPELL_WIND &&
        item != ITEM_OOT_SPELL_LOVE &&
        item != ITEM_OOT_OCARINA_FAIRY &&
        item != ITEM_OOT_OCARINA_TIME &&
        !comboIsTradeBottleOot(item) &&
        !(item >= ITEM_OOT_WEIRD_EGG && item <= ITEM_OOT_CLAIM_CHECK))
    {
        return 1;
    }
    return 0;
}

s32 Interface_CustomIsRestrictionCategoryBottle(u8 item)
{
    if (comboIsTradeBottleOot(item))
    {
        return 1;
    }
    return 0;
}

const static u8* gAmmoDigit0Tex = (u8*)0x020035c0;

void Interface_CustomDrawAmmoCount(PlayState* play, s16 button, s16 alpha) {
    s16 i;
    s16 ammo;
    s16 maxAmmo;

    OPEN_DISPS(play->state.gfxCtx);

    i = gSaveContext.save.info.equips.buttonItems[button];

    switch (i)
    {
    case ITEM_OOT_POWDER_KEG:
        ammo = gOotExtraAmmo.kegAmmo;
        maxAmmo = 1;
        break;
    case ITEM_OOT_BOMBCHU_10:
        ammo = gSave.info.inventory.ammo[ITS_OOT_BOMBCHU];
        maxAmmo = gMaxBombchuOot;
        break;
    case ITEM_OOT_BOMB_MM:
        ammo = gOotExtraAmmo.mmBombAmmo;
        maxAmmo = kMaxBombs[gOotExtraItems.mmBombBagUpgrade];
        break;
    default:
        return;
    }

    gDPPipeSync(OVERLAY_DISP++);

    if (button == 0 && play->sceneId == SCE_OOT_BOMBCHU_BOWLING_ALLEY && Flags_GetSwitch(play, 0x38)) {
        ammo = play->bombchuBowlingStatus;
        if (ammo < 0) {
            ammo = 0;
        }
    } else if (ammo == maxAmmo) {
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 255, 0, alpha);
    }

    if (ammo == 0) {
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 100, 100, 100, alpha);
    }

    i = 0;
    while (ammo >= 10) {
        i++;
        ammo -= 10;
    }

    if (i != 0) {
        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, ((u8*)gAmmoDigit0Tex + ((8 * 8) * i)), 8, 8,
                                        R_ITEM_AMMO_X(button), R_ITEM_AMMO_Y(button), 8, 8, 1 << 10, 1 << 10);
    }

    OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, ((u8*)gAmmoDigit0Tex + ((8 * 8) * ammo)), 8, 8,
                                    R_ITEM_AMMO_X(button) + 6, R_ITEM_AMMO_Y(button), 8, 8, 1 << 10, 1 << 10);

    CLOSE_DISPS();
}

void Interface_DrawAmmoCountWrapper(PlayState* play, s16 button, s16 alpha)
{
    s16 item = gSaveContext.save.info.equips.buttonItems[button];
    switch (item)
    {
    case ITEM_OOT_POWDER_KEG:
    case ITEM_OOT_BOMBCHU_10:
    case ITEM_OOT_BOMB_MM:
        Interface_CustomDrawAmmoCount(play, button, alpha);
        break;
    default:
        Interface_DrawAmmoCount(play, button, alpha);
        break;
    }
}

PATCH_CALL(0x80075ea0, Interface_DrawAmmoCountWrapper)
PATCH_CALL(0x800761c0, Interface_DrawAmmoCountWrapper)
PATCH_CALL(0x80076284, Interface_DrawAmmoCountWrapper)
PATCH_CALL(0x80076348, Interface_DrawAmmoCountWrapper)
