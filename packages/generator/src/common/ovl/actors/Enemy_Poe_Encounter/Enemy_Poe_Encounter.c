#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/enemy_soul_preview.h>

#if defined(GAME_OOT)
#define POE_SISTER_ID 0x91
#else
#define POE_SISTER_ID 0x1e8
#endif

typedef struct {
    Actor actor;
    EnemyFamily wave;
    u8 phase, spawned, delay;
} PoeEncounter;

static void encounterInit(Actor* actor, PlayState* play)
{
    PoeEncounter* encounter = (PoeEncounter*)actor;
    actor->flags = ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    actor->colChkInfo.health = 1;
    encounter->wave.flags = ENEMY_FAMILY_NO_CLEAR;
}

static void encounterDestroy(Actor* actor, PlayState* play)
{
    comboEnemyFamilyClear(&((PoeEncounter*)actor)->wave);
}

static void encounterUpdate(Actor* actor, PlayState* play)
{
    PoeEncounter* encounter = (PoeEncounter*)actor;
    static const u16 forms[3][2] = { { 0x300, 0 }, { 0x200, 0x100 }, { 0, 0 } };
    u8 count = encounter->phase == 1 ? 2 : 1;
    u8 complete = (1 << count) - 1;
    if (encounter->delay)
    {
        --encounter->delay;
        return;
    }
    if (encounter->spawned != complete)
    {
        for (u8 i = 0; i < count; ++i)
        {
            if (encounter->spawned & (1 << i))
                continue;
            Vec3f pos = actor->home.pos;
            if (count == 2)
            {
                s16 yaw = actor->shape.rot.y + (i ? 0x4000 : -0x4000);
                pos.x += 50.f * Math_SinS(yaw);
                pos.z += 50.f * Math_CosS(yaw);
            }
            if (!comboEnemyFindFloor(play, actor, &pos, 30.f, 50, 0))
                comboEnemyFindFloor(play, actor, &pos, 30.f, 150, 0);
            ActorEntry entry = { .id = POE_SISTER_ID, .params = forms[encounter->phase][i],
                .pos = { pos.x, pos.y, pos.z } };
#if defined(GAME_MM)
            entry.rot = (Vec3s){ 7, 0x7f, 0x7f };
#endif
            if (comboEnemyFamilySpawn(&encounter->wave, &play->actorCtx, &entry, play, 13))
                encounter->spawned |= 1 << i;
            else
            {
                encounter->wave.blocked = 0;
                encounter->delay = 15;
                return;
            }
        }
        return;
    }
    if (!comboEnemyFamilyDone(&encounter->wave))
        return;
    if (++encounter->phase == 3)
    {
        actor->colChkInfo.health = 0;
        Actor_Kill(actor);
        return;
    }
    memset(&encounter->wave, 0, sizeof(encounter->wave));
    encounter->wave.flags = ENEMY_FAMILY_NO_CLEAR;
    encounter->spawned = 0;
    encounter->delay = 20;
}

ActorProfile Poe_Encounter_Profile = {
    ACTOR_ENEMY_POE_ENCOUNTER, ACTORCAT_ENEMY, ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    1, sizeof(PoeEncounter), encounterInit, encounterDestroy, encounterUpdate, NULL,
};
OVL_INFO_ACTOR(ACTOR_ENEMY_POE_ENCOUNTER, Poe_Encounter_Profile);
