#include <combo.h>

typedef void (*EnIk_SetupDieFunc)(Actor*);

static void EnIk_NabooruDeath(Actor* this, GameState_Play* play)
{
    EnIk_SetupDieFunc EnIk_SetupDie = actorAddr(0x113, 0x80a685cc);
    EnIk_SetupDie(this);
    Enemy_StartFinishingBlow(play, this);
    *((char*)this + 0xaf) = 0x20;
}

PATCH_FUNC(0x80a6ac30, EnIk_NabooruDeath);
