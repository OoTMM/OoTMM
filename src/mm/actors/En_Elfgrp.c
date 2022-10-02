#include <combo.h>

static const s16 kGreatFairyRewards[] = {
    GI_MM_MASK_GREAT_FAIRY,
    GI_MM_MAGIC_UPGRADE,
    GI_MM_SPIN_UPGRADE,
    GI_MM_MAGIC_UPGRADE2,
    GI_MM_DEFENSE_UPGRADE,
    GI_MM_GREAT_FAIRY_SWORD,
};

void EnElfgrp_GiveReward(Actor* actor, GameState_Play* play)
{
    u8      fairyIndex;
    u16     gi;
    s32     override;

    fairyIndex = (actor->variable & 0xf) + 1;
    if (fairyIndex == 1 && gMmSave.playerForm == 0x04)
        fairyIndex = 0;

    if (Actor_HasParent(actor))
    {
        SetCollectibleFlag(play, fairyIndex + 1);
        /* TODO: Probably need an event flag? */
        return;
    }

    if (GetCollectibleFlag(play, fairyIndex + 1))
        return;

    gi = kGreatFairyRewards[fairyIndex];
    override = comboGetCollectibleOverride(play->sceneId, fairyIndex);
    if (override >= 0)
        gi = override;
    gi = comboProgressive(gi);
    GiveItem(actor, play, gi, 200.f, 200.f);
}
