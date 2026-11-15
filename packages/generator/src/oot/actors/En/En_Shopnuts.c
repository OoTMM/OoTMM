#include <combo.h>

static Actor* EnShopnuts_SpawnMerchant(ActorContext* ctx, PlayState* play, s16 id,
                                       f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    if ((u16)params == 0x4000)
        return NULL;
    return Actor_Spawn(ctx, play, id, x, y, z, rx, ry, rz, params);
}

PATCH_CALL(0x80b409e0, EnShopnuts_SpawnMerchant);
