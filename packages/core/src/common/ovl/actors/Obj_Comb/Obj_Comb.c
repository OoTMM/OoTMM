#include <combo.h>
#include <combo/item.h>
#include <combo/global.h>
#include <combo/csmc.h>
#include <combo/custom.h>
#include "Obj_Comb.h"

#define FLAGS 0

void ObjComb_Init(Actor_ObjComb* this, GameState_Play* play);
void ObjComb_Update(Actor_ObjComb* this, GameState_Play* play);

static int ObjComb_IsShuffled(Actor_ObjComb* this)
{
    return !!(this->isExtended && !comboXflagsGet(&this->xflag));
}

static int ObjComb_DropCustom(Actor_ObjComb* this, GameState_Play* play)
{
    if (!ObjComb_IsShuffled(this))
        return 0;
    EnItem00_DropCustomNoInertia(play, &this->actor.world.pos, &this->xflag);
    return 1;
}

static void ObjComb_InitXflag(Actor_ObjComb* this, GameState_Play* play)
{
    ComboItemOverride   o;
    Xflag*              xflag;

    /* Set the extended properties */
    xflag = &this->xflag;
    xflag->sceneId = play->sceneId;
    xflag->setupId = g.sceneSetupId;
    xflag->roomId = this->actor.room;
    xflag->sliceId = 0;
    xflag->id = this->actor.actorIndex;

#if defined(GAME_OOT)
    if (xflag->sceneId == SCE_OOT_GROTTOS)
    {
        /* Grottos */
        switch (xflag->roomId)
        {
        case 0x00:
            /* Genertic grottos */
            xflag->roomId = 0x20 | (gGrottoData & 0x1f);
            break;
        case 0x09:
            /* Scrubs x2 */
            switch (gLastScene)
            {
            case SCE_OOT_SACRED_FOREST_MEADOW: xflag->roomId = 0x21; break;
            case SCE_OOT_ZORA_RIVER: xflag->roomId = 0x24; break;
            case SCE_OOT_GERUDO_VALLEY: xflag->roomId = 0x25; break;
            case SCE_OOT_DESERT_COLOSSUS: xflag->roomId = 0x26; break;
            default: UNREACHABLE(); break;
            }
            break;
        case 0x0c:
            /* Scrubs x3 */
            switch (gLastScene)
            {
            case SCE_OOT_LON_LON_RANCH: xflag->roomId = 0x27; break;
            case SCE_OOT_GORON_CITY: xflag->roomId = 0x2a; break;
            case SCE_OOT_DEATH_MOUNTAIN_CRATER: xflag->roomId = 0x2b; break;
            case SCE_OOT_LAKE_HYLIA: xflag->roomId = 0x2d; break;
            default: UNREACHABLE(); break;
            }
            break;
        }
    }
#endif

#if defined(GAME_MM)
    if (xflag->roomId == 0x0a && gLastScene == SCE_MM_GREAT_BAY_COAST)
        xflag->roomId = 0x0f;
#endif

    /* Detect xflags */
    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));
}

#if defined(GAME_OOT)
# define OBJECT             OBJECT_GAMEPLAY_FIELD_KEEP
# define HIVE_FRAGMENT_DL   ((Gfx*)0x05009940)
# define HIVE_DL            ((Gfx*)0x050095b0)
# define HIVE_TEXTURE       0x05008900

void ObjComb_Break(Actor_ObjComb* this, GameState_Play* play);
void ObjComb_ChooseItemDrop(Actor_ObjComb* this, GameState_Play* play);
void ObjComb_SetupWait(Actor_ObjComb* this);
void ObjComb_Wait(Actor_ObjComb* this, GameState_Play* play);

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x4001FFFE, 0x00, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 900, ICHAIN_STOP),
};

