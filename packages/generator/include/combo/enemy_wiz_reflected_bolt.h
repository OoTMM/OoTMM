#ifndef COMBO_ENEMY_WIZ_REFLECTED_BOLT_H
#define COMBO_ENEMY_WIZ_REFLECTED_BOLT_H

#include <combo.h>
#include <combo/matrix.h>

#if defined(GAME_OOT)
#define WizProjectile_ToMtx ((Mtx* (*)(Mtx*))0x800ab8d8)
#else
#define WizProjectile_ToMtx ((Mtx* (*)(Mtx*))0x80181a18)
#endif

enum { WIZ_PROJECTILE_TRAIL_COUNT = 10 };

typedef struct {
    Vec3f pos[WIZ_PROJECTILE_TRAIL_COUNT];
    s16 spin;
    u8 latest;
} WizProjectileTrail;

static inline void WizProjectile_ResetTrail(WizProjectileTrail* trail, const Vec3f* pos)
{
    for (int i = 0; i < WIZ_PROJECTILE_TRAIL_COUNT; ++i) trail->pos[i] = *pos;
    trail->latest = 0;
    trail->spin = 0;
}

static inline void WizProjectile_RecordTrail(WizProjectileTrail* trail, const Vec3f* pos)
{
    if (++trail->latest == WIZ_PROJECTILE_TRAIL_COUNT) trail->latest = 0;
    trail->pos[trail->latest] = *pos;
    trail->spin += 5000;
}

static inline Gfx* WizProjectile_DrawTrail(Gfx* dl, PlayState* play, const WizProjectileTrail* trail,
                                            u32 displayList, f32 scale, int first, int ice)
{
    GraphicsContext* gfx = play->state.gfxCtx;
    for (int i = WIZ_PROJECTILE_TRAIL_COUNT - 1; i >= first; --i)
    {
        int index = trail->latest - i;
        if (index < 0) index += WIZ_PROJECTILE_TRAIL_COUNT;
        const Vec3f* pos = &trail->pos[index];
        f32 sampleScale = scale + i * .0019f;
        if (sampleScale <= 0.f) continue;
        int alpha = 255 - i * 25;
        gDPPipeSync(dl++);
        if (ice)
        {
            gDPSetPrimColor(dl++, 0, 0, 255 - i * 7, 255 - i * 3, 255 - i * 3, alpha);
            gDPSetEnvColor(dl++, 220, 255, 235, 0);
        }
        else
        {
            gDPSetPrimColor(dl++, 0, 0, 255, alpha, 0, alpha);
            gDPSetEnvColor(dl++, alpha, 0, 0, 0);
        }
        Matrix_Translate(pos->x, pos->y + 10.f, pos->z, MTXMODE_NEW);
        Matrix_Scale(sampleScale, sampleScale, sampleScale, MTXMODE_APPLY);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_RotateZS(trail->spin, MTXMODE_APPLY);
        gSPMatrix(dl++, WizProjectile_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(dl++, displayList);
    }
    return dl;
}

#if defined(GAME_OOT)
#define ACTOR_ENEMY_WIZ_REFLECTED_BOLT 0x074
#else
#define ACTOR_ENEMY_WIZ_REFLECTED_BOLT 0x058
#endif

typedef struct {
    Actor actor;
    ColliderCylinder collider;
    WizProjectileTrail trail;
    u8 timer;
    u8 tailTimer;
} WizReflectedBolt;

#endif
