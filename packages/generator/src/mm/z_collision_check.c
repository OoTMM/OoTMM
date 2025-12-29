#include <combo.h>

s32 CollisionCheck_GetToucherDamage(Collider* at, ColliderElement* atInfo, Collider* ac, ColliderElement* acInfo) {
    if ((at->actor != NULL) && (at->actor->id == ACTOR_EN_BOM || at->actor->id == ACTOR_EN_BOM_OOT) && (ac->actor != NULL) &&
        (ac->actor->id == ACTOR_PLAYER)) {
        return 8;
    }
    return atInfo->atDmgInfo.damage;
}

PATCH_FUNC(0x800E04EC, CollisionCheck_GetToucherDamage)