void ObjComb_Break(Actor_ObjComb* this, GameState_Play* play)
{
    Vec3f pos1;
    Vec3f pos;
    Vec3f velocity;
    s16 scale;
    s16 angle = 0;
    s16 gravity;
    u8 arg5;
    u8 arg6;
    f32 rand1;
    f32 rand2;
    s32 i;

    for (i = 0; i < 31; i++) {
        angle += 0x4E20;
        rand1 = Rand_ZeroOne() * 10.0f;

        pos1.x = Math_SinS(angle) * rand1;
        pos1.y = (i - 15) * 0.7f;
        pos1.z = Math_CosS(angle) * rand1;

        Math_Vec3f_Sum(&pos1, &this->actor.world.pos, &pos);

        velocity.x = (Rand_ZeroOne() - 0.5f) + pos1.x * 0.5f;
        velocity.y = (Rand_ZeroOne() - 0.5f) + pos1.y * 0.6f;
        velocity.z = (Rand_ZeroOne() - 0.5f) + pos1.z * 0.5f;

        scale = Rand_ZeroOne() * 72.0f + 25.0f;

        if (scale < 40) {
            gravity = -200;
            arg6 = 40;
        } else if (scale < 70) {
            gravity = -280;
            arg6 = 30;
        } else {
            gravity = -340;
            arg6 = 20;
        }

        rand2 = Rand_ZeroOne();

        if (rand2 < 0.1f) {
            arg5 = 96;
        } else if (rand2 < 0.8f) {
            arg5 = 64;
        } else {
            arg5 = 32;
        }

        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, gravity, arg5, arg6, 4, 0, scale, 0, 0, 80, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_FIELD_KEEP, HIVE_FRAGMENT_DL);
    }

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y - 10.0f;
    pos.z = this->actor.world.pos.z;

    SpawnSomeDust(play, &pos, 40.0f, 6, 70, 60, 1);
}

void ObjComb_ChooseItemDrop(Actor_ObjComb* this, GameState_Play* play)
{
    s16 params = this->actor.params & 0x1f;

    if (ObjComb_DropCustom(this, play))
        return;

    if ((params > 0) || (params < ITEM00_MAX))
    {
        if (params == ITEM00_HEART_PIECE) {
            if (Flags_GetCollectible(play, (this->actor.params >> 8) & 0x3F)) {
                params = -1;
            } else {
                params = (params | (((this->actor.params >> 8) & 0x3F) << 8));
            }
        } else if (Rand_ZeroOne() < 0.5f) {
            params = -1;
        }
        if (params >= 0) {
            Item_DropCollectible(play, &this->actor.world.pos, params);
        }
    }
}

void ObjComb_Init(Actor_ObjComb* this, GameState_Play* play)
{
    ObjComb_InitXflag(this, play);
    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderItems);
    ObjComb_SetupWait(this);
}

void ObjComb_SetupWait(Actor_ObjComb* this)
{
    this->actionFunc = ObjComb_Wait;
}

void ObjComb_Wait(Actor_ObjComb* this, GameState_Play* play)
{
    s32 dmgFlags;

    this->unk_1B0 -= 50;
    if (this->unk_1B0 < 0) {
        this->unk_1B0 = 0;
    }

    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        dmgFlags = this->collider.elements[0].elem.acHitElem->atDmgInfo.dmgFlags;
        if (dmgFlags & (DMG_HAMMER | DMG_ARROW | DMG_SLINGSHOT | DMG_DEKU_STICK))
        {
            this->unk_1B0 = 1500;
        }
        else
        {
            ObjComb_Break(this, play);
            ObjComb_ChooseItemDrop(this, play);
            Actor_Kill(&this->actor);
        }
    }
    else
    {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }

    if (this->actor.update != NULL)
    {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

void ObjComb_Update(Actor_ObjComb* this, GameState_Play* play)
{
    this->unk_1B2 += 0x2EE0;
    this->actionFunc(this, play);
    this->actor.shape.rot.x = Math_SinS(this->unk_1B2) * this->unk_1B0 + this->actor.home.rot.x;
}
#endif

#if defined(GAME_MM)
# define OBJECT                     OBJECT_COMB

# define OBJCOMB_GET_F(thisx)       ((thisx)->actor.params & 0xF)
# define OBJCOMB_GET_10(thisx)      (((thisx)->actor.params >> 4) & 0x1)
# define OBJCOMB_GET_1F(thisx)      ((thisx)->actor.params & 0x1F)
# define OBJCOMB_GET_3F(thisx)      ((thisx)->actor.params & 0x3F)
# define OBJCOMB_GET_80(thisx)      (((thisx)->actor.params >> 7) & 0x1)
# define OBJCOMB_GET_7F00(thisx)    (((thisx)->actor.params >> 0x8) & 0x7F)
# define OBJCOMB_GET_8000(thisx)    (((thisx)->actor.params >> 0xE) & 2)

# define HIVE_FRAGMENT_DL           ((Gfx*)0x06001040)
# define HIVE_DL                    ((Gfx*)0x06000cb0)
# define HIVE_TEXTURE               0x06000000

void func_8098DC44(Actor_ObjComb* this);
void func_8098DC60(Actor_ObjComb* this, GameState_Play* play);
void func_8098DEA0(Actor_ObjComb* this, GameState_Play* play);
void func_8098E098(Actor_ObjComb* this);
void func_8098E0B8(Actor_ObjComb* this, GameState_Play* play);

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x05CBFFBE, 0x00, 0x00 },
            TOUCH_NONE | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    ARRAY_SIZE(sJntSphElementsInit),
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 100, ICHAIN_STOP),
};

