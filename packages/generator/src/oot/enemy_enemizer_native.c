#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/data/enemy-spawn.h>
#include <combo/dma.h>
#include <combo/enemy_oot_enemizer.h>
#include <combo/enemy.h>
#include <combo/global.h>

#ifndef NativeEnemizerKnockback
# define NativeEnemizerKnockback ((void (*)(PlayState*, Actor*, float, s16, float))0x80022ec4)
#endif
void comboOotEnemizerFlareUpdate(Actor* actor, PlayState* play)
{
    ((ActorFunc)actorAddr(0x99, 0x80956378))(actor, play);
    Actor_UpdateBgCheckInfo(play, actor, 100.f, 100.f, 100.f, 3);
}

void comboOotEnemizerFlareKnockback(PlayState* play, Actor* actor, float speed, s16 rot, float yVelocity)
{
    Actor* player = &GET_PLAYER(play)->actor;
    if (*(Actor**)((u8*)actor + 0x188) == player)
    {
        Actor_PlaySfx(player, 0x83e);
        NativeEnemizerKnockback(play, actor, speed, rot, yVelocity);
    }
    else
        *(s16*)((u8*)actor + 0x4b4) = 0;
}
EXPORT_SYMBOL(ENEMY_OOT_ENEMIZER_FLARE_UPDATE_V1, comboOotEnemizerFlareUpdate);
EXPORT_SYMBOL(ENEMY_OOT_ENEMIZER_FLARE_KNOCKBACK_V1, comboOotEnemizerFlareKnockback);

int comboOotEnemizerCanSpawn(PlayState* play, const ActorEntry* entry)
{
    if (!play || !entry)
        return 0;
    for (u32 i = 0; i < ARRAY_COUNT(kEnemyOotRuntimeSpawnRules); ++i)
    {
        const EnemyOotRuntimeSpawnRule* rule = &kEnemyOotRuntimeSpawnRules[i];
        if (play->sceneId == rule->scene && play->roomCtx.curRoom.num == rule->room)
        {
            float delta = GET_PLAYER(play)->actor.world.pos.y - (float)entry->pos.y;
            return delta >= -rule->maxHeightDifference && delta <= rule->maxHeightDifference;
        }
    }
    return 1;
}

int comboOotEnemizerReDeadSwitch(PlayState* play, int flag)
{
    return flag == 0x7f ? 1 : Flags_GetSwitch(play, flag);
}

static u8 sLizalfosDefeated[64];

void comboOotEnemizerResetLizalfos(void)
{
    memset(sLizalfosDefeated, 0, sizeof(sLizalfosDefeated));
}

static u32 sOotEnemizerSwitchVrom __attribute__((section(".data"))) = 0;
void comboOotEnemizerDefeated(PlayState* play, u32 key)
{
    DmaEntry file;
    u32 header[4], row[3];
    if (!play || !sOotEnemizerSwitchVrom || !comboDmaLookup(&file, sOotEnemizerSwitchVrom) ||
        file.vstart != sOotEnemizerSwitchVrom || file.pend || file.vend < file.vstart ||
        comboDmaLoadFilePartial(header, sOotEnemizerSwitchVrom, 0, sizeof(header)) != sizeof(header) ||
        header[0] != 0x454e4f53 || header[1] != 1 || header[2] != 12 || header[3] > 2048 ||
        file.vend - file.vstart != 16 + header[3] * 12)
        return;
    for (u32 i = 0; i < header[3]; ++i)
    {
        if (comboDmaLoadFilePartial(row, sOotEnemizerSwitchVrom, 16 + i * 12, 12) != 12)
            return;
        if (row[0] == key && (row[1] >> 16) == play->sceneId && (row[2] & 0xff) < 64 && (row[2] >> 8) <= 2)
        {
            u32 flag = row[2] & 0xff;
            u32 member = row[2] >> 8;
            if (member)
            {
                sLizalfosDefeated[flag] |= 1u << (member - 1);
                if (sLizalfosDefeated[flag] != 3)
                    return;
            }
            Flags_SetSwitch(play, flag);
            return;
        }
    }
}
EXPORT_SYMBOL(ENEMY_OOT_ENEMIZER_FLOOR_ADMISSION_V1, comboOotEnemizerCanSpawn);
EXPORT_SYMBOL(ENEMY_OOT_ENEMIZER_REDEAD_SWITCH_V1, comboOotEnemizerReDeadSwitch);
EXPORT_SYMBOL(ENEMY_OOT_ENEMIZER_SWITCHES_V1, comboOotEnemizerDefeated);
EXPORT_SYMBOL(ENEMY_OOT_ENEMIZER_SWITCHES, sOotEnemizerSwitchVrom);

void comboOotEnemizerActorKill(Actor* actor)
{
    u32 key;
    if (sOotEnemizerSwitchVrom && gPlay && actor &&
        !(actor->id >= 0xc00 && actor->id < 0x1000) &&
        comboEnemyActorPlacementKey(actor, gPlay, &key))
        comboOotEnemizerDefeated(gPlay, key);
    actor->draw = NULL;
    actor->update = NULL;
    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
}
PATCH_FUNC(0x80020eb4, comboOotEnemizerActorKill);
EXPORT_SYMBOL(ENEMY_OOT_ENEMIZER_KILL_V1, comboOotEnemizerActorKill);

/* Only authored combat timers in rooms that actually received replacements.
 * Silver-rupee/puzzle timers retain their own temporary-clear producer. */
static u32 sCombatTimerRooms[101] __attribute__((section(".data")));
EXPORT_SYMBOL(ENEMY_OOT_COMBAT_TIMER_ROOMS_V1, sCombatTimerRooms);

int comboEnemyOotCombatRoomTimer(PlayState* play, s32 room)
{
    return play && play->sceneId < ARRAY_COUNT(sCombatTimerRooms) && room >= 0 && room < 32 &&
           (sCombatTimerRooms[play->sceneId] & (1u << room));
}

static s32 shuffledRoomTimerCleared(PlayState* play, s32 room)
{
    if (room < 0 || room >= 32)
        return 0;
    if (!comboEnemyOotCombatRoomTimer(play, room))
        return (play->actorCtx.flags.tempClear & (1u << room)) != 0;
    if (room != play->roomCtx.curRoom.num ||
        g.roomEnemyLackSoul || comboEnemyEncounterPending(room))
        return 0;

    static const u8 categories[] = { ACTORCAT_ENEMY, ACTORCAT_BOSS };
    for (u32 i = 0; i < ARRAY_COUNT(categories); ++i)
    {
        u8 category = categories[i];
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor->update && (actor->room < 0 || actor->room == room))
                return 0;
    }
    return 1;
}

EXPORT_SYMBOL(ENEMY_OOT_ROOMTIMER_CLEAR, shuffledRoomTimerCleared);
