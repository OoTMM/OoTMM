#include <combo.h>

void comboEnemyNativeFloorEntry(PlayState* play, const ActorEntry* source, ActorEntry* adjusted)
{
    CollisionPoly* floorPoly = NULL;
    WaterBox* waterBox;
    Vec3f origin;
    float floorY;
    float waterY;

    if (!source || !adjusted)
        return;
    *adjusted = *source;
    if (!play)
        return;
    origin.x = source->pos.x;
    origin.y = (float)source->pos.y + 2.0f;
    origin.z = source->pos.z;
    floorY = BgCheck_EntityRaycastDown1(&play->colCtx, &floorPoly, &origin);
    waterY = origin.y;
    if (WaterBox_GetSurface1(play, &play->colCtx, origin.x, origin.z, &waterY, &waterBox) &&
        waterY > floorY && waterY <= origin.y)
        floorY = waterY;
    if (floorY != BGCHECK_Y_MIN)
        adjusted->pos.y = (s16)floorY;
}

EXPORT_SYMBOL(ENEMY_NATIVE_FLOOR_V1, comboEnemyNativeFloorEntry);
