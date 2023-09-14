#include <combo.h>
#include <combo/xflags.h>
#include <combo/item.h>
#include <combo/csmc.h>

Actor_ObjGrass* gObjGrass;

static void ObjGrass_GetID(int* dstPackId, int* dstBushId, Actor_ObjGrass_PackBush* bush)
{
    u32 offset;
    u32 offsetPack;
    u32 offsetBush;
    int packId;
    int bushId;

    /* Extract the pack ID */
    offset = (u32)bush - (u32)gObjGrass;
    offsetPack = (offset - offsetof(Actor_ObjGrass_Pack, bushes) - offsetof(Actor_ObjGrass, packs));
    packId = offsetPack / sizeof(Actor_ObjGrass_Pack);

    /* Extract the bush ID */
    offset = (u32)bush - ((u32)&gObjGrass->packs[packId]);
    offsetBush = offset - offsetof(Actor_ObjGrass_Pack, bushes);
    bushId = offsetBush / sizeof(Actor_ObjGrass_PackBush);

    /* Store the IDs */
    *dstPackId = packId;
    *dstBushId = bushId;
}

void ObjGrass_GetXflag(Xflag* xflag, Actor_ObjGrass_PackBush* bush)
{
    int packId;
    int bushId;

    ObjGrass_GetID(&packId, &bushId, bush);
    memcpy(xflag, &gObjGrass->xflags[packId], sizeof(Xflag));
    xflag->sliceId = bushId;
}

static void ObjGrass_SpawnBushDrop(GameState_Play* play, int unk, Actor_ObjGrass_PackBush* bush, u16 unk2)
{
    Xflag xflag;

    if (comboConfig(CFG_MM_SHUFFLE_GRASS))
    {
        /* Extract the ID and build the xflag */
        ObjGrass_GetXflag(&xflag, bush);

        /* Check if the reward was already collected */
        if (!comboXflagsGet(&xflag))
        {
            DropCustomItem(play, &bush->position, &xflag);
            return;
        }
    }

    /* Forward */
    SpawnCollectible2(play, unk, bush, unk2);
}

PATCH_CALL(0x809a92fc, ObjGrass_SpawnBushDrop);

/*
* Carry 1 - 8046D9F0
* Carry 2 - 8046DBB0
*/

void ObjGrass_DrawBush(Actor_ObjGrass_PackBush* bush)
{
    Xflag xflag;
    void* dlist;
    ComboItemOverride o;

    o.gi = 0;
    if (comboConfig(CFG_MM_SHUFFLE_GRASS))
    {
        /* Extract the ID and build the xflag */
        ObjGrass_GetXflag(&xflag, bush);

        /* Check if the reward was already collected */
        if (!comboXflagsGet(&xflag))
            comboXflagItemOverride(&o, &xflag, 0);
    }

    /* Prepare the draw */
    csmcGrassPreDraw(gPlay, o.gi, CSMC_GRASS_NORMAL, 0, 1);
    dlist = actorAddr(AC_OBJ_GRASS, 0x809aaae0);

    /* Draw */
    OPEN_DISPS(gPlay->gs.gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(gPlay->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, (u32)dlist & 0xffffff);
    CLOSE_DISPS();
}