int func_8098CE40(Actor_ObjComb* this, GameState_Play* play)
{
    s32 phi_a2 = -1;
    s32 temp_v0 = (OBJCOMB_GET_1F(this) << 2) | 0xFF01;

    if ((u8)temp_v0 & 3) {
        phi_a2 = ((temp_v0 & 0x3FC) >> 2) & 0xFF;
    }

    return ((phi_a2 < 0) == 1) || !Flags_GetTreasure(play, phi_a2);
}

void func_8098CEAC(Actor_ObjComb* this, GameState_Play* play)
{
    Vec3f spDC;
    Vec3f spD0;
    Vec3f spC4;
    s32 i;
    f32 temp_f20;
    s16 temp_s2 = 0;
    s16 temp_f10;
    s32 phi_s0;
    s32 phi_v0;
    s32 phi_s1;

    for (i = 0; i < 31; i++) {
        temp_s2 += 0x4E20;
        temp_f20 = Rand_ZeroOne() * 10.0f;

        spDC.x = Math_SinS(temp_s2) * temp_f20;
        spDC.y = (i - 15) * 0.7f;
        spDC.z = Math_CosS(temp_s2) * temp_f20;

        Math_Vec3f_Sum(&spDC, &this->actor.world.pos, &spD0);

        spC4.x = (Rand_ZeroOne() - 0.5f) + (spDC.x * 0.5f);
        spC4.y = (Rand_ZeroOne() - 0.5f) + (spDC.y * 0.6f);
        spC4.z = (Rand_ZeroOne() - 0.5f) + (spDC.z * 0.5f);

        temp_f10 = (Rand_ZeroOne() * 72.0f) + 25.0f;

        if (temp_f10 < 40) {
            phi_s0 = -200;
            phi_s1 = 40;
        } else if (temp_f10 < 70) {
            phi_s0 = -280;
            phi_s1 = 30;
        } else {
            phi_s0 = -340;
            phi_s1 = 20;
        }

        temp_f20 = Rand_ZeroOne();
        if (temp_f20 < 0.1f) {
            phi_v0 = 0x60;
        } else if (temp_f20 < 0.8f) {
            phi_v0 = 0x40;
        } else {
            phi_v0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &spD0, &spC4, &spD0, phi_s0, phi_v0, phi_s1, 4, 0, temp_f10, 0, 0, 60, -1, OBJECT_COMB, HIVE_FRAGMENT_DL);
    }

    spD0.x = this->actor.world.pos.x;
    spD0.y = this->actor.world.pos.y - 10.0f;
    spD0.z = this->actor.world.pos.z;

    SpawnSomeDust(play, &spD0, 40.0f, 6, 70, 60, 1);
}

