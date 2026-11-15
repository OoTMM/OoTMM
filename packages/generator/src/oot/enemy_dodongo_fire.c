#include <combo.h>
#include <combo/effect.h>
#include <combo/enemy_room_models.h>
#include <combo/enemy_port.h>

extern void EffectSs_Reset(EffectSs*);
static void* dodongoFireObject(EffectSs* effect)
{
    void* object = comboEnemySceneObject(0x100a);
    if (!object)
    {
        effect->life = -1;
        effect->draw = NULL;
    }
    return object;
}

static void dodongoFireUpdate(PlayState* play, s32 index, EffectSs* effect)
{
    effect->regs[1] = (effect->regs[1] + 1) & 3;
    effect->regs[0] += effect->regs[8];
    if (effect->life < effect->regs[7])
    {
        effect->regs[5] -= effect->regs[6];
        if (effect->regs[5] < 0)
        {
            effect->regs[5] = 0;
            effect->life = -1;
        }
    }
    dodongoFireObject(effect);
}

static void dodongoFireDraw(PlayState* play, s32 index, EffectSs* effect)
{
    void* object = dodongoFireObject(effect);
    if (!object)
        return;
    u32 saved = gSegments[6];
    gSegments[6] = (u32)object & 0x1fffffff;
    OPEN_DISPS(play->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 6, object);
    f32 scale = effect->regs[0] * .01f;
    Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    ((void (*)(GraphicsContext*))0x8007e930)(play->state.gfxCtx);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 50, effect->regs[5]);
    gSPSegment(POLY_XLU_DISP++, 8, (u8*)object + 0x90a0 + effect->regs[1] * 0x200);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)0x060098a0);
    CLOSE_DISPS();
    gSegments[6] = saved;
}

static void dodongoFireSpawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel,
                             s16 scale, s16 step, s16 alpha, s16 alphaStep, s16 delay, s32 life)
{
    if (life <= 0 || life > 255)
        return;
    EffectSs effect;
    EffectSs_Reset(&effect);
    if (!dodongoFireObject(&effect))
        return;
    comboEnemyObjectPinFrames(0x100a, life + 2);
    effect.pos = *pos;
    effect.velocity = *velocity;
    effect.accel = *accel;
    effect.life = life;
    effect.type = 0xff;
    effect.priority = 128;
    effect.update = dodongoFireUpdate;
    effect.draw = dodongoFireDraw;
    effect.regs[0] = scale;
    effect.regs[1] = (play->gameplayFrames & 3) ^ 3;
    effect.regs[5] = alpha;
    effect.regs[6] = alphaStep;
    effect.regs[7] = life - delay;
    effect.regs[8] = step;
    EffectSs_Insert(play, &effect);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DODONGO_FIRE, dodongoFireSpawn);
