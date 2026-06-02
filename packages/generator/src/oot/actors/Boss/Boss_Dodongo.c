#include <combo.h>
#include <actors/Custom_En_Bom/Custom_En_Bom.h>
#include <actors/En_Clear_Tag/En_Clear_Tag.h>

static u8 sHasTakenKegDamage = 0;
static s16 sRemainingKegTimer = 0;

void BossDodongo_KillExplosive(Actor* actor)
{
    sHasTakenKegDamage = 0;
    sRemainingKegTimer = 0;
    if (actor->id == ACTOR_CUSTOM_EN_BOM)
    {
        EnBomMM* bomb = (EnBomMM*) actor;
        sHasTakenKegDamage = bomb->isPowderKeg;
        sRemainingKegTimer = bomb->timer;
    }
    Actor_Kill(actor);
}

PATCH_CALL(0x8089e050, BossDodongo_KillExplosive)

void BossDodongo_AnimateEatingBomb(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames)
{
    if (sHasTakenKegDamage && sRemainingKegTimer > 35)
    {
        mode = ANIMMODE_LOOP;
    }
    Animation_Change(skelAnime, animation, playSpeed, startFrame, endFrame, mode, morphFrames);
}

PATCH_CALL(0x8089f074, BossDodongo_AnimateEatingBomb)

void BossDodongo_AfterSetupExplode(Actor* this)
{
    if (sHasTakenKegDamage)
    {
        s16* actionTimer = (s16*)(((u8*)this)+0x1ca);
        if (sRemainingKegTimer > 35) {
            *actionTimer = sRemainingKegTimer;
        }
    }
}

typedef void (*BossDodongo_SetupDeathCutscene)(Actor*);

void BossDodongo_TakeExplosionDamage(Actor* this, PlayState* play, s16 quakeY, s16 quakeDuration)
{
    s16* health = (s16*)(((u8*)this)+0x184);
    if (sHasTakenKegDamage)
    {
        quakeY = 5;
        quakeDuration = 15;
        *health = 0;
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->world.pos.x, this->world.pos.y + 20.0f, this->world.pos.z, 0, 0, 0, CLEAR_TAG_LARGE_EXPLOSION);
        BossDodongo_SetupDeathCutscene BossDodongo_SetupDeathCutscene = actorAddr(ACTOR_BOSS_DODONGO, 0x808a1b78);
        BossDodongo_SetupDeathCutscene(this);
        Enemy_StartFinishingBlow(play, this);
    }
    else
    {
        *health -= 2;
        if (*health <= 0)
        {
            *health = 1;
        }
    }
    Actor_RequestQuakeAndRumble(this, play, quakeY, quakeDuration);
}

PATCH_CALL(0x8089f58c, BossDodongo_TakeExplosionDamage)

void BossDodongo_MovePlayerDuringCutscene(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep)
{
    f32 remainingDistance = Math_SmoothStepToF(pValue, target, fraction, step, minStep);
    if (remainingDistance > 100.0f)
    {
        remainingDistance = Math_SmoothStepToF(pValue, target, fraction, step, minStep);
    }
    if (remainingDistance > 250.0f)
    {
        remainingDistance = Math_SmoothStepToF(pValue, target, fraction, step, minStep);
    }
    if (remainingDistance > 500.0f)
    {
        Math_SmoothStepToF(pValue, target, fraction, step, minStep);
    }
}

PATCH_CALL(0x808a1d98, BossDodongo_MovePlayerDuringCutscene)
PATCH_CALL(0x808a1dc4, BossDodongo_MovePlayerDuringCutscene)
