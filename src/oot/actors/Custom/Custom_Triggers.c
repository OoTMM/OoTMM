#include <combo.h>

Actor_CustomTriggers* gActorCustomTriggers;

static void CustomTriggers_Init(Actor_CustomTriggers* this, GameState_Play* play)
{
    gActorCustomTriggers = this;
}

static void CustomTriggers_Update(Actor_CustomTriggers* this, GameState_Play* play)
{

}

ActorInit CustomTriggers_gActorInit = {
    AC_CUSTOM_TRIGGERS,
    0x6,
    0x10,
    0x1,
    sizeof(Actor_CustomTriggers),
    (ActorFunc)CustomTriggers_Init,
    NULL,
    (ActorFunc)CustomTriggers_Update,
    NULL,
};
