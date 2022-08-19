#ifndef COMBO_SHADER_H
#define COMBO_SHADER_H

#include <types.h>

typedef struct PACKED ALIGNED(4)
{
    void* func;
    u32   lists[8];
}
Shader;

extern const Shader kShaders[256];

#endif
