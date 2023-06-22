#include <combo.h>
#include <combo/item.h>

int EnKgy_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret)
    {
        if (!gMmExtraFlags.swordRazor)
            gMmExtraFlags.swordRazor = 1;
        else
            gMmExtraFlags.swordGilded = 1;
        this->attachedA = NULL;
    }
    return ret;
}

PATCH_CALL(0x80b41d84, EnKgy_HasGivenItem);

void EnKgy_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    if (!gMmExtraFlags.swordRazor)
        npc = NPC_MM_SWORD_RAZOR;
    else
        npc = NPC_MM_SWORD_GILDED;
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80b41df0, EnKgy_GiveItem);
PATCH_CALL(0x80b4246c, EnKgy_GiveItem);

int EnKgy_ShouldGiveGildedSword(void)
{
    return gMmExtraFlags.swordRazor;
}

PATCH_FUNC(0x80b40d8c, EnKgy_ShouldGiveGildedSword);

u16 EnKgy_GetMessage(void)
{
    if (gSave.playerForm != MM_PLAYER_FORM_HUMAN)
        return 0xc37;
    if (!gMmExtraFlags.swordRazor)
        return 0xc3b;
    if (!gMmExtraFlags.swordGilded)
        return 0xc45;
    return 0xc40;
}

PATCH_FUNC(0x80b41528, EnKgy_GetMessage);

u16 EnKgy_GetMessage2(void)
{
    if (gSave.playerForm != MM_PLAYER_FORM_HUMAN)
        return 0xc38;
    return 0xc3a;
}

PATCH_FUNC(0x80b41460, EnKgy_GetMessage2);
