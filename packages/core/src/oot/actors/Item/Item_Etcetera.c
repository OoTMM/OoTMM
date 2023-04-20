#include <combo.h>

static s16 ItemEtcetera_GetGI(Actor* this, s16 gi)
{
    switch (this->variable)
    {
    case 0x1:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_RUTO_LETTER, gi);
        break;
    case 0x7:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_FIRE_ARROW, gi);
        break;
    }

    return gi;
}

static int ItemEtcetera_HasGivenItem(Actor* this)
{
    if (!Actor_HasParent(this))
        return 0;
    if ((this->variable & 0xff) == 0x7)
        gOotExtraFlags.fireArrow = 1;
    return 1;
}

PATCH_CALL(0x80a5e1cc, ItemEtcetera_HasGivenItem);
PATCH_CALL(0x80a5e268, ItemEtcetera_HasGivenItem);

int ItemEtcetera_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = ItemEtcetera_GetGI(this, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80a5e230, ItemEtcetera_GiveItem);
PATCH_CALL(0x80a5e2cc, ItemEtcetera_GiveItem);

void ItemEtcetera_Draw(Actor_ItemEtcetera* this, GameState_Play* play)
{
    s16 gi;

    gi = ItemEtcetera_GetGI(&this->base, this->gi);
    comboDrawGI(play, &this->base, gi, 0);
}

PATCH_FUNC(0x80a5e610, ItemEtcetera_Draw);

void ItemEtcetera_DrawTreasureGame(Actor_ItemEtcetera* this, GameState_Play* play)
{
    s16 gi;

    if (*(u8*)((char*)play + 0x1c27) == 0)
        return;

    switch (this->base.variable & 0xff)
    {
    case 0x8:
        gi = GI_OOT_RUPEE_GREEN;
        break;
    case 0x9:
        gi = GI_OOT_RUPEE_BLUE;
        break;
    case 0xa:
        gi = GI_OOT_RUPEE_RED;
        break;
    case 0xb:
        gi = GI_OOT_RUPEE_PURPLE;
        break;
    case 0xc:
        gi = comboOverride(OV_CHEST, SCE_OOT_TREASURE_SHOP, 0x0a, GI_OOT_HEART_PIECE);
        break;
    case 0xd:
        gi = GI_OOT_SMALL_KEY;
        break;
    default:
        gi = GI_OOT_DEFENSE_UPGRADE;
        break;
    }

    comboDrawGI(play, &this->base, gi, 0);
}

PATCH_FUNC(0x80a5e5b8, ItemEtcetera_DrawTreasureGame);
