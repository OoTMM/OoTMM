#include <combo.h>
#include <combo/custom.h>
#include <combo/player.h>
#include <combo/mm/player.h>
#include <combo/math.h>
#include <combo/global.h>
#include <combo/effect.h>
#include <combo/collision.h>
#include <assets/mm/objects/gameplay_keep.h>
#include "Custom_SlingshotProjecticle.h"

#define FLAGS \
    (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

void CustomSlingshotProjectile_Init(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
);

void CustomSlingshotProjectile_Destroy(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
);

void CustomSlingshotProjectile_Update(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
);

void CustomSlingshotProjectile_Draw(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
);

void CustomSlingshotProjectile_Shoot(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
);

void CustomSlingshotProjectile_Fly(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
);

static ColliderQuadInit sQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK2,
        { (1 << 0x10), 0x00, 0x01 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_NEAREST | ATELEM_SFX_NONE,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    {
        {
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f },
        },
    },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(minVelocityY, -150, ICHAIN_STOP),
};

static void CustomSlingshotProjectile_SetProjectileSpeed(
    Actor* actor,
    f32 speed
)
{
    actor->speed = Math_CosS(actor->world.rot.x) * speed;
    actor->velocity.y = -Math_SinS(actor->world.rot.x) * speed;
}

void CustomSlingshotProjectile_SetupAction(
    Actor_CustomSlingshotProjectile* seed,
    CustomSlingshotProjectileActionFunc actionFunc
)
{
    seed->actionFunc = actionFunc;
}

void CustomSlingshotProjectile_Init(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
)
{
    Actor_ProcessInitChain(&seed->actor, sInitChain);

    Collider_InitQuad(play, &seed->collider);
    Collider_SetQuad(
        play,
        &seed->collider,
        &seed->actor,
        &sQuadInit
    );

    seed->timer = 0;
    seed->activeTimer = 0;
    seed->touchedPoly = false;

    CustomSlingshotProjectile_SetupAction(seed, CustomSlingshotProjectile_Shoot);
}

void CustomSlingshotProjectile_Destroy(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
)
{
    Collider_DestroyQuad(play, &seed->collider);
}

void CustomSlingshotProjectile_Shoot(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
)
{
    Player* player = GET_PLAYER(play);

    if (seed->actor.parent != NULL) {
        return;
    }

    if (player->unk_D57 == 0) {
        Actor_Kill(&seed->actor);
        return;
    }

    Actor_PlaySfx_Flagged(
        &seed->actor,
        NA_SE_IT_SLING_SHOT
    );

    CustomSlingshotProjectile_SetProjectileSpeed(
        &seed->actor,
        80
    );

    seed->timer = 15;

    seed->actor.shape.rot.x = 0;
    seed->actor.shape.rot.y = 0;
    seed->actor.shape.rot.z = 0;

    CustomSlingshotProjectile_SetupAction(seed, CustomSlingshotProjectile_Fly);
}

static void CustomSlingshotProjectile_SpawnSeedPop(
    PlayState* play,
    Vec3f* pos
)
{
    EffectSsStone1_Spawn(play, pos, 0);
    SoundSource_PlaySfxAtFixedWorldPos(play, pos, 20, NA_SE_IT_SLING_REFLECT);
}

