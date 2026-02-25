#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/player.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x264) = (h); } while (0)

void EnSkj_GiveItem_TreeHP(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(actor, play, gi, NPC_OOT_LOST_WOODS_SKULL_KID, a, b);
}

PATCH_CALL(0x80a6e550, EnSkj_GiveItem_TreeHP);
PATCH_CALL(0x80a6e5e8, EnSkj_GiveItem_TreeHP);

void EnSkj_GiveItem_MemoryGame(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_OOT_HEART_PIECE)
        npc = NPC_OOT_LOST_WOODS_MEMORY;
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80a6f7dc, EnSkj_GiveItem_MemoryGame);
PATCH_CALL(0x80a6f860, EnSkj_GiveItem_MemoryGame);

static void EnSkj_AfterMaskTrade(Actor* this, PlayState* play)
{
    gSave.info.eventsItem[3] |= 0x200;
    Actor_Kill(this);
}

static void EnSkj_MaskTradeCheck(Actor* this, PlayState* play)
{
    Player* link;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        EnSkj_AfterMaskTrade(this, play);
        return;
    }

    comboGiveItemNpc(this, play, GI_OOT_RUPEE_RED, NPC_OOT_MASK_SELL_SKULL, 16384.f, 16384.f);
}

static void EnSkj_MaskTradeRupees(Actor* this, PlayState* play)
{
    AddRupees(10);
    EnSkj_AfterMaskTrade(this, play);
}

static void EnSkj_InitMaskTrade(Actor* this, PlayState* play)
{
    ActorFunc func;

    if (Message_GetState(&play->msgCtx) != 2)
    {
        Message_Close(play);
        return;
    }

    if (Config_Flag(CFG_OOT_SHUFFLE_MASK_TRADES))
        func = EnSkj_MaskTradeCheck;
    else
        func = EnSkj_MaskTradeRupees;

    this->parent = NULL;
    SET_HANDLER(this, func);
    func(this, play);
}

PATCH_FUNC(0x80a6e6ac, EnSkj_InitMaskTrade);
