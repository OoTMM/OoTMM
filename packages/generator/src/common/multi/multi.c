#include "multi.h"

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

static void Multi_ProcessMessagesConnected(void)
{
    gSave.info.playerData.rupees++;
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
}
