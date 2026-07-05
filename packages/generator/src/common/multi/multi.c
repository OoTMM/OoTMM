#include <combo/mark.h>
#include "multi.h"

#if defined(GAME_OOT)
# define GAME_ID 0
#else
# define GAME_ID 1
#endif

#define HELLO_MAGIC "OoTMM\x7f\x01\x00"

MultiState gMulti;

static u8 sSessionId[16];
EXPORT_SYMBOL(MULTI_SESSION_ID, sSessionId);

static u8 sSessionSecret[8];
EXPORT_SYMBOL(MULTI_SESSION_SECRET, sSessionSecret);

static u8 sPlayerId[16];
EXPORT_SYMBOL(MULTI_PLAYER_ID, sPlayerId);

static u8 sWorldId;
EXPORT_SYMBOL(MULTI_WORLD_ID, sWorldId);

ALIGNED(4) static u8 sBuffer[128];

static int Multi_SendHello(void)
{
    MultiPacketHelloOut pkt;

    pkt.header.seq = 0;
    pkt.header.op = MULTI_OP_HELLO;
    memcpy(pkt.magic, HELLO_MAGIC, sizeof(pkt.magic));
    memcpy(pkt.sessionId, sSessionId, sizeof(pkt.sessionId));
    memcpy(pkt.sessionSecret, sSessionSecret, sizeof(pkt.sessionSecret));
    memcpy(pkt.playerId, sPlayerId, sizeof(pkt.playerId));
    pkt.playerName[0] = 'T';
    pkt.playerName[1] = 'E';
    pkt.playerName[2] = 'S';
    pkt.playerName[3] = 'T';
    pkt.playerName[4] = 0;
    pkt.playerName[5] = 0;
    pkt.playerName[6] = 0;
    pkt.playerName[7] = 0;
    pkt.worldId = sWorldId;
    pkt.multi = Config_Flag(CFG_MULTIPLAYER) ? 0x01 : 0x00;

    return IPC_Write(&pkt, sizeof(pkt));
}

static void Multi_ProcessMessagesDisconnected(void)
{
    int size;

    size = IPC_Read(sBuffer, sizeof(sBuffer));
    if (size < sizeof(MultiPacketHelloIn))
        return;
    MultiPacketHelloIn* pkt = (MultiPacketHelloIn*)sBuffer;
    if (pkt->header.op != MULTI_OP_HELLO)
        return;
    if (memcmp(pkt->magic, HELLO_MAGIC, sizeof(pkt->magic)) != 0)
        return;
    if (pkt->header.seq != 0)
        return;

    gMulti.isConnected = 1;
    gMulti.seqGame = pkt->seqGame;
    gMulti.seqNet = pkt->seqNet;

    return;
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
        q.gi = GI_RECOVERY_HEART;
    }

    Item_AddWithDecoy(play, &q);
}

u8 Multi_WorldID(void)
{
    return sWorldId;
}

