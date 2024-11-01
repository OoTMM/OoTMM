#include <combo.h>

int Sakon_ShouldEndThiefEscape(Actor_EnSuttari* sakon, PlayState* ctxt) {
    /* Check if Sakon has dropped the luggage (with speedup) or has finished the escape sequence. */
    return (sakon->runningState == 6);
}