void func_8098D19C(Actor_ObjComb* this, GameState_Play* play)
{
    Vec3f spDC;
    Vec3f spD0;
    Vec3f spC4;
    f32 temp_f20;
    s16 temp_s2 = 0;
    s16 temp_f8;
    s32 phi_s0;
    s32 phi_v0;
    s32 phi_s1;
    s32 i;

    for (i = 0; i < 31; i++) {
        temp_s2 += 0x4E20;
        temp_f20 = Rand_ZeroOne() * 10.0f;

        spDC.x = Math_SinS(temp_s2) * temp_f20;
        spDC.y = i * 0.7f;
        spDC.z = Math_CosS(temp_s2) * temp_f20;

        Math_Vec3f_Sum(&spDC, &this->actor.world.pos, &spD0);

        spC4.x = (Rand_ZeroOne() - 0.5f) + (spDC.x * 0.5f);
        spC4.y = (Rand_ZeroOne() * 0.5f) + (spDC.y * 0.5f);
        spC4.z = (Rand_ZeroOne() - 0.5f) + (spDC.z * 0.5f);

        temp_f8 = (Rand_ZeroOne() * 72.0f) + 25.0f;
        if (temp_f8 < 40) {
            phi_s0 = -200;
            phi_s1 = 40;
        } else if (temp_f8 < 70) {
            phi_s0 = -280;
            phi_s1 = 30;
        } else {
            phi_s0 = -340;
            phi_s1 = 20;
        }

        temp_f20 = Rand_ZeroOne();
        if (temp_f20 < 0.1f) {
            phi_v0 = 0x60;
        } else if (temp_f20 < 0.8f) {
            phi_v0 = 0x40;
        } else {
            phi_v0 = 0x20;
        }

        EffectSsKakera_Spawn(play, &spD0, &spC4, &spD0, phi_s0, phi_v0, phi_s1, 4, 0, temp_f8, 0, 0, 60, -1, OBJECT_COMB, HIVE_FRAGMENT_DL);
    }

    spD0.x = this->actor.world.pos.x;
    spD0.y = this->actor.world.pos.y + 20.0f;
    spD0.z = this->actor.world.pos.z;

    SpawnSomeDust(play, &spD0, 40.0f, 6, 70, 60, 1);
}

void func_8098D47C(Actor_ObjComb* this, GameState_Play* play)
{
    Vec3f spD4;
    Vec3f spC8;
    Vec3f spBC;
    f32 temp_f20;
    s16 temp_s1 = 0;
    s16 temp_f10;
    s32 phi_s0;
    s32 phi_v0;
    s32 i;

    for (i = 0; i < 21; i++) {
        temp_s1 += 0x4E20;
        temp_f20 = Rand_ZeroOne() * 10.0f;

        spD4.x = Math_SinS(temp_s1) * temp_f20;
        spD4.y = i * 0.25f;
        spD4.z = Math_CosS(temp_s1) * temp_f20;

        Math_Vec3f_Sum(&spD4, &this->actor.world.pos, &spC8);

        spBC.x = (Rand_ZeroOne() - 0.5f) + (spD4.x * 0.5f);
        spBC.y = Rand_ZeroOne() + (spD4.y * 0.6f) + 6.0f;
        spBC.z = (Rand_ZeroOne() - 0.5f) + (spD4.z * 0.5f);

        temp_f10 = (Rand_ZeroOne() * 72.0f) + 25.0f;
        if (temp_f10 < 40) {
            phi_s0 = -100;
        } else if (temp_f10 < 70) {
            phi_s0 = -180;
        } else {
            phi_s0 = -240;
        }

        if (Rand_ZeroOne() < 0.2f) {
            phi_v0 = 0x40;
        } else {
            phi_v0 = 0x20;
        }

        EffectSsKakera_Spawn(play, &spC8, &spBC, &spC8, phi_s0, phi_v0, 40, 0, 0, temp_f10, 0, 0, 80, -1, OBJECT_COMB, HIVE_FRAGMENT_DL);
    }
}

void func_8098D6E0(Actor_ObjComb* this, GameState_Play* play) {
    s32 i;
    Vec3f sp70;
    f32 temp_f0;
    f32 temp_f20;
    s32 temp;

    sp70.y = this->actor.world.pos.y + this->actor.depthInWater;

    for (temp = 0, i = 0; i < 8; i++, temp += 0x2000) {
        temp_f0 = Rand_ZeroOne();
        temp_f20 = ((1.0f - SQ(temp_f0)) * 14.0f) + 4.0f;
        sp70.x = this->actor.world.pos.x + (Math_SinS((s32)(Rand_ZeroOne() * 8000.0f) + temp) * temp_f20);
        sp70.z = this->actor.world.pos.z + (Math_CosS((s32)(Rand_ZeroOne() * 8000.0f) + temp) * temp_f20);
        EffectSsGSplash_Spawn(play, &sp70, NULL, NULL, 0, 200);
    }

    sp70.x = this->actor.world.pos.x;
    sp70.z = this->actor.world.pos.z;
    EffectSsGSplash_Spawn(play, &sp70, NULL, NULL, 0, 300);
}

