#include <combo.h>
#include <combo/item.h>

static void DemoEffect_SapphireItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NONE;
    q->gi = gComboData.giZoraSapphire;
}

static void DemoEffect_SapphireItemOverride(ComboItemOverride* o)
{
    ComboItemQuery q;

    DemoEffect_SapphireItemQuery(&q);
    comboItemOverride(o, &q);
}

void DemoEffect_TextRutoSapphire(GameState_Play* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    DemoEffect_SapphireItemQuery(&q);
    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Princess Ruto got ");
    comboTextAppendItemNameQuery(&b, &q, 0);
    comboTextAppendStr(&b, "!" TEXT_NL "But why Princess Ruto?" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void DemoEffect_DrawSapphireInJabu(Actor* this, GameState_Play* play)
{
    ComboItemOverride o;
    static const int kRotDivisor = 100;
    float angle;

    DemoEffect_SapphireItemOverride(&o);
    angle = (play->gs.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;

    ModelViewTranslate(this->position.x, this->position.y, this->position.z, MAT_SET);
    ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
    ModelViewRotateY(angle, MAT_MUL);
    comboDrawGI(play, this, o.gi, 0);
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
