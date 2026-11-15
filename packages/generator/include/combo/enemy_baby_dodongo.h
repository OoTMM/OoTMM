#ifndef COMBO_ENEMY_BABY_DODONGO_H
#define COMBO_ENEMY_BABY_DODONGO_H

#include <combo.h>

static inline void enemyBabyDodongoTerrain(PlayState* play, Actor* actor, int burrowing, int crawling)
{
    if (crawling && actor->gravity == 0.f)
        actor->gravity = -.8f;
    if (burrowing)
    {
        CollisionPoly* floor = NULL;
        s32 bgId;
        Vec3f probe = actor->world.pos;
        probe.y = MAX(probe.y, actor->home.pos.y) + 80.f;
#if defined(GAME_MM)
        f32 height = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, actor, &probe);
#else
        f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &probe);
#endif
        if (floor && height > -30000.f && height < 30000.f)
        {
            actor->home.pos.y = height;
            actor->floorHeight = height;
            actor->floorPoly = floor;
            actor->floorBgId = bgId;
        }
    }
}

#endif
