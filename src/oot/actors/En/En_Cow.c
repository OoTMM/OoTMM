#include <combo.h>

static int sCowID;

static Actor* EnCow_GetNearestCow(GameState_Play* play)
{
    Actor* cow;
    Actor* tmp;
    float cowDist;

    cow = NULL;
    cowDist = 0.f;
    tmp = play->actors[4].first;
    while (tmp)
    {
        if (tmp->id == AC_EN_COW && tmp->variable == 0 && (!cow || cowDist > tmp->xzDistanceFromLink))
        {
            cow = tmp;
            cowDist = tmp->xzDistanceFromLink;
        }
        tmp = tmp->next;
    }
    return cow;
}

static int EnCow_GetCowID(Actor* cow, GameState_Play* play)
{
    switch (play->sceneId)
    {
    case SCE_OOT_LINK_HOUSE:
        return 0;
    case SCE_OOT_GROTTOS:
        return cow->position.x > 3000.f ? 1 : 7;
    case SCE_OOT_STABLE:
        cow = EnCow_GetNearestCow(play);
        return cow->position.x < -50.f ? 2 : 3;
    case SCE_OOT_RANCH_HOUSE_SILO:
        cow = EnCow_GetNearestCow(play);
        return cow->position.z > -100.f ? 4 : 5;
    case SCE_OOT_IMPA_HOUSE:
        return 6;
    case SCE_OOT_GERUDO_VALLEY:
        return 8;
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

PATCH_CALL(0x80b77fcc, EnCow_GiveItem);
PATCH_CALL(0x80b77f30, EnCow_GiveItem);

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

PATCH_CALL(0x80b77ef4, EnCow_HasGivenItem);
