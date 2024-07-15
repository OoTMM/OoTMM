#include <combo.h>
#include <combo/item.h>
#include <combo/global.h>
#include "Obj_Kibako2.h"

#define FLAGS 0

static int ObjKibako2_IsShuffled(Actor_ObjKibako2* this)
{
    if (!this->isExtended || comboXflagsGet(&this->xflag))
        return 0;
    return 1;
}

#if defined(GAME_OOT)
# define SEGADDR_COLLIDER       ((void*)0x06000b70)
# define SEGADDR_CRATE_DL       ((void*)0x06000960)
# define SEGADDR_FRAGMENT_DL    ((void*)0x06001000)

void ObjKibako2_Destroy(Actor_ObjKibako2* this, GameState_Play* play);
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
    SpawnSomeDust(play, thisPos, 90.0f, 6, 100, 160, 1);
}

void ObjKibako2_SpawnCollectible(Actor_ObjKibako2* this, GameState_Play* play)
{
    s16 itemDropped;
    s16 collectibleFlagTemp;

    if (ObjKibako2_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->dyna.actor.world.pos, &this->xflag);
        return;
    }

    collectibleFlagTemp = this->collectibleFlag;
    itemDropped = this->dyna.actor.home.rot.x;
    if (itemDropped >= 0 && itemDropped < ITEM00_MAX) {
        Item_DropCollectible(play, &this->dyna.actor.world.pos, itemDropped | (collectibleFlagTemp << 8));
    }
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
#endif

#if defined(GAME_MM)
# define KIBAKO2_COLLECTIBLE_ID(thisx) ((thisx)->dyna.actor.variable & 0x3F)
# define KIBAKO2_COLLECTIBLE_FLAG(thisx) (((thisx)->dyna.actor.variable >> 0x8) & 0x7F)
# define KIBAKO2_SKULLTULA_SPAWN_PARAM(thisx) ((((thisx)->dyna.actor.variable & 0x1F) << 2) | 0xFF01)
# define KIBAKO2_CONTENTS(thisx) (((thisx)->dyna.actor.variable >> 0xF) & 1)

# define SEGADDR_COLLIDER       ((void*)0x06000b70)
# define SEGADDR_CRATE_DL       ((void*)0x06000960)
# define SEGADDR_FRAGMENT_DL    ((void*)0x06001040)

typedef enum
{
    OBJKIBAKO2_CONTENTS_COLLECTIBLE,
    OBJKIBAKO2_CONTENTS_SKULLTULA
}
Actor_ObjKibako2Contents;

void ObjKibako2_Destroy(Actor_ObjKibako2* this, GameState_Play* play);
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
        { 0x80000508, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 31, 48, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 200, ICHAIN_STOP),
};

int ObjKibako2_ContainsSkulltula(Actor_ObjKibako2* this, GameState_Play* play)
{
    s32 actorSpawnParam = KIBAKO2_SKULLTULA_SPAWN_PARAM(this);
    s32 flag = -1;

    if ((u16)actorSpawnParam & 3) {
        flag = ((actorSpawnParam & 0x3FC) >> 2) & 0xFF;
    }
    return !((flag >= 0) && Flags_GetTreasure(play, flag));
}

void ObjKibako2_Break(Actor_ObjKibako2* this, GameState_Play* play)
{
    Vec3f* thisPos = &this->dyna.actor.world.pos;
    Vec3f pos;
    Vec3f velocity;
    s16 angle;
    s32 i;

    for (i = 0, angle = 0; i < 0x10; i++, angle += 0x4E20) {
        f32 sin = Math_SinS(angle);
        f32 cos = Math_CosS(angle);
        f32 tempRand = Rand_ZeroOne() * 30.0f;
        s32 phi_s0;

        pos.x = sin * tempRand;
        pos.y = (Rand_ZeroOne() * 10.0f) + 2.0f;
        pos.z = cos * tempRand;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 10.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        pos.x += thisPos->x;
        pos.y += thisPos->y;
        pos.z += thisPos->z;
        tempRand = Rand_ZeroOne();
        if (tempRand < 0.05f) {
            phi_s0 = 0x60;
        } else if (tempRand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, -200, phi_s0, 28, 2, 0, (Rand_ZeroOne() * 30.0f) + 5.0f, 0, 0, 70, KAKERA_COLOR_NONE, OBJECT_KIBAKO2, SEGADDR_FRAGMENT_DL);
    }
    SpawnSomeDust(play, thisPos, 90.0f, 6, 100, 160, 1);
}

