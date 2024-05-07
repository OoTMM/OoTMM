#include <combo.h>

int BgIngate_EndArchery(ActorBgIngate* this, GameState_Play* play) {
    int finished = (this->flags & 2) == 2;
    return finished || (((gSave.eventInf[3] & 0x20) != 0) && (gSaveContext.minigameScore >= 20));
}