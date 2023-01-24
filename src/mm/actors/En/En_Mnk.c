#include <combo.h>

static void EnMnk_SongCutscene(Actor* actor, GameState_Play* play)
{
    Actor_Player* link;
    s16 gi;

    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParent(actor))
    {
        MM_SET_EVENT_WEEK(EV_MM_WEEK_MONKEY_PUNISHED);
        play->nextEntrance = ((SCE_MM_DEKU_PALACE - 3) << 9) | (1 << 4);
        play->transitionType = TRANS_TYPE_NORMAL;
        play->transitionGfx = TRANS_GFX_BLACK;
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_SONG_AWAKENING, GI_MM_SONG_AWAKENING);
        GiveItem(actor, play, gi, 200.f, 200.f);
    }
}

PATCH_FUNC(0x80ab76b0, EnMnk_SongCutscene);
