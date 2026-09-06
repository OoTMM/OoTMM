#include <combo.h>
#include <combo/custom.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/draw.h>
#include <combo/config.h>
#include <combo/oot/actors/En_Wonder_Item.h>

static Actor_EnWonderItem* sWonderItem;

static void EnWonderItem_DrawGlitter(Actor_EnWonderItem* this, PlayState* play)
{
    ComboItemOverride o;

    /* Check xflag */
    if (Xflag_GetIndirect(&this->xflag))
    {
        this->base.draw = NULL;
        return;
    }

    comboXflagItemOverride(&o, &this->xflag, 0);
    Draw_GlitterGi(play, &this->base, o.gi, o.cloakGi);
}

static void EnWonderItem_DropCustomDecoy(Actor_EnWonderItem* this, PlayState* play)
{
    ComboItemQuery q;

    comboXflagItemQuery(&q, &this->xflag, 0);
    Item_AddWithDecoy(play, &q);
    Xflag_SetIndirect(&this->xflag);
}

static void EnWonderItem_DropCustom(Actor_EnWonderItem* this, PlayState* play, s16 params)
{
    if (params & 0x8000)
        EnWonderItem_DropCustomDecoy(this, play);
    else
        EnItem00_DropCustom(play, &this->base.world.pos, &this->xflag);
}

static void EnWonderItem_ItemDropCollectible(PlayState* play, const Vec3f* pos, s16 params)
{
    /* Check for vanilla */
    if (!sWonderItem->isExtended || Xflag_GetIndirect(&sWonderItem->xflag))
        Item_DropCollectible(play, pos, params);
    else
        EnWonderItem_DropCustom(sWonderItem, play, params);
}

PATCH_CALL(0x80a662e4, EnWonderItem_ItemDropCollectible);
PATCH_CALL(0x80a6630c, EnWonderItem_ItemDropCollectible);

static void EnWonderItem_ItemDropCollectibleRandom(PlayState* play, Actor* from, const Vec3f* pos, s16 params)
{
    /* Check for vanilla */
    if (!sWonderItem->isExtended || Xflag_GetIndirect(&sWonderItem->xflag))
        Item_DropCollectibleRandom(play, from, pos, params);
    else
        EnWonderItem_DropCustom(sWonderItem, play, params);
}

PATCH_CALL(0x80a66334, EnWonderItem_ItemDropCollectibleRandom);
PATCH_CALL(0x80a66358, EnWonderItem_ItemDropCollectibleRandom);

void EnWonderItem_InitWrapper(Actor_EnWonderItem* this, PlayState* play)
{
    int switchFlag;
    Actor_EnWonderItem_Func EnWonderItem_Init;

    /* Setup the xflag */
    Xflag_Init(&this->xflag, &this->base, play);

    if (play->sceneId == SCE_OOT_CASTLE_COURTYARD && Config_Flag(CFG_OOT_SKIP_ZELDA))
        this->isExtended = FALSE;
    else
        this->isExtended = Xflag_IsShuffled(&this->xflag);

    /* Check the collectible flag */
    switchFlag = this->base.params & 0x3f;
    if (switchFlag != 0x3f && Flags_GetSwitch(play, switchFlag) && this->isExtended)
    {
        /* Flag is set but item wasn't collected - ignore the flag */
        this->base.params |= 0x3f;
    }

    /* Forward */
    EnWonderItem_Init = actorAddr(ACTOR_EN_WONDER_ITEM, 0x80a663b0);
    EnWonderItem_Init(this, play);

    /* Use draw if extended */
    if (this->isExtended)
        this->base.draw = EnWonderItem_DrawGlitter;
}

void EnWonderItem_UpdateWrapper(Actor_EnWonderItem* this, PlayState* play)
{
    Actor_EnWonderItem_Func EnWonderItem_Update;

    /* Store */
    sWonderItem = this;

    /* Forward */
    EnWonderItem_Update = actorAddr(ACTOR_EN_WONDER_ITEM, 0x80a66bcc);
    EnWonderItem_Update(this, play);
}
