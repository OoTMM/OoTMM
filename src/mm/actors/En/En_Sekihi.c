#include <combo.h>

void EnSekihi_MaybeDestroy(Actor* actor)
{
    if (!gMmExtraFlags.maskZora)
        ActorDestroy(actor);
}

PATCH_CALL(0x80a44f24, EnSekihi_MaybeDestroy);
