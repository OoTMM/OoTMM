#include <combo.h>

static void EnBsb_OnDeath(Actor* this, GameState_Play* play)
{
    /* Disable the fire ring */
    SetSwitchFlag(play, 0xb);

    /* Mark Keeta as dead */
    MM_SET_EVENT_WEEK(EV_MM_WEEK_KEETA_DEAD);

    /* Warp on top of the bridge */
    play->nextEntrance = ((SCE_MM_IKANA_GRAVEYARD - 3) << 9) | (5 << 4);
    play->transitionType = TRANS_TYPE_NORMAL;
    play->transitionGfx = TRANS_GFX_BLACK;
}

PATCH_FUNC(0x80c0d9b4, EnBsb_OnDeath);
