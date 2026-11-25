#include <combo/draw.h>
#include "En_Hit_Tag.h"

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnHitTag_Init(Actor* thisx, PlayState* play);
void EnHitTag_Destroy(Actor* thisx, PlayState* play);
void EnHitTag_Update(Actor* thisx, PlayState* play);

void EnHitTag_WaitForHit(EnHitTag* this, PlayState* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 16, 32, 0, { 0, 0, 0 } },
};


static void EnHitTag_DrawGlitter(EnHitTag* this, PlayState* play)
{
    ComboItemOverride o;
    XflagID xflag;
    s16 giList[3];
    s16 giCloakList[3];
    s16 gi;
    s16 giCloak;
    u8 giCount;
    int index;

    /* Get the gi list */
    giCount = 0;
    for (int i = 0; i < 3; ++i)
    {
        xflag = Xflag_LookupSlice(this->xflag, i);
        if (Xflag_Get(xflag))
            continue;
        Xflag_ItemOverride(&o, xflag, GI_NONE);
        if (o.gi == GI_NONE)
            continue;
        giList[giCount] = o.gi;
        giCloakList[giCount] = o.cloakGi;
        giCount++;
    }

    if (giCount == 0)
    {
        this->actor.draw = NULL;
        return;
    }

    switch (giCount)
    {
    case 1:
        gi = giList[0];
        giCloak = giCloakList[0];
        break;
    case 2:
        index = (play->state.frameCount % 12) / 6;
        gi = giList[index];
        giCloak = giCloakList[index];
        break;
    case 3:
        index = (play->state.frameCount % 12) / 4;
        gi = giList[index];
        giCloak = giCloakList[index];
        break;
    default:
        UNREACHABLE();
    }

    Draw_GlitterGi(play, &this->actor, gi, giCloak);
}

void EnHitTag_Init(Actor* thisx, PlayState* play) {
    EnHitTag* this = (EnHitTag*)thisx;
    u8 valid;
    XflagID xflag;

    /* Xflag */
    valid = 0;
    this->xflag = Xflag_Init(thisx, play);
    for (int i = 0; i < 3; ++i)
    {
        xflag = Xflag_LookupSlice(this->xflag, i);
        if (Xflag_IsShuffled(xflag))
        {
            valid = 1;
            break;
        }
    }

    if (valid)
        this->actor.draw = EnHitTag_DrawGlitter;
    else
        this->xflag = XFLAGID_NONE;

    Actor_SetScale(&this->actor, 1.0f);
    this->actionFunc = EnHitTag_WaitForHit;
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    if (!valid && Flags_GetSwitch(play, ENHITTAG_GET_SWITCH_FLAG(thisx))) {
        Actor_Kill(&this->actor);
    }
}

void EnHitTag_Destroy(Actor* thisx, PlayState* play) {
    EnHitTag* this = (EnHitTag*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnHitTag_WaitForHit(EnHitTag* this, PlayState* play) {
    Vec3f dropLocation;
    s32 i;
    XflagID xflag;

    if (this->collider.base.acFlags & AC_HIT) {
        Audio_PlaySfx(NA_SE_SY_GET_RUPY);
        Actor_Kill(&this->actor);
        dropLocation.x = this->actor.world.pos.x;
        dropLocation.y = this->actor.world.pos.y;
        dropLocation.z = this->actor.world.pos.z;

        for (i = 0; i < 3; i++) {
            xflag = Xflag_LookupSlice(this->xflag, i);
            if (Xflag_IsShuffled(xflag))
            {
                EnItem00_DropCustomEx(play, &dropLocation, xflag, XFDROP_NONE);
            }
            else
            {
                Item_DropCollectible(play, &dropLocation, ITEM00_RUPEE_GREEN);
            }
        }
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnHitTag_Update(Actor* thisx, PlayState* play) {
    EnHitTag* this = (EnHitTag*)thisx;
    this->actionFunc(this, play);
}

static ActorProfile En_Hit_Tag_Profile = {
    /**/ ACTOR_EN_HIT_TAG,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(EnHitTag),
    /**/ EnHitTag_Init,
    /**/ EnHitTag_Destroy,
    /**/ EnHitTag_Update,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_EN_HIT_TAG, En_Hit_Tag_Profile);