void func_8098D870(Actor_ObjComb* this, GameState_Play* play)
{
    int id;

    id = Item_CollectibleDropTable(OBJCOMB_GET_3F(this));
    if (id >= 0)
        Item_DropCollectible(play, &this->actor.world.pos, ((OBJCOMB_GET_7F00(this)) << 8) | id);
}

void ObjComb_SpawnBees(Actor_ObjComb* this, GameState_Play* play)
{
    s32 params = OBJCOMB_GET_F(this);
    s32 i;
    s32 phi_s5;

    if (this->actionFunc == func_8098DC60) {
        phi_s5 = 2;
    } else {
        phi_s5 = 1;
    }
    this->unk_1B6 = 0;

    for (i = 0; i < params; i++)
    {
        if (Actor_Spawn(&play->actorCtx, play, AC_EN_BEE, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, phi_s5))
        {
            this->unk_1B6 = 1;
        }
    }
}

void ObjComb_SpawnSkulltula(Actor_ObjComb* this, GameState_Play* play)
{
    s32 params;
    Actor* temp_v0;

    if (func_8098CE40(this, play))
    {
        params = (OBJCOMB_GET_1F(this) << 2) | 0xFF01;
        temp_v0 = Actor_Spawn(&play->actorCtx, play, AC_EN_SW, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.home.rot.y, 0, params);

        if (temp_v0 != NULL) {
            temp_v0->parent = &this->actor;
            if (this->actionFunc == func_8098DC60) {
                temp_v0->velocity.y = 8.0f;
                temp_v0->speed = 2.0f;
            } else {
                temp_v0->velocity.y = 10.0f;
                temp_v0->speed = 2.0f;
            }
            this->unk_1B6 = 1;
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
        }
    }
}

void func_8098DA74(Actor_ObjComb* this, GameState_Play* play)
{
    s32 temp_v0 = OBJCOMB_GET_8000(this) | OBJCOMB_GET_80(this);

    if (ObjComb_DropCustom(this, play))
        return;

    if (temp_v0 == 0)
    {
        func_8098D870(this, play);
    }
    else if (temp_v0 == 1)
    {
        ObjComb_SpawnBees(this, play);
    }
    else
    {
        ObjComb_SpawnSkulltula(this, play);
    }
}

void ObjComb_Init(Actor_ObjComb* this, GameState_Play* play)
{
    s32 sp2C = OBJCOMB_GET_8000(this) | OBJCOMB_GET_80(this);

    ObjComb_InitXflag(this, play);
    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitJntSph(play, &this->collider);

    if ((sp2C == 1) && OBJCOMB_GET_10(this) && MM_GET_EVENT_WEEK(EV_MM_WEEK_PIRATES_BEES))
    {
        Actor_Kill(&this->actor);
        return;
    }

    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderItems);

    if ((sp2C == 0) && Item_CanDropBigFairy(play, OBJCOMB_GET_3F(this), OBJCOMB_GET_7F00(this))) {
        this->unk_1B7 = 1;
        this->actor.flags |= ACTOR_FLAG_MM_10;
    }

    if ((sp2C != 2) || !func_8098CE40(this, play)) {
        this->unk_1B8 = -1;
    }
    func_8098DC44(this);
}

void func_8098DC44(Actor_ObjComb* this)
{
    this->unk_1AC = 0x2EE0;
    this->actionFunc = func_8098DC60;
}

