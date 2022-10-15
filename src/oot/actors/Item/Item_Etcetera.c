#include <combo.h>

void ItemEtcetera_DrawTreasureGame(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (*(u8*)((char*)play + 0x1c27) == 0)
        return;

    switch (this->variable & 0xff)
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

    comboDrawGI(play, this, gi, 1.f);
}

PATCH_FUNC(0x80a5e5b8, ItemEtcetera_DrawTreasureGame);
