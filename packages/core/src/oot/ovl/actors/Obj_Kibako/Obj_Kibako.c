#include <combo.h>
#include "Obj_Kibako.h"

#define FLAGS (ACTOR_FLAG_OOT_4 | ACTOR_FLAG_OOT_26)

#if defined(GAME_OOT)
# define SEGADDR_CRATE_DL       ((void*)0x05005290)
# define SEGADDR_FRAGMENT_DL    ((void*)0x05005380)
#endif

void ObjKibako_Init(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_Destroy(Actor_ObjKibako* this, GameState_Play* play2);
void ObjKibako_Update(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_Draw(Actor_ObjKibako* this, GameState_Play* play);

void ObjKibako_SetupIdle(Actor_ObjKibako* this);
void ObjKibako_Idle(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_SetupHeld(Actor_ObjKibako* this);
void ObjKibako_Held(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_SetupThrown(Actor_ObjKibako* this);
void ObjKibako_Thrown(Actor_ObjKibako* this, GameState_Play* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000002, 0x00, 0x01 },
        { 0x4FC00748, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 12, 27, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sCCInfoInit = { 0, 12, 60, MASS_HEAVY };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 60, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void ObjKibako_SpawnCollectible(Actor_ObjKibako* this, GameState_Play* play) {
    s16 collectible;

    collectible = this->actor.variable & 0x1F;
    if ((collectible >= 0) && (collectible < ITEM00_MAX)) {
        Item_DropCollectible(play, &this->actor.world.pos, collectible | (((this->actor.variable >> 8) & 0x3F) << 8));
    }
}

void ObjKibako_ApplyGravity(Actor_ObjKibako* this) {
    this->actor.velocity.y += this->actor.gravity;
    if (this->actor.velocity.y < this->actor.minVelocityY) {
        this->actor.velocity.y = this->actor.minVelocityY;
    }
}

void ObjKibako_InitCollider(Actor_ObjKibako* this, GameState_Play* play)
{
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
}

void ObjKibako_Init(Actor_ObjKibako* this, GameState_Play* play)
{
    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->actor.gravity = -1.2f;
    this->actor.minVelocityY = -13.0f;
    ObjKibako_InitCollider(this, play);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, NULL, &sCCInfoInit);
    ObjKibako_SetupIdle(this);
}

void ObjKibako_Destroy(Actor_ObjKibako* this, GameState_Play* play)
{
    Collider_DestroyCylinder(play, &this->collider);
}

void ObjKibako_AirBreak(Actor_ObjKibako* this, GameState_Play* play)
{
    s16 angle;
    s32 i;
    Vec3f* breakPos = &this->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s16 phi_s0;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 6.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        pos.x += breakPos->x;
        pos.y += breakPos->y;
        pos.z += breakPos->z;
        temp_rand = Rand_ZeroOne();
        if (temp_rand < 0.1f) {
            phi_s0 = 0x60;
        } else if (temp_rand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, breakPos, -200, phi_s0, 10, 10, 0, (Rand_ZeroOne() * 30.0f) + 10.0f,
                             0, 32, 60, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_DANGEON_KEEP, SEGADDR_FRAGMENT_DL);
    }

    SpawnSomeDust(play, &this->actor.world.pos, 40.0f, 3, 50, 140, 1);
}

void ObjKibako_WaterBreak(Actor_ObjKibako* this, GameState_Play* play) {
    s16 angle;
    s32 i;
    Vec3f* breakPos = &this->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;

    pos = *breakPos;
    pos.y += this->actor.depthInWater;
    EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 500);

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s16 phi_s0;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.18f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.18f;
        pos.x += breakPos->x;
        pos.y += breakPos->y;
        pos.z += breakPos->z;
        temp_rand = Rand_ZeroOne();
        phi_s0 = (temp_rand < 0.2f) ? 0x40 : 0x20;
        EffectSsKakera_Spawn(play, &pos, &velocity, breakPos, -180, phi_s0, 30, 30, 0, (Rand_ZeroOne() * 30.0f) + 10.0f,
                             0, 32, 70, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_DANGEON_KEEP, SEGADDR_FRAGMENT_DL);
    }
}

void ObjKibako_SetupIdle(Actor_ObjKibako* this) {
    this->actionFunc = ObjKibako_Idle;
    this->actor.colChkInfo.mass = MASS_HEAVY;
}

void ObjKibako_Idle(Actor_ObjKibako* this, GameState_Play* play)
{
    if (Actor_HasParent(&this->actor, play)) {
        ObjKibako_SetupHeld(this);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) && (this->actor.depthInWater > 19.0f)) {
        ObjKibako_WaterBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    } else if (this->collider.base.acFlags & AC_HIT) {
        ObjKibako_AirBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    } else {
        Actor_MoveWithGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR);
        if (!(this->collider.base.ocFlags1 & OC1_TYPE_PLAYER) && (this->actor.xzDistToPlayer > 28.0f)) {
            this->collider.base.ocFlags1 |= OC1_TYPE_PLAYER;
        }
        if (this->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&this->actor, &this->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            if (this->actor.xzDistToPlayer < 180.0f) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            }
        }
        if (this->actor.xzDistToPlayer < 100.0f) {
            Actor_OfferCarry(&this->actor, play);
        }
    }
}

