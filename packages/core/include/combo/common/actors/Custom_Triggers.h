#ifndef COMBO_COMMON_ACTOR_CUSTOM_TRIGGERS_H
#define COMBO_COMMON_ACTOR_CUSTOM_TRIGGERS_H

#include <combo/common/actor.h>

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

typedef struct GameState_Play GameState_Play;
typedef struct ComboItemQuery ComboItemQuery;

void CustomTriggers_Spawn(GameState_Play* play);

int CustomTriggers_GiveItem(Actor_CustomTriggers* this, GameState_Play* play, const ComboItemQuery* q);
int CustomTriggers_GiveItemNpc(Actor_CustomTriggers* this, GameState_Play* play, s16 gi, int npc);
int CustomTriggers_GiveItemDirect(Actor_CustomTriggers* this, GameState_Play* play, s16 gi);
int CustomTriggers_GiveItemNet(Actor_CustomTriggers* this, GameState_Play* play, s16 gi, u8 from, int flags);
int CustomTrigger_ItemSafe(Actor_CustomTriggers* this, GameState_Play* play);
int CustomTrigger_ItemSafeNet(Actor_CustomTriggers* this, GameState_Play* play);

#endif
