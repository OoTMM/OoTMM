#ifndef COMBO_COMMON_ACTOR_CUSTOM_TRIGGERS_H
#define COMBO_COMMON_ACTOR_CUSTOM_TRIGGERS_H

#include <combo/common/actor.h>

typedef struct
{
    Actor base;
    int   trigger;
    struct
    {
        u8 weirdEgg:1;
        u8 pocketEgg:1;
    }
    events;
}
Actor_CustomTriggers;

extern Actor_CustomTriggers* gActorCustomTriggers;

#endif
