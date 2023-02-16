#include <combo.h>

static s16 EnElforg_Item(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = GI_MM_STRAY_FAIRY;
    switch (play->sceneId)
    {
    case SCE_MM_CLOCK_TOWN_EAST:
    case SCE_MM_LAUNDRY_POOL:
        gi = comboOverride(OV_NPC, 0, NPC_MM_STRAY_FAIRY_TOWN, gi);
        break;
    default:
        gi = comboOverride(OV_SF, play->sceneId, (this->variable & 0xfe00) >> 9, gi);
        break;
    }
    return gi;
}

static void EnElforg_Draw(Actor* this, GameState_Play* play)
{
    ActorCallback draw;
    static const int kRotDivisor = 100;
    float angle;
    s16 gi;

    gi = EnElforg_Item(this, play);
    if (gi == GI_MM_STRAY_FAIRY)
    {
        draw = actorAddr(0x1b0, 0x80acd8c0);
        draw(this, play);
        return;
    }

    angle = (play->gs.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;
    ModelViewTranslate(this->position.x, this->position.y, this->position.z, MAT_SET);
    ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
    ModelViewRotateY(angle, MAT_MUL);
    comboDrawGI(play, this, gi, 0);
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
    s16 gi;

    gi = EnElforg_Item(this, play);
    PlayerDisplayTextBox(play, 0x579, NULL);
    comboAddItem(play, gi);

    // If it's a town fairy
    if ((this->variable & 0xF) == 3) {
        gMmExtraFlags2.townStrayFairy = 1;
    }
}
