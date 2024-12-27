#ifndef LOADER_H
#define LOADER_H

#include <PR/ultratypes.h>

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

#endif
