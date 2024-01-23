#include <combo.h>

static void EnDnp_KingCutscene(Actor* this, GameState_Play* play)
{
    play->nextEntrance = ((SCE_MM_DEKU_KING_CHAMBER - 3) << 9);
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_FADE_BLACK;

    MM_SET_EVENT_WEEK(EV_MM_WEEK_PRINCESS_RETURNED);

    ActorDestroy(this);
}

PATCH_FUNC(0x80b3d558, EnDnp_KingCutscene);
