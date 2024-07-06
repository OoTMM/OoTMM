#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/player.h>
#include <combo/mask.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x180) = (h); } while (0)

static void EnCs_AfterMaskTrade(Actor* this, GameState_Play* play)
{
    gSave.eventsItem[3] |= 0x400;
    SET_HANDLER(this, actorAddr(AC_EN_CS, 0x80ae1ee8));
}

static void EnCs_MaskTradeCheck(Actor* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_PLAYER(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        EnCs_AfterMaskTrade(this, play);
        return;
    }

    comboGiveItemNpc(this, play, GI_OOT_RUPEE_RED, NPC_OOT_MASK_SELL_SPOOKY, 16384.f, 16384.f);
}

static void EnCs_MaskTradeRupees(Actor* this, GameState_Play* play)
{
    AddRupees(30);
    EnCs_AfterMaskTrade(this, play);
}

static void EnCs_InitMaskTrade(Actor* this, GameState_Play* play)
{
    ActorFunc func;

    if (Message_GetState(&play->msgCtx) != 2)
    {
        Message_Close(play);
        return;
    }

    if (Config_Flag(CFG_OOT_SHUFFLE_MASK_TRADES))
        func = EnCs_MaskTradeCheck;
    else
        func = EnCs_MaskTradeRupees;

    this->parent = NULL;
    SET_HANDLER(this, func);
    func(this, play);
}

static int EnCs_ActorTalkedTo(Actor* this)
{
    Actor_Player* link;
    int ret;

    link = GET_PLAYER(gPlay);
    ret = ActorTalkedTo(this);
    if (ret && link->mask == MASK_SPOOKY && !(gSave.eventsItem[3] & 0x400))
    {
        SET_HANDLER(this, EnCs_InitMaskTrade);
        EnCs_InitMaskTrade(this, gPlay);
        return 0;
    }
    return ret;
}

PATCH_CALL(0x80ae1b3c, EnCs_ActorTalkedTo);
