#ifndef COMBO_COMMON_ACTOR_CUSTOM_TRIGGERS_H
#define COMBO_COMMON_ACTOR_CUSTOM_TRIGGERS_H

#include <combo/actor.h>

typedef struct
{
    int trigger;
    int acc;
    struct
    {
        u8 weirdEgg:1;
        u8 pocketEgg:1;
    }
    events;
}
ComboTriggersData;

typedef struct
{
    Actor base;
}
Actor_CustomTriggers;

extern Actor_CustomTriggers* gActorCustomTriggers;
extern ComboTriggersData gComboTriggersData;

typedef struct PlayState PlayState;
typedef struct ComboItemQuery ComboItemQuery;

void CustomTriggers_Spawn(PlayState* play);

int CustomTriggers_GiveItem(Actor_CustomTriggers* this, PlayState* play, const ComboItemQuery* q);
int CustomTriggers_GiveItemNpc(Actor_CustomTriggers* this, PlayState* play, s16 gi, int npc);
int CustomTriggers_GiveItemDirect(Actor_CustomTriggers* this, PlayState* play, s16 gi);
int CustomTrigger_ItemSafe(Actor_CustomTriggers* this, PlayState* play);

#endif
