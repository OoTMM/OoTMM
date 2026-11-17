#ifndef COMBO_ENEMY_FLIGHT_H
#define COMBO_ENEMY_FLIGHT_H

#include <combo.h>

typedef struct {
    Vec3f waypoint;
    Vec3f previousWaypoint;
    u8 hasWaypoint;
    u8 hasPrevious;
    u8 retry;
    u16 stalledFrames;
    f32 bestDistance;
} EnemyFlightReturn;

static inline void enemyFlightCheckWall(PlayState* play, Actor* actor, Vec3f previous)
{
    Vec3f from = previous;
    Vec3f to = actor->world.pos;
    if (from.x == to.x && from.y == to.y && from.z == to.z)
        return;
    f32 height = actor->shape.yOffset * actor->scale.y;
    from.y += height;
    to.y += height;
    Vec3f hit;
    CollisionPoly* wall;
    s32 bgId;
    if (!BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &wall, 1, 0, 0, 0, &bgId))
        return;
    actor->world.pos = previous;
    actor->wallPoly = wall;
    actor->wallBgId = bgId;
    actor->wallYaw = Math_Atan2S(wall->normal.z, wall->normal.x);
    if ((to.x - from.x) * wall->normal.x + (to.z - from.z) * wall->normal.z > 0.f)
        actor->wallYaw += 0x8000;
    actor->bgCheckFlags |= BGCHECKFLAG_WALL;
}

static inline int enemyFlightClear(PlayState* play, Vec3f* from, Vec3f* to)
{
    if (from->x == to->x && from->y == to->y && from->z == to->z)
        return 1;

    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    return !BgCheck_EntityLineTest1(&play->colCtx, from, to, &hit, &poly, 1, 1, 1, 0, &bgId);
}

static inline void enemyFlightSlideBlocked(PlayState* play, Vec3f* pos, Vec3f* next, const CollisionPoly* poly,
    int (*clearPath)(PlayState*, Vec3f*, Vec3f*))
{
    Vec3f normal = {poly->normal.x / 32767.f, poly->normal.y / 32767.f, poly->normal.z / 32767.f};
    f32 into = (next->x - pos->x) * normal.x + (next->y - pos->y) * normal.y +
               (next->z - pos->z) * normal.z;
    Vec3f slide = {next->x - into * normal.x, next->y - into * normal.y, next->z - into * normal.z};
    if (clearPath(play, pos, &slide))
        *pos = slide;
}

static inline void enemyFlightSlide(PlayState* play, Vec3f* pos, Vec3f* next)
{
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    if (BgCheck_EntityLineTest1(&play->colCtx, pos, next, &hit, &poly, 1, 1, 1, 0, &bgId))
        enemyFlightSlideBlocked(play, pos, next, poly, enemyFlightClear);
    else
        *pos = *next;
}

static inline void enemyFlightReturn(PlayState* play, EnemyFlightReturn* route, Vec3f* pos,
    Vec3f* goal, Vec3f* next, f32 distanceSq,
    int allowDetourSearch, int stepKnownClear,
    int (*clearPath)(PlayState*, Vec3f*, Vec3f*))
{
    f32 progressThreshold = route->bestDistance - 8.f;
    if (!route->bestDistance ||
        (progressThreshold > 0.f && distanceSq < progressThreshold * progressThreshold))
    {
        route->bestDistance = sqrtf(distanceSq);
        route->stalledFrames = 0;
    }
    else if (route->stalledFrames < 240)
    {
        ++route->stalledFrames;
    }

    CollisionPoly* blockedPoly = NULL;
    Vec3f hit;
    s32 bgId;
    if (!route->hasWaypoint && (stepKnownClear ||
        (pos->x == next->x && pos->y == next->y && pos->z == next->z) ||
        !BgCheck_EntityLineTest1(&play->colCtx, pos, next, &hit, &blockedPoly, 1, 1, 1, 0, &bgId)))
    {
        route->retry = 0;
        route->hasPrevious = 0;
        *pos = *next;
        return;
    }
    if (!route->hasWaypoint && !route->retry && allowDetourSearch)
    {
        f32 sideX[2], sideZ[2];
        s16 yaw = 0;
        Vec3f detour = *pos;
        f32 detourDistanceSq = 1e20f;
        for (int radius = 24; radius <= 96 && !route->hasWaypoint; radius *= 2)
        {
            for (int i = 0; i < 4; ++i)
            {
                Vec3f candidate = *pos;
                if (i < 2)
                {
                    candidate.y += i ? -radius : radius;
                }
                else
                {
                    if (radius == 24)
                    {
                        if (i == 2)
                            yaw = Math_Vec3f_Yaw(pos, goal);
                        s16 side = yaw + (i == 2 ? 0x4000 : -0x4000);
                        sideX[i - 2] = Math_SinS(side);
                        sideZ[i - 2] = Math_CosS(side);
                    }
                    candidate.x += radius * sideX[i - 2];
                    candidate.z += radius * sideZ[i - 2];
                }
                if (!clearPath(play, pos, &candidate))
                    continue;
                if (clearPath(play, &candidate, goal))
                {
                    route->waypoint = candidate;
                    route->hasWaypoint = 1;
                    break;
                }
                if (radius < 48 || i < 2)
                    continue;
                if (route->hasPrevious)
                {
                    f32 dx = candidate.x - route->previousWaypoint.x;
                    f32 dy = candidate.y - route->previousWaypoint.y;
                    f32 dz = candidate.z - route->previousWaypoint.z;
                    if (dx * dx + dy * dy + dz * dz < 25.f * 25.f)
                        continue;
                }
                f32 dx = candidate.x - goal->x;
                f32 dy = candidate.y - goal->y;
                f32 dz = candidate.z - goal->z;
                f32 distanceSq = dx * dx + dy * dy + dz * dz;
                if (distanceSq < detourDistanceSq)
                {
                    detour = candidate;
                    detourDistanceSq = distanceSq;
                }
            }
        }
        if (!route->hasWaypoint && detourDistanceSq < 1e20f)
        {
            route->previousWaypoint = *pos;
            route->hasPrevious = 1;
            route->waypoint = detour;
            route->hasWaypoint = 1;
        }
        route->retry = 8;
    }
    if (route->retry)
        --route->retry;
    if (route->hasWaypoint)
    {
        Vec3f step = *pos;
        Math_StepToF(&step.x, route->waypoint.x, 5.f);
        Math_StepToF(&step.y, route->waypoint.y, 5.f);
        Math_StepToF(&step.z, route->waypoint.z, 5.f);
        if (!clearPath(play, pos, &step))
        {
            route->hasWaypoint = 0;
        }
        else
        {
            *pos = step;
            f32 dx = step.x - route->waypoint.x;
            f32 dy = step.y - route->waypoint.y;
            f32 dz = step.z - route->waypoint.z;
            if (dx * dx + dy * dy + dz * dz < 1.f)
                route->hasWaypoint = 0;
        }
    }
    else
    {
        enemyFlightSlideBlocked(play, pos, next, blockedPoly, clearPath);
    }
}

#endif
