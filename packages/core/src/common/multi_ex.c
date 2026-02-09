#include <combo.h>
#include <combo/multi.h>
#include <sys/emu.h>

#define MAX_PACKET_SIZE 512

#define OP_NOP          0x00
#define OP_ENTRY_PUSH   0x01

#define ENTRY_TYPE_ITEM 0x01

static char sMultiId[16];
EXPORT_SYMBOL(MULTI_ID, sMultiId);

typedef struct
{
    char id[16];
    u32  token;
    u32  nextPacketId;
    char buffer[512];
    u16  bufferSize;
    int  socketConnected;
}
MultiContext;

static MultiContext sCtx;
EXPORT_SYMBOL(MULTI_ID, sCtx.id);

static int MultiEx_SendFull(const void* data, u32 size)
{
    int ret;

    if (size == 0)
        return 1;

    for (;;)
    {
        ret = emuSysSocketSend(0, data, size);
        if (ret < 0)
            return 0;
        if (ret == size)
            return 1;
        data = (const char*)data + ret;
        size -= ret;
    }
}

static int MultiEx_RecvFull(void* data, u32 size)
{
    int ret;

    if (size == 0)
        return 1;

    for (;;)
    {
        ret = emuSysSocketRecv(0, data, size);
        if (ret <= 0)
            return 0;
        if (ret == size)
            return 1;
        data = (char*)data + ret;
        size -= ret;
    }
}

static int MultiEx_SendBufferedPacket(void)
{
    char header[10];

    /* Create the packet header */
    memcpy(&header[0], &sCtx.bufferSize, 2);
    memcpy(&header[2], &sCtx.token, 4);
    memcpy(&header[6], &sCtx.nextPacketId, 4);

    /* Send header */
    if (!MultiEx_SendFull(header, 10))
        return 0;

    if (sCtx.bufferSize)
    {
        /* Send packet data */
        if (!MultiEx_SendFull(sCtx.buffer, sCtx.bufferSize))
            return 0;
    }

    /* Increment next packet id */
    sCtx.nextPacketId++;
    return 1;
}

static int MultiEx_RecvBufferedPacket(void)
{
    char header[10];
    u16 length;
    u32 token;
    u32 packetId;

    /* Receive header */
    if (!MultiEx_RecvFull(header, 10))
        return 0;

    /* Parse header */
    memcpy(&length, &header[0], 2);
    memcpy(&token, &header[2], 4);
    memcpy(&packetId, &header[6], 4);

    /* Validate packet length */
    if (length > MAX_PACKET_SIZE)
        return 0;
    sCtx.bufferSize = length;

    /* Validate token */
    if (token != sCtx.token)
        return 0;

    /* Validate packet id */
    if (packetId != sCtx.nextPacketId)
        return 0;

    /* Receive packet data */
    if (length)
    {
        if (!MultiEx_RecvFull(sCtx.buffer, sCtx.bufferSize))
            return 0;
    }

    /* Increment next packet id */
    sCtx.nextPacketId++;

    return 1;
}

int MultiEx_IsMultiplayer(void)
{
    return Config_Flag(CFG_MULTIPLAYER);
}

int MultiEx_IsSupported(void)
{
    static char sSupported = -1;

    if (sSupported < 0)
    {
        if (emuSysCount() >= 5)
            sSupported = 1;
        else
            sSupported = 0;
    }
    return sSupported;
}

static int Multi_InitSession(void)
{
    char buf[6];

    if (!MultiEx_SendFull("OoTMM\x00", 6))
        return 0;
    if (!MultiEx_RecvFull(buf, 6))
        return 0;
    if (memcmp(buf, "OoTMM\x00", 6) != 0)
        return 0;
    if (!MultiEx_RecvFull(&sCtx.token, 4))
        return 0;
    if (!MultiEx_RecvFull(&sCtx.nextPacketId, 4))
        return 0;

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
            if (emuSysSocketIsValid(0) == 0)
                return 1;
            emuSysSocketClose(0);
            if (emuSysSocketOpen(0) == 0 && Multi_InitSession())
                return 1;
        }
    } else {
        if (emuSysSocketIsValid(0) == 0)
            return 1;
        if (emuSysSocketOpenAsync(0) == 0)
            return 1;
    }

    return 0;
}

int MultiEx_Open(void)
{
    if (sCtx.socketConnected)
        return 1;

    sCtx.socketConnected = MultiEx_OpenImpl();
    return sCtx.socketConnected;
}

void MultiEx_Close(void)
{
    if (sCtx.socketConnected)
    {
        emuSysSocketClose(0);
        sCtx.socketConnected = 0;
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

static void MultiEx_SendEntry(u8 type, const void* data, u16 size)
{
    if (!sCtx.socketConnected && !MultiEx_IsMultiplayer())
        return;

    for (;;)
    {
        sCtx.buffer[0] = type;
        memcpy(&sCtx.buffer[1], data, size);
        sCtx.bufferSize = size + 1;

        if (MultiEx_SendBufferedPacket() && MultiEx_RecvBufferedPacket())
            return;

        /* There was an error somewhere */
        MultiEx_Close();
        MultiEx_Open();

        /* If we're not in multiplayer, the error isn't fatal */
        if (!MultiEx_IsMultiplayer())
            return;
    }
}

void MultiEx_SendEntryItem(const MultiEntryItem* blob)
{
    if (!MultiEx_Open())
        return;

    MultiEx_SendEntry(ENTRY_TYPE_ITEM, blob, sizeof(MultiEntryItem));
}
