#ifndef COMBO_OOT_VARIANT_COMPAT_H
#define COMBO_OOT_VARIANT_COMPAT_H

#include "oot_compat.h"

#define COLLECTIBLE_DROP_TABLE_1 1
#define COLLECTIBLE_DROP_TABLE_2 2
#define COLLECTIBLE_DROP_TABLE_13 13
#define BODYBREAK_OBJECT_SLOT_DEFAULT -1
#define BODYBREAK_STATUS_FINISHED 0
#include <combo/enemy_bodybreak.h>

#define BodyBreak_Alloc enemyBodyBreakAlloc
#define BodyBreak_SetInfo enemyBodyBreakInfo
#define BodyBreak_SpawnParts enemyBodyBreakSpawn
extern void FOREIGN_ENEMY_API_DEAD_SOUND(PlayState*, Vec3f*, u16, s16, s16, s32);
#define EffectSsDeadSound_SpawnStationary FOREIGN_ENEMY_API_DEAD_SOUND
extern void FOREIGN_ENEMY_API_DUST_CLOUD(PlayState*, Vec3f*, f32, s32, s16, s16, u8);
#define func_80033480 FOREIGN_ENEMY_API_DUST_CLOUD
extern void FOREIGN_ENEMY_API_SHOCKWAVE(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16, s16);
#define EffectSsBlast_SpawnWhiteShockwaveSetScale FOREIGN_ENEMY_API_SHOCKWAVE
extern void FOREIGN_ENEMY_API_DEAD_DS_STATIONARY(PlayState*, Vec3f*, s16, s16, s16, s32);
#define EffectSsDeadDs_SpawnStationary FOREIGN_ENEMY_API_DEAD_DS_STATIONARY
extern void FOREIGN_ENEMY_API_DEAD_DD(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16, s16, s16, s16, s16, s16, s16, s16, s16, s32);
#define EffectSsDeadDd_Spawn FOREIGN_ENEMY_API_DEAD_DD
#define Effect_Delete ((void (*)(PlayState*, s32))0x800afdcc)
static inline void variantBlureSpace(void* effect)
{
    if (effect) ((void (*)(void*))0x800a8514)(effect);
}

#define EffectBlure_AddSpace variantBlureSpace
#define Actor_WorldDistXYZToActor ((f32 (*)(Actor*, Actor*))0x800b6dcc)
static inline Actor* variantCollidedExplosive(PlayState* play, Collider* collider)
{
    if ((collider->acFlags & AC_HIT) && collider->ac && collider->ac->category == ACTORCAT_EXPLOSIVE)
    {
        collider->acFlags &= ~AC_HIT;
        return collider->ac;
    }
    return NULL;
}

#define Actor_GetCollidedExplosive variantCollidedExplosive
#define Gfx_SetupDL_60NoCDXlu ((void (*)(GraphicsContext*))0x8012c974)
#define gEffFire1DL ((Gfx*)0x04052a10)

#endif
