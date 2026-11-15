#include <combo.h>
#include <combo/custom.h>
#include <combo/enemy_wiz_reflected_bolt.h>
static ColliderCylinderInit sCollider = {
    { COL_MATERIAL_NONE, AT_ON | AT_TYPE_PLAYER, AC_NONE, OC1_NONE, OC2_NONE, COLSHAPE_CYLINDER },
    { ELEM_MATERIAL_UNK0, { 0x20, 0, 2 }, { 0, 0, 0 }, ATELEM_ON | ATELEM_SFX_NORMAL, ACELEM_NONE, OCELEM_NONE },
    { 25, 25, -5, { 0, 0, 0 } },
};

static void WizBolt_Init(Actor* actor, PlayState* play)
{
    WizReflectedBolt* bolt = (WizReflectedBolt*)actor;
    Collider_InitCylinder(play, &bolt->collider);
    Collider_SetCylinder(play, &bolt->collider, actor, &sCollider);
    bolt->timer = 100;
    bolt->tailTimer = 12;
    WizProjectile_ResetTrail(&bolt->trail, &actor->world.pos);
    Actor_SetScale(actor, .01f);
    if (!comboEnemySceneObject(CUSTOM_OBJECT_ID_WIZ_REFLECTED_BOLT))
        Actor_Kill(actor);
}

static void WizBolt_Destroy(Actor* actor, PlayState* play)
{
    Collider_DestroyCylinder(play, &((WizReflectedBolt*)actor)->collider);
}

static void WizBolt_Update(Actor* actor, PlayState* play)
{
    WizReflectedBolt* bolt = (WizReflectedBolt*)actor;
    f32 scale = actor->scale.x;
    if (!bolt->timer)
    {
        if (scale > 0.f)
        {
            Math_ApproachF(&scale, 0.f, .2f, .01f);
            Actor_SetScale(actor, scale < .001f ? 0.f : scale);
        }
        else if (!--bolt->tailTimer)
        {
            Actor_Kill(actor);
        }
        return;
    }

    Math_ApproachF(&scale, .01f, .2f, .01f);
    Actor_SetScale(actor, scale);
    Vec3f next = { actor->world.pos.x + actor->velocity.x, actor->world.pos.y + actor->velocity.y,
                   actor->world.pos.z + actor->velocity.z };
    Vec3f impact;
    CollisionPoly* poly;
    s32 bgId;
    if (BgCheck_EntityLineTest1(&play->colCtx, &actor->world.pos, &next, &impact, &poly, 1, 1, 1, 0, &bgId))
    {
        actor->world.pos = impact;
        WizProjectile_RecordTrail(&bolt->trail, &impact);
        bolt->timer = 0;
        return;
    }
    actor->world.pos = next;
    WizProjectile_RecordTrail(&bolt->trail, &next);
    --bolt->timer;
    bolt->collider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    Collider_UpdateCylinder(actor, &bolt->collider);
    CollisionCheck_SetAT(play, &play->colChkCtx, &bolt->collider.base);
}

static void WizBolt_Draw(Actor* actor, PlayState* play)
{
    WizReflectedBolt* bolt = (WizReflectedBolt*)actor;
    void* model = comboEnemySceneObject(CUSTOM_OBJECT_ID_WIZ_REFLECTED_BOLT);
    if (!model)
        return;
    GraphicsContext* gfx = play->state.gfxCtx;
    int first = actor->scale.x > 0.f ? 0 : 6 - (bolt->tailTimer + 1) / 2;
    Gfx_SetupDL25_Xlu(gfx);
    OPEN_DISPS(gfx);
    gSPSegment(POLY_XLU_DISP++, 6, model);
    Matrix_Push();
    POLY_XLU_DISP = WizProjectile_DrawTrail(POLY_XLU_DISP, play, &bolt->trail, CUSTOM_OBJECT_WIZ_REFLECTED_BOLT_0,
                                            actor->scale.x, first, actor->params != 0);
    Matrix_Pop();
    gSPSegment(POLY_XLU_DISP++, 6, gSegments[6] + K0BASE);
    CLOSE_DISPS();
}

static ActorProfile sWizBoltProfile = {
    ACTOR_ENEMY_WIZ_REFLECTED_BOLT,
    ACTORCAT_PROP,
    ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(WizReflectedBolt),
    WizBolt_Init,
    WizBolt_Destroy,
    WizBolt_Update,
    WizBolt_Draw,
};
OVL_INFO_ACTOR(ACTOR_ENEMY_WIZ_REFLECTED_BOLT, sWizBoltProfile);
