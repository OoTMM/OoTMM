#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/enemy_bodybreak.h>
#include <combo/enemy_native.h>
#include <combo/enemy_port.h>
extern void enemyFlame(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16, s16, s16, s16, s16, s16, s16, s16, s16, s32, s16);
extern void comboEnemyKnockbackLargeNoDamage(PlayState*, Actor*, float, s16, float);

static void enemyBodyPartUpdate(Actor* actor, PlayState* play) {
    EnemyBodyPart* part = (EnemyBodyPart*)actor;
    Actor_MoveWithGravity(actor);
    if (!part->action) {
        part->action = 1;
        actor->world.rot.y = Rand_ZeroOne() * 20000.f;
        if (actor->params == 1) {
            part->timer = (s16)(Rand_ZeroOne() * 17.f) + 5;
            actor->velocity.y = Rand_ZeroOne() * 5.f + 4.f;
            actor->gravity = -.6f - Rand_ZeroOne() * .5f;
        } else {
            if (actor->params == 11) {
                EnemyNativeSegments scope = { 0 };
                if (comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN)) {
                    ((void (*)(PlayState*, Actor*, Vec3f*, s16, s16, s16, s16))0x800b2df4)(
                        play, actor, &actor->world.pos, 40, 0x8001, 0, -1);
                    comboEnemyNativeLeave(&scope);
                }
            }
            actor->speed = (Rand_ZeroOne() - .5f) * 3.f;
            part->timer = (s16)(Rand_ZeroOne() * 17.f) + 10;
            actor->velocity.y = Rand_ZeroOne() * 3.f + 8.f;
            actor->gravity = -.6f - Rand_ZeroOne() * .3f;
        }
        part->rotZSpeed = .15f;
        return;
    }
    if (part->timer <= 0) {
        Vec3f zero = { 0 };
        enemyFlame(play, &actor->world.pos, &zero, &zero, (s16)(actor->scale.y * 100.f) * 40, 7, 255, 255, 255, 255, 0,
                   actor->params == 1 ? 255 : 0, actor->params == 1 ? 0 : 255, 1, 9, 1);
        Actor_Kill(actor);
        return;
    }
    part->timer--;
    part->rotZ += part->rotZSpeed;
}

static void enemyBodyPartDraw(Actor* actor, PlayState* play) {
    EnemyBodyPart* part = (EnemyBodyPart*)actor;
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_RotateZ(part->rotZ, MTXMODE_APPLY);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    ((void (*)(Actor*, PlayState*, s32))0x800b8050)(actor, play, 0);
    if (part->displayList) {
        gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, part->displayList);
    }
    CLOSE_DISPS();
}

static void moblinDust(PlayState* play, Vec3f* origin, float spread, s32 count, s16 scale, s16 step, u8 lighting) {
    Vec3f velocity = { 0, 0, 0 }, acceleration = { 0, 0.3f, 0 };
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    for (s32 i = count; i >= 0; --i) {
        Vec3f pos;
        pos.x = origin->x + (Rand_ZeroOne() - 0.5f) * spread;
        pos.y = origin->y + (Rand_ZeroOne() - 0.5f) * spread;
        pos.z = origin->z + (Rand_ZeroOne() - 0.5f) * spread;
        s16 size = (s16)(scale * Rand_ZeroOne() * 0.2f) + scale;
        ((void (*)(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16))(lighting ? 0x800b1210 : 0x800b11a0))(
            play, &pos, &velocity, &acceleration, size, step);
    }
    comboEnemyNativeLeave(&scope);
}

typedef struct {
    Actor actor;
    Vec3f origin;
    s16 timer;
    u8 active;
} MoblinWave;

static void moblinWaveInit(Actor* actor, PlayState* play) {
    if (actor->params != -1)
        return;
    MoblinWave* wave = (MoblinWave*)actor;
    Actor* owner = actor->parent;
    wave->origin = owner ? owner->world.pos : actor->world.pos;
    if (owner && owner->child == actor)
        owner->child = NULL;
    actor->parent = NULL;
    wave->timer = 20;
    wave->active = 0;
    actor->speed = 18.0f;
    actor->gravity = 0;
    actor->velocity.y = 0;
}

static void moblinWaveDestroy(Actor* actor, PlayState* play) {
    if (actor->params == -1)
        comboEnemySfxStopPosition(&actor->projectedPos, 0x30e0);
}

static void moblinWaveDraw(Actor* actor, PlayState* play) {
    if (actor->params != -1)
        enemyBodyPartDraw(actor, play);
}

static void moblinWaveUpdate(Actor* actor, PlayState* play) {
    if (actor->params != -1) {
        enemyBodyPartUpdate(actor, play);
        return;
    }
    MoblinWave* wave = (MoblinWave*)actor;
    Player* player = GET_PLAYER(play);
    Actor_MoveWithGravity(actor);
    Actor_UpdateBgCheckInfo(play, actor, 5, 15, 0, UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_4);
    if (!wave->active) {
        wave->active = 1;
        return;
    }
    if (--wave->timer == 0) {
        Actor_Kill(actor);
        return;
    }
    if (actor->xyzDistToPlayerSq <= 40.0f * 40.0f) {
        s8 invincibility = player->invincibilityTimer;
        if (player->invincibilityTimer <= 0) {
            if (player->invincibilityTimer > -40) {
                player->invincibilityTimer = 0;
                play->damagePlayer(play, -8);
            } else
                player->invincibilityTimer = 0;
        }
        float distance = Math_Vec3f_DistXZ(&wave->origin, &player->actor.world.pos);
        comboEnemyKnockbackLargeNoDamage(play, actor, (650 - distance) * 0.04f + 4, actor->world.rot.y, 8);
        player->invincibilityTimer = invincibility;
        wave->timer = 1;
    }
    moblinDust(play, &actor->world.pos, 0, 1, 300, 150, 1);
    EnemyNativeSegments scope = { 0 };
    if (comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN)) {
        Vec3f velocity = { Rand_CenteredFloat(16), 8, 0 }, acceleration = { 0, -1.5f, 0 };
        EffectSsHahen_Spawn(play, &actor->world.pos, &velocity, &acceleration, 20, (s32)((Rand_ZeroOne() * 5 + 12) * 2),
                            -1, 10, NULL);
        comboEnemyNativeLeave(&scope);
    }
    comboEnemySfxPosition(&actor->projectedPos, 0x30e0);
}

_Static_assert(sizeof(MoblinWave) == sizeof(EnemyBodyPart), "shared helper actor allocation");
static ActorProfile sMoblinWaveProfile = {
    0xc07,
    ACTORCAT_ITEMACTION,
    ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(MoblinWave),
    moblinWaveInit,
    moblinWaveDestroy,
    moblinWaveUpdate,
    moblinWaveDraw,
};

OVL_INFO_NATIVE_ENEMY(7, sMoblinWaveProfile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PART_INIT, moblinWaveInit);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PART_DESTROY, moblinWaveDestroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PART_UPDATE, moblinWaveUpdate);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PART_DRAW, moblinWaveDraw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PART_PROFILE, sMoblinWaveProfile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 7, 1, 0xd9d46a12 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PART_ORIGIN, kNativePortOrigin);
