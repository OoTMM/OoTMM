#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/player.h>
#include <combo/mask.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x180) = (h); } while (0)

static void EnMm_AfterMaskTrade(Actor* this, GameState_Play* play)
{
    gSave.eventsItem[3] |= 0x800;
    SET_HANDLER(this, actorAddr(AC_EN_MM, 0x80ad111c));
}

static void EnMm_MaskTradeCheck(Actor* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_PLAYER(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        EnMm_AfterMaskTrade(this, play);
        return;
    }

    comboGiveItemNpc(this, play, GI_OOT_RUPEE_RAINBOW, NPC_OOT_MASK_SELL_BUNNY, 16384.f, 16384.f);
}

static void EnMm_MaskTradeRupees(Actor* this, GameState_Play* play)
{
    AddRupees(500);
    EnMm_AfterMaskTrade(this, play);
}

static void EnMm_InitMaskTrade(Actor* this, GameState_Play* play)
{
    ActorFunc func;

    if (Message_GetState(&play->msgCtx) != 2)
    {
        Message_Close(play);
        return;
    }

    if (Config_Flag(CFG_OOT_SHUFFLE_MASK_TRADES))
        func = EnMm_MaskTradeCheck;
    else
        func = EnMm_MaskTradeRupees;

    this->parent = NULL;
    SET_HANDLER(this, func);
    func(this, play);
}

static int EnMm_ActorTalkedTo(Actor* this)
{
    Actor_Player* link;
    int ret;

    link = GET_PLAYER(gPlay);
    ret = ActorTalkedTo(this);
    if (ret && link->mask == MASK_BUNNY && !(gSave.eventsItem[3] & 0x800))
    {
        SET_HANDLER(this, EnMm_InitMaskTrade);
        EnMm_InitMaskTrade(this, gPlay);
        return 0;
    }
    return ret;
}

PATCH_CALL(0x80ad0bd4, EnMm_ActorTalkedTo);