void CustomSlingshotProjectile_Fly(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
)
{
    CollisionPoly* hitPoly;
    Vec3f hitPoint;
    s32 bgId;
    s32 collided;

    if (DECR(seed->timer) == 0) {
        Actor_Kill(&seed->actor);
        return;
    }

    if (seed->timer < 8) {
        seed->actor.gravity = -0.4f;
    }

    collided = !!(seed->collider.base.atFlags & AT_HIT);

    if (collided || seed->touchedPoly) {
        if (collided) {
            seed->actor.world.pos.x =
                (seed->actor.world.pos.x + seed->actor.prevPos.x) * 0.5f;
            seed->actor.world.pos.y =
                (seed->actor.world.pos.y + seed->actor.prevPos.y) * 0.5f;
            seed->actor.world.pos.z =
                (seed->actor.world.pos.z + seed->actor.prevPos.z) * 0.5f;
        }

        CustomSlingshotProjectile_SpawnSeedPop(
            play,
            &seed->actor.world.pos
        );

        Actor_Kill(&seed->actor);
        return;
    }

    Actor_MoveWithGravity(&seed->actor);

    seed->touchedPoly = BgCheck_EntityLineTest1(
        &play->colCtx,
        &seed->actor.prevPos,
        &seed->actor.world.pos,
        &hitPoint,
        &hitPoly,
        true,
        true,
        true,
        true,
        &bgId
    );

    if (seed->touchedPoly) {
        Math_Vec3f_Copy(&seed->actor.world.pos, &hitPoint);
    }
}

void CustomSlingshotProjectile_Update(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
)
{
    Player* player = GET_PLAYER(play);

    if (!(player->stateFlags1 & PLAYER_STATE1_MM_20000000)) {
        seed->actionFunc(seed, play);
        Actor_SetFocus(&seed->actor, 0.0f);
        seed->activeTimer++;
    }
}

static void CustomSlingshotProjectile_UpdateCollider(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
)
{
    static Vec3f sPosAOffset = { 0.0f,  400.0f, 1500.0f };
    static Vec3f sPosBOffset = { 0.0f, -400.0f, 1500.0f };

    Vec3f posA;
    Vec3f posB;

    if (seed->actionFunc != CustomSlingshotProjectile_Fly) {
        return;
    }
    Matrix_MultVec3f(&sPosAOffset, &posA);
    Matrix_MultVec3f(&sPosBOffset, &posB);
    Collider_UpdatePlayerWeapon(play, &seed->collider, &seed->weaponInfo, &posA, &posB);
}

void CustomSlingshotProjectile_Draw(
    Actor_CustomSlingshotProjectile* seed,
    PlayState* play
)
{
    u8 alpha;
    s32 spin;

    Matrix_Translate(seed->actor.world.pos.x, seed->actor.world.pos.y, seed->actor.world.pos.z, MTXMODE_NEW);
    Matrix_Scale(seed->actor.scale.x, seed->actor.scale.y, seed->actor.scale.z, MTXMODE_APPLY);
    if (seed->actor.speed != 0.0f) { alpha = (Math_CosS(seed->timer * 5000) * 127.5f) + 127.5f;
        OPEN_DISPS(play->state.gfxCtx);
        Gfx_SetupDL25_Xlu2(play->state.gfxCtx);
        gDPPipeSync(POLY_XLU_DISP++);
        gSPTexture(POLY_XLU_DISP++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gSPClearGeometryMode(POLY_XLU_DISP++, G_CULL_BACK);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, alpha);
        Matrix_Push();
        Matrix_Mult( &play->billboardMtxF, MTXMODE_APPLY);
        spin = (play->gameplayFrames & 0xFF) * 4000;
        Matrix_RotateZS(spin, MTXMODE_APPLY);
        Matrix_Scale(50, 50, 50, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gEffSparklesDL);
        Matrix_Pop();
        Matrix_RotateYS(seed->actor.world.rot.y, MTXMODE_APPLY);
        CLOSE_DISPS();
    }

    CustomSlingshotProjectile_UpdateCollider(seed, play);
}

ActorProfile Custom_SlingshotProjectile_InitVars = {
    ACTOR_CUSTOM_SLINGSHOT_PROJECTILE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Actor_CustomSlingshotProjectile),
    (ActorFunc)CustomSlingshotProjectile_Init,
    (ActorFunc)CustomSlingshotProjectile_Destroy,
    (ActorFunc)CustomSlingshotProjectile_Update,
    (ActorFunc)CustomSlingshotProjectile_Draw,
};

OVL_INFO_ACTOR(
    ACTOR_CUSTOM_SLINGSHOT_PROJECTILE,
    Custom_SlingshotProjectile_InitVars
);