#include "multi.h"
#include <combo/player.h>
#include <combo/config.h>
#include <combo/item.h>
#include <combo/actor.h>
#include <combo/global.h>
#include <combo/instances.h>

#if 0
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

static int Multi_CanReceiveItem(PlayState* play)
{
    Player* link;

    if (gSaveContext.gameMode || (gSaveContext.minigameState == 1))
        return 0;
    if (Message_GetState(&play->msgCtx) != 0)
        return 0;
    link = GET_PLAYER(play);
    if (link->stateFlags1 & (PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN | PLAYER_ACTOR_STATE_EPONA))
        return 0;

    return 1;
}

static void Multi_GiveItem(PlayState* play, s16 gi, u8 from, int flags)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.gi = gi;
    q.from = from;

    if ((flags & OVF_PRECOND) && (!isItemLicensed(gi)))
    {
        bzero(&q, sizeof(q));
        q.ovType = OV_NONE;
        q.gi = RECOVERY_HEART;
    }

    Item_AddWithDecoy(play, &q);
}

static void Multi_ReceiveItem(PlayState* play, NetContext* net)
{
    s16 gi;
    u8 ovType;
    u8 sceneId;
    u8 roomId;
    u8 id;
    u8 isMarked;
    u8 needsMarking;
    u8 isSamePlayer;

    gi = net->cmdIn.itemRecv.gi;
    isMarked = 0;
    needsMarking = 0;
    isSamePlayer = (net->cmdIn.itemRecv.playerFrom == gComboConfig.playerId);
    if (isSamePlayer)
    {
        if (!(net->cmdIn.itemRecv.flags & OVF_RENEW))
        {
            needsMarking = 1;
            ovType = (net->cmdIn.itemRecv.key >> 24) & 0xff;
            sceneId = (net->cmdIn.itemRecv.key >> 16) & 0xff;
            roomId = (net->cmdIn.itemRecv.key >> 8) & 0xff;
            id = net->cmdIn.itemRecv.key & 0xff;
            if (net->cmdIn.itemRecv.game)
                isMarked = Multi_IsMarkedMm(play, ovType, sceneId, roomId, id);
            else
                isMarked = Multi_IsMarkedOot(play, ovType, sceneId, roomId, id);
        }
        else
        {
            for (int i = 0; i < ARRAY_COUNT(gSharedCustomSave.netGiSkip); ++i)
            {
                if (gSharedCustomSave.netGiSkip[i] == gi)
                {
                    isMarked = 1;
                    gSharedCustomSave.netGiSkip[i] = GI_NONE;
                    break;
                }
            }
        }
    }

    if (!isMarked && gi != GI_NOTHING)
    {
        /* Need to actually give the item */
        if (!Multi_CanReceiveItem(play) || g.decoysCount)
            return;
        Multi_GiveItem(play, gi, net->cmdIn.itemRecv.playerFrom, net->cmdIn.itemRecv.flags);
    }

    /* Triggers the side-effect */
    if (needsMarking)
    {
        if (net->cmdIn.itemRecv.game)
            Multi_SetMarkedMm(play, ovType, sceneId, roomId, id);
        else
            Multi_SetMarkedOot(play, ovType, sceneId, roomId, id);
    }

    /* Mark as obtained on the network */
    bzero(&net->cmdIn, sizeof(net->cmdIn));
    gSaveLedgerBase++;
    net->ledgerBase = gSaveLedgerBase;
}

static void Multi_ProcessItems(PlayState* play, NetContext* net)
{
    u8 playerTo;

    if (net->cmdIn.op == NET_OP_ITEM_RECV)
    {
        playerTo = net->cmdIn.itemRecv.playerTo;
        if (playerTo != gComboConfig.playerId && playerTo != PLAYER_ALL)
        {
            bzero(&net->cmdIn, sizeof(net->cmdIn));
            gSaveLedgerBase++;
            net->ledgerBase = gSaveLedgerBase;
        }
        else
        {
            Multi_ReceiveItem(play, net);
        }
    }
}
#endif
