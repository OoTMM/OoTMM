#include "multi.h"
#include <sys/emu.h>

int Multi_ProtocolSend(void)
{
    gMultiCtx.pkt.token = gMultiCtx.token;
    gMultiCtx.pkt.id = gMultiCtx.nextPacketId++;

    if (emuSysSendIPC(&gMultiCtx.pkt, gMultiCtx.pkt.size + 8) < 0)
        return 0;
    return 1;
}

int Multi_ProtocolRecv(void)
{
    MultiPacket* pkt;
    int res;

    pkt = &gMultiCtx.pkt;
    res = emuSysRecvIPC(pkt, MAX_PACKET_SIZE);
    if (res < 8)
        return 0;
    if (pkt->token != gMultiCtx.token)
        return 0;
    if (pkt->id != gMultiCtx.nextPacketId)
        return 0;
    gMultiCtx.nextPacketId++;
    pkt->size = res - 8;
    return 1;
}
