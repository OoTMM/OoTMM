#include <combo.h>
#include <combo/item.h>

/* TODO: Check for repeats */
static void EnMnk_SongCutscene(Actor* actor, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParent(actor))
    {
        MM_SET_EVENT_WEEK(EV_MM_WEEK_MONKEY_PUNISHED);
        play->nextEntrance = ((SCE_MM_DEKU_PALACE - 3) << 9) | (1 << 4);
        play->transitionTrigger = TRANS_TRIGGER_NORMAL;
        play->transitionType = TRANS_TYPE_BLACK;
        return;
    }

    comboGiveItemNpc(actor, play, GI_MM_SONG_AWAKENING, NPC_MM_SONG_AWAKENING, 200.f, 200.f);
}

PATCH_FUNC(0x80ab76b0, EnMnk_SongCutscene);
