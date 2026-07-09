#include "multi.h"

#define MAX_WISPS 16

char sWispNames[MAX_WISPS][9];
u16 sWispIDs[MAX_WISPS];
u16 sWispColors[MAX_WISPS];
s16 sWispTargetPosX[MAX_WISPS];
s16 sWispTargetPosY[MAX_WISPS];
s16 sWispTargetPosZ[MAX_WISPS];
u8 sWispTTL[MAX_WISPS];

float sWispCurPosX[MAX_WISPS];
float sWispCurPosY[MAX_WISPS];
float sWispCurPosZ[MAX_WISPS];

#if defined(GAME_OOT)
static u16 Multi_GetSceneKey(PlayState* play)
{
    u16 sceneKey;
    int instanceId;

    instanceId = Instance_Get(play);
    if (instanceId)
        return 0x100 + ((u16)instanceId - 1);

    sceneKey = play->sceneId;
    switch (sceneKey)
    {
    case SCE_OOT_MARKET_CHILD_DAY:
    case SCE_OOT_MARKET_CHILD_NIGHT:
        sceneKey = SCE_OOT_MARKET_ADULT;
        break;
    case SCE_OOT_MARKET_ENTRANCE_CHILD_DAY:
    case SCE_OOT_MARKET_ENTRANCE_CHILD_NIGHT:
        sceneKey = SCE_OOT_MARKET_ENTRANCE_ADULT;
        break;
    case SCE_OOT_BACK_ALLEY_NIGHT:
        sceneKey = SCE_OOT_BACK_ALLEY_DAY;
        break;
    case SCE_OOT_HYRULE_CASTLE:
        if (gSave.age == AGE_ADULT)
            sceneKey = 0x80;
        break;
    }
    return play->sceneId;
}
#endif

#if defined(GAME_MM)
static u16 Multi_GetSceneKey(PlayState* play)
{
    u16 sceneKey;
    int instanceId;

    instanceId = Instance_Get(play);
    if (instanceId)
        return (0x100 + ((u16)instanceId - 1)) | 0x8000;

    sceneKey = play->sceneId;
    switch (sceneKey)
    {
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        sceneKey = SCE_MM_TEMPLE_STONE_TOWER;
        break;
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        sceneKey = SCE_MM_SOUTHERN_SWAMP;
        break;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        sceneKey = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
        break;
    case SCE_MM_GORON_VILLAGE_SPRING:
        sceneKey = SCE_MM_GORON_VILLAGE_WINTER;
        break;
    case SCE_MM_TWIN_ISLANDS_SPRING:
        sceneKey = SCE_MM_TWIN_ISLANDS_WINTER;
        break;
    }

    return sceneKey | 0x8000;
}
#endif

void Multi_UpdateWisps(void)
{
    for (int i = 0; i < MAX_WISPS; ++i)
    {
        if (sWispTTL[i])
        {
            sWispCurPosX[i] += (float)(sWispTargetPosX[i] - sWispCurPosX[i]) * 0.2f;
            sWispCurPosY[i] += (float)(sWispTargetPosY[i] - sWispCurPosY[i]) * 0.2f;
            sWispCurPosZ[i] += (float)(sWispTargetPosZ[i] - sWispCurPosZ[i]) * 0.2f;
            --sWispTTL[i];
        }
    }
}

static int Multi_GetWispSlot(u16 id)
{
    /* Find existing wisp */
    for (int i = 0; i < MAX_WISPS; ++i)
    {
        if (sWispIDs[i] == id && sWispTTL[i])
            return i;
    }

    /* Find free slot */
    for (int i = 0; i < MAX_WISPS; ++i)
    {
        if (!sWispTTL[i])
            return i;
    }

    return -1;
}

void Multi_UpdateWisp(PlayState* play, MultiPacketPositionIn* pkt)
{
    int slot;

    if (pkt->key != Multi_GetSceneKey(play))
        return;
    slot = Multi_GetWispSlot(pkt->id);
    if (slot < 0)
        return;

    /* We found valid wisp data */
    memcpy(sWispNames[slot], pkt->name, sizeof(pkt->name));
    sWispIDs[slot] = pkt->id;
    sWispColors[slot] = pkt->color;
    sWispTargetPosX[slot] = pkt->x;
    sWispTargetPosY[slot] = pkt->y;
    sWispTargetPosZ[slot] = pkt->z;

#if defined(GAME_MM)
    if (play->sceneId == SCE_MM_TEMPLE_STONE_TOWER_INVERTED)
        sWispTargetPosY[slot] *= -1;
#endif

    if (!sWispTTL[slot])
    {
        sWispCurPosX[slot] = (float)sWispTargetPosX[slot];
        sWispCurPosY[slot] = (float)sWispTargetPosY[slot];
        sWispCurPosZ[slot] = (float)sWispTargetPosZ[slot];
    }
    sWispTTL[slot] = 50;
}

