#include <combo.h>
#include <combo/custom.h>

#define AGE_MODEL_CMD_END     0x00000000
#define AGE_MODEL_CMD_WRITE16 0x00000001
#define AGE_MODEL_CMD_WRITE32 0x00000002
#define AGE_MODEL_CMD_COPY32  0x00000003

static u32 AgeModel_ReadU32(const u8* p)
{
    return ((u32)p[0] << 24) |
           ((u32)p[1] << 16) |
           ((u32)p[2] << 8) |
           ((u32)p[3]);
}

static void ComboPlayer_RestoreChildModelTables(void)
{
    u8 data[CUSTOM_MM_AGE_MODEL_CHILD_TABLES_SIZE];
    u8* p;
    u32 addr;
    u32 size;

    LoadFile(data, CUSTOM_MM_AGE_MODEL_CHILD_TABLES_VROM, CUSTOM_MM_AGE_MODEL_CHILD_TABLES_SIZE);

    p = data;

    for (;;)
    {
        addr = AgeModel_ReadU32(p + 0x00);
        size = AgeModel_ReadU32(p + 0x04);
        p += 0x08;

        if (addr == 0 || size == 0)
            break;

        memcpy((void*)addr, p, size);
        osWritebackDCache((void*)addr, size);

        p += size;
    }
}

static void ComboPlayer_ApplyAdultModelCommandRaw(u32 op, u32 addr, u32 value)
{
    switch (op)
    {
        case AGE_MODEL_CMD_WRITE16:
            *(u16*)addr = (u16)value;
            osWritebackDCache((void*)addr, sizeof(u16));
            break;

        case AGE_MODEL_CMD_WRITE32:
            *(u32*)addr = value;
            osWritebackDCache((void*)addr, sizeof(u32));
            break;

        case AGE_MODEL_CMD_COPY32:
            *(u32*)addr = *(u32*)value;
            osWritebackDCache((void*)addr, sizeof(u32));
            break;
    }
}

static void ComboPlayer_ApplyAdultModelTables(void)
{
    u8 cmds[CUSTOM_MM_AGE_MODEL_TABLES_SIZE];
    u8* p;
    u32 op;
    u32 addr;
    u32 value;

    LoadFile(cmds, CUSTOM_MM_AGE_MODEL_TABLES_VROM, CUSTOM_MM_AGE_MODEL_TABLES_SIZE);

    p = cmds;

    for (;;)
    {
        op = AgeModel_ReadU32(p + 0x00);
        addr = AgeModel_ReadU32(p + 0x04);
        value = AgeModel_ReadU32(p + 0x08);
        p += 0x0c;

        if (op == AGE_MODEL_CMD_END)
            break;

        ComboPlayer_ApplyAdultModelCommandRaw(op, addr, value);
    }
}

void ComboPlayer_ApplyAgeModelTablesForAdult(s32 isAdult)
{
    ComboPlayer_RestoreChildModelTables();

    if (isAdult)
        ComboPlayer_ApplyAdultModelTables();
    osWritebackDCache((void*)0x801bfe00, 0x4f8);
    osWritebackDCache((void*)0x801c0d78, 0x20);
    osWritebackDCache((void*)0x801c2730, 0x10);
    osWritebackDCache((void*)0x801dca58, 0x14);
}

void ComboPlayer_ApplyAgeModelTables(void)
{
    ComboPlayer_ApplyAgeModelTablesForAdult(comboIsLinkAdult());
}