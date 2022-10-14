#include <combo.h>

static int checkSetEvent(Actor* actor, int event)
{
    if (GetEventChk(event))
    {
        ActorDestroy(actor);
        return 1;
    }
    if (Actor_HasParent(actor))
    {
        SetEventChk(event);
        ActorDestroy(actor);
        return 1;
    }
    return 0;
}

static void EnXc_Update_LightArrow(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(this))
    {
        SetEventChk(EV_OOT_CHK_LIGHT_ARROW);
        ActorDestroy(this);
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_ZELDA_LIGHT_ARROW, GI_OOT_ARROW_LIGHT);
        GiveItem(this, play, gi, 10000.f, 500.f);
    }
}

static void EnXc_Update_ForestMeadow(Actor* actor, GameState_Play* play)
{
    s16 gi;

    if (checkSetEvent(actor, EV_OOT_CHK_SONG_TP_FOREST))
        return;
    if (LINK.base.position.z < -2225.f)
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_SHEIK_FOREST, GI_OOT_SONG_TP_FOREST);
        GiveItem(actor, play, gi, 10000.f, 50.f);
    }
}

void EnXc_Update(Actor* actor, GameState_Play* play)
{
    switch (actor->variable)
    {
    case 0x4:
        EnXc_Update_LightArrow(actor, play);
        break;
    case 0x6:
        EnXc_Update_ForestMeadow(actor, play);
        break;
    default:
        ActorDestroy(actor);
        break;
    }
}

PATCH_FUNC(0x808e3060, EnXc_Update);
