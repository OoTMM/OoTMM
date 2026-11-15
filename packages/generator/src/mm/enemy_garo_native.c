#include <combo.h>
#include <combo/enemy_garo.h>
#include <combo/enemy_native.h>
#define NativeGaroSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
static int sGaroSelected;
void comboEnemyNativeGaroIntro(EnJso* actor)
{
    if (sGaroSelected)
    {
        actor->csId = -1;
        return;
    }
    void (*intro)(EnJso*) = actorAddr(0x113, 0x809adc7c);
    if (intro)
        intro(actor);
}

static void garoKneel(EnJso* actor, PlayState* play)
{
    if (actor->timer)
        return;
    static const Vec3f directions[9] = { { 1, 0, .5f }, { 1, 0, -.5f }, { -1, 0, .5f }, { -1, 0, -.5f }, { .5f, 0, 1 }, { -.5f, 0, 1 }, { .5f, 0, -1 }, { -.5f, 0, -1 }, { 0, 0, 0 } };
    void (*flame)(PlayState*, Vec3f*, const Vec3f*, const Vec3f*, s16, s16, u8) = (void*)0x800b3030;
    void (*sound)(PlayState*, Vec3f*, s32, u16) = (void*)0x800f0590;
    int i;
    for (i = 0; i < 9; ++i)
    {
        Vec3f pos = actor->actor.world.pos;
        pos.x += Rand_CenteredFloat(30.f);
        pos.z += Rand_CenteredFloat(30.f);
        if (actor->actor.floorHeight > -30000.f)
            pos.y = actor->actor.floorHeight;
        flame(play, &pos, &directions[i], &directions[i], 100 + Rand_ZeroFloat(100.f), 20, 1);
    }
    sound(play, &actor->actor.world.pos, 10, 0x321f);
    Actor_Kill(&actor->actor);
}

static void garoIdle(EnJso* garo, PlayState* play)
{
    garo->actor.speed = 0.f;
    if (comboEnemyGaroInRange(&garo->actor, play, 450.f, 50.f))
    {
        ((void (*)(EnJso*, PlayState*))actorAddr(0x113, 0x809ae9b0))(garo, play);
        garo->attackTimer = 20;
    }
    else if (!garo->disableAnimations)
    {
        SkelAnime_Update(&garo->skelAnime);
    }
}

static void garoUpdate(Actor* actor, PlayState* play)
{
    EnJso* garo = (EnJso*)actor;
    ActorFunc update = actorAddr(0x113, 0x809b02cc);
    EnJsoActionFunc dead = actorAddr(0x113, 0x809af99c);
    if (garo->action == 2 && garo->actionFunc != garoIdle && comboEnemyGaroCanRest(garo, play))
    {
        ((void (*)(EnJso*, s32))actorAddr(0x113, 0x809adbc8))(garo, 1);
        actor->speed = 0.f;
        garo->actionFunc = garoIdle;
    }
    if (!actor->colChkInfo.health && garo->actionFunc == dead && !garo->timer)
    {
        void (*anim)(EnJso*, s32) = actorAddr(0x113, 0x809adbc8);
        s32* attacking = actorAddr(0x113, 0x809b0f40);
        s32* locked = actorAddr(0x113, 0x809b0f44);
        if (anim)
            anim(garo, 11);
        if (garo->isAttacking && attacking)
            *attacking = 0;
        if (garo->isPlayerLockedOn && locked)
            *locked = 0;
        garo->isAttacking = garo->isPlayerLockedOn = 0;
        garo->timer = 25;
        garo->actionFunc = garoKneel;
    }
    if (update)
        update(actor, play);
}

static void garoFini(Actor* actor, PlayState* play)
{
    EnJso* garo = (EnJso*)actor;
    ActorFunc fini = actorAddr(0x113, 0x809adb24);
    if (garo->isAttacking)
        *(s32*)actorAddr(0x113, 0x809b0f40) = 0;
    if (garo->isPlayerLockedOn)
        *(s32*)actorAddr(0x113, 0x809b0f44) = 0;
    fini(actor, play);
}

static void garoActivate(Actor* actor, PlayState* play)
{
    EnJso* garo = (EnJso*)actor;
    void (*circle)(EnJso*, PlayState*) = actorAddr(0x113, 0x809ae9b0);
    if (!actor->update || !circle)
        return;
    u32 previousSegment = gSegments[6];
    Actor_SetObjectDependency(play, actor);
    if (!gSegments[6])
    {
        gSegments[6] = previousSegment;
        return;
    }
    actor->parent = NULL;
    actor->flags &= ~((1u << 20) | (1u << 27));
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED | 5;
    actor->shape.yOffset = 970.f;
    actor->shape.shadowScale = 17.f;
    garo->scale = .035f;
    garo->swordState = 0;
    garo->attackTimer = 20;
    actor->draw = actorAddr(0x113, 0x809b0bb0);
    circle(garo, play);
    gSegments[6] = previousSegment;
    actor->update = garoUpdate;
    actor->fini = garoFini;
}

static void garoInit(Actor* actor, PlayState* play)
{
    ActorFunc init = actorAddr(0x113, 0x809ad8e0);
    int previous = sGaroSelected;
    actor->init = NULL;
    sGaroSelected = 1;
    if (init)
        init(actor, play);
    sGaroSelected = previous;
    garoActivate(actor, play);
}

Actor* comboEnemyNativeGaroSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    int previous;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != 0x113 || entry->params != 0)
        return NULL;
    previous = sGaroSelected;
    sGaroSelected = 1;
    actor = NativeGaroSpawnEntry(ctx, entry, play);
    sGaroSelected = previous;
    if (actor && actor->init)
        actor->init = garoInit;
    else if (actor)
        garoActivate(actor, play);
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_GARO_V1, comboEnemyNativeGaroSpawn);
EXPORT_SYMBOL(ENEMY_NATIVE_GARO_INTRO_V1, comboEnemyNativeGaroIntro);

PATCH_VROM_U32(0xe23eac, 0);
