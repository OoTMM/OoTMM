#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/draw.h>

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

static const int DoorWarp1_GetID(Actor* this, PlayState* play)
{
    if ((this->params & 0xff) != 0x01)
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

static const BlueWarpData* DoorWarp1_GetData(Actor* this, PlayState* play)
{
    int id;

    id = DoorWarp1_GetID(this, play);
    if (id < 0)
        return NULL;
    return &kBlueWarpData[id];
}

static void DoorWarp1_ItemOverride(ComboItemOverride* o, const BlueWarpData* data)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.ovType = OV_NPC;
    q.gi = data->gi;
    q.id = data->npc;

    comboItemOverride(o, &q);
}

int DoorWarp1_Collide(Actor* this, PlayState* play)
{
    float dist;

    dist = this->xzDistToPlayer;
    if (dist < 0.f)
        dist = -dist;
    if (dist < 60.f)
    {
        dist = GET_PLAYER(play)->actor.world.pos.y - this->world.pos.y;
        if (dist < 0.f)
            dist = -dist;
        if (dist < 20.f)
            return 1;
    }
    return 0;
}

int DoorWarp1_ShouldTrigger(Actor* this, PlayState* play)
{
    Player* link;
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
    if (Actor_HasParentZ(this))
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
    link = GET_PLAYER(play);
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
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

void DoorWarp1_AfterDraw(Actor* this, PlayState* play)
{
    static const int kRotDivisor = 100;
    const BlueWarpData* data;
    ComboItemOverride o;
    Player* player;
    float angle;
    float scale;
    float giantScale;

    player = GET_PLAYER(play);
    data = DoorWarp1_GetData(this, play);
    if (data == NULL)
        return;
    if (gMmExtraBoss.items & (1 << data->index))
        return;

    DoorWarp1_ItemOverride(&o, data);
    angle = (play->state.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;
    giantScale = (player->currentMask == PLAYER_MASK_GIANT) ? 0.1f : 1.f;
    scale = 0.35f * giantScale;
    Matrix_Translate(this->world.pos.x, this->world.pos.y + 35.f * giantScale, this->world.pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_RotateY(angle, MTXMODE_APPLY);
    Draw_GiCloaked(play, this, o.gi, o.cloakGi, DRAW_RAW);
}

PATCH_FUNC(0x808b866c, DoorWarp1_ShouldTrigger);

static void DoorWarp1_TriggerBossWarpPad(Actor* this, PlayState* play)
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
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_BLACK;

    gIsEntranceOverride = 1;
}

PATCH_FUNC(0x808b9524, DoorWarp1_TriggerBossWarpPad);

static void DoorWarp1_SetExitFadeHook(PlayState* play)
{
    Scene_SetExitFade(play);

    /* In ER, this warp just exits */
    if (Config_Flag(CFG_ER_INDOORS) && play->sceneId == SCE_MM_DEKU_SHRINE)
    {
        play->nextEntrance = ENTR_MM_DEKU_PALACE_EXTERIOR_FROM_SHRINE;
        gSaveContext.respawnFlag = 0;
        gIsEntranceOverride = 1;
    }
}

PATCH_CALL(0x808ba518, DoorWarp1_SetExitFadeHook);
