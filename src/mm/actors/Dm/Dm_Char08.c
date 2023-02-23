#include <combo.h>

void DmChar08_WarpToTemple(Actor* this, GameState_Play* play)
{
    play->nextEntrance = ((SCE_MM_TEMPLE_GREAT_BAY - 3) << 9) | (0 << 4);
    play->transitionType = TRANS_TYPE_NORMAL;
    play->transitionGfx = TRANS_GFX_BLACK;

    gIsEntranceOverride = 1;
}

PATCH_FUNC(0x80aafa18, DmChar08_WarpToTemple);

static void DmChar08_AfterWarpFromTemple(Actor* this, GameState_Play* play)
{
    gIsEntranceOverride = 1;
}

PATCH_FUNC(0x80aafb94, DmChar08_AfterWarpFromTemple);

void DmChar08_Emerge(GameState_Play* play, Actor* this)
{
    this->position.x = -6480.f;
    this->position.z = 1750.f;
    this->speedRot.x = 0;
    this->speedRot.y = -0x7234;
    this->speedRot.z = 0;
    this->flags |= 1;

    *(u16*)((char*)this + 0xbc) = 0;
    *(u16*)((char*)this + 0xbe) = 0x8dcc;
    *(u16*)((char*)this + 0xc0) = 0;

    *((char*)this + 0x1ff) = 2;
    *((char*)this + 0x202) = 2;
    *((char*)this + 0x203) = 99;
    *((char*)this + 0x200) = 1;
    *((char*)this + 0x207) = 0;
    *((char*)this + 0x208) = 0;
}
