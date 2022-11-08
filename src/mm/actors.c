#include <combo.h>

static void spawnBossReward(void* const_1, GameState_Play* play, u16 gi, float x, float y, float z)
{
    SpawnActor(const_1, play, AC_ITEM_CUSTOM, x, y, z, 0, 0, 0x1000 | 0x1e, gi);;
}

Actor* hookSpawnActorEx(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int unk1, int unk2, int unk3)
{
    switch (actorId)
    {
    case AC_DOOR_WARP1:
        switch (play->sceneId)
        {
        case SCE_MM_LAIR_ODOLWA:
            spawnBossReward(const_1, play, GI_MM_REMAINS_ODOLWA, x, y, z);
            break;
        case SCE_MM_LAIR_GOHT:
            spawnBossReward(const_1, play, GI_MM_REMAINS_GOHT, x, y, z);
            break;
        case SCE_MM_LAIR_GYORG:
            spawnBossReward(const_1, play, GI_MM_REMAINS_GYORG, x, y, z);
            break;
        }
        break;
    case AC_ITEM_B_HEART:
        actorId = AC_ITEM_CUSTOM;
        rz = 0x1000 | 0x1f;
        variable = GI_MM_HEART_CONTAINER;
        break;
    }
    return SpawnActorEx(const_1, play, actorId, x, y, z, rx, ry, rz, variable, unk1, unk2, unk3);
}

int EnGo_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnDnh_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnShn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnPm_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b);
int EnAn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnPst_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnNb_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnAl_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnBjt_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);

static int Actor_ByteCode_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (actor->id)
    {
    case 0x138:
        return EnGo_GiveItem(actor, play, gi, a, b);
    case 0x168:
        return EnDnh_GiveItem(actor, play, gi, a, b);
    case 0x1c5:
        return EnShn_GiveItem(actor, play, gi, a, b);
    case 0x1d5:
        return EnPm_GiveItem(actor, play, gi, a, b);
    case 0x1f2:
        return EnPst_GiveItem(actor, play, gi, a, b);
    case 0x202:
        return EnAn_GiveItem(actor, play, gi, a, b);
    case 0x243:
        return EnNb_GiveItem(actor, play, gi, a, b);
    case 0x262:
        return EnAl_GiveItem(actor, play, gi, a, b);
    case 0x27d:
        return EnBjt_GiveItem(actor, play, gi, a, b);
    default:
        return GiveItem(actor, play, gi, a, b);
    }
}

PATCH_CALL(0x8010aa34, Actor_ByteCode_GiveItem);

void EnGo_AfterGivingItem(Actor* actor);
void EnDnh_AfterGivingItem(Actor* actor);
void EnPm_AfterGivingItem(Actor* actor);
void EnAn_AfterGivingItem(Actor* actor);
void EnNb_AfterGivingItem(Actor* actor);

static int Actor_ByteCode_HasParent(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
    {
        switch (actor->id)
        {
        case 0x138:
            EnGo_AfterGivingItem(actor);
            break;
        case 0x168:
            EnDnh_AfterGivingItem(actor);
            break;
        case 0x1d5:
            EnPm_AfterGivingItem(actor);
            break;
        case 0x202:
            EnAn_AfterGivingItem(actor);
            break;
        case 0x243:
            EnNb_AfterGivingItem(actor);
            break;
        }
    }
    return ret;
}

PATCH_CALL(0x8010a9f8, Actor_ByteCode_HasParent);
