#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define RECOVERY_HEART GI_OOT_RECOVERY_HEART
#else
# define RECOVERY_HEART GI_MM_RECOVERY_HEART
#endif

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

#if defined(GAME_OOT)
static int EnCow_GetCowID(Actor* cow, GameState_Play* play)
{
    cow = EnCow_GetNearestCow(play);
    switch (play->sceneId)
    {
    case SCE_OOT_LINK_HOUSE:
        return 0;
    case SCE_OOT_GROTTOS:
        return cow->position.x > 3000.f ? 1 : 7;
    case SCE_OOT_STABLE:
        return cow->position.x < -50.f ? 2 : 3;
    case SCE_OOT_RANCH_HOUSE_SILO:
        return cow->position.z > -100.f ? 4 : 5;
    case SCE_OOT_IMPA_HOUSE:
        return 6;
    case SCE_OOT_GERUDO_VALLEY:
        return 8;
    case SCE_OOT_INSIDE_JABU_JABU:
        return 9;
    }
    return -1;
}
#endif

#if defined(GAME_MM)
static int EnCow_GetCowID(Actor* cow, GameState_Play* play)
{
    s32 sceneId;

    sceneId = play->sceneId;
    if (sceneId == SCE_MM_GROTTOS)
        sceneId = gLastScene;
    cow = EnCow_GetNearestCow(play);
    switch (sceneId)
    {
    case SCE_MM_TERMINA_FIELD:
        return cow->position.z > 930.f ? 0x13 : 0x14;
    case SCE_MM_GREAT_BAY_COAST:
        return cow->position.z > 930.f ? 0x15 : 0x16;
    case SCE_MM_RANCH_HOUSE_BARN:
        return cow->position.x < -100.f ? 0x10 : cow->position.z < -100.f ? 0x12 : 0x11;
    case SCE_MM_BENEATH_THE_WELL:
        return 0x17;
    }
    return -1;
}
#endif

static void EnCow_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int id;
    Actor_Player* link;

    /* Make sure any dialog is closed */
    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;
    Message_Close(play);

    /* Get the cow ID and attached item */
    id = EnCow_GetCowID(this, play);
    if (id != -1)
    {
        sCowID = id;
        gi = comboOverride(OV_COW, 0, id, gi);
        if (gCowFlags & (1 << id))
        {
            gi = comboRenewable(gi, RECOVERY_HEART);
        }
    }

    /* Give the item */
    GiveItem(this, play, gi, 10000.f, 10000.f);
}

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


static int EnCow_TalkedTo(Actor* this, GameState_Play* play)
{
    return 1;
}

#if defined(GAME_OOT)
PATCH_CALL(0x80b77fcc, EnCow_GiveItem);
PATCH_CALL(0x80b77f30, EnCow_GiveItem);
PATCH_CALL(0x80b77ef4, EnCow_HasGivenItem);
PATCH_CALL(0x80b78084, EnCow_TalkedTo);
#endif

#if defined(GAME_MM)
PATCH_CALL(0x8099cc50, EnCow_GiveItem);
PATCH_CALL(0x8099cbb4, EnCow_GiveItem);
PATCH_CALL(0x8099cb78, EnCow_HasGivenItem);
PATCH_CALL(0x8099cd08, EnCow_TalkedTo);
#endif
