#include <combo.h>

static const s16 kGreatFairyRewards[] = {
    GI_MM_MASK_GREAT_FAIRY,
    GI_MM_MAGIC_UPGRADE,
    GI_MM_SPIN_UPGRADE,
    GI_MM_MAGIC_UPGRADE2,
    GI_MM_DEFENSE_UPGRADE,
    GI_MM_GREAT_FAIRY_SWORD,
};

static const s16 kGreatFairyNPCs[] = {
    NPC_MM_GREAT_FAIRY_TOWN,
    NPC_MM_GREAT_FAIRY_TOWN_ALT,
    NPC_MM_GREAT_FAIRY_SWAMP,
    NPC_MM_GREAT_FAIRY_MOUNTAIN,
    NPC_MM_GREAT_FAIRY_OCEAN,
    NPC_MM_GREAT_FAIRY_VALLEY,
};

void EnElfgrp_GiveReward(Actor* actor, GameState_Play* play)
{
    u8      fairyIndex;
    u8      mask;
    u16     gi;

    fairyIndex = (actor->variable & 0xf) + 1;
    if (fairyIndex == 1 && gMmSave.playerForm == 0x04)
        fairyIndex = 0;
    mask = (1 << fairyIndex);

    if (gMmExtraFlags.greatFairies & mask)
        return;

    if (Actor_HasParent(actor))
    {
        gMmExtraFlags.greatFairies |= mask;
        actor->attachedA = NULL;
        /* TODO: Probably need an event flag? */
        return;
    }

    gi = comboOverride(OV_NPC, 0, kGreatFairyNPCs[fairyIndex], kGreatFairyRewards[fairyIndex]);
    GiveItem(actor, play, gi, 200.f, 200.f);
}
