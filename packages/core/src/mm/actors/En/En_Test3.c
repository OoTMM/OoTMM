#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0xd94) = (h); } while (0)

static void EnTest3_GivePendantOfMemories(Actor* this, GameState_Play* play)
{
    s16 gi;
    int npc;

    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);

    if (Actor_HasParent(this))
    {
        MM_SET_EVENT_WEEK(MM_EV(50, 7));
        MM_SET_EVENT_WEEK(MM_EV(51, 3));
        gMmExtraFlags2.pendant = 1;
        return;
    }

    if (gMmExtraFlags2.pendant)
    {
        gi = GI_MM_RECOVERY_HEART;
        npc = -1;
    }
    else
    {
        gi = GI_MM_PENDANT_OF_MEMORIES;
        npc = NPC_MM_PENDANT_OF_MEMORIES;
    }
    comboGiveItemNpc(this, play, gi, npc, 9999.f, 9999.f);
}

int EnTest3_TalkedTo(Actor* this, GameState_Play* play)
{
    int ret;
    u8 state;

    state = *(u8*)((char*)this + 0xd88);
    ret = ActorTalkedTo(this);
    if (ret && state == 3 && !MM_GET_EVENT_WEEK(MM_EV(51, 3)))
    {
        SET_HANDLER(this, EnTest3_GivePendantOfMemories);
        EnTest3_GivePendantOfMemories(this, play);
        return 0;
    }
    return ret;
}

PATCH_CALL(0x80a3f758, EnTest3_TalkedTo);

void EnTest3_AfterUpdate(Actor* this, GameState_Play* play)
{
    int* state;

    state = actorAddr(0x159, 0x80a41d68);
    if (*state == 2)
    {
        /* Anju and Kafei reunited */
        if (Actor_HasParent(this) || gMmExtraFlags2.maskCouple)
        {
            gMmExtraFlags2.maskCouple = 1;
            return;
        }

        comboGiveItemNpc(this, play, GI_MM_MASK_COUPLE, NPC_MM_MASK_COUPLE, 100.f, 50.f);
    }
}