void func_8098DC60(Actor_ObjComb* this, GameState_Play* play)
{
    this->unk_1AA += this->unk_1AC;
    if (this->unk_1B3) {
        this->collider.base.acFlags &= ~AC_HIT;
    }

    this->unk_1A8 -= 0x32;
    if (this->unk_1A8 < 0) {
        this->unk_1A8 = 0;
    }

    if (this->unk_1B3)
    {
        if (this->collider.elements->elem.acHitElem->atDmgInfo.dmgFlags & 0x0182C29C)
        {
            func_8098CEAC(this, play);
            func_8098DA74(this, play);
            Actor_Kill(&this->actor);
        }
        else
        {
            s32 dmgFlags = this->collider.elements->elem.acHitElem->atDmgInfo.dmgFlags;

            if (dmgFlags & 0x13820) {
                this->unk_1A8 = 0xDAC;
                this->unk_1AC = 0x36B0;
            } else {
                this->unk_1A8 = 0x5DC;
                this->unk_1AC = 0x2EE0;
            }

            if ((this->unk_1B2 <= 0) && (dmgFlags & 0x13820)) {
                if (this->unk_1B5 == 0) {
                    this->unk_1B5 = 1;
                    this->actor.flags |= ACTOR_FLAG_MM_10;
                }
                this->unk_1B2 = 20;
            }
        }
    } else {
        if (this->unk_1B8 >= 0) {
            if (this->unk_1B8 == 0) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_STALGOLD_ROLL);
                if (Rand_ZeroOne() < 0.1f) {
                    this->unk_1B8 = Rand_S16Offset(40, 80);
                } else {
                    this->unk_1B8 = 8;
                }
            } else {
                this->unk_1B8--;
            }
        }
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }

    if (this->actor.update != NULL) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }

    this->actor.shape.rot.x = (s32)(Math_SinS(this->unk_1AA) * this->unk_1A8) + this->actor.home.rot.x;
}

void func_8098DE58(Actor_ObjComb* this)
{
    this->actor.flags |= ACTOR_FLAG_MM_10;
    this->unk_1B4 = 100;
    this->actor.minVelocityY = -20.0f;
    this->actor.gravity = -1.5f;
    this->actor.speed = 0.0f;
    this->actionFunc = func_8098DEA0;
}

void func_8098DEA0(Actor_ObjComb* this, GameState_Play* play) {
    this->unk_1B4--;
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->unk_1B4 <= 0)) {
        func_8098DA74(this, play);
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) && (this->actor.depthInWater > 30.0f)) {
            func_8098D47C(this, play);
        } else {
            func_8098D19C(this, play);
        }
        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_HONEYCOMB_BROKEN);
        func_8098E098(this);
    } else {
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
            func_8098D6E0(this, play);
        }

        if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER) {
            this->actor.gravity = -0.5f;
            this->actor.velocity.y *= 0.8f;
            this->unk_1AE >>= 1;
            this->unk_1B0 >>= 1;
        } else {
            this->actor.gravity = -1.5f;
            this->actor.velocity.y *= 0.96f;

            this->unk_1AE += 0x1F4;
            if (this->unk_1AE > 0x7D0) {
                this->unk_1AE = 0x7D0;
            }

            this->unk_1B0 += 0xC8;
            if (this->unk_1B0 > 0x258) {
                this->unk_1B0 = 0x258;
            }
            Actor_PlaySfx_Flagged(&this->actor, NA_SE_EV_HONEYCOMB_FALL - SFX_FLAG);
        }

        Actor_MoveWithGravity(&this->actor);
        this->actor.shape.rot.x += this->unk_1AE;
        this->actor.shape.rot.y += this->unk_1B0;
        Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 12.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

void func_8098E098(Actor_ObjComb* this)
{
    this->actor.draw = NULL;
    this->unk_1B4 = 20;
    this->actionFunc = func_8098E0B8;
}

void func_8098E0B8(Actor_ObjComb* this, GameState_Play* play)
{
    this->unk_1B4--;
    if (this->unk_1B4 <= 0) {
        Actor_Kill(&this->actor);
        return;
    }

    if ((this->unk_1B4 == 10) && (this->unk_1B6 != 0) && (this->unk_1B5 == 2) && (this->actor.csId > CS_ID_NONE)) {
        if (CutsceneManager_GetCurrentCsId() == this->actor.csId) {
            Player_SetCsActionWithHaltedActors(play, &this->actor, PLAYER_CSACTION_4);
        }
    }
}

