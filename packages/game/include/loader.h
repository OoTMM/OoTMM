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

#endif
