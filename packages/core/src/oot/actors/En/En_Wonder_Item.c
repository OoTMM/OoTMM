#include <combo.h>
#include <combo/custom.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/oot/actors/En_Wonder_Item.h>

static const Vtx kGlitterVtx[] = {
    {{ { -10, -10, 0 }, 0, { 0, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ {  10, -10, 0 }, 0, { 0x200, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ {  10,  10, 0 }, 0, { 0x200, 0x200 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { -10,  10, 0 }, 0, { 0, 0x200 }, { 0xff, 0xff, 0xff, 0xff } }},
};

static const Gfx kDlistGlitter[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(0x06000000, G_IM_FMT_IA, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_PASS2),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsSPVertex(&kGlitterVtx, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

static Actor_EnWonderItem* sWonderItem;

static void EnWonderItem_DrawGlitter(Actor_EnWonderItem* this, GameState_Play* play)
{
    void* tex;
    float alpha;

    /* Check xflag */
    if (comboXflagsGet(&this->xflag))
    {
        this->base.draw = NULL;
        return;
    }

    /* Compute alpha */
    if (this->base.xzDistanceFromLink > 1000.f)
        return;
    else if (this->base.xzDistanceFromLink <= 300.f)
        alpha = 1.f;
    else
        alpha = (1000.f - this->base.xzDistanceFromLink) * (1.f / 700.f);

    /* Compute the texture */
    tex = comboCacheGetFile(CUSTOM_GLITTER_ADDR);
    if (!tex)
        return;
    if (play->gs.frameCount & 4)
        tex = (void*)((u32)tex + 16 * 8);

    /* Prepare the Matrix */
    ModelViewTranslate(this->base.world.pos.x, this->base.world.pos.y, this->base.world.pos.z, MAT_SET);
    ModelViewUnkTransform(&play->billboardMtxF);
    ModelViewTranslate(0, 20.f, 0.f, MAT_MUL);

    /* Draw the display list */
    OPEN_DISPS(play->gs.gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 0x06, (u32)tex - 0x80000000);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (alpha * 0.75f) * 255);
    gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistGlitter - 0x80000000);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (alpha * 0.25f) * 255);
    gSPClearGeometryMode(POLY_XLU_DISP++, G_ZBUFFER);
    gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistGlitter - 0x80000000);
    gSPSetGeometryMode(POLY_XLU_DISP++, G_ZBUFFER);
    CLOSE_DISPS();
}

static void EnWonderItem_DropCustomDecoy(Actor_EnWonderItem* this, GameState_Play* play)
{
    ComboItemOverride o;

    comboXflagItemOverride(&o, &this->xflag, 0);
    Item_AddWithDecoy(play, &o);
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

    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));

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
