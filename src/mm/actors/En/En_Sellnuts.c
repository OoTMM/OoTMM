#include <combo.h>

void EnSellnuts_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags.scrubTown = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80adbbfc, EnSellnuts_HasGivenItem);

int EnSellnuts_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    /* Unfreeze player */
    GET_LINK(play)->state &= ~PLAYER_ACTOR_STATE_CUTSCENE_FROZEN;

    if (gMmExtraFlags.scrubTown)
        gi = GI_MM_RECOVERY_HEART;
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_LAND, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80adbc48, EnSellnuts_GiveItem);
