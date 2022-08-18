#include <combo.h>

void RequestDma(void*, void*, u32, u32, int, void*, int);

static void fixupDisplayList(u32* dl, u32 size)
{
    u32* end = (u32*)((char*)dl + size);

    while (dl < end)
    {
        if (dl[0] == 0xfd900000)
        {
            switch (dl[1])
            {
            case 0x04032930:
                dl[1] = 0x04044bb0;
                break;
            }
        }
        dl += 2;
    }
}

/* TODO: Refactor and share this */
void hook_LoadObjectGi(void* player, u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;
    OSMesg msg;
    OSMesg msgInQ;
    OSMesgQueue msgQ;
    int extended;

    extended = 0;
    if (objectId)
    {
        *((u8*)player + 0x1b8) = 1;

        if (objectId & 0x1000)
        {
            table = kExtraObjectsTable;
            objectId &= 0xfff;
            extended = 1;
        }
        else
        {
            table = kObjectsTable;
        }

        vromStart = table[objectId].vromStart;
        vromEnd = table[objectId].vromEnd;

        osCreateMesgQueue((OSMesgQueue*)((char*)player + 0x1dc), (OSMesg*)((char*)player + 0x1f4), 1);

        if (extended)
        {
            osCreateMesgQueue(&msgQ, &msgInQ, 1);
            RequestDma(
                (char*)player + 0x1bc,
                *(void**)((char*)player + 0x1f8),
                vromStart,
                vromEnd - vromStart,
                0,
                &msgQ,
                0
            );
            osRecvMesg(&msgQ, &msg, 1);
            fixupDisplayList(*(u32**)((char*)player + 0x1f8), vromEnd - vromStart);
            osSendMesg((OSMesgQueue*)((char*)player + 0x1dc), msg, 1);
        }
        else
        {
            RequestDma(
                (char*)player + 0x1bc,
                *(void**)((char*)player + 0x1f8),
                vromStart,
                vromEnd - vromStart,
                0,
                (char*)player + 0x1dc,
                0
            );
        }
    }
}
