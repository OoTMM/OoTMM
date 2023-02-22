#include <combo.h>

typedef struct
{
    Actor base;
}
Actor_CustomWarp;

static void CustomWarp_Init(Actor_CustomWarp* this, GameState_Play* play)
{

}

static void CustomWarp_Update(Actor_CustomWarp* this, GameState_Play* play)
{

}

static void CustomWarp_Draw(Actor_CustomWarp* this, GameState_Play* play)
{

}

ActorInit CustomWarp_gActorInit = {
    AC_CUSTOM_WARP,
    0x6,
    0x10,
    0x1,
    sizeof(Actor_CustomWarp),
    (ActorFunc)CustomWarp_Init,
    NULL,
    (ActorFunc)CustomWarp_Update,
    (ActorFunc)CustomWarp_Draw,
};
