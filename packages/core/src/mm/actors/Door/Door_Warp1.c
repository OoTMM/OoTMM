#include <combo.h>
#include <combo/item.h>

typedef struct
{
    s16 gi;
    s16 npc;
    s8  index;
}
BlueWarpData;

static const BlueWarpData kBlueWarpData[] = {
    { GI_MM_REMAINS_ODOLWA,   NPC_MM_REMAINS_ODOLWA,   0 },
    { GI_MM_REMAINS_GOHT,     NPC_MM_REMAINS_GOHT,     1 },
    { GI_MM_REMAINS_GYORG,    NPC_MM_REMAINS_GYORG,    2 },
    { GI_MM_REMAINS_TWINMOLD, NPC_MM_REMAINS_TWINMOLD, 3 },
};

static const int DoorWarp1_GetID(Actor* this, GameState_Play* play)
{
    if ((this->variable & 0xff) != 0x01)
        return -1;
    switch (play->sceneId)
    {
    case SCE_MM_LAIR_ODOLWA:
        return 0;
    case SCE_MM_LAIR_GOHT:
        return 1;
    case SCE_MM_LAIR_GYORG:
        return 2;
    case SCE_MM_LAIR_TWINMOLD:
        return 3;
    }
    return -1;
}

static const BlueWarpData* DoorWarp1_GetData(Actor* this, GameState_Play* play)
{
    int id;

    id = DoorWarp1_GetID(this, play);
    if (id < 0)
        return NULL;
    return &kBlueWarpData[id];
}

static s16 DoorWarp1_GetGI(const BlueWarpData* data)
{
    ComboItemQuery q = ITEM_QUERY_INIT;
    ComboItemOverride o;

    q.ovType = OV_NPC;
    q.gi = data->gi;
    q.id = data->npc;

    comboItemOverride(&o, &q);
    return o.gi;
}

int DoorWarp1_Collide(Actor* this, GameState_Play* play)
{
    float dist;

    dist = this->xzDistanceFromLink;
    if (dist < 0.f)
        dist = -dist;
    if (dist < 60.f)
    {
        dist = GET_LINK(play)->base.world.pos.y - this->world.pos.y;
        if (dist < 0.f)
            dist = -dist;
        if (dist < 20.f)
            return 1;
    }
    return 0;
}

int DoorWarp1_ShouldTrigger(Actor* this, GameState_Play* play)
{
    Actor_Player* link;
    int id;
    const BlueWarpData* data;
    s16 gi;
    int npc;

    /* Check for collision */
    if (!DoorWarp1_Collide(this, play))
        return 0;

    /* Fetch extended blue warp data */
    id = DoorWarp1_GetID(this, play);
    if (id == -1)
        return 1;
    data = &kBlueWarpData[id];

    /* Check if the item is being obtained */
    if (Actor_HasParent(this))
    {
        if (!gMmExtraFlags2.songOath)
        {
            gMmExtraFlags2.songOath = 1;
            this->parent = NULL;
        }
        else
        {
            gMmExtraBoss.items |= (1 << data->index);
        }
    }

    /* Check if we are obtaining an item */
    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return 0;

    /* Check if we already have the item */
    if (gMmExtraBoss.items & (1 << data->index))
    {
        comboTriggerWarp(play, id + 8);
        return 0;
    }

    /* Give the correct item */
    if (!gMmExtraFlags2.songOath)
    {
        gi = GI_MM_SONG_ORDER;
        npc = NPC_MM_SONG_ORDER;
    }
    else
    {
        gi = data->gi;
        npc = data->npc;
    }
    comboGiveItemNpc(this, play, gi, npc, 9999.f, 9999.f);
    return 0;
}

void DoorWarp1_AfterDraw(Actor* this, GameState_Play* play)
{
    static const int kRotDivisor = 100;
    const BlueWarpData* data;
    float angle;
    s16 gi;

    data = DoorWarp1_GetData(this, play);
    if (data == NULL)
        return;
    if (gMmExtraBoss.items & (1 << data->index))
        return;
    gi = DoorWarp1_GetGI(data);

    angle = (play->gs.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;
    ModelViewTranslate(this->world.pos.x, this->world.pos.y + 35.f, this->world.pos.z, MAT_SET);
    ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
    ModelViewRotateY(angle, MAT_MUL);
    comboDrawGI(play, this, gi, DRAW_RAW);
}

PATCH_FUNC(0x808b866c, DoorWarp1_ShouldTrigger);

static void DoorWarp1_TriggerBossWarpPad(Actor* this, GameState_Play* play)
{
    s32 entrance;

    switch (play->sceneId)
    {
    case SCE_MM_TEMPLE_WOODFALL:
        entrance = ENTR_MM_BOSS_TEMPLE_WOODFALL;
        break;
    case SCE_MM_TEMPLE_SNOWHEAD:
        entrance = ENTR_MM_BOSS_TEMPLE_SNOWHEAD;
        break;
    case SCE_MM_TEMPLE_GREAT_BAY:
        entrance = ENTR_MM_BOSS_TEMPLE_GREAT_BAY;
        break;
    case SCE_MM_TEMPLE_STONE_TOWER:
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        entrance = ENTR_MM_BOSS_TEMPLE_STONE_TOWER;
        break;
    default:
        UNREACHABLE();
        break;
    }

    play->nextEntrance = entrance;
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_FADE_BLACK;

    gIsEntranceOverride = 1;
}

PATCH_FUNC(0x808b9524, DoorWarp1_TriggerBossWarpPad);
