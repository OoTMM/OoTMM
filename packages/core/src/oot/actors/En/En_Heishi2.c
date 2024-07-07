#include <combo.h>
#include <combo/inventory.h>
#include <combo/config.h>
#include <combo/player.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x24c) = (h); } while (0)

void EnHeishi2_AfterZeldaLetter(GameState_Play* play)
{
    Message_Close(play);
    comboRemoveTradeItemChild(XITEM_OOT_CHILD_ZELDA_LETTER);
}

PATCH_CALL(0x8097e120, EnHeishi2_AfterZeldaLetter);

static void EnHeishi2_AfterMaskTrade(Actor* this, GameState_Play* play)
{
    gSave.eventsMisc[7] |= 0x80;
    gSave.eventsItem[3] |= 0x100;
    SET_HANDLER(this, actorAddr(AC_EN_HEISHI2, 0x8097d9b0));
}

static void EnHeishi2_MaskTradeCheck(Actor* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_PLAYER(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        EnHeishi2_AfterMaskTrade(this, play);
        return;
    }

    comboGiveItemNpc(this, play, GI_OOT_RUPEE_RED, NPC_OOT_MASK_SELL_KEATON, 16384.f, 16384.f);
}

static void EnHeishi2_MaskTradeRupees(Actor* this, GameState_Play* play)
{
    AddRupees(15);
    EnHeishi2_AfterMaskTrade(this, play);
}

static void EnHeishi2_InitMaskTrade(Actor* this, GameState_Play* play)
{
    ActorFunc func;

    if (Message_GetState(&play->msgCtx) != 2)
    {
        Message_Close(play);
        return;
    }

    if (Config_Flag(CFG_OOT_SHUFFLE_MASK_TRADES))
        func = EnHeishi2_MaskTradeCheck;
    else
        func = EnHeishi2_MaskTradeRupees;

    this->parent = NULL;
    SET_HANDLER(this, func);
    func(this, play);
}

PATCH_FUNC(0x8097e158, EnHeishi2_InitMaskTrade);
