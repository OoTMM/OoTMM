#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/actor.h>
#include <combo/time.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x1ac) = (h); } while (0)

static void sendNetOwl(GameState_Play* play, int owlId)
{
    NetContext* net;
    int npc;
    s16 gi;

    if (!Config_Flag(CFG_MULTIPLAYER))
        return;

    if (owlId == 0xf)
    {
        gi = GI_MM_OWL_HIDDEN;
        npc = NPC_MM_OWL_HIDDEN;
    }
    else
    {
        gi = GI_MM_OWL_GREAT_BAY + owlId;
        npc = NPC_MM_OWL_GREAT_BAY + owlId;
    }

    net = netMutexLock();
    netWaitCmdClear();
    bzero(&net->cmdOut, sizeof(net->cmdOut));
    net->cmdOut.op = NET_OP_ITEM_SEND;
    net->cmdOut.itemSend.playerFrom = gComboConfig.playerId;
    net->cmdOut.itemSend.playerTo = gComboConfig.playerId;
    net->cmdOut.itemSend.game = 1;
    net->cmdOut.itemSend.gi = gi;
    net->cmdOut.itemSend.key = ((u32)OV_NPC << 24) | npc;
    net->cmdOut.itemSend.flags = 0;
    netMutexUnlock();

    /* Mark the NPC as obtained */
    BITMAP8_SET(gSharedCustomSave.mm.npc, npc);
}

void ObjWarpstone_GiveItem(Actor* this, GameState_Play* play)
{
    Actor_Player* link;
    void (*next)(Actor*, GameState_Play*);
    int npc;
    s16 gi;
    u8 id;

    /* Fix a vanilla bug where day-night transitions can mess up the owl */
    if (gSave.time >= CLOCK_TIME(5, 59) && gSave.time <= CLOCK_TIME(6, 1))
        return;
    if (gSave.time >= CLOCK_TIME(17, 59) && gSave.time <= CLOCK_TIME(18, 1))
        return;

    id = this->variable & 0xf;

    if (!Config_Flag(CFG_MM_OWL_SHUFFLE) || (id == 0xf))
    {
        gMmOwlFlags |= ((u32)1 << id);
        sendNetOwl(play, id);
        next = actorAddr(AC_OBJ_WARPSTONE, 0x80b92c48);
        SET_HANDLER(this, next);
        next(this, play);
        return;
    }

    link = GET_PLAYER(play);
    if (Actor_HasParentZ(this))
    {
        /* Prevents duping */
        EnableOwl(id);

        if (!(link->state & PLAYER_ACTOR_STATE_GET_ITEM))
        {
            this->parent = NULL;
            next = actorAddr(AC_OBJ_WARPSTONE, 0x80b92c48);
            SET_HANDLER(this, next);
            next(this, play);
        }
        return;
    }

    /* Give the check */
    npc = NPC_MM_OWL_GREAT_BAY + id;
    gi = GI_MM_OWL_GREAT_BAY + id;
    comboGiveItemNpc(this, play, gi, npc, 9999.f, 9999.f);
}

static void ObjWarpstone_Text(GameState_Play* play)
{
    char* b;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Save the game and keep playing?" TEXT_NL TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN "Yes" TEXT_NL "No" TEXT_END);
}

int ObjWarpstone_TalkedTo(Actor* this, GameState_Play* play)
{
    if (ActorTalkedTo(this))
    {
        ObjWarpstone_Text(play);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80b92ea8, ObjWarpstone_TalkedTo);

void ObjWarpstone_Save(GameState_Play* play)
{
    Save_DoSave(play, SF_OWL);
    PlaySound(0x4823);
    Message_Close(play);
}