void Multi_ResetWisps(void)
{
    for (int i = 0; i < MAX_WISPS; ++i)
    {
        sWispTTL[i] = 0;
    }
}

static const u32 kWispColors[] = {
    0xff0000cc, 0x00ff00cc, 0x0000ffcc, 0xffff00cc,
    0xff00ffcc, 0x00ffffcc, 0x000000cc, 0xffffffcc,
    0x7f0000cc, 0x007f00cc, 0x00007fcc, 0x7f7f00cc,
    0x7f007fcc, 0x007f7fcc, 0x7f7f7fcc, 0x3f3f3fcc,
};

static void Multi_DrawWisp(PlayState* play, int slot)
{
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Translate(sWispCurPosX[slot], sWispCurPosY[slot], sWispCurPosZ[slot], MTXMODE_NEW);
    Gfx_DrawFlameColor(play, kWispColors[sWispColors[slot] & 0xf], 0.35f, -50.0f);
    CLOSE_DISPS();
}

static void Multi_WispsDrawLabel(Gfx** gfxP, PlayState* play, int index)
{
    Player* player;
    int width;
    Vec3f wisp;
    Vec3f pos;
    float invW;
    int x;
    int y;
    float playerDist;

    player = GET_PLAYER(play);
    if (!player || player->actor.id != ACTOR_PLAYER)
        return;
    wisp.x = sWispCurPosX[index];
    wisp.y = sWispCurPosY[index];
    wisp.z = sWispCurPosZ[index];
    playerDist = Math_Vec3f_DistXYZ(&player->actor.world.pos, &wisp);
    if (playerDist > 200.f)
        return;

    width = (strlen(sWispNames[index]) * 7 + 1);
    wisp.y += 55.f;
    Actor_ProjectPos(play, &wisp, &pos, &invW);
    pos.x *= invW;
    pos.y *= invW;
    pos.z *= invW;
    if (pos.z <= 0.f)
        return;
    pos.x *= pos.z;
    pos.y *= pos.z;
    x = ((pos.x + 1.f) * 0.5f) * 320 - width / 2;
    y = ((-pos.y + 1.f) * 0.5f) * 240;

    if (x < 0 || x + width > 320 || y < 0 || y + 8 > 240) return;

    FontCustom_DrawStr(gfxP, x, y, sWispNames[index]);
}

static void Multi_WispsDrawLabels(PlayState* play)
{
    Gfx* plusOne;
    Gfx* polyOpaP;

    OPEN_DISPS(play->state.gfxCtx);
    polyOpaP = POLY_OPA_DISP;
    plusOne = Gfx_Open(polyOpaP);
    gSPDisplayList(OVERLAY_DISP++, plusOne);

    FontCustom_Setup(&plusOne);
    for (int i = 0; i < MAX_WISPS; ++i)
    {
        if (sWispTTL[i])
            Multi_WispsDrawLabel(&plusOne, play, i);
    }

    gSPEndDisplayList(plusOne++);
    Gfx_Close(polyOpaP, plusOne);
    POLY_OPA_DISP = plusOne;
    CLOSE_DISPS();
}

void Multi_DrawWisps(PlayState* play)
{
    int started;

    started = 0;
    for (int i = 0; i < MAX_WISPS; ++i)
    {
        if (!sWispTTL[i])
            continue;

        if (!started)
        {
            Gfx_SetupDL25_Xlu(play->state.gfxCtx);
            started = 1;
        }

        Multi_DrawWisp(play, i);
    }

    if (started)
        Multi_WispsDrawLabels(play);
}

void Multi_SendPosition(PlayState* play)
{
    MultiPacketPositionOut pkt;
    Player* player;

    if (!gMulti.isConnected)
        return;

    player = GET_PLAYER(play);
    if (!player || (player->actor.id != ACTOR_PLAYER))
        return;

    pkt.header.op = MULTI_OP_POSITION;
    pkt.key = Multi_GetSceneKey(play);
    pkt.x = (s16)player->actor.world.pos.x;
    pkt.y = (s16)player->actor.world.pos.y;
    pkt.z = (s16)player->actor.world.pos.z;
#if defined(GAME_MM)
    if (play->sceneId == SCE_MM_TEMPLE_STONE_TOWER_INVERTED)
        pkt.y *= -1;
#endif

    Multi_SendPacket(&pkt.header, sizeof(pkt));
}
