#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/actor.h>

void EnMaYto_WarpWrapper(Actor* this, GameState_Play* play)
{
    ActorFunc EnMaYto_Warp;

    EnMaYto_Warp = actorAddr(AC_EN_MA_YTO, 0x80b905b0);
    EnMaYto_Warp(this, play);

    if (Config_Flag(CFG_ER_REGIONS_OVERWORLD))
        gPlay->nextEntrance = 0x6480;
}

int EnMaYto_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParentZ(this);
    if (ret)
        gMmExtraFlags.maskRomani = 1;
    return ret;
}

PATCH_CALL(0x80b900d4, EnMaYto_HasGivenItem);

void EnMaYto_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gMmExtraFlags.maskRomani)
    {
        gi = GI_MM_RUPEE_GOLD;
    }
    else
    {
        npc = NPC_MM_CREMIA_ESCORT;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80b9012c, EnMaYto_GiveItem);
PATCH_CALL(0x80b9014c, EnMaYto_GiveItem);
