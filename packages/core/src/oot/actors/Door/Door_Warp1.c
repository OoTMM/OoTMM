#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/draw.h>

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

static const int DoorWarp1_GetID(GameState_Play* play)
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

    return id;
}

static const BlueWarpData* DoorWarp1_GetData(GameState_Play* play)
{
    int id;

    id = DoorWarp1_GetID(play);
    if (id < 0)
        return NULL;
    return &kBlueWarpData[id];
}

int DoorWarp1_Collide(Actor* this, GameState_Play* play)
{
    float dist;

    dist = this->xzDistToPlayer;
    if (dist < 0.f)
        dist = -dist;
    if (dist < 60.f)
    {
        dist = GET_PLAYER(play)->actor.world.pos.y - this->world.pos.y;
        if (dist < 0.f)
            dist = -dist;
        if (dist < 20.f)
            return 1;
    }
    return 0;
}

static s16 DoorWarp1_GetGI(const BlueWarpData* data)
{
    ComboItemQuery q = ITEM_QUERY_INIT;
    ComboItemOverride o;

    q.ovType = OV_NPC;
    q.id = data->npc;
    q.gi = data->gi;

    comboItemOverride(&o, &q);
    return o.gi;
}

int DoorWarp1_ShouldTrigger(Actor* this, GameState_Play* play)
{
    int id;
    const BlueWarpData* data;

    if (this->parent && this->parent->id != AC_PLAYER)
        this->parent = NULL;

    if (DoorWarp1_Collide(this, play))
    {
        id = DoorWarp1_GetID(play);
        if (id == -1)
            return 1;
        data = &kBlueWarpData[id];

        if (!GetEventChk(data->event))
        {
            if (Actor_HasParentZ(this))
            {
                SetEventChk(data->event);
            }
            else
            {
                comboGiveItemNpc(this, play, data->gi, data->npc, 16384.f, 16384.f);
            }
            return 0;
        }

        if ((GET_PLAYER(play)->state & 0x400) != 0)
            return 0;

        comboTriggerWarp(play, id);
    }
    return 0;
}

PATCH_FUNC(0x809056e8, DoorWarp1_ShouldTrigger);

void DoorWarp1_AfterDrawWarp(Actor* this, GameState_Play* play)
{
    static const int kRotDivisor = 100;
    float angle;
    const BlueWarpData* data;
    s16 gi;

    data = DoorWarp1_GetData(play);
    if (!data)
        return;
    if (GetEventChk(data->event))
        return;

    angle = (play->gs.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;
    gi = DoorWarp1_GetGI(data);

    ModelViewTranslate(this->world.pos.x, this->world.pos.y + 35.f, this->world.pos.z, MAT_SET);
    ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
    ModelViewRotateY(angle, MAT_MUL);
    Draw_Gi(play, this, gi, DRAW_RAW);
}
