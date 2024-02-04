#include <combo.h>

static void BgHakaCurtain_HandlerRoomClear(Actor* this, GameState_Play* play)
{
    float max;

    max = this->home.pos.y + 150.f;
    if (this->world.pos.y < max)
        this->world.pos.y += 1.f;
    if (this->world.pos.y > max)
        this->world.pos.y = max;
}

PATCH_FUNC(0x80b6dd00, BgHakaCurtain_HandlerRoomClear);
