#include <combo.h>
#include <combo/multi.h>
#include <sys/emu.h>

#define OP_NOP          0x00
#define OP_ENTRY_PUSH   0x01

#define ENTRY_TYPE_ITEM 0x01

static char sMultiId[16];
EXPORT_SYMBOL(MULTI_ID, sMultiId);

static char sSocketConnected = 0;

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
    if (sSocketConnected)
        return 1;

    sSocketConnected = MultiEx_OpenImpl();
    return sSocketConnected;
}

void MultiEx_Close(void)
{
    if (sSocketConnected)
    {
        emuSysSocketClose(0);
        sSocketConnected = 0;
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

static u32 crc32(const void* data, u32 size)
{
    u32 crc = 0xffffffff;
    const u8* p = (const u8*)data;
    for (u32 i = 0; i < size; i++)
    {
        crc ^= p[i];
        for (u8 j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xedb88320;
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xffffffff;
}

static int Multi_SendPacket(const void* data, u16 size)
{
    u32 checksum;
    char header[6];

    checksum = crc32(data, size);
    memcpy(&header[0], &size, 2);
    memcpy(&header[2], &checksum, 4);
    if (!MultiEx_SendFull(header, 6))
        return 0;
    if (!MultiEx_SendFull(data, size))
        return 0;
    return 1;
}

static void MultiEx_SendEntry(u8 type, const void* data, u16 size)
{
    char buf[64];
    char retBuf[1];

    if (!sSocketConnected && !MultiEx_IsMultiplayer())
        return;

    buf[0] = OP_ENTRY_PUSH;
    buf[1] = type;
    memcpy(&buf[2], data, size);

    for (;;)
    {
        if (Multi_SendPacket(buf, size + 2))
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
