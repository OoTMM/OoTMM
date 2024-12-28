#ifndef SAVE_H
#define SAVE_H

#include <save/oot.h>
#include <save/mm.h>

typedef struct
{
    OotSave oot;
    MmSave  mm;
    u8      unused[0x6000 - sizeof(OotSave) - sizeof(MmSave) - 4];
    u32     checksum;
}
Save;

typedef union
{
    struct
    {
        char magic[16];
        u8   audioSetting;
        u8   zTargetSetting;
    };
    u8 raw[128];
}
SaveOptions;

extern Save gSave;
extern SaveOptions gSaveOptions;
extern s8 gSaveFileNum;

#define gOotSave (gSave.oot)
#define gMmSave  (gSave.mm)

u32  SaveRaw_Checksum(void);
int  SaveRaw_Read(void);
int  SaveRaw_ReadFrom(int id);
void SaveRaw_Write(void);
void SaveRaw_WriteTo(int id);
void SaveRaw_OptionsRead(void);
void SaveRaw_OptionsWrite(void);

#endif
