#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x1ac) = (h); } while (0)

void ObjWarpstone_GiveItem(Actor* this, GameState_Play* play)
{
    Actor_Player* link;
    void (*next)(Actor*, GameState_Play*);
    int npc;
    s16 gi;
    u8 id;

    id = this->variable & 0xf;

    if (!comboConfig(CFG_MM_OWL_SHUFFLE) || (id == 0xf))
    {
        gMmOwlFlags |= ((u32)1 << id);
        next = actorAddr(AC_OBJ_WARPSTONE, 0x80b92c48);
        SET_HANDLER(this, next);
        next(this, play);
        return;
    }

    link = GET_LINK(play);
    if (Actor_HasParent(this))
    {
        /* Prevents duping */
        EnableOwl(id);

        if (!(link->state & PLAYER_ACTOR_STATE_GET_ITEM))
        {
            this->attachedA = NULL;
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
    comboSave(play, SF_OWL);
    PlaySound(0x4823);
    Message_Close(play);
}
