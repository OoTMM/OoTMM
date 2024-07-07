#include <combo.h>
#include <combo/player.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x33c) = (h); } while (0)

static void EnMa4_HandleLearnSongEpona(Actor* this, GameState_Play* play)
{
    Actor_Player* link;
    void* handler;

    link = GET_PLAYER(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        gMmExtraFlags.songEpona = 1;
    }

    if (gMmExtraFlags.songEpona)
    {
        handler = actorAddr(AC_EN_MA4, 0x80abe560);
        SET_HANDLER(this, handler);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_SONG_EPONA, NPC_MM_SONG_EPONA, 10000.f, 5000.f);
}

PATCH_FUNC(0x80abf4a8, EnMa4_HandleLearnSongEpona);
