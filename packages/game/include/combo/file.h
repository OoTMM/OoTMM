#ifndef INCLUDED_COMBO_FILE_H
#define INCLUDED_COMBO_FILE_H

#include <PR/ultratypes.h>

#define FILEID_OOT_DMADATA  0x00000002
#define FILEID_MM_DMADATA   0x00000003


typedef struct
{
    u32 id;
    u32 offset;
    u32 size;
    u32 flags;
}
FileInfo;

typedef struct
{
    u32 tableOffset;
    u32 tableEntries;
}
FileConfig;

extern const FileConfig kLoaderFileConfig;

typedef struct
{
    u32 vstart;
    u32 vsize;
    u32 id;
}
FileDmaData;

u32 File_Offset(int index);
u32 File_Size(int index);
u32 File_Flags(int index);
int File_IndexFromID(u32 id);
int File_DmaData(u32 dmaId, u32 vaddr, FileDmaData* out);

void  File_CacheReset(void);
void  File_CacheTick(void);
void* File_CacheLoad(u32 id);

#endif
