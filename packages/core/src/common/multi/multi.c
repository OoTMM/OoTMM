#include "multi.h"
#include <sys/emu.h>

MultiContext gMultiCtx;
EXPORT_SYMBOL(MULTI_SESSION_ID, gMultiCtx.sessionId);
EXPORT_SYMBOL(MULTI_SESSION_SECRET, gMultiCtx.sessionSecret);
EXPORT_SYMBOL(MULTI_PLAYER_UNIQUE_ID, gMultiCtx.playerUniqueId);
EXPORT_SYMBOL(MULTI_PLAYER_ID, gMultiCtx.playerId);

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

int Multi_IsMultiplayer(void)
{
    return Config_Flag(CFG_MULTIPLAYER);
}

int Multi_IsSupported(void)
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
    Multi_ExportFilename(buf + 6);
    memcpy(buf + 14, gMultiCtx.sessionId, 16);
    memcpy(buf + 30, &gMultiCtx.sessionSecret, 4);
    memcpy(buf + 34, &gMultiCtx.playerUniqueId, 8);
    buf[42] = gMultiCtx.playerId;

    if (emuSysSendIPC(buf, 43) < 0)
        return 0;
    if (emuSysRecvIPC(buf, 14) < 14)
        return 0;
    if (memcmp(buf, "OoTMM\x00", 6) != 0)
        return 0;
    memcpy(&gMultiCtx.token, &buf[6], 4);
    memcpy(&gMultiCtx.nextPacketId, &buf[10], 4);

    return 1;
}

static int Multi_OpenImpl(void)
{
    if (Multi_IsMultiplayer())
    {
        if (!Multi_IsSupported())
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

int Multi_Open(void)
{
    if (gMultiCtx.isConnected)
        return 1;

    gMultiCtx.isConnected = Multi_OpenImpl();
    return gMultiCtx.isConnected;
}

void Multi_Close(void)
{
    if (gMultiCtx.isConnected)
    {
        emuSysCloseIPC();
        gMultiCtx.isConnected = 0;
    }
}

void Multi_Update(PlayState* play)
{
    /* Handle title screen and other similar stuff */
    if (gSaveContext.fileNum == 0xff)
    {
        Multi_Close();
        return;
    }

    /* Otherwise, make sure the network is open */
    if (!Multi_Open())
        return;

    Multi_WispsUpdate(play);
}

static void Multi_SendWal(u8 op, const void* data, u16 size)
{
    MultiPacket* pkt;

    if (!gMultiCtx.isConnected && !Multi_IsMultiplayer())
        return;

    pkt = &gMultiCtx.pkt;
    for (;;)
    {
        pkt->size = size + 1;
        pkt->data[0] = op;
        memcpy(&pkt->data[1], data, size);

        if (Multi_ProtocolSend() && Multi_ProtocolRecv())
            return;

        /* There was an error somewhere */
        Multi_Close();
        Multi_Open();

        /* If we're not in multiplayer, the error isn't fatal */
        if (!Multi_IsMultiplayer())
            return;
    }
}

void Multi_SendEntryItem(const MultiWriteWalItem* blob)
{
    if (!Multi_Open())
        return;

    Multi_SendWal(OP_WRITE_WAL_ITEM, blob, sizeof(MultiWriteWalItem));
}
