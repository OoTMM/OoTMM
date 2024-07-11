#include <combo.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/actor.h>

void EnMm_InitWrapper(Actor* this, GameState_Play* play)
{
    ActorFunc EnMm_Init;

    if (Config_Flag(CFG_ER_OVERWORLD) || Config_Flag(CFG_ER_INDOORS))
    {
        if (gSharedCustomSave.storedSirloin && play->sceneId != SCE_MM_MOUNTAIN_VILLAGE_WINTER)
        {
            Actor_Kill(this);
            return;
        }
    }

    EnMm_Init = actorAddr(AC_EN_MM, 0x80965c0c);
    EnMm_Init(this, play);
}

static int EnMm_ActorHasParent(Actor* this)
{
    Actor_Player* link;

    if (!Actor_HasParentZ(this))
        return 0;

    if (Config_Flag(CFG_ER_OVERWORLD) || Config_Flag(CFG_ER_INDOORS))
    {
        if (gPlay->sceneId == SCE_MM_GORON_SHRINE)
        {
            link = GET_PLAYER(gPlay);
            gSharedCustomSave.storedSirloin = 1;
            PlaySound(0x4802);
            link->actor.child = NULL;
            this->parent = NULL;
            Actor_Kill(this);
            return 0;
        }
    }

    return 1;
}

PATCH_CALL(0x80965dcc, EnMm_ActorHasParent);
