#include <combo.h>
#include <combo/item.h>

int EnSth_HasGivenItemMaskOfTruth(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskTruth = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80b67b70, EnSth_HasGivenItemMaskOfTruth);

void EnSth_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
        npc = NPC_MM_SPIDER_HOUSE_OCEAN;
        break;
    case GI_MM_MASK_TRUTH:
        if (gMmExtraFlags2.maskTruth)
            gi = GI_MM_RECOVERY_HEART;
        else
            npc = NPC_MM_SPIDER_HOUSE_SWAMP;
        break;
    }

    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80b67524, EnSth_GiveItem);
PATCH_CALL(0x80b67c00, EnSth_GiveItem);

void EnSth_AfterInit(Actor* this, GameState_Play* play)
{
    /* Spawn the cursed skull if required */
    if ((this->variable & 0xf) == 4 && gSave.skullCountOcean < 30)
    {
        SpawnActor(
            (char*)play + 0x1ca0,
            play,
            0xd4,
            -110.f, 650.f, 250.f,
            0, 0, 0,
            0
        );
    }
}
