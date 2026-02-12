#include <combo.h>
#include <combo/multi.h>
#include <sys/emu.h>

#define MAX_PACKET_SIZE 512

#define OP_NOP              0x00
#define OP_WRITE_WAL_ITEM   0x01

typedef struct
{
    u32     token;
    u32     id;
    char    data[MAX_PACKET_SIZE - 8];
    u16     size;
}
MultiPacket;

typedef struct
{
    char        sessionId[16];
    u32         sessionSecret;
    u64         playerUniqueId;
    u8          playerId;
    u8          teamId;
    u32         token;
    u32         nextPacketId;
    int         isConnected;
    MultiPacket pkt;
}
MultiContext;

MultiContext sCtx;
EXPORT_SYMBOL(MULTI_SESSION_ID, sCtx.sessionId);
EXPORT_SYMBOL(MULTI_SESSION_SECRET, sCtx.sessionSecret);
EXPORT_SYMBOL(MULTI_PLAYER_UNIQUE_ID, sCtx.playerUniqueId);
EXPORT_SYMBOL(MULTI_PLAYER_ID, sCtx.playerId);
EXPORT_SYMBOL(MULTI_TEAM_ID, sCtx.teamId);

int MultiEx_IsMultiplayer(void)
{
    return Config_Flag(CFG_MULTIPLAYER);
}

int MultiEx_IsSupported(void)
{
    static char sSupported = -1;

    if (sSupported < 0)
    {
        if (emuSysCount() >= 6)
            sSupported = 1;
        else
            sSupported = 0;
    }
    return sSupported;
}

static int Multi_InitSession(void)
{
    char buf[128];

    memcpy(buf + 0, "OoTMM\x00", 6);
    memcpy(buf + 6, sCtx.sessionId, 16);
    memcpy(buf + 22, &sCtx.sessionSecret, 4);
    memcpy(buf + 26, &sCtx.playerUniqueId, 8);
    buf[34] = sCtx.playerId;
    buf[35] = sCtx.teamId;

    if (emuSysSendIPC(buf, 36) < 0)
        return 0;
    if (emuSysRecvIPC(buf, 14) < 14)
        return 0;
    if (memcmp(buf, "OoTMM\x00", 6) != 0)
        return 0;
    memcpy(&sCtx.token, &buf[6], 4);
    memcpy(&sCtx.nextPacketId, &buf[10], 4);

    return 1;
}

static int MultiEx_OpenImpl(void)
{
    if (MultiEx_IsMultiplayer())
    {
        if (!MultiEx_IsSupported())
            Fault_AddHungupAndCrashImpl("Multiplayer not supported", "Please use a supported system");

        for (;;)
        {
            if (emuSysValidIPC() >= 0)
                return 1;
            if (emuSysOpenIPC() >= 0 && Multi_InitSession())
                return 1;
        }
    }

    return 0;
}

int MultiEx_Open(void)
{
    if (sCtx.isConnected)
        return 1;

    sCtx.isConnected = MultiEx_OpenImpl();
    return sCtx.isConnected;
}

void MultiEx_Close(void)
{
    if (sCtx.isConnected)
    {
        emuSysCloseIPC();
        sCtx.isConnected = 0;
    }
}

void MultiEx_Update(PlayState* play)
{
    /* Handle title screen and other similar stuff */
    if (gSaveContext.fileNum == 0xff)
    {
        MultiEx_Close();
        return;
    }

    /* Otherwise, make sure the network is open */
    if (!MultiEx_Open())
        return;
}

static int MultiEx_ProtocolSend(void)
{
    sCtx.pkt.token = sCtx.token;
    sCtx.pkt.id = sCtx.nextPacketId++;

    if (emuSysSendIPC(&sCtx.pkt, sCtx.pkt.size + 8) < 0)
        return 0;
    return 1;
}

static int MultiEx_ProtocolRecv(void)
{
    MultiPacket* pkt;
    int res;

    pkt = &sCtx.pkt;
    res = emuSysRecvIPC(pkt, MAX_PACKET_SIZE);
    if (res < 8)
        return 0;
    if (pkt->token != sCtx.token)
        return 0;
    if (pkt->id != sCtx.nextPacketId)
        return 0;
    sCtx.nextPacketId++;
    pkt->size = res - 8;
    return 1;
}

static void MultiEx_SendWal(u8 op, const void* data, u16 size)
{
    MultiPacket* pkt;

    if (!sCtx.isConnected && !MultiEx_IsMultiplayer())
        return;

    pkt = &sCtx.pkt;
    for (;;)
    {
        pkt->size = size + 1;
        pkt->data[0] = op;
        memcpy(&pkt->data[1], data, size);

        if (MultiEx_ProtocolSend() && MultiEx_ProtocolRecv())
            return;

        /* There was an error somewhere */
        MultiEx_Close();
        MultiEx_Open();

        /* If we're not in multiplayer, the error isn't fatal */
        if (!MultiEx_IsMultiplayer())
            return;
    }
}

void MultiEx_SendEntryItem(const MultiWriteWalItem* blob)
{
    if (!MultiEx_Open())
        return;

    MultiEx_SendWal(OP_WRITE_WAL_ITEM, blob, sizeof(MultiWriteWalItem));
}
