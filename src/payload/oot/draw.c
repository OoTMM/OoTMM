#include <combo.h>

static void setObjectSegment(GfxContext* gfx, void* buffer)
{
    OPEN_DISPS(gfx);
    gSPSegment(POLY_OPA_DISP++, 0x06, buffer);
    gSPSegment(POLY_XLU_DISP++, 0x06, buffer);
    CLOSE_DISPS();
}

void comboDrawObject(GameState_Play* play, Actor* actor, u16 objectId, u16 shaderId)
{
    void* objBuffer;

    objBuffer = comboGetObject(objectId);
    setObjectSegment(play->gs.gfx, objBuffer);
    PreDraw1(actor, play, 0);
    PreDraw2(actor, play, 0);
    kShaders[shaderId - 1].func(&play->gs, shaderId - 1);
}
