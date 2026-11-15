#include <combo.h>
#include <combo/item.h>
#include <combo/draw.h>
#include <combo/actor.h>

static void EnElforg_ItemQuery(ComboItemQuery* q, Actor* this, PlayState* play)
{
    bzero(q, sizeof(*q));

    q->gi = GI_MM_STRAY_FAIRY;
    switch (play->sceneId)
    {
    case SCE_MM_CLOCK_TOWN_EAST:
    case SCE_MM_LAUNDRY_POOL:
        q->ovType = OV_NPC;
        q->id = NPC_MM_STRAY_FAIRY_TOWN;
        break;
    default:
        q->ovType = OV_SF;
        q->sceneId = Play_SceneKey(play->sceneId);
        q->id = (this->params & 0xfe00) >> 9;
        break;
    }
}

static void EnElforg_ItemOverride(ComboItemOverride* o, Actor* this, PlayState* play)
{
    ComboItemQuery q;

    EnElforg_ItemQuery(&q, this, play);
    comboItemOverride(o, &q);
}

static void EnElforg_Draw(Actor* this, PlayState* play)
{
    ComboItemOverride o;
    ActorCallback draw;
    static const int kRotDivisor = 100;
    float angle;
    s16 gi;

    EnElforg_ItemOverride(&o, this, play);
    angle = (play->state.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;
    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        angle = -angle;
    }

    if (gi == GI_MM_STRAY_FAIRY)
    {
        draw = actorAddr(0x1b0, 0x80acd8c0);
        draw(this, play);
        return;
    }

    Matrix_Translate(this->world.pos.x, this->world.pos.y, this->world.pos.z, MTXMODE_NEW);
    Matrix_Scale(0.35f, 0.35f, 0.35f, MTXMODE_APPLY);
    Matrix_RotateY(angle, MTXMODE_APPLY);
    Draw_Gi(play, this, gi, 0);
}

void EnElforg_DrawWrapper(Actor* this, PlayState* play)
{
    ActorCallback draw;

    switch (play->sceneId)
    {
    case SCE_MM_CLOCK_TOWN_EAST:
    case SCE_MM_LAUNDRY_POOL:
    case SCE_MM_TEMPLE_WOODFALL:
    case SCE_MM_TEMPLE_SNOWHEAD:
    case SCE_MM_TEMPLE_GREAT_BAY:
    case SCE_MM_TEMPLE_STONE_TOWER:
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        draw = EnElforg_Draw;
        break;
    default:
        draw = actorAddr(0x1b0, 0x80acd8c0);
        break;
    }
    draw(this, play);
}

void EnElforg_GiveItem(PlayState* play, Actor* this)
{
    ComboItemQuery q;
    ComboItemOverride o;

    EnElforg_ItemQuery(&q, this, play);
    comboItemOverride(&o, &q);
    PlayerDisplayTextBox(play, 0x579, NULL);
    comboAddItemEx(play, &q, 1);
    comboPlayItemFanfare(o.gi, 1);

    /* If it's a town fairy */
    if ((this->params & 0xF) == 3) {
        gMmExtraFlags2.townStrayFairy = 1;
    }
}

#include <combo/enemy_placement.h>
static void EnElforg_TrappedByReplacement(Actor* this, PlayState* play)
{
    Vec3f pos;
    int status = comboEnemyFairyHost(this, play, &pos);
    ActorCallback* action = (ActorCallback*)((u8*)this + 0x22c);
    if (status < 0)
    {
        ActorCallback setup = actorAddr(ACTOR_EN_ELFORG, 0x80acd6a8);
        setup(this, play);
        return;
    }
    if (status == 1 || status == 2)
        this->world.pos = this->home.pos = pos;
    if (status == 2)
    {
        ((void (*)(Actor*))actorAddr(ACTOR_EN_ELFORG, 0x80acc470))(this);
        *action = actorAddr(ACTOR_EN_ELFORG, 0x80acd2e4);
        this->draw = EnElforg_DrawWrapper;
        Actor_PlaySfx(this, NA_SE_EV_CHIBI_FAIRY_SAVED);
    }
    play->actorCtx.flags |= 8;
}
static void EnElforg_BindReplacement(Actor* this, PlayState* play)
{
    Vec3f pos;
    if (comboEnemyFairyHost(this, play, &pos) < 0)
    {
        ((ActorCallback)actorAddr(ACTOR_EN_ELFORG, 0x80acd6a8))(this, play);
        return;
    }
    *(ActorCallback*)((u8*)this + 0x22c) = EnElforg_TrappedByReplacement;
    EnElforg_TrappedByReplacement(this, play);
}

PATCH_CALL(0x80acc728, EnElforg_BindReplacement);
