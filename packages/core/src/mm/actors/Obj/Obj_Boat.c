#include <combo.h>

f32 ObjBoat_GetTopSpeed(Actor_ObjBoat* boat, GameState_Play* play)
{
    if (boat->base.variable == 0x47F)
    {
        Vec3f platformProximity = { -455.0, 0.0, -680.0};
        int nearPlatform = (VectDist(&boat->base.world.pos, &platformProximity) < 750.0);
        int nearEnd = (boat->speedMultiplier > 0 && boat->pathProgress >= 0xc);
        if (!nearPlatform && !nearEnd)
        {
            return 7.5;
        }
    }
    return 3.0;
}

f32 ObjBoat_GetAccelSpeed(Actor_ObjBoat* boat, GameState_Play* play)
{
    if (boat->base.variable == 0x47F && (boat->speedMultiplier < 0 || boat->pathProgress < 0xC))
    {
        return 0.2;
    }
    return 0.05;
}
