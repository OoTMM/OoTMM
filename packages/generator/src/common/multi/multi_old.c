#if 0
#include "multi.h"
#include <combo/player.h>
#include <combo/config.h>
#include <combo/item.h>
#include <combo/actor.h>
#include <combo/global.h>
#include <combo/instances.h>
#include <combo/mark.h>

#if defined(GAME_OOT)
# define RECOVERY_HEART GI_OOT_RECOVERY_HEART
#else
# define RECOVERY_HEART GI_MM_RECOVERY_HEART
#endif

#if defined(GAME_OOT)
static u16 GetSceneKey(PlayState* play)
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
static u16 GetSceneKey(PlayState* play)
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

static void processMessagesSendPlayerPos(PlayState* play, NetContext* net)
{
    Player* link;
    int index;
    NetMsg* msg;

    /* Find a suitable index */
    index = -1;
    for (int i = 0; i < NET_MSG_MAX; ++i)
    {
        if (net->msgInSize[i] == 0 && net->msgOutSize[i] == 0)
        {
            index = i;
            break;
        }
    }
    if (index < 0)
        return;

    /* We have a suitable index, send the message */
    link = GET_PLAYER(play);
    if (!link || (link->actor.id != ACTOR_PLAYER))
        return;

    msg = &net->msgBuffer[index];
    msg->op = NETMSG_PLAYER_POS;
    msg->playerPos.frameCount = play->state.frameCount;
    msg->playerPos.x = (s16)link->actor.world.pos.x;
    msg->playerPos.y = (s16)link->actor.world.pos.y;
    msg->playerPos.z = (s16)link->actor.world.pos.z;
    msg->playerPos.sceneKey = GetSceneKey(play);
#if defined(GAME_MM)
    if (play->sceneId == SCE_MM_TEMPLE_STONE_TOWER_INVERTED)
        msg->playerPos.y *= -1;
#endif
    net->msgOutSize[index] = 0x10;
}

typedef struct
{
    u16     clientId;
    u8      ttl;
    u32     frameCount;
    Vec3f   pos;
    Vec3f   target;
}
PlayerWisp;

static PlayerWisp sPlayerWisps[16];

static void updateWisp(PlayerWisp* wisp, NetMsgPlayerPos* playerPos)
{
    if (playerPos->frameCount < wisp->frameCount)
        return;

    wisp->ttl = 20;
    wisp->frameCount = playerPos->frameCount;
    wisp->target.x = playerPos->x;
    wisp->target.y = playerPos->y;
    wisp->target.z = playerPos->z;
}

static void processMessageInPlayerPos(PlayState* play, NetContext* net, NetMsgPlayerPos* playerPos, u16 clientId)
{
    u16 sceneKey;
    int freeIndex;

    sceneKey = GetSceneKey(play);
    if (playerPos->sceneKey != sceneKey)
        return;

#if defined(GAME_MM)
    if (play->sceneId == SCE_MM_TEMPLE_STONE_TOWER_INVERTED)
        playerPos->y *= -1;
#endif

    freeIndex = -1;
    for (int i = 0; i < ARRAY_COUNT(sPlayerWisps); ++i)
    {
        if (sPlayerWisps[i].ttl && sPlayerWisps[i].clientId == clientId)
        {
            updateWisp(&sPlayerWisps[i], playerPos);
            return;
        }
        else if (!sPlayerWisps[i].ttl && freeIndex < 0)
        {
            freeIndex = i;
        }
    }

    if (freeIndex >= 0)
    {
        sPlayerWisps[freeIndex].ttl = 20;
        sPlayerWisps[freeIndex].clientId = clientId;
        sPlayerWisps[freeIndex].frameCount = playerPos->frameCount;
        sPlayerWisps[freeIndex].pos.x = playerPos->x;
        sPlayerWisps[freeIndex].pos.y = playerPos->y;
        sPlayerWisps[freeIndex].pos.z = playerPos->z;
        sPlayerWisps[freeIndex].target.x = playerPos->x;
        sPlayerWisps[freeIndex].target.y = playerPos->y;
        sPlayerWisps[freeIndex].target.z = playerPos->z;
    }
}

static void processMessageIn(PlayState* play, NetContext* net, NetMsg* msg, u16 clientId)
{
    if (msg->op == NETMSG_PLAYER_POS)
        processMessageInPlayerPos(play, net, &msg->playerPos, clientId);
}

static void Multi_ProcessMessages(PlayState* play, NetContext* net)
{
    /* Send pos */
    if ((play->state.frameCount & 3) == 0)
        processMessagesSendPlayerPos(play, net);

    /* Decrement wisps ttl */
    for (int i = 0; i < ARRAY_COUNT(sPlayerWisps); ++i)
    {
        if (sPlayerWisps[i].ttl)
            --sPlayerWisps[i].ttl;
        if (sPlayerWisps[i].ttl)
        {
            sPlayerWisps[i].pos.x += (sPlayerWisps[i].target.x - sPlayerWisps[i].pos.x) * 0.25f;
            sPlayerWisps[i].pos.y += (sPlayerWisps[i].target.y - sPlayerWisps[i].pos.y) * 0.25f;
            sPlayerWisps[i].pos.z += (sPlayerWisps[i].target.z - sPlayerWisps[i].pos.z) * 0.25f;
        }
    }

    for (int i = 0; i < NET_MSG_MAX; ++i)
    {
        if (net->msgInSize[i])
        {
            processMessageIn(play, net, &net->msgBuffer[i], net->msgClientId[i]);
            net->msgInSize[i] = 0;
        }
    }
}

static const u32 kWispColors[] = {
    0xff0000cc, 0x00ff00cc, 0x0000ffcc, 0xffff00cc,
    0xff00ffcc, 0x00ffffcc, 0x000000cc, 0xffffffcc,
    0x7f0000cc, 0x007f00cc, 0x00007fcc, 0x7f7f00cc,
    0x7f007fcc, 0x007f7fcc, 0x7f7f7fcc, 0x3f3f3fcc,
};

static void drawSingleWisp(PlayState* play, const PlayerWisp* wisp)
{
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Translate(wisp->pos.x, wisp->pos.y, wisp->pos.z, MTXMODE_NEW);
    Gfx_DrawFlameColor(play, kWispColors[wisp->clientId & 0xf], 0.35f, -50.0f);
    CLOSE_DISPS();
}

void Multi_DrawWisps(PlayState* play)
{
    if (!Config_Flag(CFG_MULTIPLAYER))
        return;

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    for (int i = 0; i < ARRAY_COUNT(sPlayerWisps); ++i)
    {
        if (sPlayerWisps[i].ttl)
            drawSingleWisp(play, &sPlayerWisps[i]);
    }
}

void Multi_ResetWisps(void)
{
    if (!Config_Flag(CFG_MULTIPLAYER))
        return;
    bzero(sPlayerWisps, sizeof(sPlayerWisps));
}

void Multi_UpdateOld(PlayState* play)
{
    NetContext* ctx;

    if (!Config_Flag(CFG_MULTIPLAYER))
        return;

    ctx = netMutexLock();
    Multi_ProcessMessages(play, ctx);
    netMutexUnlock();
}
#endif