static int MultiProcessMessageItemWAL(MultiPacketWalItemIn* pkt, int size)
{
    PlayState* play;
    s16 gi;
    u8 ovType;
    u8 sceneId;
    u8 roomId;
    u8 id;
    u8 isMarked;
    u8 isSamePlayer;
    u8 needsMark;

    if (!Item_IsPlayerSelf(pkt->to))
        return 1;
    play = gPlay;

    gSave.info.playerData.rupees = 13;

    /* The item is for us, we just need to make sure it's safe to get it */
    gi = pkt->gi;
    isMarked = 0;
    needsMark = 0;
    isSamePlayer = Item_IsPlayerSelf(pkt->from);
    if (isSamePlayer)
    {
        if (!(pkt->flags & OVF_RENEW))
        {
            needsMark = 1;
            ovType = (pkt->key >> 24) & 0xff;
            sceneId = (pkt->key >> 16) & 0xff;
            roomId = (pkt->key >> 8) & 0xff;
            id = pkt->key & 0xff;
            if (pkt->game)
                isMarked = Mark_GetMm(play, ovType, sceneId, roomId, id);
            else
                isMarked = Mark_GetOot(play, ovType, sceneId, roomId, id);
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
        if (!Item_SafeToReceive(play) || g.decoysCount)
            return 0;
        Multi_GiveItem(play, gi, pkt->from, pkt->flags);
        if (needsMark)
        {
            if (pkt->game)
                Mark_SetMm(play, ovType, sceneId, roomId, id);
            else
                Mark_SetOot(play, ovType, sceneId, roomId, id);
        }
    }

    return 1;
}

static int MultiProcessMessageWAL(MultiPacketWalInHeader* pkt, int size)
{
    int increment;

    gSave.info.playerData.rupees = 9;
    if (pkt->index != gSharedCustomSave.walIndex)
        return 1;

    gSave.info.playerData.rupees = 10;
    gSave.info.playerData.rupees = pkt->type;

    increment = 0;
    switch (pkt->type)
    {
    case WAL_ITEM:
        gSave.info.playerData.rupees = 11;
        if (size < sizeof(MultiPacketWalItemIn))
            return 0;
        gSave.info.playerData.rupees = 12;
        MultiPacketWalItemIn* itemPkt = (MultiPacketWalItemIn*)pkt;
        increment = MultiProcessMessageItemWAL(itemPkt, size);
        break;
    }

    if (increment)
        gSharedCustomSave.walIndex++;

    return 1;
}

static int MultiProcessMessage(MultiPacketHeader* pkt, int size)
{
    /* DEBUG */
    gSave.info.playerData.rupees = 6;

    switch (pkt->op)
    {
    case MULTI_OP_WAL:
        gSave.info.playerData.rupees = 7;
        if (size < sizeof(MultiPacketWalInHeader))
            return 0;
        gSave.info.playerData.rupees = 8;
        MultiPacketWalInHeader* walPkt = (MultiPacketWalInHeader*)pkt;
        return MultiProcessMessageWAL(walPkt, size);
    }

    return 1;
}

static void Multi_ProcessMessagesConnected(void)
{
    int size;

    gSave.info.playerData.rupees = 1;
    for (;;)
    {
        size = IPC_Read(sBuffer, sizeof(sBuffer));
        if (size <= 0)
            break;

        gSave.info.playerData.rupees = 2;

        if (size < sizeof(MultiPacketHeader))
        {
            gMulti.isConnected = 0;
            return;
        }

        gSave.info.playerData.rupees = 3;

        MultiPacketHeader* pkt = (MultiPacketHeader*)sBuffer;
        if (pkt->seq != gMulti.seqNet++)
        {
            gMulti.isConnected = 0;
            return;
        }

        gSave.info.playerData.rupees = 4;

        if (!MultiProcessMessage(pkt, size))
        {
            gMulti.isConnected = 0;
            return;
        }
    }
}

static void Multi_ProcessMessages(void)
{
    if (gMulti.isConnected)
        Multi_ProcessMessagesConnected();
    else
        Multi_ProcessMessagesDisconnected();
}

static void Multi_TryConnect(void)
{
    if (gMulti.ttl)
        return;

    gMulti.ttl = 30;
    if (!Multi_SendHello())
        return;
}

static int Multi_SendPacket(MultiPacketHeader* pkt, u32 size)
{
    pkt->seq = gMulti.seqGame++;
    return IPC_Write(pkt, size);
}

static void Multi_QueryWal(void)
{
    MultiPacketWalQuery pkt;
    pkt.header.op = MULTI_OP_WAL_QUERY;
    pkt.index = gSharedCustomSave.walIndex;
    if (!Multi_SendPacket(&pkt.header, sizeof(pkt)))
        gMulti.isConnected = 0;
}

void Multi_Update(PlayState* play)
{
    /* Ignore non-play */
    /* Decrease TTL, if any */
    if (gMulti.ttl)
        --gMulti.ttl;

    /* Refresh the IPC state and detect disconnects */
    IPC_Refresh();
    if (gMulti.isConnected && !IPC_IsConnected())
    {
        gMulti.isConnected = 0;
        gMulti.ttl = 0;
    }

    if (!gMulti.isConnected && IPC_IsConnected())
        Multi_TryConnect();

    if (IPC_IsConnected())
        Multi_ProcessMessages();

    if (gMulti.isConnected)
    {
        Multi_QueryWal();
    }
}

void Multi_Disconnect(void)
{
    gMulti.isConnected = 0;
    gMulti.ttl = 0;
}

void Multi_SendItem(u8 to, s16 gi, s16 flags, u32 key)
{
    /* TODO: Store in persistance */
    if (!gMulti.isConnected)
        return;

    MultiPacketWalItemOut pkt;
    pkt.wal.header.op = MULTI_OP_WAL;
    pkt.wal.type = WAL_ITEM;
    pkt.to = to;
    pkt.game = GAME_ID;
    pkt.gi = gi;
    pkt.flags = flags;
    pkt.key = key;

    Multi_SendPacket(&pkt.wal.header, sizeof(pkt));
}

void Multi_SendSelfItem(s16 gi, s16 flags, u32 key)
{
    Multi_SendItem(sWorldId, gi, flags, key);
}
