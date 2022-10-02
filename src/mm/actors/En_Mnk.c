#include <combo.h>

void EnMnk_SongCutscene(Actor* actor, GameState_Play* play)
{
    if (!Actor_HasParent(actor))
    {
        comboGiveItem(actor, play, GI_MM_SONG_AWAKENING, 200.f, 200.f);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_MONKEY_PUNISHED);
    }
}
