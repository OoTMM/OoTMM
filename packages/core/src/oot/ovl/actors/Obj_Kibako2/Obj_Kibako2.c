#include <combo.h>
#include "Obj_Kibako2.h"

#define FLAGS 0

#if defined(GAME_OOT)
# define SEGADDR_COLLIDER       ((void*)0x06000b70)
# define SEGADDR_CRATE_DL       ((void*)0x06000960)
# define SEGADDR_FRAGMENT_DL    ((void*)0x06001000)
#endif

void ObjKibako2_Init(Actor_ObjKibako2* this, GameState_Play* play);
void ObjKibako2_Destroy(Actor_ObjKibako2* this, GameState_Play* play);
void ObjKibako2_Update(Actor_ObjKibako2* this, GameState_Play* play);
void ObjKibako2_Draw(Actor_ObjKibako2* this, GameState_Play* play);
void ObjKibako2_Idle(Actor_ObjKibako2* this, GameState_Play* play);
void ObjKibako2_Kill(Actor_ObjKibako2* this, GameState_Play* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x40000040, 0x00, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 31, 48, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

static void ObjKibako2_InitCollider(Actor_ObjKibako2* this, GameState_Play* play)
{
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->dyna.actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->dyna.actor, &this->collider);
}

void ObjKibako2_Break(Actor_ObjKibako2* this, GameState_Play* play)
{
    Vec3f* thisPos;
    Vec3f pos;
    Vec3f velocity;
    s16 angle;
    s32 i;

    thisPos = &this->dyna.actor.world.pos;
    for (i = 0, angle = 0; i < 0x10; i++, angle += 0x4E20)
    {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s32 phi_s0;

        temp_rand = Rand_ZeroOne() * 30.0f;
        pos.x = sn * temp_rand;
        pos.y = (Rand_ZeroOne() * 10.0f) + 2.0f;
        pos.z = cs * temp_rand;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 10.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        pos.x += thisPos->x;
        pos.y += thisPos->y;
        pos.z += thisPos->z;
        temp_rand = Rand_ZeroOne();
        if (temp_rand < 0.05f) {
            phi_s0 = 0x60;
        } else if (temp_rand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, -200, phi_s0, 28, 2, 0, (Rand_ZeroOne() * 30.0f) + 5.0f, 0, 0, 70, KAKERA_COLOR_NONE, OBJECT_KIBAKO2, SEGADDR_FRAGMENT_DL);
    }
    SomethingPotBreak(play, thisPos, 90.0f, 6, 100, 160, 1);
}

void ObjKibako2_SpawnCollectible(Actor_ObjKibako2* this, GameState_Play* play)
{
    s16 itemDropped;
    s16 collectibleFlagTemp;

    collectibleFlagTemp = this->collectibleFlag;
    itemDropped = this->dyna.actor.home.rot.x;
    if (itemDropped >= 0 && itemDropped < ITEM00_MAX) {
        Item_DropCollectible(play, &this->dyna.actor.world.pos, itemDropped | (collectibleFlagTemp << 8));
    }
}

void ObjKibako2_Init(Actor_ObjKibako2* this, GameState_Play* play)
{
    CollisionHeader* colHeader;
    u32 bgId;

    colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, 0);
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    ObjKibako2_InitCollider(this, play);
    CollisionHeader_GetVirtual(SEGADDR_COLLIDER, &colHeader);
    bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    this->dyna.bgId = bgId;
    this->collectibleFlag = this->dyna.actor.home.rot.z & 0x3F;
    this->actionFunc = ObjKibako2_Idle;
    this->dyna.actor.home.rot.z = this->dyna.actor.world.rot.z = this->dyna.actor.shape.rot.z = this->dyna.actor.world.rot.x = this->dyna.actor.shape.rot.x = 0;
}

void ObjKibako2_Destroy(Actor_ObjKibako2* this, GameState_Play* play)
{
    Collider_DestroyCylinder(play, &this->collider);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void ObjKibako2_Idle(Actor_ObjKibako2* this, GameState_Play* play)
{
    if ((this->collider.base.acFlags & AC_HIT) || (this->dyna.actor.home.rot.z != 0) || UnkFuncActorCollision(play, &this->dyna.actor) != NULL)
    {
        ObjKibako2_Break(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        this->dyna.actor.flags |= ACTOR_FLAG_OOT_4;
        DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
        this->dyna.actor.draw = NULL;
        this->actionFunc = ObjKibako2_Kill;
    }
    else if (this->dyna.actor.xzDistToPlayer < 600.0f)
    {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

void ObjKibako2_Kill(Actor_ObjKibako2* this, GameState_Play* play) {
    s16 params = this->dyna.actor.variable;

    if ((params & 0x8000) == 0)
    {
        Actor_Spawn(
            &play->actorCtx, play, AC_EN_SW,
            this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z,
            0, this->dyna.actor.shape.rot.y, 0,
            params | 0x8000
        );
    }
    ObjKibako2_SpawnCollectible(this, play);
    Actor_Kill(&this->dyna.actor);
}

void ObjKibako2_Update(Actor_ObjKibako2* this, GameState_Play* play)
{
    this->actionFunc(this, play);
}

void ObjKibako2_Draw(Actor_ObjKibako2* this, GameState_Play* play)
{
    Gfx_DrawDListOpa(play, SEGADDR_CRATE_DL);
}

ActorInit Obj_Kibako2_InitVars = {
    AC_OBJ_KIBAKO2,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_KIBAKO2,
    sizeof(Actor_ObjKibako2),
    (ActorFunc)ObjKibako2_Init,
    (ActorFunc)ObjKibako2_Destroy,
    (ActorFunc)ObjKibako2_Update,
    (ActorFunc)ObjKibako2_Draw,
};

OVL_ACTOR_INFO(AC_OBJ_KIBAKO2, Obj_Kibako2_InitVars);