void ObjKibako2_SpawnCollectible(Actor_ObjKibako2* this, GameState_Play* play)
{
    s32 dropItem00Id;

    if (ObjKibako2_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->dyna.actor.world.pos, &this->xflag);
        return;

    }

    dropItem00Id = Item_CollectibleDropTable(KIBAKO2_COLLECTIBLE_ID(this));
    if (dropItem00Id > ITEM00_NO_DROP) {
        Item_DropCollectible(play, &this->dyna.actor.world.pos,
                             dropItem00Id | KIBAKO2_COLLECTIBLE_FLAG(this) << 8);
    }
}

void ObjKibako2_SpawnSkulltula(Actor_ObjKibako2* this, GameState_Play* play)
{
    s16 yRotation;
    s32 actorSpawnParam;
    Actor* skulltula;

    if (ObjKibako2_ContainsSkulltula(this, play)) {
        actorSpawnParam = KIBAKO2_SKULLTULA_SPAWN_PARAM(this);
        yRotation = (Rand_Next() >> 0x11) + this->dyna.actor.yawTowardsPlayer + 0xC000;
        skulltula =
            Actor_Spawn(&play->actorCtx, play, AC_EN_SW, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                        this->dyna.actor.world.pos.z, 0, yRotation, 0, actorSpawnParam);
        if (skulltula != NULL) {
            skulltula->parent = &this->dyna.actor;
            skulltula->velocity.y = 13.0f;
            skulltula->speed = 0.0f;
        }
    }
}

void ObjKibako2_SpawnContents(Actor_ObjKibako2* this, GameState_Play* play)
{
    if (KIBAKO2_CONTENTS(this) == OBJKIBAKO2_CONTENTS_COLLECTIBLE) {
        ObjKibako2_SpawnCollectible(this, play);
    } else {
        ObjKibako2_SpawnSkulltula(this, play);
    }
}

