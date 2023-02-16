#include <combo.h>

static void BgHakaCurtain_HandlerRoomClear(Actor* this, GameState_Play* play)
{
    float max;

    max = this->initPos.y + 150.f;
    if (this->position.y < max)
        this->position.y += 1.f;
    if (this->position.y > max)
        this->position.y = max;
}

PATCH_FUNC(0x80b6dd00, BgHakaCurtain_HandlerRoomClear);
