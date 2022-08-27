#include <combo.h>

s8 LoadObject(void* const_1, s16 objectId);
s8 GetObject(void* const_1, s16 objectId);
void ActorSetScale(Actor* actor, float scale);

static int shouldSpawn(Actor_ItemEtcetera* item, GameState_Play* play)
{
    switch ((item->base.initRot.z) & 0xf000)
    {
    case 0x1000:
        /* Collectible */
        if (GetCollectibleFlag(play, item->base.initRot.z & 0x1f))
            return 0;
        break;
    case 0x2000:
        /* Special */
        if (GetEventChk(item->base.initRot.z & 0xff))
            return 0;
        break;
    }
    return 1;
}

static void ItemEtcetera_LoadedUpdate(Actor_ItemEtcetera* item, GameState_Play* play)
{
    u8 flagId;

    if (!HasActorGivenItem(&item->base))
    {
        item->base.rot2.z = 0;
        item->base.rot2.y += 0x400;

        /* Another item might have been collected */
        item->gi = comboProgressiveChestItem(item->gi);

        float rangeScale = 1.f;

        if ((item->base.initRot.z & 0xf000) == 0x2000)
        {
            /* For quest items, we need a larger radius */
            switch (item->base.initRot.z & 0xff)
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
        switch ((item->base.initRot.z) & 0xf000)
        {
        case 0x1000:
            /* Collectible */
            SetCollectibleFlag(play, item->base.initRot.z & 0x1f);
            break;
        case 0x2000:
            /* Special */
            SetEventChk(item->base.initRot.z & 0xff);
            break;
        }
        ActorDestroy(&item->base);
    }
}

/* We use Item_Etcetera for all out-of-chest items */
/* Variable changed a bit: */
/* rz: top nibble type, 0x1 collectible, 0x2 special */
/* 0x003f: Flag */
/* 0x0040: Special (Saria's Ocarina)... */
/* 0xff00: GI */
void hookItemEtcetera_Init(Actor_ItemEtcetera* item, GameState_Play* play)
{
    char* ovlBase;
    s16 gi;
    s32 override;
    const GetItem* giItem;

    if (!shouldSpawn(item, play))
    {
        ActorDestroy(&item->base);
        return;
    }

    item->base.rot2.z = 0;
    ovlBase = (char*)(gActorOvl[0x10f].data);
    gi = item->base.variable;
    override = -1;
    switch ((item->base.initRot.z) & 0xf000)
    {
    case 0x1000:
        /* Collectible */
        override = comboGetCollectibleOverride(play->sceneId, item->base.initRot.z & 0x1f);
        break;
    case 0x2000:
        /* Special */
        override = comboGetSpecialOverride(item->base.initRot.z & 0xff);
        break;
    }
    if (override >= 0)
        gi = override;
    gi = comboProgressiveChestItem(gi);

    giItem = kExtendedGetItems + (gi - 1);
    item->objIndex = GetObject(play->objTable, giItem->objectId);
    if (item->objIndex < 0)
        item->objIndex = LoadObject(play->objTable, giItem->objectId);
    item->shaderId = giItem->shaderId - 1;
    item->gi = gi;
    item->update = ItemEtcetera_LoadedUpdate;
    item->draw = (Actor_ItemEtcetera_Func)(ovlBase + 0x650);
    item->load = (Actor_ItemEtcetera_Func)(ovlBase + 0x19c);
    ActorSetScale((Actor*)item, 0.25f);
    item->base.position.y += 15.f;
}
