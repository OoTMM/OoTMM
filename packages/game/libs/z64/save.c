#include <combo.h>

void Flash_Read(void* dst, u32 offset, u32 size);
void Flash_Write(void* src, u32 offset, u32 size);

u32 SaveRaw_Checksum(void)
{
    u32 sum;
    u32* ptr;

    sum = 0;
    ptr = (u32*)&gSave;
    for (u32 i = 0; i < (sizeof(gSave) / sizeof(u32)) - 1; i++)
    {
        sum += ptr[i];
    }

    return sum;
}

static u32 SaveRaw_Offset(int id, int isBackup)
{
    return 0x2000 + sizeof(Save) * id * 2 + (!!isBackup) * sizeof(Save);
}

int SaveRaw_Read(void)
{
    return SaveRaw_ReadFrom(gSaveFileNum);
}

static int SaveRaw_TryReadFrom(u32 addr)
{
    u32 sum;

    Flash_Read(&gSave, addr, sizeof(gSave));
    sum = SaveRaw_Checksum();
    return (sum == gSave.checksum);
}

int SaveRaw_ReadFrom(int id)
{
    u32 addr;

    addr = SaveRaw_Offset(id, 0);
    if (SaveRaw_TryReadFrom(addr))
        return 1;
    addr = SaveRaw_Offset(id, 1);
    return SaveRaw_TryReadFrom(addr);
}

void SaveRaw_Write(void)
{
    SaveRaw_WriteTo(gSaveFileNum);
}

void SaveRaw_WriteTo(int id)
{
    u32 addr;

    gSave.checksum = SaveRaw_Checksum();
    addr = SaveRaw_Offset(id, 0);
    Flash_Write(&gSave, addr, sizeof(gSave));
    addr = SaveRaw_Offset(id, 1);
    Flash_Write(&gSave, addr, sizeof(gSave));
}

void SaveRaw_OptionsRead(void)
{
    Flash_Read(&gSaveOptions, 0, sizeof(gSaveOptions));
}

void SaveRaw_OptionsWrite(void)
{
    Flash_Write(&gSaveOptions, 0, sizeof(gSaveOptions));
}
