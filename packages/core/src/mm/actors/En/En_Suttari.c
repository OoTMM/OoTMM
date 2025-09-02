#include <combo.h>

int Sakon_ShouldEndThiefEscape(EnSuttari* sakon, PlayState* ctxt) {
    /* Check if Sakon has dropped the luggage (with speedup) or has finished the escape sequence. */
    return (sakon->animIndex == 6);
}
