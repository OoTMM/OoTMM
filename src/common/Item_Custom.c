#include <combo.h>

static int shouldSpawn(Actor_ItemCustom* item, GameState_Play* play)
{
    switch (item->type)
    {
    case 0x1:
        /* Collectible */
        if (GetCollectibleFlag(play, item->flag))
            return 0;
        break;
#if defined(GAME_OOT)
    case 0x2:
        /* Special */
        if (GetEventChk(item->flag))
            return 0;
        break;
#endif
    }
    return 1;
}

static void handleFireArrow(Actor_ItemCustom* item, GameState_Play* play)
{
#if defined(GAME_OOT)
    CutsceneContext* c;

    if (item->type == 0x02 && item->flag == EV_OOT_CHK_ARROW_FIRE)
    {
        c = &play->cutscene;
        item->base.modelOffsetY = 50.f;

        if (!c->active || c->unk_28 == NULL || *c->unk_28 == 2)
        {
            item->base.gravity = -0.1f;
            item->base.minVelocityY = -4.0f;
            ActorSetCollisionCylinder(play, &item->base, 10.f, 10.f, 0, 5);
            ActorUpdateVelocity(&item->base);
        }
    }
#endif
}

/* Variables: */
/* RZ  0xf000: Type */
/* RZ  0x00ff: Flag */
/* Var 0x0fff: GI */
static void ItemCustom_Init(Actor_ItemCustom* item, GameState_Play* play)
{
    item->type = ((item->base.initRot.z) & 0xf000) >> 12;
    item->flag = (item->base.initRot.z) & 0xff;
    item->gi = (item->base.variable) & 0x0fff;

    if (!shouldSpawn(item, play))
    {
        ActorDestroy(&item->base);
        return;
    }

    item->base.rot2.z = 0;
    switch (item->type)
    {
    case 0x1:
        /* Collectible */
        item->gi = comboOverride(OV_COLLECTIBLE, play->sceneId, item->flag, item->gi);
        break;
    case 0x2:
        /* Special */
        item->gi = comboOverride(OV_SPECIAL, 0, item->flag, item->gi);
        break;
    }

    ActorSetScale((Actor*)item, 0.25f);
    item->base.position.y += 15.f;
}

static void ItemCustom_Fini(Actor_ItemCustom* item, GameState_Play* play)
{
}

static int isBossReward(Actor_ItemCustom* item, GameState_Play* play)
{
#if defined(GAME_OOT)
    if (item->type == 0x2)
    {
        switch (item->flag & 0xff)
        {
            case EV_OOT_CHK_STONE_EMERALD:
            case EV_OOT_CHK_STONE_RUBY:
            case EV_OOT_CHK_STONE_SAPPHIRE:
            case EV_OOT_CHK_MEDALLION_FOREST:
            case EV_OOT_CHK_MEDALLION_FIRE:
            case EV_OOT_CHK_MEDALLION_WATER:
            case EV_OOT_CHK_MEDALLION_SHADOW:
            case EV_OOT_CHK_MEDALLION_SPIRIT:
                return 1;
        }
    }
#endif

#if defined(GAME_MM)
    if ((item->flag & 0xff) == 0x1e)
    {
        switch (play->sceneId)
        {
        case SCE_MM_LAIR_ODOLWA:
        case SCE_MM_LAIR_GOHT:
        case SCE_MM_LAIR_GYORG:
        case SCE_MM_LAIR_TWINMOLD:
            return 1;
        }
    }
#endif

    return 0;
}

static void ItemCustom_Update(Actor_ItemCustom* item, GameState_Play* play)
{
    float rangeScale;

    handleFireArrow(item, play);

    if (!Actor_HasParent(&item->base))
    {
        item->base.rot2.z = 0;
        item->base.rot2.y += 0x400;

        /* Another item might have been collected */
        item->gi = comboProgressive(item->gi);

        rangeScale = 1.f;
        if (isBossReward(item, play))
            rangeScale = 3.25f;

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
#if defined(GAME_OOT)
        case 0x2:
            /* Special */
            SetEventChk(item->flag);
            if (item->flag == EV_OOT_CHK_RUTO_LETTER)
                SetSwitchFlag(play, 0xb);
#endif
            break;
        }
        ActorDestroy(&item->base);
    }
}

static void ItemCustom_Draw(Actor_ItemCustom* item, GameState_Play* play)
{
    comboDrawGI(play, &item->base, item->gi);
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
