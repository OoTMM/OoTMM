#include <combo/draw.h>
#include "En_Invisible_Ruppe.h"

#define FLAGS 0x00000000

void EnInvisibleRuppe_Init(Actor* thisx, PlayState* play);
void EnInvisibleRuppe_Destroy(Actor* thisx, PlayState* play);
void EnInvisibleRuppe_Update(Actor* thisx, PlayState* play);

void func_80C258A0(EnInvisibleRuppe* this, PlayState* play);
void func_80C2590C(EnInvisibleRuppe* this, PlayState* play);
void func_80C259E8(EnInvisibleRuppe* this, PlayState* play);

ActorProfile En_Invisible_Ruppe_Profile = {
    /**/ ACTOR_EN_INVISIBLE_RUPPE,
    /**/ ACTORCAT_NPC,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(EnInvisibleRuppe),
    /**/ EnInvisibleRuppe_Init,
    /**/ EnInvisibleRuppe_Destroy,
    /**/ EnInvisibleRuppe_Update,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_EN_INVISIBLE_RUPPE, En_Invisible_Ruppe_Profile);

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_NO_PUSH | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },

    {
        ELEM_MATERIAL_UNK4,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_NONE,
        OCELEM_ON,
    },

    { 10, 30, 0, { 0, 0, 0 } },
};

void func_80C258A0(EnInvisibleRuppe* this, PlayState* play) {
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    Actor_UpdateBgCheckInfo(play, &this->actor, 32.0f, 30.0f, 0.0f, UPDBGCHECKINFO_FLAG_FLOOR);
}

static const u8 kRupeeItem00[] = {
    ITEM00_RUPEE_GREEN,
    ITEM00_RUPEE_BLUE,
    ITEM00_RUPEE_RED
};

void func_80C2590C(EnInvisibleRuppe* this, PlayState* play) {
    u8 index;
    ComboItemQuery q;

    if (this->collider.base.ocFlags1 & OC1_HIT) {
        index = INVISIBLERUPPE_GET_3(&this->actor);
        if (index <= 2)
        {
            if (this->xflag != XFLAGID_NONE)
            {
                Xflag_ItemQuery(&q, this->xflag, 0);
                Item_AddWithDecoy(play, &q);
                Xflag_Set(this->xflag);
                this->actor.draw = NULL;
            }
            else
            {
                Audio_PlaySfx(NA_SE_SY_GET_RUPY);
                Item_DropCollectible(play, &this->actor.world.pos, 0x8000 | kRupeeItem00[index]);
            }
        }

        if (this->switchFlag > SWITCH_FLAG_NONE) {
            Flags_SetSwitch(play, this->switchFlag);
        }

        this->actionFunc = func_80C259E8;
    }
}

void func_80C259E8(EnInvisibleRuppe* this, PlayState* play) {
    Actor_Kill(&this->actor);
}

static void EnInvisibleRupee_DrawGlitter(EnInvisibleRuppe* this, PlayState* play)
{
    ComboItemOverride o;

    Xflag_ItemOverride(&o, this->xflag, 0);
    Draw_GlitterGi(play, &this->actor, o.gi, o.cloakGi);
}

void EnInvisibleRuppe_Init(Actor* thisx, PlayState* play) {
    EnInvisibleRuppe* this = (EnInvisibleRuppe*)thisx;

    this->xflag = Xflag_InitEx(thisx, play);
    if (!Xflag_IsShuffledEx(this->xflag))
        this->xflag = XFLAGID_NONE;
    else
        this->actor.draw = EnInvisibleRupee_DrawGlitter;

    this->switchFlag = INVISIBLERUPPE_GET_SWITCH_FLAG(&this->actor);

    if (this->switchFlag == INVISIBLERUPPE_SWITCH_FLAG_NONE) {
        this->switchFlag = SWITCH_FLAG_NONE;
    }

    if (this->xflag == XFLAGID_NONE && (this->switchFlag > SWITCH_FLAG_NONE) && Flags_GetSwitch(play, this->switchFlag)) {
        Actor_Kill(&this->actor);
        return;
    }

    Collider_InitCylinder(play, &this->collider);
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);

    this->actionFunc = func_80C2590C;
}

void EnInvisibleRuppe_Destroy(Actor* thisx, PlayState* play) {
    EnInvisibleRuppe* this = (EnInvisibleRuppe*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnInvisibleRuppe_Update(Actor* thisx, PlayState* play) {
    EnInvisibleRuppe* this = (EnInvisibleRuppe*)thisx;

    if (this->xflag != XFLAGID_NONE && Xflag_Get(this->xflag))
    {
        this->xflag = XFLAGID_NONE;
        this->actor.draw = NULL;
    }

    this->actionFunc(this, play);
    func_80C258A0(this, play);
}
