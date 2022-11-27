#include <combo.h>

void PreDraw1(Actor* actor, GameState_Play* play, int unk);
void PreDraw2(Actor* actor, GameState_Play* play, int unk);

void comboSetObjectSegment(GfxContext* gfx, void* buffer)
{
    /* Set the segment in the display list */
    OPEN_DISPS(gfx);
    gSPSegment(POLY_OPA_DISP++, 0x06, buffer);
    gSPSegment(POLY_XLU_DISP++, 0x06, buffer);
    CLOSE_DISPS();

    /* Set the segment on the CPU side - required for Moon tear */
    gSegments[0x06] = (u32)buffer - 0x80000000;
}

void comboDrawObject(GameState_Play* play, Actor* actor, u16 objectId, u16 shaderId, int flags)
{
    void* objBuffer;

    if (objectId)
    {
        objBuffer = comboGetObject(objectId);
        comboSetObjectSegment(play->gs.gfx, objBuffer);
    }
    if (!(flags & DRAW_NO_PRE1))
        PreDraw1(actor, play, 0);
    if (!(flags & DRAW_NO_PRE2))
        PreDraw2(actor, play, 0);
    kShaders[shaderId - 1].func(&play->gs, shaderId - 1);
}

void comboDrawGI(GameState_Play* play, Actor* actor, int gi, int flags)
{
    const GetItem* giEntry;

    giEntry = kExtendedGetItems + gi - 1;
    comboDrawObject(play, actor, giEntry->objectId, giEntry->shaderId, flags);
}
