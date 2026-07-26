#include <combo/mark.h>
#include <combo/context.h>
#include "multi.h"

#if defined(GAME_OOT)
# define GAME_ID 0
#else
# define GAME_ID 1
#endif

#define TTL_RESEND 60

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

static void Multi_ExportFilename(char* dst)
{
    int len;
    u8 c;

    len = 7;
    while (len)
    {
        if (gSave.info.playerData.playerName[len - 1] != FILENAME_SPACE)
            break;
        len--;
    }

    for (int i = len; i < 8; ++i)
        dst[i] = 0;

    for (int i = 0; i < len; ++i)
    {
        c = (u8)gSave.info.playerData.playerName[i];
        if (c == FILENAME_DASH)
            c = '-';
        else if (c == FILENAME_PERIOD)
            c = '.';
        else if (c == FILENAME_SPACE)
            c = ' ';
        else if (c >= FILENAME_DIGIT('0') && c <= FILENAME_DIGIT('9'))
            c = '0' + (c - FILENAME_DIGIT('0'));
        else if (c >= FILENAME_UPPERCASE('A') && c <= FILENAME_UPPERCASE('Z'))
            c = 'A' + (c - FILENAME_UPPERCASE('A'));
        else if (c >= FILENAME_LOWERCASE('a') && c <= FILENAME_LOWERCASE('z'))
            c = 'a' + (c - FILENAME_LOWERCASE('a'));
        else
            c = '_';

        dst[i] = (char)c;
    }
}

static int Multi_SendHello(void)
{
    MultiPacketHelloOut pkt;

    pkt.header.seq = 0;
    pkt.header.op = MULTI_OP_HELLO;
    memcpy(pkt.magic, HELLO_MAGIC, sizeof(pkt.magic));
    memcpy(pkt.sessionId, sSessionId, sizeof(pkt.sessionId));
    memcpy(pkt.sessionSecret, sSessionSecret, sizeof(pkt.sessionSecret));
    memcpy(pkt.playerId, sPlayerId, sizeof(pkt.playerId));
    Multi_ExportFilename((char*)pkt.playerName);
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

    gComboCtx.isMultiConnected = 1;
    gComboCtx.multiSeqGame = pkt->seqGame;
    gComboCtx.multiSeqNet = pkt->seqNet;

    return;
}

