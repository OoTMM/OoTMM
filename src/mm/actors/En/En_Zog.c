#include <combo.h>

void EnZog_GiveItem(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(this))
    {
        if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        {
            play->nextEntrance = 0x68a0;
            play->transitionType = TRANS_TYPE_NORMAL;
            play->transitionGfx = TRANS_GFX_BLACK;
            ActorDestroy(this);
        }
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_ZORA, GI_MM_MASK_ZORA);
        GiveItem(this, play, gi, 9999.f, 9999.f);
    }
}

PATCH_FUNC(0x80b943c0, EnZog_GiveItem);
