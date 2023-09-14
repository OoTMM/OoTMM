#include <combo.h>
#include <combo/xflags.h>

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

static void ObjGrass_SpawnBushDrop(GameState_Play* play, int unk, Actor_ObjGrass_PackBush* bush, u16 unk2)
{
    int packId;
    int bushId;
    Xflag xflag;

    if (comboConfig(CFG_MM_SHUFFLE_GRASS))
    {
        /* Extract the ID and build the xflag */
        ObjGrass_GetID(&packId, &bushId, bush);
        memcpy(&xflag, &gObjGrass->xflags[packId], sizeof(Xflag));
        xflag.sliceId = bushId;

        /* Check if the reward was already collected */
        if (!comboXflagsGet(&xflag))
        {
            DropCustomItem(play, &bush->position, &xflag);
        }
    }
}

PATCH_CALL(0x809a92fc, ObjGrass_SpawnBushDrop);
