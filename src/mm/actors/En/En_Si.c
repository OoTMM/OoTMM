#include <combo.h>

static s16 EnSi_GetOverride(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (play->sceneId == SCE_MM_SPIDER_HOUSE_OCEAN)
        gi = GI_MM_GS_TOKEN_OCEAN;
    else
        gi = GI_MM_GS_TOKEN_SWAMP;
    return comboOverride(OV_CHEST, play->sceneId, (this->variable & 0xfc) >> 2, gi);
}

void EnSi_AddItem(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = EnSi_GetOverride(this, play);
    SetChestFlag(play, (this->variable & 0xfc) >> 2);
    PlayerDisplayTextBox(play, 0x52, NULL);
    comboAddItem(play, gi);
    PlaySoundSpecial(0x39);
}

PATCH_FUNC(0x8098cad0, EnSi_AddItem);

void EnSi_Draw(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = EnSi_GetOverride(this, play);
    comboDrawGI(play, this, gi, 0);
}

PATCH_FUNC(0x8098cd0c, EnSi_Draw);
