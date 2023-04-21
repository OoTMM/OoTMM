#include <combo.h>

void DemoEffect_TextRutoSapphire(GameState_Play* play)
{
    char* b;
    char* start;
    s16 gi;

    gi = comboOverrideEx(OV_NPC, 0, NPC_OOT_BLUE_WARP_BARINADE, GI_OOT_STONE_SAPPHIRE, OVF_PROGRESSIVE | OVF_DOWNGRADE);
    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Princess Ruto got ");
    comboTextAppendItemName(&b, gi, 0);
    comboTextAppendStr(&b, "!" TEXT_NL "But why Princess Ruto?" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void DemoEffect_DrawSapphireInJabu(Actor* this, GameState_Play* play)
{
    static const int kRotDivisor = 100;
    float angle;
    s16 gi;

    angle = (play->gs.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;
    gi = comboOverrideEx(OV_NPC, 0, NPC_OOT_BLUE_WARP_BARINADE, GI_OOT_STONE_SAPPHIRE, OVF_PROGRESSIVE | OVF_DOWNGRADE);

    ModelViewTranslate(this->position.x, this->position.y, this->position.z, MAT_SET);
    ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
    ModelViewRotateY(angle, MAT_MUL);
    comboDrawGI(play, this, gi, 0);
}

void DemoEffect_DrawSpiritualLoader(Actor* this, GameState_Play* play)
{
    void (*DemoEffect_DrawSpiritual)(Actor*, GameState_Play*);

    if (play->sceneId == SCE_OOT_INSIDE_JABU_JABU)
        DemoEffect_DrawSpiritual = DemoEffect_DrawSapphireInJabu;
    else
        DemoEffect_DrawSpiritual = actorAddr(0x8b, 0x8092e3f8);
    *(void**)((char*)this + 0x188) = DemoEffect_DrawSpiritual;
    DemoEffect_DrawSpiritual(this, play);
}
