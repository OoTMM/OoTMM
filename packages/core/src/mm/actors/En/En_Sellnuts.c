#include <combo.h>
#include <combo/item.h>

int EnSellnuts_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags.scrubTown = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80adbbfc, EnSellnuts_HasGivenItem);

void EnSellnuts_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    /* Unfreeze player */
    GET_LINK(play)->state &= ~PLAYER_ACTOR_STATE_CUTSCENE_FROZEN;

    comboRemoveTradeItem1(XITEM_MM_TRADE1_MOON_TEAR);
    if (gMmExtraFlags.scrubTown)
        gi = GI_MM_RECOVERY_HEART;
    else
        npc = NPC_MM_SCRUB_LAND;
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80adbc48, EnSellnuts_GiveItem);
