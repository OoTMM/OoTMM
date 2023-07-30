#include <combo.h>

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

s32 Magic_ChateauActive()
{
    return comboConfig(CFG_SHARED_MAGIC) && MM_GET_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI);
}

s32 Magic_Decrement()
{
    if (Magic_ChateauActive())
    {
        return 0;
    }
    gSaveContext.save.playerData.magicAmount--;
    return 1;
}

s32 Magic_RequestChangeCustom(GameState_Play* play, s16 amount, s16 type)
{
    if (type != MAGIC_CONSUME_WAIT_PREVIEW && type != MAGIC_ADD && Magic_ChateauActive())
    {
        amount = 0;
    }
    return Magic_RequestChange(play, amount, type);
}

void Magic_UpdateCustom(GameState_Play* play)
{
    if (Magic_ChateauActive())
    {
        // R_MAGIC_FILL_COLOR(0) = 0; // Red
        R_MAGIC_FILL_COLOR(1) = 0; // Green
        R_MAGIC_FILL_COLOR(2) = 200; // Blue
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
        // R_MAGIC_FILL_COLOR(0) = 0; // Red
        R_MAGIC_FILL_COLOR(1) = 200; // Green
        R_MAGIC_FILL_COLOR(2) = 0; // Blue
    }
    Magic_Update(play);
}

PATCH_CALL(0x80832578, Magic_RequestChangeCustom) // Player - Magic Arrows
PATCH_CALL(0x808FF258, Magic_RequestChangeCustom) // Spin Attack - Weird Check in Initializer

PATCH_CALL(0x80078B78, Magic_UpdateCustom)
