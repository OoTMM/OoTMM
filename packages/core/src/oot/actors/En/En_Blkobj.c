#include <combo.h>

static Actor* En_Blkobj_Actor_Find_Hook(void* actorCtx, s32 actorId, s32 actorCategory)
{
    // Check for enemy soul shuffle
    if(comboConfig(CFG_OOT_SOULS))
    {
        if(!(gOotSouls1 & (1 << (GI_OOT_SOUL_DARK_LINK - GI_OOT_SOUL_STALFOS))))
        {
            return 1;
        }
    }
    return ActorFind(actorCtx, actorId, actorCategory);
}

PATCH_CALL(0x80a8ec2c, En_Blkobj_Actor_Find_Hook);
