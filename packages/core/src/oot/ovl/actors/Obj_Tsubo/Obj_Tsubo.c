#include <combo.h>
#include <combo/object_id.h>
#include <combo/collision.h>
#include <combo/math.h>
#include <combo/rand.h>
#include <combo/player.h>
#include <combo/effect.h>
#include <combo/csmc.h>
#include <combo/global.h>
#include "Obj_Tsubo.h"

#define FLAGS (ACTOR_FLAG_OOT_4 | ACTOR_FLAG_OOT_23)

#if defined(GAME_OOT)
# define DL_WHOLE_KEEP          0x05017870
# define DL_FRAGMENT_KEEP       0x05017a60
# define DL_WHOLE_OBJ_TSUBO     0x060017c0
# define DL_FRAGMENT_OBJ_TSUBO  0x06001960
#endif

void ObjTsubo_Init(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_Destroy(Actor_ObjTsubo* this, GameState_Play* play2);
void ObjTsubo_Update(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_Draw(Actor_ObjTsubo* this, GameState_Play* play);

void ObjTsubo_SpawnCollectible(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_ApplyGravity(Actor_ObjTsubo* this);
s32 ObjTsubo_SnapToFloor(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_InitCollider(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_AirBreak(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_WaterBreak(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_SetupWaitForObject(Actor_ObjTsubo* this);
void ObjTsubo_WaitForObject(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_SetupIdle(Actor_ObjTsubo* this);
void ObjTsubo_Idle(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_SetupLiftedUp(Actor_ObjTsubo* this);
void ObjTsubo_LiftedUp(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_SetupThrown(Actor_ObjTsubo* this);
void ObjTsubo_Thrown(Actor_ObjTsubo* this, GameState_Play* play);

static s16 D_80BA1B50 = 0;
static s16 D_80BA1B54 = 0;
static s16 D_80BA1B58 = 0;
static s16 D_80BA1B5C = 0;

static s16 sObjectIds[] = { OBJECT_GAMEPLAY_DANGEON_KEEP, OBJECT_TSUBO };

static Gfx* D_80BA1B84[] = { (Gfx*)DL_WHOLE_KEEP, (Gfx*)DL_WHOLE_OBJ_TSUBO };

static Gfx* D_80BA1B8C[] = { (Gfx*)DL_FRAGMENT_KEEP, (Gfx*)DL_FRAGMENT_OBJ_TSUBO };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HARD,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000002, 0x00, 0x01 },
        { 0x4FC1FFFE, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 9, 26, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit[] = {{ 0, 12, 60, MASS_IMMOVABLE }};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -1200, ICHAIN_CONTINUE), ICHAIN_F32_DIV1000(minVelocityY, -20000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 150, ICHAIN_CONTINUE),   ICHAIN_F32(uncullZoneForward, 900, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),   ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

#if defined(GAME_OOT)
void ObjTsubo_Alias(Actor_ObjTsubo* this)
{
    Xflag* xflag;

    xflag = &this->xflag;

    switch (xflag->sceneId)
    {
    case SCE_OOT_KAKARIKO_VILLAGE:
    case SCE_OOT_LON_LON_RANCH:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            xflag->id += 1;
        }
        break;
    case SCE_OOT_ZORA_DOMAIN:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch (xflag->id)
            {
            case 1: xflag->id = 25; break;
            case 2: xflag->id = 22; break;
            case 3: xflag->id = 24; break;
            case 4: xflag->id = 21; break;
            case 5: xflag->id = 23; break;
            }
        }
        break;
    case SCE_OOT_ZORA_FOUNTAIN:
        if (xflag->setupId == 1)
            xflag->setupId = 0;
        break;
    case SCE_OOT_DEATH_MOUNTAIN_CRATER:
        if (xflag->setupId == 0)
        {
            xflag->setupId = 2;
            xflag->id -= 2;
        }
        break;
    case SCE_OOT_GORON_CITY:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch (xflag->roomId)
            {
            case 1: xflag->id += 4; break;
            case 3: xflag->id += 33; break;
            }
        }
        break;
    case SCE_OOT_LAIR_GANONDORF:
        xflag->sceneId = SCE_OOT_GANON_TOWER;
        xflag->roomId = 8;
        xflag->id -= 46;
        break;
    case SCE_OOT_GANON_TOWER_COLLAPSING:
        xflag->sceneId = SCE_OOT_GANON_TOWER;
        xflag->roomId = 8;
        xflag->id -= 15;
        break;
    default:
        break;
    }
}
#endif

int ObjTsubo_IsShuffled(Actor_ObjTsubo* this)
{
    if (!this->isExtended || comboXflagsGet(&this->xflag))
        return 0;
    return 1;
}

void ObjTsubo_SpawnCollectible(Actor_ObjTsubo* this, GameState_Play* play)
{
    s16 dropParams = this->actor.params & 0x1F;

    /* Potsanity */
    if (ObjTsubo_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
        return;
    }

    if ((dropParams >= 0) && (dropParams < ITEM00_MAX))
    {
        Item_DropCollectible(play, &this->actor.world.pos, (dropParams | (((this->actor.params >> 9) & 0x3F) << 8)));
    }
}

void ObjTsubo_ApplyGravity(Actor_ObjTsubo* this)
{
    this->actor.velocity.y += this->actor.gravity;
    if (this->actor.velocity.y < this->actor.minVelocityY)
    {
        this->actor.velocity.y = this->actor.minVelocityY;
    }
}

s32 ObjTsubo_SnapToFloor(Actor_ObjTsubo* this, GameState_Play* play)
{
    CollisionPoly* groundPoly;
    Vec3f pos;
    s32 bgId;
    f32 groundY;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + 20.0f;
    pos.z = this->actor.world.pos.z;
    groundY = BgCheck_EntityRaycastDown4(&play->colCtx, &groundPoly, &bgId, &this->actor, &pos);
    if (groundY > BGCHECK_Y_MIN)
    {
        this->actor.world.pos.y = groundY;
        Math_Vec3f_Copy(&this->actor.home.pos, &this->actor.world.pos);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void ObjTsubo_InitCollider(Actor_ObjTsubo* this, GameState_Play* play)
{
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
}

void ObjTsubo_Init(Actor_ObjTsubo* this, GameState_Play* play)
{
    ComboItemOverride o;

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->actor.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    /* Fix the aliases */
    ObjTsubo_Alias(this);

    /* Detect xflags */
    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ObjTsubo_InitCollider(this, play);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, NULL, sColChkInfoInit);
    if (!ObjTsubo_SnapToFloor(this, play))
    {
        Actor_Kill(&this->actor);
        return;
    }
    this->requiredObjectSlot = Object_GetSlot(&play->objectCtx, sObjectIds[(this->actor.params >> 8) & 1]);
    if (this->requiredObjectSlot < 0)
    {
        Actor_Kill(&this->actor);
    }
    else
    {
        ObjTsubo_SetupWaitForObject(this);
    }
}

void ObjTsubo_Destroy(Actor_ObjTsubo* this, GameState_Play* play)
{
    Collider_DestroyCylinder(play, &this->collider);
}

void ObjTsubo_AirBreak(Actor_ObjTsubo* this, GameState_Play* play)
{
    f32 rand;
    s16 angle;
    Vec3f pos;
    Vec3f velocity;
    f32 sins;
    f32 coss;
    s32 arg5;
    s32 i;

    for (i = 0, angle = 0; i < 15; i++, angle += 0x4E20)
    {
        sins = Math_SinS(angle);
        coss = Math_CosS(angle);
        pos.x = sins * 8.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = coss * 8.0f;
        velocity.x = pos.x * 0.23f;
        velocity.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        velocity.z = pos.z * 0.23f;
        Math_Vec3f_Sum(&pos, &this->actor.world.pos, &pos);
        rand = Rand_ZeroOne();
        if (rand < 0.2f) {
            arg5 = 96;
        } else if (rand < 0.6f) {
            arg5 = 64;
        } else {
            arg5 = 32;
        }
        EffectSsKakera_Spawn(
            play, &pos, &velocity,
            &this->actor.world.pos, -240, arg5, 10, 10, 0,
            (Rand_ZeroOne() * 95.0f) + 15.0f, 0, 32, 60, KAKERA_COLOR_NONE,
            sObjectIds[(this->actor.params >> 8) & 1], D_80BA1B8C[(this->actor.params >> 8) & 1]
        );
    }
    SpawnSomeDust(play, &this->actor.world.pos, 30.0f, 4, 20, 50, 1);
}

void ObjTsubo_WaterBreak(Actor_ObjTsubo* this, GameState_Play* play)
{
    s16 angle;
    Vec3f pos = this->actor.world.pos;
    Vec3f velocity;
    s32 phi_s0;
    s32 i;

    pos.y += this->actor.depthInWater;
    EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 400);
    for (i = 0, angle = 0; i < 15; i++, angle += 0x4E20)
    {
        f32 sins = Math_SinS(angle);
        f32 coss = Math_CosS(angle);

        pos.x = sins * 8.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = coss * 8.0f;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        Math_Vec3f_Sum(&pos, &this->actor.world.pos, &pos);
        phi_s0 = (Rand_ZeroOne() < .2f) ? 64 : 32;
        EffectSsKakera_Spawn(
            play, &pos, &velocity,
            &this->actor.world.pos, -180, phi_s0, 30, 30, 0,
            (Rand_ZeroOne() * 95.0f) + 15.0f, 0, 32, 70, KAKERA_COLOR_NONE,
            sObjectIds[(this->actor.params >> 8) & 1], D_80BA1B8C[(this->actor.params >> 8) & 1]
        );
    }
}

void ObjTsubo_SetupWaitForObject(Actor_ObjTsubo* this)
{
    this->actionFunc = ObjTsubo_WaitForObject;
}

void ObjTsubo_WaitForObject(Actor_ObjTsubo* this, GameState_Play* play)
{
    if (Object_IsLoaded(&play->objectCtx, this->requiredObjectSlot))
    {
        this->actor.draw = ObjTsubo_Draw;
        this->actor.objectSlot = this->requiredObjectSlot;
        ObjTsubo_SetupIdle(this);
        this->actor.flags &= ~ACTOR_FLAG_OOT_4;
    }
}

void ObjTsubo_SetupIdle(Actor_ObjTsubo* this)
{
    this->actionFunc = ObjTsubo_Idle;
}

void ObjTsubo_Idle(Actor_ObjTsubo* this, GameState_Play* play)
{
    s16 temp_v0;
    s32 phi_v1;

    if (Actor_HasParent(&this->actor, play))
    {
        ObjTsubo_SetupLiftedUp(this);
    }
    else if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) && (this->actor.depthInWater > 15.0f))
    {
        ObjTsubo_WaterBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        ObjTsubo_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    }
    else if ((this->collider.base.acFlags & AC_HIT) && (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & (DMG_SWORD | DMG_RANGED | DMG_HAMMER | DMG_BOOMERANG | DMG_EXPLOSIVE)))
    {
        ObjTsubo_AirBreak(this, play);
        ObjTsubo_SpawnCollectible(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&this->actor);
    }
    else
    {
        if (this->actor.xzDistToPlayer < 600.0f)
        {
            Collider_UpdateCylinder(&this->actor, &this->collider);
            this->collider.base.acFlags &= ~AC_HIT;
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            if (this->actor.xzDistToPlayer < 150.0f)
            {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            }
        }

        if (this->actor.xzDistToPlayer < 100.0f)
        {
            temp_v0 = this->actor.yawTowardsPlayer - GET_PLAYER(play)->actor.world.rot.y;
            phi_v1 = ABS(temp_v0);
            if (phi_v1 >= 0x5556)
            {
                // GI_NONE in this case allows the player to lift the actor
                Actor_OfferGetItem(&this->actor, play, GI_NONE, 30.0f, 30.0f);
            }
        }
    }
}

void ObjTsubo_SetupLiftedUp(Actor_ObjTsubo* this)
{
    this->actionFunc = ObjTsubo_LiftedUp;
    this->actor.room = -1;
    //! @bug: This is an unsafe cast, although the sound effect will still play
    Player_PlaySfx((Actor_Player*)&this->actor, NA_SE_PL_PULL_UP_POT);
    this->actor.flags |= ACTOR_FLAG_OOT_4;
}

void ObjTsubo_LiftedUp(Actor_ObjTsubo* this, GameState_Play* play)
{
    if (Actor_HasNoParent(&this->actor, play))
    {
        this->actor.room = play->roomCtx.curRoom.num;
        ObjTsubo_SetupThrown(this);
        ObjTsubo_ApplyGravity(this);
        Actor_UpdatePos(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 5.0f, 15.0f, 0.0f, UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_OOT_7);
    }
}

void ObjTsubo_SetupThrown(Actor_ObjTsubo* this) {
    this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speed;
    this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speed;
    this->actor.colChkInfo.mass = 240;
    D_80BA1B50 = (Rand_ZeroOne() - 0.7f) * 2800.0f;
    D_80BA1B58 = (Rand_ZeroOne() - 0.5f) * 2000.0f;
    D_80BA1B54 = 0;
    D_80BA1B5C = 0;
    this->actionFunc = ObjTsubo_Thrown;
}

void ObjTsubo_Thrown(Actor_ObjTsubo* this, GameState_Play* play)
{
    if ((this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL)) || (this->collider.base.atFlags & AT_HIT))
    {
        ObjTsubo_AirBreak(this, play);
        ObjTsubo_SpawnCollectible(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&this->actor);
    }
    else if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH)
    {
        ObjTsubo_WaterBreak(this, play);
        ObjTsubo_SpawnCollectible(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&this->actor);
    }
    else
    {
        ObjTsubo_ApplyGravity(this);
        Actor_UpdatePos(&this->actor);
        Math_StepToS(&D_80BA1B54, D_80BA1B50, 0x64);
        Math_StepToS(&D_80BA1B5C, D_80BA1B58, 0x64);
        this->actor.shape.rot.x += D_80BA1B54;
        this->actor.shape.rot.y += D_80BA1B5C;
        Actor_UpdateBgCheckInfo(play, &this->actor, 5.0f, 15.0f, 0.0f, UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_OOT_7);
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
}

void ObjTsubo_Update(Actor_ObjTsubo* this, GameState_Play* play)
{
    this->actionFunc(this, play);
}

void ObjTsubo_Draw(Actor_ObjTsubo* this, GameState_Play* play)
{
    int type;
    ComboItemOverride o;

    if (ObjTsubo_IsShuffled(this))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    if (this->actor.params & (1 << 8))
        type = CSMC_POT_NORMAL;
    else
        type = CSMC_POT_NORMAL_DANGEON;
    csmcPotPreDraw(&this->actor, play, o.gi, type);
    Gfx_DrawDListOpa(play, D_80BA1B84[(this->actor.params >> 8) & 1]);
}

static ActorProfile ObjTsubo_InitVars =
{
    AC_OBJ_TSUBO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Actor_ObjTsubo),
    (ActorFunc)ObjTsubo_Init,
    (ActorFunc)ObjTsubo_Destroy,
    (ActorFunc)ObjTsubo_Update,
    NULL,
};

OVL_ACTOR_INFO(AC_OBJ_TSUBO, ObjTsubo_InitVars);
