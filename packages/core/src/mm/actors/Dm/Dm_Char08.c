#include <combo.h>

void DmChar08_WarpToTemple(Actor* this, PlayState* play)
{
    play->nextEntrance = ((SCE_MM_TEMPLE_GREAT_BAY - 3) << 9) | (0 << 4);
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_BLACK;

    gIsEntranceOverride = 1;
}

PATCH_FUNC(0x80aafa18, DmChar08_WarpToTemple);

static void DmChar08_AfterWarpFromTemple(Actor* this, PlayState* play)
{
    gIsEntranceOverride = 1;
}

PATCH_FUNC(0x80aafb94, DmChar08_AfterWarpFromTemple);

void DmChar08_Emerge(PlayState* play, Actor* this)
{
    this->world.pos.x = -6480.f;
    this->world.pos.z = 1750.f;
    this->world.rot.x = 0;
    this->world.rot.y = -0x7234;
    this->world.rot.z = 0;
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

void func_80AAFA18(Actor* this, PlayState* play) {
    play->nextEntrance = ENTR_MM_TEMPLE_GREAT_BAY;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
    play->transitionTrigger = TRANS_TRIGGER_START;
    Audio_PlaySfx(NA_SE_SY_WHITE_OUT_T);
}

PATCH_FUNC(0x80aafa18, func_80AAFA18);
