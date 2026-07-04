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
}
