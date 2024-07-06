#include <combo.h>

void EnSekihi_MaybeDestroy(Actor* actor)
{
    if (!gMmExtraFlags.maskZora)
        Actor_Kill(actor);
}

PATCH_CALL(0x80a44f24, EnSekihi_MaybeDestroy);
