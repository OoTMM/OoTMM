#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x234) = (h); } while (0)

static void EnPoComposer_HandlerStorms(Actor* this, GameState_Play* play)
{
    /* Mark the valley as purified */
    MM_SET_EVENT_WEEK(MM_EV(14, 2));
    ActorDestroy(this);

    /* Reload the room */
    play->nextEntrance = ((SCE_MM_IKANA_CANYON - 3) << 9) | (14 << 4);
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
}

void EnPoComposer_SetHandlerStorms(Actor* this)
{
    SET_HANDLER(this, EnPoComposer_HandlerStorms);
}
