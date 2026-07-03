#include "multi.h"

#define HELLO_MAGIC "OoTMM\x7f\x01\x00"

MultiState gMulti;

static int Multi_SendHello(void)
{
    MultiPacketHelloOut pkt;

    pkt.header.seqGame = 0;
    pkt.header.seqNet = 0;
    pkt.header.op = MULTI_OP_HELLO;
    memcpy(pkt.magic, HELLO_MAGIC, sizeof(pkt.magic));

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

    if (!gMulti.isConnected)
        Multi_TryConnect();
}
