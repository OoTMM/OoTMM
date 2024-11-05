#include <combo.h>
#include <combo/player.h>
#include <combo/item.h>

static void EnToto_GiveTroupeLeaderMask(Actor* this, PlayState* play)
{
    Player* link;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        gMmExtraFlags2.maskTroupeLeader = 1;
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_MASK_TROUPE_LEADER, NPC_MM_MASK_TROUPE_LEADER, 16384.f, 16384.f);
}

void EnToto_UpdateWrapper(Actor* this, PlayState* play)
{
    ActorFunc EnToto_Update;

    if ((MM_GET_EVENT_WEEK(EV_MM_WEEK_SOUND_TEST_REWARD1) || MM_GET_EVENT_WEEK(EV_MM_WEEK_SOUND_TEST_REWARD2)) && play->sceneId == SCE_MM_MILK_BAR && !gMmExtraFlags2.maskTroupeLeader)
    {
        EnToto_GiveTroupeLeaderMask(this, play);
    }
    else
    {
        EnToto_Update = actorAddr(ACTOR_EN_TOTO, 0x80ba4e2c);
        EnToto_Update(this, play);
    }
}
