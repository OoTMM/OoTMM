#include <combo.h>

typedef struct
{
    u16 npc;
    s16 gi;
    u16 event;
}
BlueWarpData;

static const BlueWarpData kBlueWarpData[] = {
    { NPC_OOT_BLUE_WARP_GOHMA,          GI_OOT_STONE_EMERALD,       EV_OOT_CHK_STONE_EMERALD },
    { NPC_OOT_BLUE_WARP_KING_DODONGO,   GI_OOT_STONE_RUBY,          EV_OOT_CHK_STONE_RUBY },
    { NPC_OOT_BLUE_WARP_BARINADE,       GI_OOT_STONE_SAPPHIRE,      EV_OOT_CHK_STONE_SAPPHIRE },
    { NPC_OOT_BLUE_WARP_PHANTOM_GANON,  GI_OOT_MEDALLION_FOREST,    EV_OOT_CHK_MEDALLION_FOREST },
    { NPC_OOT_BLUE_WARP_VOLVAGIA,       GI_OOT_MEDALLION_FIRE,      EV_OOT_CHK_MEDALLION_FIRE },
    { NPC_OOT_BLUE_WARP_MORPHA,         GI_OOT_MEDALLION_WATER,     EV_OOT_CHK_MEDALLION_WATER },
    { NPC_OOT_BLUE_WARP_BONGO_BONGO,    GI_OOT_MEDALLION_SHADOW,    EV_OOT_CHK_MEDALLION_SHADOW },
    { NPC_OOT_BLUE_WARP_TWINROVA,       GI_OOT_MEDALLION_SPIRIT,    EV_OOT_CHK_MEDALLION_SPIRIT },
};

static const BlueWarpData* DoorWarp1_GetData(GameState_Play* play)
{
    int id;

    id = -1;
    switch (play->sceneId)
    {
    case SCE_OOT_LAIR_GOHMA:
        id = 0;
        break;
    case SCE_OOT_LAIR_KING_DODONGO:
        id = 1;
        break;
    case SCE_OOT_LAIR_BARINADE:
        id = 2;
        break;
    case SCE_OOT_LAIR_PHANTOM_GANON:
        id = 3;
        break;
    case SCE_OOT_LAIR_VOLVAGIA:
        id = 4;
        break;
    case SCE_OOT_LAIR_MORPHA:
        id = 5;
        break;
    case SCE_OOT_LAIR_BONGO_BONGO:
        id = 6;
        break;
    case SCE_OOT_LAIR_TWINROVA:
        id = 7;
        break;
    }

    if (id < 0)
        return NULL;
    return &kBlueWarpData[id];
}

void DoorWarp1_AfterDrawWarp(Actor* this, GameState_Play* play)
{
    static const int kRotDivisor = 100;
    float angle;
    const BlueWarpData* data;
    s16 gi;

    if (this->variable & 0xff00)
        return;
    data = DoorWarp1_GetData(play);
    if (!data)
        return;
    if (GetEventChk(data->event))
        return;

    angle = (play->gs.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;
    gi = comboOverride(OV_NPC, 0, data->npc, data->gi);

    ModelViewTranslate(this->position.x, this->position.y + 35.f, this->position.z, MAT_SET);
    ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
    ModelViewRotateY(angle, MAT_MUL);
    comboDrawGI(play, this, gi, DRAW_RAW);
}
