#include <combo.h>
#include <combo/item.h>
/*0x80B66D30 to 0x80B6D660*/
u8 EnSth_skullMax()
{
    if (comboConfig(CFG_MM_SKULL_COUNT_0))
        return 0x00;
    if (comboConfig(CFG_MM_SKULL_COUNT_5))
        return 0x05;
    if (comboConfig(CFG_MM_SKULL_COUNT_10))
        return 0x0a;
    if (comboConfig(CFG_MM_SKULL_COUNT_15))
        return 0x0f;
    if (comboConfig(CFG_MM_SKULL_COUNT_20))
        return 0x14;
    if (comboConfig(CFG_MM_SKULL_COUNT_25))
        return 0x19;
    return 0x1e;
}

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
    if ((this->variable & 0xf) == 4 && gSave.skullCountOcean < EnSth_skullMax())
    {
        SpawnActor(
            &play->actorCtx,
            play,
            0xd4,
            -110.f, 650.f, 250.f,
            0, 0, 0,
            0
        );
    }
}

s16 EnSth_newSkull(s16 sceneId)
{
    if (sceneId == SCE_MM_SPIDER_HOUSE_OCEAN && gSave.skullCountOcean>=EnSth_skullMax() && gSave.skullCountOcean<30){
        return (gSave.skullCountOcean+(30-EnSth_skullMax())) & 0xFFFF;
    }
    if (sceneId == SCE_MM_SPIDER_HOUSE_SWAMP && gSave.skullCountSwamp>=EnSth_skullMax() && gSave.skullCountSwamp<30){
        return ((gSave.skullCountSwamp+(30-EnSth_skullMax())) & 0xFFFF);
    }
}

PATCH_FUNC(0x8012f22c, EnSth_newSkull);

s16 EnSth_fixTextboxes(s16 sceneId){
    if (sceneId == SCE_MM_SPIDER_HOUSE_OCEAN){
        return (gSave.skullCountOcean & 0xFFFF);
    }
    else{
        return (gSave.skullCountSwamp & 0xFFFF);
    }
}

PATCH_CALL(0x8011fb60, EnSth_fixTextboxes);