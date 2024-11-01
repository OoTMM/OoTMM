#include <combo.h>
#include <combo/hint.h>

void EnGs_TalkedTo(Actor* this, PlayState* play)
{
    u8 key;

    key = this->params & 0x1f;
    if (key == 0x18)
        key = (gGrottoData & 0x1f) | 0x20;
    Hint_Display(play, key);
}
