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
    TransitionContext* t;

    t = &gPlay->transition;
    t->type = TRANS_TYPE_NORMAL;
    t->gfx = TRANS_GFX_SHORTCUT;

    switch (gPlay->sceneId)
    {
    case SCE_OOT_LAKE_HYLIA:
        t->entrance = 0x27e;
        break;
    case SCE_OOT_DEATH_MOUNTAIN_TRAIL:
        t->entrance = 0x554;
        break;
    }
}

PATCH_CALL(0x80ab1548, EnOwl_StartCutscene);
