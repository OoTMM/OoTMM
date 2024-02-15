#include <combo.h>

void EnOwl_AfterInit(Actor* this)
{
    switch ((this->variable & 0xfc0) >> 6)
    {
    case 0x07:
    case 0x08:
    case 0x09:
        break;
    default:
        ActorDestroy(this);
        break;
    }
}

static void EnOwl_StartCutscene(void)
{
    gPlay->transitionTrigger = TRANS_TYPE_NORMAL;
    gPlay->transitionType = TRANS_GFX_SHORTCUT;

    switch (gPlay->sceneId)
    {
    case SCE_OOT_LAKE_HYLIA:
        gPlay->nextEntranceIndex = ENTR_OOT_FIELD_OWL;
        break;
    case SCE_OOT_DEATH_MOUNTAIN_TRAIL:
        gPlay->nextEntranceIndex = ENTR_OOT_VILLAGE_OWL;
        break;
    }
	gIsEntranceOverride = 1;
}

PATCH_CALL(0x80ab1548, EnOwl_StartCutscene);
