#include <combo.h>

static void EnMnk_SongCutscene(Actor* actor, GameState_Play* play)
{
    s16 gi;

    if (!Actor_HasParent(actor))
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_SONG_AWAKENING, GI_MM_SONG_AWAKENING);
        GiveItem(actor, play, gi, 200.f, 200.f);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_MONKEY_PUNISHED);
    }
}

PATCH_FUNC(0x80ab76b0, EnMnk_SongCutscene);
