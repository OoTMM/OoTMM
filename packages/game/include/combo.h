#ifndef COMBO_H
#define COMBO_H

#include <PR/ultratypes.h>

#define GAME_NONE 0
#define GAME_OOT  1
#define GAME_MM   2

#define FILEID_OOT_DMADATA  0x00000002
#define FILEID_MM_DMADATA   0x00000003

typedef struct
{
    void*   ram;
    u32     rom;
    u32     size;
    void (*entrypoint)(void);
}
GameConfig;

extern const GameConfig gLoaderGameConfigs[];

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

#endif
