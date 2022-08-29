#include <combo.h>

static const s16 kGreatFairyRewards[] = {
    GI_OOT_MAGIC_UPGRADE,
    GI_OOT_MAGIC_UPGRADE2,
    GI_OOT_DEFENSE_UPGRADE,
    GI_OOT_SPELL_FIRE,
    GI_OOT_SPELL_WIND,
    GI_OOT_SPELL_LOVE,
};

void BgDyYoseizo_Update(Actor* actor, GameState_Play* play)
{
    u8 index;
    s32 override;
    u16 itemId;
    u32 collected;

    if (GetSwitchFlag(play, 0x38))
    {
        index = play->spawnId;
        if (play->sceneId == SCE_GREAT_FAIRY_FOUNTAIN_SPELLS)
            index += 3;
        itemId = kGreatFairyRewards[index];
        override = comboGetCollectibleOverride(play->sceneId, play->spawnId);
        if (override > -1)
            itemId = override;
        itemId = comboProgressiveChestItem(itemId);

        /* Collectible flags don't work here for some reason, use unused flags */
        collected = gSave.perm[SCE_GREAT_FAIRY_FOUNTAIN_UPGRADES].unused & (1 << index );
        if (!HasActorGivenItem(actor) && !collected)
        {
            GiveItem(actor, play, itemId, 400.f, 400.f);
            gSave.perm[SCE_GREAT_FAIRY_FOUNTAIN_UPGRADES].unused |= (1 << index);
        }
        else
        {
            if (collected)
            {
                /* Refill */
                if (gSave.magicUpgrade)
                {
                    gSave.magicSize = 0;
                    gSaveContext.magicTarget = gSave.magicUpgrade2 ? 0x60 : 0x30;
                }
            }
            ActorDestroy(actor);
        }
    }
}
