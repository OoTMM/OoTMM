#include <combo.h>

void EnMm_InitWrapper(Actor* this, GameState_Play* play)
{
    ActorFunc EnMm_Init;

    if (comboConfig(CFG_ER_OVERWORLD) || comboConfig(CFG_ER_INDOORS))
    {
        if (gSharedCustomSave.storedSirloin && play->sceneId != SCE_MM_MOUNTAIN_VILLAGE_WINTER)
        {
            ActorDestroy(this);
            return;
        }
    }

    EnMm_Init = actorAddr(AC_EN_MM, 0x80965c0c);
    EnMm_Init(this, play);
}

static int EnMm_ActorHasParent(Actor* this)
{
    Actor_Player* link;

    if (!Actor_HasParent(this))
        return 0;

    if (comboConfig(CFG_ER_OVERWORLD) || comboConfig(CFG_ER_INDOORS))
    {
        if (gPlay->sceneId == SCE_MM_GORON_SHRINE)
        {
            link = GET_LINK(gPlay);
            gSharedCustomSave.storedSirloin = 1;
            PlaySound(0x4802);
            link->base.child = NULL;
            this->parent = NULL;
            ActorDestroy(this);
            return 0;
        }
    }

    return 1;
}

PATCH_CALL(0x80965dcc, EnMm_ActorHasParent);