static void Multi_GiveItem(PlayState* play, s16 gi, u8 from, int flags, const char* name)
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

    Item_AddWithDecoyNamed(play, &q, name);
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

    /* The item is for us, we just need to make sure it's safe to get it */
    gi = pkt->gi;
    isMarked = 0;
    needsMark = 0;
    isSamePlayer = Item_IsPlayerSelf(pkt->wal.from);
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
            for (int i = 0; i < ARRAY_COUNT(gSharedCustomSave.multi.giSkip); ++i)
            {
                if (gSharedCustomSave.multi.giSkip[i] == gi)
                {
                    isMarked = 1;
                    gSharedCustomSave.multi.giSkip[i] = GI_NONE;
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
        Multi_GiveItem(play, gi, pkt->wal.from, pkt->flags, (char*)pkt->wal.playerName);
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

    if (pkt->index != gSharedCustomSave.multi.walIndex)
        return 1;

    increment = 0;
    switch (pkt->type)
    {
    case WAL_ITEM:
        if (size < sizeof(MultiPacketWalItemIn))
            return 0;
        MultiPacketWalItemIn* itemPkt = (MultiPacketWalItemIn*)pkt;
        increment = MultiProcessMessageItemWAL(itemPkt, size);
        break;
    }

    if (increment)
        gSharedCustomSave.multi.walIndex++;

    return 1;
}

static void MultiProcessAck(MultiPacketWalAckIn* pkt)
{
    if (pkt->token != gSharedCustomSave.multi.sendBufferChecksum)
        return;

    gSharedCustomSave.multi.sendBufferSize = 0;
    gMulti.ttlResend = 0;
}

static int MultiProcessMessage(MultiPacketHeader* pkt, int size)
{
    switch (pkt->op)
    {
    case MULTI_OP_WAL:
        if (size < sizeof(MultiPacketWalInHeader))
            return 0;
        MultiPacketWalInHeader* walPkt = (MultiPacketWalInHeader*)pkt;
        return MultiProcessMessageWAL(walPkt, size);
    case MULTI_OP_WAL_ACK:
        if (size < sizeof(MultiPacketWalAckIn))
            return 0;
        MultiPacketWalAckIn* ackPkt = (MultiPacketWalAckIn*)pkt;
        MultiProcessAck(ackPkt);
        break;
    case MULTI_OP_POSITION:
        if (size < sizeof(MultiPacketPositionIn))
            return 0;
        MultiPacketPositionIn* posPkt = (MultiPacketPositionIn*)pkt;
        Multi_UpdateWisp(gPlay, posPkt);
        break;
    }

    return 1;
}

static void Multi_ProcessMessagesConnected(void)
{
    int size;

    for (;;)
    {
        size = IPC_Read(sBuffer, sizeof(sBuffer));
        if (size <= 0)
            break;

        if (size < sizeof(MultiPacketHeader))
        {
            gComboCtx.isMultiConnected = 0;
            return;
        }

        MultiPacketHeader* pkt = (MultiPacketHeader*)sBuffer;
        if (pkt->seq != gComboCtx.multiSeqNet++)
        {
            gComboCtx.isMultiConnected = 0;
            return;
        }

        if (!MultiProcessMessage(pkt, size))
        {
            gComboCtx.isMultiConnected = 0;
            return;
        }
    }
}

static void Multi_ProcessMessages(void)
{
    if (gComboCtx.isMultiConnected)
        Multi_ProcessMessagesConnected();
    else
        Multi_ProcessMessagesDisconnected();
}

int Multi_SendPacket(MultiPacketHeader* pkt, u32 size)
{
    pkt->seq = gComboCtx.multiSeqGame++;
    return IPC_Write(pkt, size);
}

static void Multi_Resend(void)
{
    if (!Config_Flag(CFG_MULTIPLAYER))
        return;

    if (!gComboCtx.isMultiConnected)
        return;

    if (!gSharedCustomSave.multi.sendBufferSize)
        return;

    if (gMulti.ttlResend)
    {
        gMulti.ttlResend--;
        return;
    }

    gMulti.ttlResend = TTL_RESEND;
    MultiPacketHeader* pkt = (MultiPacketHeader*)&gSharedCustomSave.multi.sendBuffer;
    Multi_SendPacket(pkt, gSharedCustomSave.multi.sendBufferSize);
}

static void Multi_TryConnect(void)
{
    if (gMulti.ttl)
        return;

    gMulti.ttl = 30;
    if (!Multi_SendHello())
        return;
}

static void Multi_QueryWal(void)
{
    MultiPacketWalQuery pkt;
    pkt.header.op = MULTI_OP_WAL_QUERY;
    pkt.index = gSharedCustomSave.multi.walIndex;
    if (!Multi_SendPacket(&pkt.header, sizeof(pkt)))
        gComboCtx.isMultiConnected = 0;
}

void Multi_Update(PlayState* play)
{
    /* Ignore non-play */
    /* Decrease TTL, if any */
    if (gMulti.ttl)
        --gMulti.ttl;

    /* Refresh the IPC state and detect disconnects */
    IPC_Refresh();
    if (gComboCtx.isMultiConnected && !IPC_IsConnected())
    {
        gComboCtx.isMultiConnected = 0;
        gMulti.ttl = 0;
    }

    if (!gComboCtx.isMultiConnected && IPC_IsConnected())
        Multi_TryConnect();

    if (IPC_IsConnected())
    {
        Multi_ProcessMessages();
    }

    if (gComboCtx.isMultiConnected)
    {
        Multi_Resend();
        Multi_QueryWal();
        Multi_SendPosition(play);
    }

    Multi_UpdateWisps();
}

void Multi_Disconnect(void)
{
    gComboCtx.isMultiConnected = 0;
    gMulti.ttl = 0;
}

static void Multi_EnsureSendBufferEmpty(void)
{
    for (;;)
    {
        if (!gSharedCustomSave.multi.sendBufferSize)
            return;

        if (gMulti.ttl)
            --gMulti.ttl;

        IPC_Refresh();
        if (gComboCtx.isMultiConnected && !IPC_IsConnected())
        {
            gComboCtx.isMultiConnected = 0;
            gMulti.ttl = 0;
        }

        if (!gComboCtx.isMultiConnected && IPC_IsConnected())
            Multi_TryConnect();

        if (IPC_IsConnected())
            Multi_ProcessMessages();

        if (gComboCtx.isMultiConnected)
        {
            Multi_Resend();
        }

        Sleep_Usec(1000000 / 20);
    }
}

void Multi_SendItem(u8 to, s16 gi, s16 flags, u32 key)
{
    u32 token;

    if (Config_Flag(CFG_MULTIPLAYER))
        Multi_EnsureSendBufferEmpty();
    else if (!gComboCtx.isMultiConnected)
        return;

    MultiPacketWalItemOut pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.wal.token = 0;
    pkt.wal.header.op = MULTI_OP_WAL;
    pkt.wal.type = WAL_ITEM;
    pkt.to = to;
    pkt.game = GAME_ID;
    pkt.gi = comboItemResolve(gPlay, gi);
    pkt.flags = flags;
    pkt.key = key;

    token = Multi_CRC32(&pkt, sizeof(pkt));
    pkt.wal.token = token;

    Multi_SendPacket(&pkt.wal.header, sizeof(pkt));

    if (Config_Flag(CFG_MULTIPLAYER))
    {
        /* Store persistently */
        memcpy(&gSharedCustomSave.multi.sendBuffer, &pkt, sizeof(pkt));
        gSharedCustomSave.multi.sendBufferSize = sizeof(pkt);
        gSharedCustomSave.multi.sendBufferChecksum = token;
        gMulti.ttlResend = TTL_RESEND;
    }
}

void Multi_SendSelfItem(s16 gi, s16 flags, u32 key)
{
    Multi_SendItem(sWorldId, gi, flags, key);
}
