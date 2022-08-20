#include <combo.h>

s8 LoadObject(void* const_1, s16 objectId);
s8 GetObject(void* const_1, s16 objectId);
void ActorSetScale(Actor* actor, float scale);

/* We use Item_Etcetera for all out-of-chest items */
void hookItemEtcetera_Init(Actor_ItemEtcetera* item, GameState_Play* play)
{
    char* ovlBase;

    ovlBase = (char*)(gActorOvl[0x10f].data);

    item->objIndex = GetObject(play->objTable, 0xbd);
    if (item->objIndex < 0)
        item->objIndex = LoadObject(play->objTable, 0xbd);
    item->shaderId = 19;
    item->gi = 1;
    item->update = (Actor_ItemEtcetera_Func)(ovlBase + 0x1f4);
    item->draw = (Actor_ItemEtcetera_Func)(ovlBase + 0x650);
    item->load = (Actor_ItemEtcetera_Func)(ovlBase + 0x19c);
    ActorSetScale((Actor*)item, 0.25f);
    item->base.position.y += 15.f;
}

void hookItemEtcetera_Update(Actor_ItemEtcetera* item, GameState_Play* play)
{
    item->load(item, play);
    item->base.rot2.y += 0x400;
}
