#include "Item_B_Heart.h"
#include <combo/draw.h>
#include <assets/mm/objects/object_gi_hearts.h>

#define FLAGS 0x00000000

void ItemBHeart_Init(Actor* thisx, PlayState* play);
void ItemBHeart_Destroy(Actor* thisx, PlayState* play);
void ItemBHeart_Update(Actor* thisx, PlayState* play);
void ItemBHeart_Draw(Actor* thisx, PlayState* play);

void ItemBHeart_UpdateModel(ItemBHeart* this, PlayState* play);

ActorProfile Item_B_Heart_Profile = {
    /**/ ACTOR_ITEM_B_HEART,
    /**/ ACTORCAT_BOSS,
    /**/ FLAGS,
    /**/ OBJECT_GI_HEARTS,
    /**/ sizeof(ItemBHeart),
    /**/ ItemBHeart_Init,
    /**/ ItemBHeart_Destroy,
    /**/ ItemBHeart_Update,
    /**/ ItemBHeart_Draw,
};

OVL_INFO_ACTOR(ACTOR_ITEM_B_HEART, Item_B_Heart_Profile);

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDistance, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 800, ICHAIN_STOP),
};

void ItemBHeart_Init(Actor* thisx, PlayState* play) {
    ItemBHeart* this = (ItemBHeart*)thisx;

    if (Flags_GetCollectible(play, 0x1F)) {
        Actor_Kill(&this->actor);
        return;
    }
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.8f);
    if (this->actor.params == BHEART_PARAM_SMALL) {
        this->baseScale = BHEART_SCALE_SMALL;
    } else {
        this->baseScale = BHEART_SCALE_NORMAL;
    }
    this->actor.world.pos.y += 20.0f * this->baseScale;
}

void ItemBHeart_Destroy(Actor* thisx, PlayState* play) {
}

static void ItemBHeart_ItemQuery(ComboItemQuery* q, PlayState* play)
{
    bzero(q, sizeof(ComboItemQuery));

    q->ovType = OV_COLLECTIBLE;
    q->gi = GI_MM_HEART_CONTAINER;
    q->sceneId = comboSceneKey(play->sceneId);
    q->id = 0x1f;
}

static void ItemBHeart_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    ItemBHeart_ItemQuery(&q, play);
    comboGiveItem(this, play, &q, a, b);
}

/**
 * Adjusts size and handles collection (if of proper baseScale)
 */
void ItemBHeart_Update(Actor* thisx, PlayState* play) {
    ItemBHeart* this = (ItemBHeart*)thisx;

    ItemBHeart_UpdateModel(this, play);

    if (!(this->baseScale < BHEART_SCALE_MIN_COLLECTIBLE)) {
        if (Actor_HasParent(&this->actor, play)) {
            Flags_SetCollectible(play, 0x1F);
            Actor_Kill(&this->actor);
            return;
        }
        ItemBHeart_GiveItem(&this->actor, play, GI_MM_HEART_CONTAINER, 30.0f, 80.0f);
    }
}

/**
 * Rotate continuously while approaching 40% of object's unit scale.
 */
void ItemBHeart_UpdateModel(ItemBHeart* this, PlayState* play) {
    this->actor.shape.rot.y += 0x400;
    Math_ApproachF(&this->variableScale, 0.4f, 0.1f, 0.01f);
    Actor_SetScale(&this->actor, this->variableScale * this->baseScale);
}

void ItemBHeart_Draw(Actor* this, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    s16 gi;

    ItemBHeart_ItemQuery(&q, play);
    comboItemOverride(&o, &q);

    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-this->shape.rot.y * 2, MTXMODE_APPLY);
    }
    Draw_Gi(play, this, gi, DRAW_RAW);
}
