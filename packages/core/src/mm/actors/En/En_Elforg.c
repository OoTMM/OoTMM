#include <combo.h>
#include <combo/item.h>

static void EnElforg_ItemQuery(ComboItemQuery* q, Actor* this, GameState_Play* play)
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
        q->sceneId = comboSceneKey(play->sceneId);
        q->id = (this->variable & 0xfe00) >> 9;
        break;
    }
}

static void EnElforg_ItemOverride(ComboItemOverride* o, Actor* this, GameState_Play* play)
{
    ComboItemQuery q;

    EnElforg_ItemQuery(&q, this, play);
    comboItemOverride(o, &q);
}

static void EnElforg_Draw(Actor* this, GameState_Play* play)
{
    ComboItemOverride o;
    ActorCallback draw;
    static const int kRotDivisor = 100;
    float angle;

    EnElforg_ItemOverride(&o, this, play);
    if (o.gi == GI_MM_STRAY_FAIRY)
    {
        draw = actorAddr(0x1b0, 0x80acd8c0);
        draw(this, play);
        return;
    }

    angle = (play->gs.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;
    ModelViewTranslate(this->position.x, this->position.y, this->position.z, MAT_SET);
    ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
    ModelViewRotateY(angle, MAT_MUL);
    comboDrawGI(play, this, o.gi, 0);
}

void EnElforg_DrawWrapper(Actor* this, GameState_Play* play)
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

void EnElforg_GiveItem(GameState_Play* play, Actor* this)
{
    ComboItemQuery q;

    EnElforg_ItemQuery(&q, this, play);
    PlayerDisplayTextBox(play, 0x579, NULL);
    comboAddItemEx(play, &q, 1);

    // If it's a town fairy
    if ((this->variable & 0xF) == 3) {
        gMmExtraFlags2.townStrayFairy = 1;
    }
}
