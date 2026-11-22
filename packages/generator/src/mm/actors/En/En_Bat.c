#include <combo.h>
#include <combo/enemy_flight.h>

static void EnBat_UpdateBgCheck(PlayState* play, Actor* actor, f32 wallHeight, f32 wallRadius, f32 ceilingHeight, u32 flags)
{
    Vec3f previous = actor->prevPos;
    Actor_UpdateBgCheckInfo(play, actor, wallHeight, wallRadius, ceilingHeight,
        flags | UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_2 | UPDBGCHECKINFO_FLAG_MM_4);
    if (actor->colChkInfo.health != 0)
        enemyFlightCheckWall(play, actor, previous);
}

PATCH_VROM_U32(0xeae094, 0);
PATCH_CALL(0x80a445cc, EnBat_UpdateBgCheck);