void ObjComb_Update(Actor_ObjComb* this, GameState_Play* play)
{
    this->unk_1B3 = (this->collider.base.acFlags & AC_HIT) != 0;
    if (this->unk_1B3) {
        this->collider.base.acFlags &= ~AC_HIT;
    }

    if (this->unk_1B2 > 0) {
        this->unk_1B2--;
        if (this->unk_1B2 == 0) {
            func_8098DE58(this);
        }
    }

    this->actionFunc(this, play);

    if (this->actor.update == NULL) {
        if ((this->unk_1B5 == 2) && (CutsceneManager_GetCutsceneScriptIndex(this->actor.csId) == -1)) {
            CutsceneManager_Stop(this->actor.csId);
            this->unk_1B5 = 0;
        }
    } else {
        if (this->unk_1B5 != 0) {
            Actor_SetFocus(&this->actor, 0.0f);
            if (this->unk_1B5 == 1) {
                if (CutsceneManager_IsNext(this->actor.csId)) {
                    CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
                    if (this->actor.csId > CS_ID_NONE) {
                        Player_SetCsActionWithHaltedActors(play, &this->actor, PLAYER_CSACTION_1);
                    }

                    if (((OBJCOMB_GET_8000(this) | OBJCOMB_GET_80(this)) == 1) && OBJCOMB_GET_10(this))
                    {
                        MM_SET_EVENT_WEEK(EV_MM_WEEK_PIRATES_BEES);
                    }

                    this->unk_1B5 = 2;
                } else {
                    CutsceneManager_Queue(this->actor.csId);
                }
            }
        }

        if (this->unk_1B7 != 0) {
            play->actorCtx.flags |= ACTORCTX_FLAG_3;
            this->actor.flags |= ACTOR_FLAG_MM_10;
        }
    }
}
#endif

static const Gfx sListLoadTextureDefault[] = {
    gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 255),
    gsDPLoadTextureBlock(HIVE_TEXTURE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx sListLoadTextureCustom[] = {
    gsDPLoadTextureBlock(0x09000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static void ObjComb_DrawImpl(Actor_ObjComb* this, GameState_Play* play)
{
    OPEN_DISPS(play->gs.gfx);
    Gfx_SetupDL25_Opa(play->gs.gfx);
    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y + (118.0f * this->actor.scale.y), this->actor.world.pos.z, MAT_SET);
    Matrix_RotateY(BINANG_TO_RAD(this->actor.shape.rot.y), MAT_MUL);
    Matrix_RotateX(BINANG_TO_RAD(this->actor.shape.rot.x), MAT_MUL);
    Matrix_RotateZ(BINANG_TO_RAD(this->actor.shape.rot.z), MAT_MUL);
    Matrix_Translate(0.0f, -(this->actor.scale.y * 118.0f), 0.0f, MAT_MUL);
    Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MAT_MUL);

    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, HIVE_DL);

    Collider_UpdateSpheres(0, &this->collider);
    CLOSE_DISPS();
}

static int ObjComb_CsmcType(Actor_ObjComb* this, GameState_Play* play)
{
    ComboItemOverride o;

    if (!ObjComb_IsShuffled(this))
        return CSMC_NORMAL;

    if (!csmcEnabled())
        return CSMC_MAJOR;

    comboXflagItemOverride(&o, &this->xflag, 0);
    return csmcFromItem(o.gi);
}

static void ObjComb_Draw(Actor_ObjComb* this, GameState_Play* play)
{
    int                 type;
    void*               tex;
    const void*         preList;
    const Color_RGB8*   color;

    OPEN_DISPS(play->gs.gfx);
    type = ObjComb_CsmcType(this, play);
    if (type == CSMC_NORMAL)
    {
        preList = sListLoadTextureDefault;
    }
    else
    {
        tex = comboCacheGetFile(CUSTOM_HIVE_ADDR);
        if (!tex)
            return;
        gSPSegment(POLY_OPA_DISP++, 0x09, (u32)tex & 0xffffff);
        preList = sListLoadTextureCustom;
        color = csmcTypeColor(type);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color->r, color->g, color->b, 255);
    }
    gSPSegment(POLY_OPA_DISP++, 0x08, (u32)preList & 0xffffff);
    ObjComb_DrawImpl(this, play);
    CLOSE_DISPS();
}

static void ObjComb_Destroy(Actor_ObjComb* this, GameState_Play* play)
{
    Collider_DestroyJntSph(play, &this->collider);
}

static const ActorProfile ObjComb_InitVars =
{
    AC_OBJ_COMB,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT,
    sizeof(Actor_ObjComb),
    (ActorFunc)ObjComb_Init,
    (ActorFunc)ObjComb_Destroy,
    (ActorFunc)ObjComb_Update,
    (ActorFunc)ObjComb_Draw,
};

OVL_ACTOR_INFO(AC_OBJ_COMB, ObjComb_InitVars);