void ObjKibako2_Destroy(Actor_ObjKibako2* this, GameState_Play* play)
{
    Collider_DestroyCylinder(play, &this->collider);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

s32 ObjKibako2_ShouldBreak(Actor_ObjKibako2* this)
{
    u8 acFlags = this->collider.base.acFlags;
    s32 shouldBreak = FALSE;

    if (this->collider.base.acFlags & AC_HIT) {
        Actor* ac = this->collider.base.ac;
        this->collider.base.acFlags = acFlags & ~AC_HIT;
        if (ac != NULL) {
            if (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & (1 << 31)) {
                // Powder Keg
                if (Math3D_Vec3fDistSq(&this->dyna.actor.world.pos, &ac->world.pos) < SQ(160.0f)) {
                    shouldBreak = TRUE;
                }
            } else if (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & (1 << 3)) {
                // Explosives
                if (Math3D_Vec3fDistSq(&this->dyna.actor.world.pos, &ac->world.pos) < SQ(100.0f)) {
                    shouldBreak = TRUE;
                }
            } else if (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & (1 << 8 | 1 << 10)) {
                // Goron Punch/Pound
                shouldBreak = TRUE;
            }
        }
    } else if (this->dyna.actor.home.rot.z != 0) {
        shouldBreak = TRUE;
    }
    return shouldBreak;
}

void ObjKibako2_Idle(Actor_ObjKibako2* this, GameState_Play* play)
{
    if (ObjKibako2_ShouldBreak(this))
    {
        ObjKibako2_Break(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        this->dyna.actor.flags |= ACTOR_FLAG_MM_10;
        DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
        this->dyna.actor.draw = NULL;
        this->actionFunc = ObjKibako2_Kill;
    }
    else if (this->dyna.actor.xzDistToPlayer < 600.0f)
    {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

void ObjKibako2_Kill(Actor_ObjKibako2* this, GameState_Play* play)
{
    ObjKibako2_SpawnContents(this, play);
    Actor_Kill(&this->dyna.actor);
}
#endif

static void ObjKibako2_InitCollider(Actor_ObjKibako2* this, GameState_Play* play)
{
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->dyna.actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->dyna.actor, &this->collider);
}

#if defined(GAME_OOT)
static void ObjKibako2_Alias(Actor_ObjKibako2* this)
{
}
#endif

#if defined(GAME_MM)
static void ObjKibako2_Alias(Actor_ObjKibako2* this)
{
}
#endif

static void ObjKibako2_InitXflag(Actor_ObjKibako2* this, GameState_Play* play)
{
    ComboItemOverride o;

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->dyna.actor.room;
    this->xflag.sliceId = 0;
    this->xflag.id = this->dyna.actor.actorIndex;

    /* Fix the aliases */
    ObjKibako2_Alias(this);

    /* Detect xflags */
    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));
}

#if defined(GAME_OOT)
static void ObjKibako2_Init(Actor_ObjKibako2* this, GameState_Play* play)
{
    CollisionHeader* colHeader;
    u32 bgId;

    ObjKibako2_InitXflag(this, play);
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
#endif

#if defined(GAME_MM)
static void ObjKibako2_Init(Actor_ObjKibako2* this, GameState_Play* play)
{
    s32 contents;

    ObjKibako2_InitXflag(this, play);
    contents = KIBAKO2_CONTENTS(this);
    DynaPolyActor_Init(&this->dyna, 0);
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    ObjKibako2_InitCollider(this, play);
    DynaPolyActor_LoadMesh(play, &this->dyna, SEGADDR_COLLIDER);
    this->dyna.actor.home.rot.z = 0;
    this->dyna.actor.world.rot.z = 0;
    this->dyna.actor.shape.rot.z = 0;
    this->dyna.actor.world.rot.x = 0;
    this->dyna.actor.shape.rot.x = 0;
    if (contents == OBJKIBAKO2_CONTENTS_COLLECTIBLE) {
        if (Item_CanDropBigFairy(play, KIBAKO2_COLLECTIBLE_ID(this),
                                 KIBAKO2_COLLECTIBLE_FLAG(this))) {
            this->unk_1AC = 1;
            this->dyna.actor.flags |= ACTOR_FLAG_MM_10;
        }
    }
    if ((contents != OBJKIBAKO2_CONTENTS_SKULLTULA) || !ObjKibako2_ContainsSkulltula(this, play)) {
        this->skulltulaNoiseTimer = -1;
    }
    this->actionFunc = ObjKibako2_Idle;
}
#endif

static void ObjKibako2_Update(Actor_ObjKibako2* this, GameState_Play* play)
{
#if defined(GAME_MM)
    if (this->unk_1AC != 0) {
        play->actorCtx.flags |= ACTORCTX_FLAG_3;
    }

    if (this->skulltulaNoiseTimer >= 0)
    {
        if (this->skulltulaNoiseTimer == 0)
        {
            Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_STALGOLD_ROLL);
            if (Rand_ZeroOne() < 0.1f) {
                this->skulltulaNoiseTimer = Rand_S16Offset(40, 80);
            } else {
                this->skulltulaNoiseTimer = 8;
            }
        } else {
            this->skulltulaNoiseTimer--;
        }
    }
#endif

    this->actionFunc(this, play);
}

static void ObjKibako2_Draw(Actor_ObjKibako2* this, GameState_Play* play)
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
