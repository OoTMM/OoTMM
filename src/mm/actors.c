#include <combo.h>

int EnGo_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnDnh_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnShn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnPm_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b);
int EnAn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnPst_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnNb_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnAl_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnBjt_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);

static int Actor_ByteCode_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (actor->id)
    {
    case 0x138:
        return EnGo_GiveItem(actor, play, gi, a, b);
    case 0x168:
        return EnDnh_GiveItem(actor, play, gi, a, b);
    case 0x1c5:
        return EnShn_GiveItem(actor, play, gi, a, b);
    case 0x1d5:
        return EnPm_GiveItem(actor, play, gi, a, b);
    case 0x1f2:
        return EnPst_GiveItem(actor, play, gi, a, b);
    case 0x202:
        return EnAn_GiveItem(actor, play, gi, a, b);
    case 0x243:
        return EnNb_GiveItem(actor, play, gi, a, b);
    case 0x262:
        return EnAl_GiveItem(actor, play, gi, a, b);
    case 0x27d:
        return EnBjt_GiveItem(actor, play, gi, a, b);
    default:
        return GiveItem(actor, play, gi, a, b);
    }
}

PATCH_CALL(0x8010aa34, Actor_ByteCode_GiveItem);

void EnGo_AfterGivingItem(Actor* actor);
void EnDnh_AfterGivingItem(Actor* actor);
void EnPm_AfterGivingItem(Actor* actor);
void EnAn_AfterGivingItem(Actor* actor);
void EnNb_AfterGivingItem(Actor* actor);

static int Actor_ByteCode_HasParent(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
    {
        switch (actor->id)
        {
        case 0x138:
            EnGo_AfterGivingItem(actor);
            break;
        case 0x168:
            EnDnh_AfterGivingItem(actor);
            break;
        case 0x1d5:
            EnPm_AfterGivingItem(actor);
            break;
        case 0x202:
            EnAn_AfterGivingItem(actor);
            break;
        case 0x243:
            EnNb_AfterGivingItem(actor);
            break;
        }
    }
    return ret;
}

PATCH_CALL(0x8010a9f8, Actor_ByteCode_HasParent);

static void Actor_ByteCode_RemoveItem(s16 item, s16 slot)
{
    switch (item)
    {
    case ITEM_MM_LETTER_TO_KAFEI:
        comboRemoveTradeItem3(XITEM_MM_TRADE3_LETTER_TO_KAFEI);
        break;
    case ITEM_MM_PENDANT_OF_MEMORIES:
        comboRemoveTradeItem3(XITEM_MM_TRADE3_PENDANT_OF_MEMORIES);
        break;
    case ITEM_MM_LETTER_TO_MAMA:
        comboRemoveTradeItem2(XITEM_MM_TRADE2_LETTER_TO_MAMA);
        break;
    default:
        RemoveItem(item, slot);
        break;
    }
}

PATCH_CALL(0x8010bc60, Actor_ByteCode_RemoveItem);
