#include <combo.h>
#include <combo/effect.h>
#include <combo/custom.h>
#include <combo/player.h>
#include <combo/mm/player.h>
#include <combo/math.h>
#include <combo/global.h>
#include <combo/effect.h>
#include <combo/collision.h>
#include "Custom_Boomerang.h"

#define ACTOR_FLAG_HOOKSHOT_ATTACHED                (1 << 13)
#define PLAYER_FOCUS_ACTOR(player)                  (*(Actor**)((u8*)(player) + 0x730))
#define PLAYER_ZORA_BOOMERANG_ACTOR(player)         (*(Actor**)((u8*)(player) + 0xA7C))

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

extern int prepareObject(PlayState* play, u16 objectId);

void CustomBoomerang_Init(Actor_CustomBoomerang* boom, PlayState* play);
void CustomBoomerang_Destroy(Actor_CustomBoomerang* boom, PlayState* play);
void CustomBoomerang_Update(Actor_CustomBoomerang* boom, PlayState* play);
void CustomBoomerang_Draw(Actor_CustomBoomerang* boom, PlayState* play);
void CustomBoomerang_Fly(Actor_CustomBoomerang* boom, PlayState* play);

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
        { 0x00000010, 0x00, 0x01 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_NEAREST | ATELEM_SFX_NORMAL,
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
    ICHAIN_S8(attentionRangeType, ATTENTION_RANGE_5, ICHAIN_CONTINUE),
    ICHAIN_VEC3S(shape.rot, 0, ICHAIN_STOP),
};

static void CustomBoomerang_SetProjectileSpeed(Actor* actor, f32 speed)
{
    actor->speed = Math_CosS(actor->world.rot.x) * speed;
    actor->velocity.y = -Math_SinS(actor->world.rot.x) * speed;
}

void CustomBoomerang_SetupAction(Actor_CustomBoomerang* boom, CustomBoomerangActionFunc actionFunc)
{
    boom->actionFunc = actionFunc;
}

void CustomBoomerang_Init(Actor_CustomBoomerang* boom, PlayState* play)
{
    static u8 sP1StartColor[4] = { 255, 255, 100, 255 };
    static u8 sP2StartColor[4] = { 255, 255, 100, 64 };
    static u8 sP1EndColor[4]   = { 255, 255, 100, 0 };
    static u8 sP2EndColor[4]   = { 255, 255, 100, 0 };
    boom->actor.room = -1;
    EffectBlureInit1 blureInit;
    s32 i;

    Actor_ProcessInitChain(&boom->actor, sInitChain);

    bzero(&blureInit, sizeof(blureInit));

    for (i = 0; i < 4; i++) {
        blureInit.p1StartColor[i] = sP1StartColor[i];
        blureInit.p2StartColor[i] = sP2StartColor[i];
        blureInit.p1EndColor[i] = sP1EndColor[i];
        blureInit.p2EndColor[i] = sP2EndColor[i];
    }

    blureInit.elemDuration = 8;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 0;

    boom->effectIndex = -1;
    Effect_Add(play, &boom->effectIndex, EFFECT_BLURE1, 0, 0, &blureInit);

    Collider_InitQuad(play, &boom->collider);
    Collider_SetQuad(play, &boom->collider, &boom->actor, &sQuadInit);

    CustomBoomerang_SetupAction(boom, CustomBoomerang_Fly);
}

void CustomBoomerang_Destroy(Actor_CustomBoomerang* boom, PlayState* play)
{
    if (boom->effectIndex >= 0) {
        Effect_Destroy(play, boom->effectIndex);
    }

    Collider_DestroyQuad(play, &boom->collider);
}

/* ============================================================================
 * Main flight behavior
 * ========================================================================== */