void ObjKibako_SetupHeld(Actor_ObjKibako* this)
{
    this->actionFunc = ObjKibako_Held;
    this->actor.room = -1;
    //! @bug: This is an unsafe cast, although the sound effect will still play
    Player_PlaySfx((void*)&this->actor, NA_SE_PL_PULL_UP_WOODBOX);
}

void ObjKibako_Held(Actor_ObjKibako* this, GameState_Play* play)
{
    if (Actor_HasNoParent(&this->actor, play))
    {
        this->actor.room = play->roomCtx.curRoom.num;
        if (fabsf(this->actor.speed) < 0.1f)
        {
            Actor_PlaySfx(&this->actor, NA_SE_EV_PUT_DOWN_WOODBOX);
            ObjKibako_SetupIdle(this);
            this->collider.base.ocFlags1 &= ~OC1_TYPE_PLAYER;
        }
        else
        {
            ObjKibako_SetupThrown(this);
            ObjKibako_ApplyGravity(this);
            Actor_UpdatePos(&this->actor);
        }
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR);
    }
}

void ObjKibako_SetupThrown(Actor_ObjKibako* this)
{
    this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speed;
    this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speed;
    this->actor.colChkInfo.mass = 240;
    this->actionFunc = ObjKibako_Thrown;
}

void ObjKibako_Thrown(Actor_ObjKibako* this, GameState_Play* play)
{
    if ((this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL)) || (this->collider.base.atFlags & AT_HIT))
    {
        ObjKibako_AirBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    }
    else if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH)
    {
        ObjKibako_WaterBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    }
    else
    {
        ObjKibako_ApplyGravity(this);
        Actor_UpdatePos(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR);
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
}

void ObjKibako_Update(Actor_ObjKibako* this, GameState_Play* play)
{
    this->actionFunc(this, play);
}

void ObjKibako_Draw(Actor_ObjKibako* this, GameState_Play* play)
{
    Gfx_DrawDListOpa(play, SEGADDR_CRATE_DL);
}

ActorInit Obj_Kibako_InitVars =
{
    AC_OBJ_KIBAKO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(Actor_ObjKibako),
    (ActorFunc)ObjKibako_Init,
    (ActorFunc)ObjKibako_Destroy,
    (ActorFunc)ObjKibako_Update,
    (ActorFunc)ObjKibako_Draw,
};

OVL_ACTOR_INFO(AC_OBJ_KIBAKO, Obj_Kibako_InitVars);
