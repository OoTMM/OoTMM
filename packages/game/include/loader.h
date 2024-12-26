#ifndef LOADER_H
#define LOADER_H

#include <PR/ultratypes.h>

typedef struct
{
    u32 id;
    u32 start;
    u32 size;
    void (*entrypoint)(void);
}
GameConfig;

extern GameConfig gLoaderGameConfigs[4];

#endif
