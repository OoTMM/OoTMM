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

static void EnOwl_StartCutscene(Actor* this, GameState_Play* play)
{
    TransitionContext* t;

    t = &play->transition;
    t->type = TRANS_TYPE_NORMAL;
    t->gfx = TRANS_GFX_SHORTCUT;
    switch ((this->variable & 0xfc0) >> 6)
    {
    case 7:
        t->entrance = 0x27e;
        break;
    case 8:
    case 9:
        t->entrance = 0x554;
        break;
    }
}

PATCH_FUNC(0x80ab1414, EnOwl_StartCutscene);