void CustomBoomerang_Fly(Actor_CustomBoomerang* boom, PlayState* play)
{
    Actor* target;
    Player* player;
    s32 collided;
    s16 yawTarget;
    s16 yawDiff;
    s16 pitchTarget;
    s16 pitchDiff;
    float distXYZScale;
    float distFromLink;
    s32 hitDynaId;
    Vec3f hitPoint;

    player = GET_PLAYER(play);
    target = boom->moveTo;

    if (target != NULL) {
        yawTarget = Actor_WorldYawTowardPoint(&boom->actor, &target->focus.pos);
        yawDiff = boom->actor.world.rot.y - yawTarget;

        pitchTarget = Actor_WorldPitchTowardPoint(&boom->actor, &target->focus.pos);
        pitchDiff = boom->actor.world.rot.x - pitchTarget;

        distXYZScale =
            (200.0f - Math_Vec3f_DistXYZ(&boom->actor.world.pos, &target->focus.pos)) * 0.005f;

        if (distXYZScale < 0.12f) {
            distXYZScale = 0.12f;
        }

        if ((target != &player->actor) && ((target->update == NULL) || (ABS(yawDiff) > 0x4000))) {
            boom->moveTo = NULL;
        } else {
            Math_ScaledStepToS(
                &boom->actor.world.rot.y,
                yawTarget,
                (s16)(ABS(yawDiff) * distXYZScale)
            );

            Math_ScaledStepToS(
                &boom->actor.world.rot.x,
                pitchTarget,
                (s16)(ABS(pitchDiff) * distXYZScale)
            );
        }
    }

    CustomBoomerang_SetProjectileSpeed(&boom->actor, 12.0f);
    Actor_MoveWithGravity(&boom->actor);

    Actor_PlaySfx_Flagged(&boom->actor, NA_SE_IT_BOOMERANG_FLY - SFX_FLAG);

    collided = !!(boom->collider.base.atFlags & AT_HIT);

    if (collided) {
        if ((boom->collider.base.at->id == ACTOR_EN_ITEM00) ||
            (boom->collider.base.at->id == ACTOR_EN_SI)) {
            boom->grabbed = boom->collider.base.at;

            if (boom->collider.base.at->id == ACTOR_EN_SI) {
                boom->collider.base.at->flags |= ACTOR_FLAG_HOOKSHOT_ATTACHED;
            } else {
                boom->collider.base.at->gravity = 0.0f;
            }
        }
    }

    if (DECR(boom->returnTimer) == 0) {
        distFromLink = Math_Vec3f_DistXYZ(&boom->actor.world.pos, &player->actor.focus.pos);
        boom->moveTo = &player->actor;

        if (distFromLink < 40.0f) {
            target = boom->grabbed;

            if (target != NULL) {
                Math_Vec3f_Copy(&target->world.pos, &player->actor.world.pos);

                if (target->id == ACTOR_EN_ITEM00) {
                    target->gravity = -0.9f;
                    target->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH);
                } else {
                    target->flags &= ~ACTOR_FLAG_HOOKSHOT_ATTACHED;
                }
            }

            player->stateFlags1 &= ~PLAYER_STATE1_MM_2000000;
            PLAYER_ZORA_BOOMERANG_ACTOR(player) = NULL;
            Actor_Kill(&boom->actor);
        }
    } else {
        collided = BgCheck_EntityLineTest1(
            &play->colCtx,
            &boom->actor.prevPos,
            &boom->actor.world.pos,
            &hitPoint,
            &boom->actor.wallPoly,
            true,
            true,
            true,
            true,
            &hitDynaId
        );

        if (collided) {
            if (func_800B90AC(play, &boom->actor, boom->actor.wallPoly, hitDynaId, &hitPoint)) {
                collided = false;
            } else {
                CollisionCheck_SpawnShieldParticlesMetal(play, &hitPoint);
            }
        }

        if (collided) {
            boom->actor.world.rot.x = -boom->actor.world.rot.x;
            boom->actor.world.rot.y += 0x8000;
            boom->moveTo = &player->actor;
            boom->returnTimer = 0;
        }
    }

    target = boom->grabbed;

    if (target != NULL) {
        if (target->update == NULL) {
            boom->grabbed = NULL;
        } else {
            Math_Vec3f_Copy(&target->world.pos, &boom->actor.world.pos);
        }
    }
}

void CustomBoomerang_Update(Actor_CustomBoomerang* boom, PlayState* play)
{
    Player* player = GET_PLAYER(play);

    if (!(player->stateFlags1 & PLAYER_STATE1_MM_20000000)) {
        boom->actionFunc(boom, play);
        Actor_SetFocus(&boom->actor, 0.0f);
        boom->activeTimer++;
    }
}

void CustomBoomerang_Draw(Actor_CustomBoomerang* boom, PlayState* play)
{
    static Vec3f sPosAOffset = { -960.0f, 0.0f, 0.0f };
    static Vec3f sPosBOffset = {  960.0f, 0.0f, 0.0f };

    Vec3f posA;
    Vec3f posB;
    void* blure;

    if (!prepareObject(play, CUSTOM_OBJECT_ID_EQ_BOOMERANG_FLIGHT)) {
        return;
    }

    OPEN_DISPS(play->state.gfxCtx);

    Matrix_Translate(
        boom->actor.world.pos.x,
        boom->actor.world.pos.y,
        boom->actor.world.pos.z,
        MTXMODE_NEW
    );

    Matrix_Scale(
        boom->actor.scale.x,
        boom->actor.scale.y,
        boom->actor.scale.z,
        MTXMODE_APPLY
    );

    Matrix_RotateYS(boom->actor.world.rot.y, MTXMODE_APPLY);
    Matrix_RotateZS(0x1F40, MTXMODE_APPLY);
    Matrix_RotateXS(boom->actor.world.rot.x, MTXMODE_APPLY);

    Matrix_MultVec3f(&sPosAOffset, &posA);
    Matrix_MultVec3f(&sPosBOffset, &posB);

    if (Collider_UpdatePlayerWeapon(play, &boom->collider, &boom->weaponInfo, &posA, &posB)) {
        blure = Effect_GetByIndex(boom->effectIndex);

        if (blure != NULL) {
            EffectBlure_AddVertex(blure, &posA, &posB);
        }
    }

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    Matrix_RotateYS(
        boom->activeTimer * 0x2EE0,
        MTXMODE_APPLY
    );

    gSPMatrix(
        POLY_OPA_DISP++,
        Matrix_Finalize(play->state.gfxCtx),
        G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
    );

    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_EQ_BOOMERANG_FLIGHT_0);

    CLOSE_DISPS();
}

ActorProfile Custom_Boomerang_InitVars = {
    ACTOR_CUSTOM_BOOMERANG,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Actor_CustomBoomerang),
    (ActorFunc)CustomBoomerang_Init,
    (ActorFunc)CustomBoomerang_Destroy,
    (ActorFunc)CustomBoomerang_Update,
    (ActorFunc)CustomBoomerang_Draw,
};

OVL_INFO_ACTOR(ACTOR_CUSTOM_BOOMERANG, Custom_Boomerang_InitVars);
