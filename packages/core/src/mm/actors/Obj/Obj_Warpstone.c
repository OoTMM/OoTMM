#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x1ac) = (h); } while (0)

void ObjWarpstone_GiveItem(Actor* this, GameState_Play* play)
{
    Actor_Player* link;
    void (*next)(Actor*, GameState_Play*);
    s16 id;
    s16 gi;

    id = this->variable & 0xf;

    if (!comboConfig(CFG_MM_OWL_SHUFFLE))
    {
        gMmOwlFlags |= (1 << id);
        next = actorAddr(0x223, 0x80b92c48);
        SET_HANDLER(this, next);
        next(this, play);
        return;
    }

    link = GET_LINK(play);
    if (Actor_HasParent(this))
    {
        if (!(link->state & PLAYER_ACTOR_STATE_GET_ITEM))
        {
            this->attachedA = NULL;
            next = actorAddr(0x223, 0x80b92c48);
            SET_HANDLER(this, next);
            next(this, play);
        }
        return;
    }

    /* Give the check */
    gi = comboOverride(OV_NPC, 0, NPC_MM_OWL_GREAT_BAY + id, GI_MM_OWL_GREAT_BAY + id);
    GiveItem(this, play, gi, 9999.f, 9999.f);
}

static void ObjWarpstone_Text(GameState_Play* play)
{
    char* b;

    b = play->textBuffer;
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
    gSave.isOwlSave = 1;
    PrepareSave(&play->sramCtx);
    comboWriteSave();
    PlaySound(0x4823);
    Message_Close(play);
}
