#ifndef COMBO_SHADER_H
#define COMBO_SHADER_H

#include <combo/game_state.h>
#include <combo/types.h>

typedef void (*ShaderFunc)(GameState* gs, s16 index);

typedef struct PACKED ALIGNED(4)
{
    ShaderFunc  func;
    u32         lists[8];
}
Shader;

extern const Shader kShaders[256];

#endif
