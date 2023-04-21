#ifndef COMBO_COMMON_ACTOR_INIT_H
#define COMBO_COMMON_ACTOR_INIT_H

#include <combo/types.h>

typedef struct GameState_Play GameState_Play;
typedef struct Actor Actor;
typedef void (*ActorFunc)(Actor*, GameState_Play*);

typedef struct
{
    s16         id;
    u8          type;
    s32         flags;
    s16         objectId;
    s32         instanceSize;
    ActorFunc   init;
    ActorFunc   fini;
    ActorFunc   update;
    ActorFunc   draw;
}
ActorInit;

_Static_assert(sizeof(ActorInit) == 0x20, "ActorInit size is wrong");

#endif
