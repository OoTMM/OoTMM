#include <combo.h>
#include <combo/dungeon.h>
#include <combo/entrance.h>
#include <combo/config.h>
#include <combo/actor.h>
#include <combo/custom.h>
#include "Custom_Song_Tag.h"

static void CustomSongTag_Init(Actor_CustomSongTag* this, PlayState* play)
{
    this->base.room = 0xff;
    Actor_SetScale(&this->base, 1.375f);
    CollisionPoly* poly;
    s32 bgId;
    Vec3f pos;
    pos.x = this->base.world.pos.x;
    pos.y = this->base.world.pos.y + 20.f;
    pos.z = this->base.world.pos.z;
    float yIntersect = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, &this->base, &pos);
    if (yIntersect > BGCHECK_Y_MIN)
    {
        this->base.world.pos.y = yIntersect;
    }
    if (poly)
    {
        this->base.floorPoly = poly;
        Actor_MatchFloorRotation(&this->base, this->base.shape.rot.y, &this->base.shape.rot);
    }
}

static void CustomSongTag_Update(Actor_CustomSongTag* this, PlayState* play)
{
}

static Gfx kDlistLoadTexture[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTextureH[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0, G_TX_MIRROR, G_TX_NOMIRROR, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTextureHV[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTexturePad[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx* kDlistsLoadTextures[] = {
    kDlistLoadTextureH,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTextureHV,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTextureH,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
    kDlistLoadTexture,
};

static const u32 kSongTagColors[] = {
    0xffff00,
    0xff8800,
    0x3da81d,
    0xa3a300,
    0xee2222,
    0x8888ff,
    0x00ff00,
    0xff0000,
    0x0000ff,
    0xff8000,
    0x8000ff,
    0xffff00,
    0xff00ff,
    0xe1d007,
    0x00ff00,
    0x9c8429,
    0x9c8429,
    0x84c6c6,
    0xff8000,
    0xffffff,
};

static const u32 kSongTagTextureAddrs[] = {
    CUSTOM_SONG_TAG_LULLABY_ADDR,
    CUSTOM_SONG_TAG_EPONA_ADDR,
    CUSTOM_SONG_TAG_SARIA_ADDR,
    CUSTOM_SONG_TAG_STORMS_ADDR,
    CUSTOM_SONG_TAG_SUN_ADDR,
    CUSTOM_SONG_TAG_TIME_ADDR,
    CUSTOM_SONG_TAG_MINUET_ADDR,
    CUSTOM_SONG_TAG_BOLERO_ADDR,
    CUSTOM_SONG_TAG_SERENADE_ADDR,
    CUSTOM_SONG_TAG_REQUIEM_ADDR,
    CUSTOM_SONG_TAG_NOCTURNE_ADDR,
    CUSTOM_SONG_TAG_PRELUDE_ADDR,
    CUSTOM_SONG_TAG_HEALING_ADDR,
    CUSTOM_SONG_TAG_SOARING_ADDR,
    CUSTOM_SONG_TAG_AWAKENING_ADDR,
    CUSTOM_SONG_TAG_GORON_ADDR,
    CUSTOM_SONG_TAG_GORON_HALF_ADDR,
    CUSTOM_SONG_TAG_ZORA_ADDR,
    CUSTOM_SONG_TAG_EMPTINESS_ADDR,
    CUSTOM_SONG_TAG_ORDER_ADDR,
};

static Vtx kQuad[] = {
    {{ { -26, 0, -26 }, 0, { 0, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { -26, 0, 26 }, 0, { 0, 2048 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { 26, 0, 26 }, 0, { 2048, 2048 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { 26, 0, -26 }, 0, { 2048, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
};

static const Gfx kDlistSongTag[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(0x06000000),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA_PRIM, G_CC_PASS2),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_DECAL2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(&kQuad, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

static void CustomSongTag_Draw(Actor_CustomSongTag* this, PlayState* play)
{
    if (this->intendedRoom != 0xff && this->intendedRoom != play->roomCtx.curRoom.num && this->intendedRoom != play->roomCtx.prefRoom.num)
    {
        return;
    }
    int song;
    float padScale;
    Gfx* loader;
    void* tex;
    u32 color;
    u8 r;
    u8 g;
    u8 b;
    u8 vertical;

    song = gComboConfig.songEventsMm[this->base.params];
    // song = (play->gameplayFrames >> 4) % ARRAY_COUNT(kDlistsLoadTextures);
    loader = kDlistsLoadTextures[song];
    color = kSongTagColors[song];
    r = (color >> 16) & 0xff;
    g = (color >>  8) & 0xff;
    b = (color >>  0) & 0xff;

    /* Compute the texture */
    tex = comboCacheGetFile(kSongTagTextureAddrs[song]);
    if (!tex)
        return;
    loader[0].words.w1 = (u32)tex;

    if (padScale)
    {
        tex = comboCacheGetFile(CUSTOM_SONG_TAG_PAD_ADDR);
        if (!tex)
            return;
        kDlistLoadTexturePad[0].words.w1 = (u32)tex;
    }

    // Matrix_Translate(pos.x, pos.y, pos.z, MTXMODE_NEW);
    // if (vertical)
    //     Matrix_RotateX(M_PI_2, MTXMODE_APPLY);
    // if (rot)
    //     Matrix_RotateY(rot, MTXMODE_APPLY);

    /* Draw the display list */
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    if (padScale)
    {
        Matrix_Push();
        Matrix_Translate(0.f, vertical ? 0.f : -0.05f, vertical ? -0.05f : 0.f, MTXMODE_APPLY);
        Matrix_Scale(padScale, 1.f, padScale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(POLY_XLU_DISP++, 0x06, (u32)kDlistLoadTexturePad);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 200, 200, 200, 255);
        gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistSongTag);
        Matrix_Pop();
    }
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 0x06, (u32)loader);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, r, g, b, 255);
    gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistSongTag);
    CLOSE_DISPS();
}

ActorProfile Custom_Song_Tag_InitVars = {
    ACTOR_CUSTOM_SONG_TAG,
    ACTORCAT_PROP,
    ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Actor_CustomSongTag),
    (ActorFunc)CustomSongTag_Init,
    NULL,
    (ActorFunc)CustomSongTag_Update,
    (ActorFunc)CustomSongTag_Draw,
};

OVL_INFO_ACTOR(ACTOR_CUSTOM_SONG_TAG, Custom_Song_Tag_InitVars);
