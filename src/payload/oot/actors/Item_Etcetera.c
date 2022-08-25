#include <combo.h>

s8 LoadObject(void* const_1, s16 objectId);
s8 GetObject(void* const_1, s16 objectId);
void ActorSetScale(Actor* actor, float scale);

static void ItemEtcetera_LoadedUpdate(Actor_ItemEtcetera* item, GameState_Play* play)
{
    u8 flagId;

    if (!IsActorDead(&item->base))
    {
        item->base.rot2.y += 0x400;

        /* Another item might have been collected */
        item->gi = comboProgressiveChestItem(item->gi);

        SetChestItemInRange(&item->base, play, item->gi, 30.f, 50.f);
    }
    else
    {
        flagId = item->base.variable & 0x3f;
        if (item->base.variable & 0x40)
        {
            /* Special item */
            switch (flagId)
            {
            case 0:
                SetEventChk(EV_CHK_SARIA_OCARINA);
                break;
            case 1:
                SetEventChk(EV_CHK_ZELDA_LETTER);
                break;
            case 2:
                SetEventChk(EV_CHK_SONG_ZELDA);
                break;
            case 3:
                SetEventChk(EV_CHK_SONG_SUN);
                break;
            }
        }
        ActorDestroy(&item->base);
    }
}

/* We use Item_Etcetera for all out-of-chest items */
/* Variable changed a bit: */
/* 0x003f: Flag */
/* 0x0040: Special (Saria's Ocarina)... */
/* 0xff00: GI */
void hookItemEtcetera_Init(Actor_ItemEtcetera* item, GameState_Play* play)
{
    char* ovlBase;
    s16 gi;
    s16 override;
    const GetItem* giItem;

    ovlBase = (char*)(gActorOvl[0x10f].data);
    gi = (item->base.variable >> 8);
    if (item->base.variable & 0x40)
    {
        /* Special */
        override = comboGetSpecialOverride(item->base.variable & 0x3f);
        if (override >= 0)
            gi = override;
    }
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
