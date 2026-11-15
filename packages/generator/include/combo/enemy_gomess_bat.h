#ifndef COMBO_ENEMY_GOMESS_BAT_H
#define COMBO_ENEMY_GOMESS_BAT_H

#include <combo/enemy_flight.h>

#if defined(GAME_MM)
#define ACTOR_ENEMY_GOMESS_BAT 0x126
#else
#define ACTOR_ENEMY_GOMESS_BAT 0x075
#endif

static inline s16 gomessBatAtan2S(f32 y, f32 x)
{
#if defined(GAME_MM)
    return Math_Atan2S(y, x);
#else
    return Math_Atan2S(x, y);
#endif
}

enum { GOMESS_BAT_COUNT = 10 };
enum {
    GOMESS_BAT_ALIVE,
    GOMESS_BAT_DEATH_FLY,
    GOMESS_BAT_DEATH_DIVE,
    GOMESS_BAT_DEATH_LANDED,
    GOMESS_BAT_DEATH_SETTLED
};

typedef struct EnemyGomessBat {
    Actor actor;
    ColliderJntSph body;
    ColliderJntSphElement elements[3];
    Vec3f effects[3];
    Vec3f effectOffset[3];
    Vec3f effectVelocity[3];
    struct EnemyGomessBat** ownerSlot;
    struct EnemyGomessBat** followerSlot;
    struct EnemyGomessBat** targetSlot;
    u8* swarmHits;
    u16* dropCooldown;
    u32* deathSfxFrame;
    u32* loopSfxFrame;
    u8* scatterRequest;
    EnemyFlightReturn returnRoute;
    f32 fallSpeed[3];
    f32 speed;
    f32 deathVelocityY;
    s16 fallYaw[3];
    s16 yaw;
    s16 pitch;
    s16 scatterYaw;
    s16 scatterPitch;
    u8 effectFrame[3];
    u8 falling[3];
    u8 attackTimer;
    u8 scatterTimer;
    u8 gather;
    u8 launch;
    u8 returning;
    u8 following;
    u8 regrouping;
    u8 crowdState;
    u8 recallDistance;
    u8 leaderTimer;
    u8 attackCooldown;
    u8 hitRecovery[3];
    u8 deathStage;
    u8 deathAlpha;
} EnemyGomessBat;

#include <combo/matrix.h>

#if defined(GAME_MM)
#define GomessBat_ToMtx ((Mtx* (*)(Mtx*))0x80181a18)
#else
#define GomessBat_ToMtx ((Mtx* (*)(Mtx*))0x800ab8d8)
#endif

typedef struct {
    Mtx matrix;
    s16 yaw, pitch;
    f32 scale;
    int valid;
} GomessBatDrawPose;

static inline void* Gomess_AllocDrawData(GraphicsContext* gfx, size_t size, size_t opaCommands)
{
    size_t required = ALIGN(size, 16) + (opaCommands + 2) * sizeof(Gfx);
    if ((uintptr_t)gfx->polyOpa.end < (uintptr_t)gfx->polyOpa.append + required) return NULL;
    return GRAPH_ALLOC(gfx, size);
}

static inline u32 GomessBat_VisibleEffects(const EnemyGomessBat* bat)
{
    u32 flying = 0;
    u32 fallingEffects = 0;
    for (int i = 0; i < ARRAY_COUNT(bat->effects); ++i)
    {
        if (bat->falling[i])
        {
            fallingEffects |= 1u << i;
        }
        else if (!bat->hitRecovery[i])
        {
            flying |= 1u << i;
        }
    }
    return flying | (fallingEffects << 8);
}

static inline void GomessBat_DrawEffects(EnemyGomessBat* bat, PlayState* play, int* materialReady, GomessBatDrawPose* pose, u32 effectsMask)
{
    if (!bat->deathAlpha) return;
    f32 scale = MIN(.007f, bat->deathAlpha * .000035f);
    u32 flying = effectsMask & 0xff;
    u32 fallingEffects = effectsMask >> 8;
    u32 active = flying | fallingEffects;
    int visible = (active & 1) + ((active >> 1) & 1) + ((active >> 2) & 1);
    if (!visible) return;

    Mtx* matrices = Gomess_AllocDrawData(play->state.gfxCtx, visible * sizeof(*matrices),
                                        2 * visible + (*materialReady ? 0 : 2));
    if (!matrices) return;

    OPEN_DISPS(play->state.gfxCtx);
    if (!*materialReady)
    {
        Matrix_Push();
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, 0x06009f10);
        *materialReady = 1;
    }

    if (flying && (!pose->valid || pose->yaw != bat->yaw || pose->pitch != bat->pitch || pose->scale != scale))
    {
        pose->yaw = bat->yaw;
        pose->pitch = bat->pitch;
        pose->scale = scale;
        pose->valid = 1;
        Matrix_RotateZYX(-bat->pitch, bat->yaw, 0, MTXMODE_NEW);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        GomessBat_ToMtx(&pose->matrix);
    }
    for (int falling = !flying; falling <= 1; ++falling)
    {
        u32 effects = falling ? fallingEffects : flying;
        if (!effects) continue;
        for (int i = 0; i < ARRAY_COUNT(bat->effects); ++i)
        {
            if (!(effects & (1u << i))) continue;
            Vec3f* pos = &bat->effects[i];
            Mtx* matrix = matrices++;
            if (falling)
            {
                Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
                Matrix_RotateZYX(0x4000, bat->fallYaw[i], 0, MTXMODE_APPLY);
                Matrix_Scale(.007f, .007f, .007f, MTXMODE_APPLY);
                GomessBat_ToMtx(matrix);
            }
            else
            {
                *matrix = pose->matrix;
                u16* words = (u16*)matrix;
                s32 xyz[3] = {pos->x * 65536.f, pos->y * 65536.f, pos->z * 65536.f};
                for (int axis = 0; axis < 3; ++axis)
                {
                    words[12 + axis] = xyz[axis] >> 16;
                    words[28 + axis] = xyz[axis];
                }
            }
            gSPMatrix(POLY_OPA_DISP++, matrix,
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_OPA_DISP++, 0x06009f88 + bat->effectFrame[i] * 0x100);
        }
    }
    CLOSE_DISPS();
}

#undef GomessBat_ToMtx

#endif
