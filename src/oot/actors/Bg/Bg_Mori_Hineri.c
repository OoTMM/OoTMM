#include <combo.h>

void BgMoriHineri_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback draw;
    s16 gi;

    /* Init CSMC */
    gi = comboOverrideEx(OV_CHEST, play->sceneId, 0xe, GI_OOT_BOSS_KEY, OVF_NO_PROGRESSIVE);
    comboCsmcPreDraw(this, play, gi);

    /* Draw */
    draw = actorAddr(0x068, 0x80913944);
    draw(this, play);
}
