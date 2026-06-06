#include <combo.h>

void EnWarpUzu_TelescopeER(Actor* this, PlayState* play)
{
    play->nextEntrance = ENTR_MM_PIRATE_FORTRESS_TELESCOPE;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
    *(void**)((char*)this + 0x144) = Actor_Noop;
    gIsEntranceOverride = 1;
}
