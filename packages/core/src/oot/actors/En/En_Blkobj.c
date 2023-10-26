#include <combo.h>
#include <combo/souls.h>

static Actor* En_Blkobj_Actor_Find_Hook(void* actorCtx, s32 actorId, s32 actorCategory)
{
    /* Check for enemy soul shuffle */
    if (!comboHasSoulOot(GI_OOT_SOUL_ENEMY_DARK_LINK))
    {
        return (void*)1;
    }
    return ActorFind(actorCtx, actorId, actorCategory);
}

PATCH_CALL(0x80a8ec2c, En_Blkobj_Actor_Find_Hook);
