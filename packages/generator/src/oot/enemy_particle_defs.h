#ifndef COMBO_ENEMY_PARTICLE_DEFS_H
#define COMBO_ENEMY_PARTICLE_DEFS_H

#define ENEMY_PARTICLE_BILLBOARD 1
#define ENEMY_PARTICLE_ROTATE_Z 2
#define ENEMY_MM_ICE_SHARD_OBJECT 0x1167
#define ENEMY_MM_ICE_SHARD_MODEL 0x06000a38

typedef struct
{
    u16 objectId;
    u16 keepObjectId;
    u32 model;
    s16 life;
    u8 priority;
    u8 flags;
    float scaleBase;
    float scaleUnits;
    float initialRotationRange;
    float spinRange;
    s16 spinBase;
} EnemyParticleDescriptor;

#endif
