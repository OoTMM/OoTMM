#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/inventory.h>
#include <combo/actor.h>

static void EnKz_MaybeSetMovedPos(Actor* this, PlayState* play)
{
    int isOpen;
    void (*EnKz_SetMovedPos)(Actor*, PlayState*);

    if (Config_Flag(CFG_OOT_KZ_OPEN))
        isOpen = 1;
    else if (gSave.age == AGE_ADULT && Config_Flag(CFG_OOT_KZ_OPEN_ADULT))
        isOpen = 1;
    else if (GetEventChk(EV_OOT_CHK_KING_ZORA_MOVED))
        isOpen = 1;
    else
        isOpen = 0;

    if (isOpen)
    {
        EnKz_SetMovedPos = actorAddr(ACTOR_EN_KZ, 0x80ad65ec);
        EnKz_SetMovedPos(this, play);
    }
}

PATCH_CALL(0x80ad67a4, EnKz_MaybeSetMovedPos);

int EnKz_HasGiveItem(Actor* this)
{
    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        gOotExtraFlags.tunicZora = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80ad6c5c, EnKz_HasGiveItem);

void EnKz_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    if (!(GET_PLAYER(play)->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);

    if (!gOotExtraFlags.tunicZora)
    {
        npc = NPC_OOT_ZORA_KING_TUNIC;
    }
    else
    {
        npc = NPC_OOT_TRADE_EYEBALL_FROG;
        comboRemoveTradeItemAdult(XITEM_OOT_ADULT_PRESCRIPTION);
    }

    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80ad6cc8, EnKz_GiveItem);

void EnKz_TakeRutoLetter(PlayState* play, u16 oldItem, u16 newItem)
{
    gOotExtraFlags.rutoLetterUsed |= 1;
    if (!Config_Flag(CFG_SHARED_BOTTLES) || (gOotExtraFlags.rutoLetterUsed & 3) == 3)
    {
        Inventory_ReplaceItem(play, oldItem, newItem);
    }
}

PATCH_CALL(0x80ad6afc, EnKz_TakeRutoLetter)
