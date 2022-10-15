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

static void EnXc_Update_DeathMountainCrater(Actor* actor, GameState_Play* play)
{
    s16 gi;
    float x;
    float y;
    float z;

    if (checkSetEvent(actor, EV_OOT_CHK_SONG_TP_FIRE))
        return;

    x = LINK.base.position.x;
    y = LINK.base.position.y;
    z = LINK.base.position.z;

    if (x <= -784.f || x >= -584.f)
        return;

    if (y <= 447.f || y >= 647.f)
        return;

    if (z <= -446.f || z >= -246.f)
        return;

    gi = comboOverride(OV_NPC, 0, NPC_OOT_SHEIK_FIRE, GI_OOT_SONG_TP_FIRE);
    GiveItem(actor, play, gi, 10000.f, 50.f);
}

static void EnXc_Update_TempleOfTime(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (!gSave.quest.medallionForest || GetEventChk(EV_OOT_CHK_SONG_TP_LIGHT))
        ActorDestroy(this);

    if (checkSetEvent(this, EV_OOT_CHK_SONG_TP_LIGHT))
        return;

    gi = comboOverride(OV_NPC, 0, NPC_OOT_SHEIK_LIGHT, GI_OOT_SONG_TP_LIGHT);
    GiveItem(this, play, gi, 10000.f, 50.f);
}

static void EnXc_Init(Actor* this, GameState_Play* play)
{
    if (gSave.age == AGE_CHILD)
    {
        ActorDestroy(this);
    }
}

void EnXc_Update(Actor* this, GameState_Play* play)
{
    switch (this->variable)
    {
    case 0x4:
        EnXc_Update_LightArrow(this, play);
        break;
    case 0x6:
        EnXc_Update_ForestMeadow(this, play);
        break;
    case 0x7:
        EnXc_Update_DeathMountainCrater(this, play);
        break;
    case 0x9:
        EnXc_Update_TempleOfTime(this, play);
        break;
    default:
        ActorDestroy(this);
        break;
    }
}

PATCH_FUNC(0x808e30a8, EnXc_Init);
PATCH_FUNC(0x808e3060, EnXc_Update);
