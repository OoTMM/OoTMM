#include <combo.h>

void EnBji01_TelescopeER(Actor* this, PlayState* play)
{
    play->nextEntrance = ENTRANCE(TERMINA_FIELD, 10);
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
    *(void**)((char*)this + 0x144) = Actor_Noop;
    gIsEntranceOverride = 1;
}
