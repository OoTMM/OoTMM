#include <combo.h>

static u16 getGI(void)
{
    u16 gi;
    s32 override;

    gi = GI_OOT_SLINGSHOT;
    override = comboGetNpcOverride(0x08);
    if (override >= 0)
        gi = (u16)override;
    gi = comboProgressive(gi);
    return gi;
}

void EnExItem_Draw(Actor* actor, GameState_Play* play)
{
    float scale;

    scale = *(float*)(((char*)actor) + 0x154);
    ActorSetScale(actor, scale);
    comboDrawGI(play, actor, getGI());
}

PATCH_FUNC(0x80ad9f78, EnExItem_Draw);
