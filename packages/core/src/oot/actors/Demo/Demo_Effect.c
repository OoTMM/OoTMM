#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/draw.h>
#include <combo/actor.h>

static void DemoEffect_SapphireItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NONE;
    q->gi = gComboConfig.giZoraSapphire;
}

static void DemoEffect_SapphireItemOverride(ComboItemOverride* o)
{
    ComboItemQuery q;

    DemoEffect_SapphireItemQuery(&q);
    comboItemOverride(o, &q);
}

void DemoEffect_TextRutoSapphire(PlayState* play)
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

static void DemoEffect_DrawSapphireInJabu(Actor* this, PlayState* play)
{
    ComboItemOverride o;
    static const int kRotDivisor = 100;
    float angle;

    DemoEffect_SapphireItemOverride(&o);
    angle = (play->state.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;

    Matrix_Translate(this->world.pos.x, this->world.pos.y, this->world.pos.z, MTXMODE_NEW);
    Matrix_Scale(0.35f, 0.35f, 0.35f, MTXMODE_APPLY);
    Matrix_RotateY(angle, MTXMODE_APPLY);
    Draw_GiCloaked(play, this, o.gi, o.cloakGi, 0);
}

void DemoEffect_DrawSpiritualLoader(Actor* this, PlayState* play)
{
    void (*DemoEffect_DrawSpiritual)(Actor*, PlayState*);

    if (play->sceneId == SCE_OOT_INSIDE_JABU_JABU)
        DemoEffect_DrawSpiritual = DemoEffect_DrawSapphireInJabu;
    else
        DemoEffect_DrawSpiritual = actorAddr(0x8b, 0x8092e3f8);
    *(void**)((char*)this + 0x188) = DemoEffect_DrawSpiritual;
    DemoEffect_DrawSpiritual(this, play);
}
