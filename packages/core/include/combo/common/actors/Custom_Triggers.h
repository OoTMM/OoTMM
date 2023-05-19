#ifndef COMBO_COMMON_ACTOR_CUSTOM_TRIGGERS_H
#define COMBO_COMMON_ACTOR_CUSTOM_TRIGGERS_H

#include <combo/common/actor.h>

typedef struct
{
    Actor   base;
    int     trigger;
    int     acc;
    struct
    {
        u8 weirdEgg:1;
        u8 pocketEgg:1;
    }
    events;
}
Actor_CustomTriggers;

extern Actor_CustomTriggers* gActorCustomTriggers;

typedef struct GameState_Play GameState_Play;
int CustomTriggers_GiveItem(Actor_CustomTriggers* this, GameState_Play* play, s16 gi);
int CustomTriggers_GiveItemNpc(Actor_CustomTriggers* this, GameState_Play* play, s16 gi, s16 npcId);
int CustomTrigger_ItemSafe(Actor_CustomTriggers* this, GameState_Play* play);

#endif
