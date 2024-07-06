#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/actor.h>
#include <combo/csmc.h>

#if defined(GAME_OOT)
# define RECOVERY_HEART         GI_OOT_RECOVERY_HEART
# define MILK                   GI_OOT_MILK
# define HEAD_ROT_OFFSET        0x260
#else
# define RECOVERY_HEART         GI_MM_RECOVERY_HEART
# define MILK                   GI_MM_MILK
# define HEAD_ROT_OFFSET        0x268
#endif

static int sCowID;

static const Gfx kDlistColoredRing[] = {
    gsSPBranchList(0x060026c8),
};

static const Gfx kDlistColoredRingMajor[] = {
    gsDPSetPrimColor(0, 0x80, 255, 255, 0, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingMinor[] = {
    gsDPSetPrimColor(0, 0x80, 0x30, 0x0f, 0x02, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingBossKey[] = {
    gsDPSetPrimColor(0, 0x80, 0x00, 0x00, 0xff, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingKey[] = {
    gsDPSetPrimColor(0, 0x80, 0x44, 0x44, 0x44, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingSpider[] = {
    gsDPSetPrimColor(0, 0x80, 0xff, 0xff, 0xff, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingFairy[] = {
    gsDPSetPrimColor(0, 0x80, 0xff, 0x7a, 0xfb, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingHeart[] = {
    gsDPSetPrimColor(0, 0x80, 0xff, 0x00, 0x00, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingSoul[] = {
    gsDPSetPrimColor(0, 0x80, 0x34, 0x0b, 0x9c, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingMap[] = {
    gsDPSetPrimColor(0, 0x80, 0xc7, 0x50, 0x00, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

static const Gfx kDlistColoredRingRenewable[] = {
    gsDPSetPrimColor(0, 0x80, 0, 255, 0, 255),
    gsSPBranchList((u32)kDlistColoredRing - 0x80000000),
};

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
        count = play->actorCtx.actors[i].count;
        tmp = play->actorCtx.actors[i].first;
        while (tmp && count)
        {
            if (tmp->id == AC_EN_COW && tmp->variable != 1 && (!cow || cowDist > tmp->xzDistToPlayer))
            {
                cow = tmp;
                cowDist = tmp->xzDistToPlayer;
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
    switch (play->sceneId)
    {
    case SCE_OOT_LINK_HOUSE:
        return 0;
    case SCE_OOT_GROTTOS:
        return cow->world.pos.x > 3000.f ? 1 : 7;
    case SCE_OOT_STABLE:
        return cow->world.pos.x < -50.f ? 2 : 3;
    case SCE_OOT_RANCH_HOUSE_SILO:
        return cow->world.pos.z > -100.f ? 4 : 5;
    case SCE_OOT_IMPA_HOUSE:
        return 6;
    case SCE_OOT_GERUDO_VALLEY:
        return 8;
    case SCE_OOT_INSIDE_JABU_JABU:
        return cow->room == 0x04 ? 9 : -1;
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
    switch (sceneId)
    {
    case SCE_MM_TERMINA_FIELD:
        return cow->world.pos.z > 930.f ? 0x13 : 0x14;
    case SCE_MM_GREAT_BAY_COAST:
        return cow->world.pos.z > 930.f ? 0x15 : 0x16;
    case SCE_MM_RANCH_HOUSE_BARN:
        return cow->world.pos.x < -100.f ? 0x10 : cow->world.pos.z < -100.f ? 0x12 : 0x11;
    case SCE_MM_BENEATH_THE_WELL:
        return 0x17;
    }
    return -1;
}
#endif

static void EnCow_ItemQuery(GameState_Play* play, Actor* this, ComboItemQuery* q, s16 gi, int* outId)
{
    int id;

    memset(q, 0, sizeof(ComboItemQuery));
    q->gi = gi;
    id = EnCow_GetCowID(this, play);
    if (id != -1)
    {
        if (outId)
            *outId = id;
        q->ovType = OV_COW;
        q->id = id;
        q->giRenew = RECOVERY_HEART;
        if (gCowFlags & (1 << id))
            q->ovFlags |= OVF_RENEW;
    }
}

static const Gfx* EnCow_RingList(GameState_Play* play, Actor* this)
{
    ComboItemQuery q;
    ComboItemOverride o;
    s16 gi;
    s16 giRenew;
    int type;

    if (!csmcEnabledCow())
    {
        EnCow_ItemQuery(play, this, &q, GI_NONE, NULL);
        q.giRenew = GI_NONE;
        comboItemOverride(&o, &q);

        if (o.gi == GI_NONE)
            return NULL;
        else
            return (void*)0x06002628;
    }

    EnCow_ItemQuery(play, this, &q, GI_NONE, NULL);
    q.giRenew = GI_NONE;
    comboItemOverride(&o, &q);
    gi = o.gi;
    q.ovFlags |= OVF_RENEW;
    comboItemOverride(&o, &q);
    giRenew = o.gi;

    if (gi == GI_NONE)
        return NULL;

    type = csmcFromItem(gi);
    if (type == CSMC_NORMAL && giRenew != GI_NONE)
        return kDlistColoredRingRenewable;

    switch (type)
    {
    case CSMC_NORMAL:
        return kDlistColoredRingMinor;
    case CSMC_BOSS_KEY:
        return kDlistColoredRingBossKey;
    case CSMC_KEY:
        return kDlistColoredRingKey;
    case CSMC_SPIDER:
        return kDlistColoredRingSpider;
    case CSMC_FAIRY:
        return kDlistColoredRingFairy;
    case CSMC_HEART:
        return kDlistColoredRingHeart;
    case CSMC_SOUL:
        return kDlistColoredRingSoul;
    case CSMC_MAP_COMPASS:
        return kDlistColoredRingMap;
    case CSMC_MAJOR:
    default:
        return kDlistColoredRingMajor;
    }
}

int EnCow_OverrideLimbDraw(GameState_Play* play, int limbIndex, Gfx** dlist, Vec3f* pos, Vec3s* rot, Actor* this)
{
    const Vec3s* headRot;

    switch (limbIndex)
    {
    case 2:
        headRot = (const Vec3s*)((char*)this + HEAD_ROT_OFFSET);
        rot->y += headRot->y;
        rot->x += headRot->x;
        break;
    case 5:
        *dlist = (Gfx*)EnCow_RingList(play, this);
        break;
    }

    return FALSE;
}

static void EnCow_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    Actor* cow;
    ComboItemQuery q;
    Actor_Player* link;

    /* Get the nearest cow */
    cow = EnCow_GetNearestCow(play);

    /* Make sure any dialog is closed */
    link = GET_PLAYER(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;
    Message_Close(play);

    /* Get the cow ID and attached item */
    EnCow_ItemQuery(play, cow, &q, MILK, &sCowID);

    /* Give the item */
    comboGiveItem(this, play, &q, 10000.f, 10000.f);
}

static int EnCow_HasGivenItem(Actor* this)
{
    Actor_Player* link;

    link = GET_PLAYER(gPlay);
    if (Actor_HasParentZ(this) && !(link->state & PLAYER_ACTOR_STATE_GET_ITEM))
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
