#include <combo.h>

static int sCowID;

static Actor* EnCow_GetNearestCow(GameState_Play* play)
{
    Actor* cow;
    Actor* tmp;
    u32 count;
    float cowDist;

    cow = NULL;
    cowDist = 0.f;
    for (int i = 0; i < 12; ++i)
    {
        count = play->actors[i].count;
        tmp = play->actors[i].first;
        while (tmp && count)
        {
            if (tmp->id == AC_EN_COW && tmp->variable != 1 && (!cow || cowDist > tmp->xzDistanceFromLink))
            {
                cow = tmp;
                cowDist = tmp->xzDistanceFromLink;
            }
            tmp = tmp->next;
            count--;
        }
    }
    return cow;
}

static int EnCow_GetCowID(Actor* cow, GameState_Play* play)
{
    s32 sceneId;

    sceneId = play->sceneId;
    if (sceneId == SCE_MM_GROTTOS)
        sceneId = gLastScene;
    switch (sceneId)
    {
    case SCE_MM_TERMINA_FIELD:
        cow = EnCow_GetNearestCow(play);
        return cow->position.z > 930.f ? 0x13 : 0x14;
    case SCE_MM_GREAT_BAY_COAST:
        cow = EnCow_GetNearestCow(play);
        return cow->position.z > 930.f ? 0x15 : 0x16;
    case SCE_MM_RANCH_HOUSE_BARN:
        cow = EnCow_GetNearestCow(play);
        return cow->position.x < -100.f ? 0x10 : cow->position.z < -100.f ? 0x12 : 0x11;
    case SCE_MM_BENEATH_THE_WELL:
        return 0x17;
    }
    return -1;
}

static void EnCow_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int id;

    id = EnCow_GetCowID(this, play);
    if (id != -1 && (gCowFlags & (1 << id)) == 0)
    {
        sCowID = id;
        gi = comboOverride(OV_COW, 0, id, gi);
    }
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x8099cc50, EnCow_GiveItem);
PATCH_CALL(0x8099cbb4, EnCow_GiveItem);

static int EnCow_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        if (sCowID != -1)
        {
            gCowFlags |= 1 << sCowID;
            sCowID = -1;
        }
        return 1;
    }
    return 0;
}

PATCH_CALL(0x8099cb78, EnCow_HasGivenItem);
