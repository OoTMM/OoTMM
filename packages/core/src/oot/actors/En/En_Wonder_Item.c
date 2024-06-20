#include <combo.h>
#include <combo/custom.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/draw.h>
#include <combo/config.h>
#include <combo/oot/actors/En_Wonder_Item.h>

static Actor_EnWonderItem* sWonderItem;

static void EnWonderItem_Alias(Actor_EnWonderItem* this)
{
    Xflag* xflag;

    xflag = &this->xflag;

    switch (xflag->sceneId)
    {
    case SCE_OOT_SACRED_FOREST_MEADOW:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            xflag->id -= 3;
        }
        break;
    case SCE_OOT_HYRULE_FIELD:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            xflag->id += 2;
        }
        break;
    case SCE_OOT_LON_LON_RANCH:
        if (xflag->setupId == 3)
        {
            xflag->setupId = 2;
            xflag->id += 16;
        }
        break;
    case SCE_OOT_GERUDO_FORTRESS:
        if (xflag->setupId != 0)
        {
            xflag->setupId = 0;
            if (xflag->id == 5)
                xflag->id = 8;
            else
                xflag->id = 7;
        }
        break;
    case SCE_OOT_DESERT_COLOSSUS:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch (xflag->id)
            {
            case 22: xflag->id = 19; break;
            case 23: xflag->id = 21; break;
            case 24: xflag->id = 20; break;
            case 25: xflag->id = 23; break;
            }
        }
        break;
    case SCE_OOT_KAKARIKO_VILLAGE:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            xflag->id += 7;
        }
        break;
    }
}

static void EnWonderItem_DrawGlitter(Actor_EnWonderItem* this, GameState_Play* play)
{
    ComboItemOverride o;

    /* Check xflag */
    if (comboXflagsGet(&this->xflag))
    {
        this->base.draw = NULL;
        return;
    }

    comboXflagItemOverride(&o, &this->xflag, 0);
    Draw_GlitterGi(play, &this->base, o.gi);
}

static void EnWonderItem_DropCustomDecoy(Actor_EnWonderItem* this, GameState_Play* play)
{
    ComboItemQuery q;

    comboXflagItemQuery(&q, &this->xflag, 0);
    Item_AddWithDecoy(play, &q);
    comboXflagsSet(&this->xflag);
}

static void EnWonderItem_DropCustom(Actor_EnWonderItem* this, GameState_Play* play, s16 params)
{
    if (params & 0x8000)
        EnWonderItem_DropCustomDecoy(this, play);
    else
        EnItem00_DropCustom(play, &this->base.world.pos, &this->xflag);
}

static void EnWonderItem_ItemDropCollectible(GameState_Play* play, const Vec3f* pos, s16 params)
{
    /* Check for vanilla */
    if (!sWonderItem->isExtended || comboXflagsGet(&sWonderItem->xflag))
        Item_DropCollectible(play, pos, params);
    else
        EnWonderItem_DropCustom(sWonderItem, play, params);
}

PATCH_CALL(0x80a662e4, EnWonderItem_ItemDropCollectible);
PATCH_CALL(0x80a6630c, EnWonderItem_ItemDropCollectible);

static void EnWonderItem_ItemDropCollectibleRandom(GameState_Play* play, Actor* from, const Vec3f* pos, s16 params)
{
    /* Check for vanilla */
    if (!sWonderItem->isExtended || comboXflagsGet(&sWonderItem->xflag))
        Item_DropCollectibleRandom(play, from, pos, params);
    else
        EnWonderItem_DropCustom(sWonderItem, play, params);
}

PATCH_CALL(0x80a66334, EnWonderItem_ItemDropCollectibleRandom);
PATCH_CALL(0x80a66358, EnWonderItem_ItemDropCollectibleRandom);

void EnWonderItem_InitWrapper(Actor_EnWonderItem* this, GameState_Play* play)
{
    int switchFlag;
    ComboItemOverride o;
    Actor_EnWonderItem_Func EnWonderItem_Init;

    /* Setup the xflag */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;
    EnWonderItem_Alias(this);

    if (play->sceneId == SCE_OOT_CASTLE_COURTYARD && Config_Flag(CFG_OOT_SKIP_ZELDA))
    {
        this->isExtended = 0;
    }
    else
    {
        comboXflagItemOverride(&o, &this->xflag, 0);
        this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));
    }

    /* Check the collectible flag */
    switchFlag = this->base.variable & 0x3f;
    if (switchFlag != 0x3f && GetSwitchFlag(play, switchFlag) && this->isExtended)
    {
        /* Flag is set but item wasn't collected - ignore the flag */
        this->base.variable |= 0x3f;
    }

    /* Forward */
    EnWonderItem_Init = actorAddr(AC_EN_WONDER_ITEM, 0x80a663b0);
    EnWonderItem_Init(this, play);

    /* Use draw if extended */
    if (this->isExtended)
        this->base.draw = EnWonderItem_DrawGlitter;
}

void EnWonderItem_UpdateWrapper(Actor_EnWonderItem* this, GameState_Play* play)
{
    Actor_EnWonderItem_Func EnWonderItem_Update;

    /* Store */
    sWonderItem = this;

    /* Forward */
    EnWonderItem_Update = actorAddr(AC_EN_WONDER_ITEM, 0x80a66bcc);
    EnWonderItem_Update(this, play);
}
