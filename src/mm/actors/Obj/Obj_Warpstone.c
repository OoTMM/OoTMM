#include <combo.h>

void ObjWarpstone_Save(GameState_Play* play)
{
    gSave.isOwlSave = 1;
    PrepareSave(&play->sramCtx);
    comboWriteSave();
    PlaySound(0x4823);
    Message_Close(play);
}
