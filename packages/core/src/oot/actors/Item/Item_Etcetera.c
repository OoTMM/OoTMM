#include <combo.h>
#include <combo/item.h>

static void ItemEtcetera_ItemQuery(ComboItemQuery* q, Actor* this, s16 gi)
{
    bzero(q, sizeof(ComboItemQuery));

    q->gi = gi;
    switch (this->variable & 0xff)
    {
    case 0x1:
        q->ovType = OV_NPC;
        q->id = NPC_OOT_RUTO_LETTER;
        break;
    case 0x7:
        q->ovType = OV_NPC;
        q->id = NPC_OOT_FIRE_ARROW;
        q->gi = GI_OOT_ARROW_FIRE;
        break;
    case 0xc:
        q->ovType = OV_CHEST;
        q->sceneId = SCE_OOT_TREASURE_SHOP;
        q->id = 0x0a;
        break;
    default:
        q->ovType = OV_NONE;
    }
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

void ItemEtcetera_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    ItemEtcetera_ItemQuery(&q, this, gi);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80a5e230, ItemEtcetera_GiveItem);
PATCH_CALL(0x80a5e2cc, ItemEtcetera_GiveItem);

void ItemEtcetera_Draw(Actor_ItemEtcetera* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    ItemEtcetera_ItemQuery(&q, &this->base, this->gi);
    comboItemOverride(&o, &q);
    comboDrawGI(play, &this->base, o.gi, 0);
}

PATCH_FUNC(0x80a5e610, ItemEtcetera_Draw);

void ItemEtcetera_DrawTreasureGame(Actor_ItemEtcetera* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
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
        gi = GI_OOT_HEART_PIECE;
        break;
    case 0xd:
        gi = GI_OOT_SMALL_KEY;
        break;
    default:
        gi = GI_OOT_DEFENSE_UPGRADE;
        break;
    }

    ItemEtcetera_ItemQuery(&q, &this->base, gi);
    comboItemOverride(&o, &q);
    comboDrawGI(play, &this->base, o.gi, 0);
}

PATCH_FUNC(0x80a5e5b8, ItemEtcetera_DrawTreasureGame);
