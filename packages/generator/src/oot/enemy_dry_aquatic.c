#include <combo.h>
#include <combo/actor_id.h>
#include <combo/enemy_placement.h>

static f32 drySurface(Actor* actor, f32 originY)
{
    f32 floor = actor->floorHeight > -30000.f ? actor->floorHeight : originY;
    return floor + (actor->id == ACTOR_EN_WEIYER ? 28.f : 2.f);
}

static s32 enemyDryWaterSurface(PlayState* play, CollisionContext* colCtx, f32 x, f32 z,
                                f32* surface, WaterBox** waterBox)
{
    s32 (*nativeSurface)(PlayState*, CollisionContext*, f32, f32, f32*, WaterBox**) = (void*)0x80034908;
    s32 found = nativeSurface(play, colCtx, x, z, surface, waterBox);
    f32 originY;
    Actor* owner = comboEnemyNativeDryAquaticOwner(play, &originY);

    if (!owner || (found && *surface > owner->floorHeight + (owner->id == ACTOR_EN_WEIYER ? 5.f : 0.f)))
        return found;
    static WaterBox dryBox;
    *surface = drySurface(owner, originY);
    *waterBox = &dryBox;
    return 1;
}

PATCH_CALL(0x8086e53c, enemyDryWaterSurface);
PATCH_CALL(0x8086fe1c, enemyDryWaterSurface);
PATCH_CALL(0x80b3b00c, enemyDryWaterSurface);

static void enemyDryStingerBgCheck(PlayState* play, Actor* actor, f32 wallHeight, f32 wallRadius,
                                   f32 ceilingHeight, u32 flags)
{
    Actor_UpdateBgCheckInfo(play, actor, wallHeight, wallRadius, ceilingHeight, flags);
    f32 originY;
    if (actor->depthInWater > -30000.f || comboEnemyNativeDryAquaticOwner(play, &originY) != actor)
        return;
    actor->depthInWater = drySurface(actor, originY) - actor->world.pos.y;
    if (actor->depthInWater >= 0.f)
        actor->bgCheckFlags |= BGCHECKFLAG_WATER;
}

PATCH_CALL(0x80b3bfd4, enemyDryStingerBgCheck);
