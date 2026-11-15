#ifndef COMBO_ENEMY_EYEGORE_NATIVE_H
#define COMBO_ENEMY_EYEGORE_NATIVE_H

#include <combo.h>

static inline int Eyegore_CanMove(PlayState* play, Actor* actor, Vec3f from, Vec3f to)
{
    if (fabsf(to.x - from.x) + fabsf(to.z - from.z) < 0.01f)
        return 1;
    Vec3f probe = to;
    probe.y = from.y + 32.f;
    from.y += 32.f;
    Vec3f hit;
    CollisionPoly* poly = NULL;
    s32 bgId;
    if (BgCheck_EntityLineTest1(&play->colCtx, &from, &probe, &hit, &poly, 1, 0, 0, 0, &bgId))
        return 0;
    f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &probe);
    return poly && fabsf(floor - to.y) <= 36.f;
}

static inline f32 Eyegore_LaserWallFraction(PlayState* play, Vec3f base, Vec3f beam)
{
    Vec3f end = { base.x + beam.x, base.y + beam.y, base.z + beam.z };
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    if (!BgCheck_EntityLineTest1(&play->colCtx, &base, &end, &hit, &poly, 1, 0, 1, 0, &bgId))
        return 1.f;
    f32 length = Math_Vec3f_DistXYZ(&base, &end);
    return length > 0.01f ? MIN(1.f, Math_Vec3f_DistXYZ(&base, &hit) / length) : 1.f;
}

static inline int Eyegore_LaserHitsSurface(PlayState* play, Vec3f base, Vec3f beam)
{
    Vec3f end = { base.x + beam.x, base.y + beam.y, base.z + beam.z };
    CollisionPoly* poly = NULL;
    Vec3f hit;
    s32 bgId;
    return BgCheck_EntityLineTest1(&play->colCtx, &base, &end, &hit, &poly, 1, 1, 1, 0, &bgId);
}

#endif
