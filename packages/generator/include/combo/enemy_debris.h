#ifndef COMBO_ENEMY_DEBRIS_H
#define COMBO_ENEMY_DEBRIS_H

#include <combo.h>
#include <combo/effect.h>

#define ENEMY_DEBRIS_ACTOR_BUDGET 24
#define ENEMY_DEBRIS_SHARD_LIMIT 24
#define ENEMY_DEBRIS_BUBBLE_LIMIT 24
#define ENEMY_DEBRIS_DEATH_FX_LIMIT 32

#define ENEMY_DEBRIS_ACTOR_OPA_MIN 0x3000u
#define ENEMY_DEBRIS_ACTOR_XLU_MIN 0x1000u
#define ENEMY_DEBRIS_EFFECT_OPA_MIN 0x1800u
#define ENEMY_DEBRIS_EFFECT_XLU_MIN 0x0800u

static inline unsigned comboEnemyDebrisGfxFree(const DisplayListBuffer* buf)
{
    uintptr_t start = (uintptr_t)buf->ptr;
    uintptr_t head = (uintptr_t)buf->append;
    uintptr_t tail = (uintptr_t)buf->end;
    if (!start || head < start || tail < head || tail - start > buf->size)
        return 0;
    return (unsigned)(tail - head);
}

static inline int comboEnemyDebrisGfxRoom(const GraphicsContext* gfx, unsigned opa, unsigned xlu)
{
    return gfx && comboEnemyDebrisGfxFree(&gfx->polyOpa) >= opa &&
           comboEnemyDebrisGfxFree(&gfx->polyXlu) >= xlu;
}

static inline int comboEnemyDebrisHiploop(u16 params)
{
    return params == 8 || params == 10 || params == 13 || params == 17 || params == 23 || params == 26;
}

static inline int comboEnemyDebrisOotPart(u16 params)
{
    /* 12/13 reassemble Stalfos, negative params can be damaging shockwaves.
     * Keep 5..8 (armor-related) unrestricted at spawn as well. */
    return (params >= 1 && params <= 4) || params == 9 || params == 10 || params == 11 || params == 14;
}

static __attribute__((noinline, unused)) unsigned comboEnemyDebrisWeight(s16 id, u16 params)
{
#if defined(GAME_OOT)
    if (id == 0xd2d && params >= 10 && params < 24)
        return 4;
    if (id == 0xde9 && comboEnemyDebrisHiploop(params))
        return 2;
    if (id == 0xd80 && params >= 10 && params <= 14)
        return 1;
    if ((id == ACTOR_EN_PART && comboEnemyDebrisOotPart(params)) ||
        (id == ACTOR_EN_BA && params == 3) ||
        (id == ACTOR_EN_GOMA && params >= 10 && params <= 24))
        return 1;
#else
    if (id == ACTOR_BOSS_05 && params >= 10 && params < 24)
        return 4;
    if (id == ACTOR_EN_PP && comboEnemyDebrisHiploop(params))
        return 2;
    if ((id == ACTOR_EN_PR2 && params >= 10 && params <= 14) ||
        (id == ACTOR_EN_PART && (params == 1 || params == 4)) ||
        (id == 0xc07 && comboEnemyDebrisOotPart(params)) ||
        (id == 0xcde && params == 3) ||
        (id == 0xc2b && params >= 10 && params <= 24))
        return 1;
#endif
    return 0;
}

int comboEnemyDebrisActorAllowed(ActorContext* ctx, s16 id, u16 params);
void comboEnemyDebrisPostFrame(PlayState* play);

static inline unsigned comboEnemyDebrisEffectClass(s32 type)
{
    if (type == 0x0f || type == 0x19 || type == 0x1a || type == 0x1b)
        return 1;
    if (type == 0x07 || type == 0x0e)
        return 2;
    if (type == 0x00 || (type >= 0x20 && type <= 0x22))
        return 3;
    return 0;
}

static inline int comboEnemyDebrisEffectAllowed(s32 type)
{
    unsigned cls = comboEnemyDebrisEffectClass(type);
    unsigned used = 0;
    unsigned limit;
    if (!cls)
        return 1;
    if (!gEffectSsTable.data)
        return 0;

    limit = cls == 1 ? ENEMY_DEBRIS_SHARD_LIMIT :
            cls == 2 ? ENEMY_DEBRIS_BUBBLE_LIMIT : ENEMY_DEBRIS_DEATH_FX_LIMIT;
    for (u32 i = 0; i < gEffectSsTable.size; ++i)
    {
        EffectSs* effect = &gEffectSsTable.data[i];
        if (effect->life >= 0 && comboEnemyDebrisEffectClass(effect->type) == cls && ++used >= limit)
            return 0;
    }
    return 1;
}

#endif
