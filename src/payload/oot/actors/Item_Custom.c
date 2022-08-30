#include <combo.h>

void ActorSetScale(Actor* actor, float scale);

static int shouldSpawn(Actor_ItemCustom* item, GameState_Play* play)
{
    switch (item->type)
    {
    case 0x1:
        /* Collectible */
        if (GetCollectibleFlag(play, item->flag))
            return 0;
        break;
    case 0x2:
        /* Special */
        if (GetEventChk(item->flag))
            return 0;
        break;
    }
    return 1;
}

/* Variables: */
/* RZ  0xf000: Type */
/* RZ  0x00ff: Flag */
/* Var 0x0fff: GI */
static void ItemCustom_Init(Actor_ItemCustom* item, GameState_Play* play)
{
    s32 override;

    item->type = ((item->base.initRot.z) & 0xf000) >> 12;
    item->flag = (item->base.initRot.z) & 0xff;
    item->gi = (item->base.variable) & 0x0fff;

    if (!shouldSpawn(item, play))
    {
        ActorDestroy(&item->base);
        return;
    }

    item->base.rot2.z = 0;
    override = -1;
    switch (item->type)
    {
    case 0x1:
        /* Collectible */
        override = comboGetCollectibleOverride(play->sceneId, item->flag);
        break;
    case 0x2:
        /* Special */
        override = comboGetSpecialOverride(item->flag);
        break;
    }
    if (override >= 0)
        item->gi = override;
    item->gi = comboProgressiveChestItem(item->gi);

    ActorSetScale((Actor*)item, 0.25f);
    item->base.position.y += 15.f;
}

static void ItemCustom_Fini(Actor_ItemCustom* item, GameState_Play* play)
{
}

static void ItemCustom_Update(Actor_ItemCustom* item, GameState_Play* play)
{
    u8 flagId;
    float rangeScale;

    if (!HasActorGivenItem(&item->base))
    {
        item->base.rot2.z = 0;
        item->base.rot2.y += 0x400;

        /* Another item might have been collected */
        item->gi = comboProgressiveChestItem(item->gi);

        rangeScale = 1.f;
        if (item->type == 0x2)
        {
            /* For quest items, we need a larger radius */
            switch (item->flag & 0xff)
            {
            case EV_CHK_STONE_EMERALD:
            case EV_CHK_STONE_RUBY:
            case EV_CHK_STONE_SAPPHIRE:
                rangeScale = 3.25f;
                break;
            }
        }

        GiveItem(&item->base, play, item->gi, 30.f * rangeScale, 50.f * rangeScale);
    }
    else
    {
        switch (item->type)
        {
        case 0x1:
            /* Collectible */
            SetCollectibleFlag(play, item->flag);
            break;
        case 0x2:
            /* Special */
            SetEventChk(item->flag);
            break;
        }
        ActorDestroy(&item->base);
    }
}

static void ItemCustom_Draw(Actor_ItemCustom* item, GameState_Play* play)
{
    const GetItem* gi;

    gi = kExtendedGetItems + item->gi - 1;
    comboDrawObject(play, &item->base, gi->objectId, gi->shaderId);
}

ActorInit gActorInit_ItemCustom = {
    AC_ITEM_CUSTOM,
    0x6,
    0x10,
    0x1,
    sizeof(Actor_ItemCustom),
    (ActorFunc)ItemCustom_Init,
    (ActorFunc)ItemCustom_Fini,
    (ActorFunc)ItemCustom_Update,
    (ActorFunc)ItemCustom_Draw,
};
