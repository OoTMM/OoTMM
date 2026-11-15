#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/custom.h>
#include <combo/mask.h>
#include <combo/object.h>
#include <combo/player.h>
#include <combo/sfx.h>

typedef struct DancingRedead DancingRedead;
struct DancingRedead {
    Actor actor;
    Vec3s firePos[10];
    SkelAnime skelAnime;
    Vec3s jointTable[26];
    Vec3s morphTable[26];
    u8 grabState;
    u8 isMourning;
    u8 playerStunWaitTimer;
    u8 grabWaitTimer;
    void (*actionFunc)(DancingRedead*, PlayState*);
    s16 timer;
    s16 headYRotation;
    s16 upperBodyYRotation;
    s16 rdFlags;
    s16 alpha;
    s16 sunsSongStunTimer;
    u8 stunnedBySunsSong;
    u8 grabDamageTimer;
    u8 fireTimer;
    u8 action;
    u8 damageReaction;
    u8 unk31D;
    ColliderCylinder collider;
};
ASSERT_SIZE(DancingRedead, 0x35c);
ASSERT_OFFSET(DancingRedead, skelAnime, 0x178);
ASSERT_OFFSET(DancingRedead, actionFunc, 0x2f8);
ASSERT_OFFSET(DancingRedead, collider, 0x310);

#define REDEAD_ID 0x90
#define REDEAD_UPDATE 0x8093b9a0
#define REDEAD_SETUP_IDLE 0x80939d38
#define REDEAD_ACTION_DANCE 12

static void redeadIdle(DancingRedead* this)
{
    ((void (*)(DancingRedead*))actorAddr(REDEAD_ID, REDEAD_SETUP_IDLE))(this);
}

static int redeadCanDance(DancingRedead* this)
{
    return this->actor.colChkInfo.health && this->action != 1 &&
           this->action != 8 && this->action != 9 && this->action != 10 &&
           !(this->collider.base.acFlags & AC_HIT);
}

static void redeadDanceWait(DancingRedead* this, PlayState* play)
{
    SkelAnime_Update(&this->skelAnime);
}

static void redeadDance(DancingRedead* this, PlayState* play)
{
    SkelAnime_Update(&this->skelAnime);
    Math_SmoothStepToS(&this->headYRotation, 0, 1, 0x64, 0);
    Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, 0x64, 0);
    this->isMourning = 0;
    if ((play->gameplayFrames & 0x5f) == 0)
        Actor_PlaySfx(&this->actor, NA_SE_EN_REDEAD_CRY);

    if ((this->actor.params & 0xff) == 7)
    {
        if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame))
            this->timer = 0x1112;
        else if (Animation_OnFrame(&this->skelAnime, 15.f))
            this->timer = 0x199a;
        this->actor.world.rot.y -= this->timer;
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }
}

void EnRd_DanceUpdate(Actor* actor, PlayState* play)
{
    static AnimationHeader* const dances[] = {
        (AnimationHeader*)CUSTOM_OBJECT_REDEAD_DANCES_0,
        (AnimationHeader*)CUSTOM_OBJECT_REDEAD_DANCES_1,
        (AnimationHeader*)CUSTOM_OBJECT_REDEAD_DANCES_2,
    };
    DancingRedead* this = (DancingRedead*)actor;
    u16 type = actor->params & 0xff;
    u32 savedSegment = gSegments[0x0a];

    if (type >= 5 && type <= 7)
    {
        int masked = GET_PLAYER(play)->mask == MASK_SPOOKY;
        int dancing = this->actionFunc == redeadDance || this->actionFunc == redeadDanceWait;
        if (!masked && dancing)
            redeadIdle(this);
        else if (masked && redeadCanDance(this))
        {
            void* data = comboGetObject(CUSTOM_OBJECT_ID_REDEAD_DANCES);
            if (data)
            {
                gSegments[0x0a] = (u32)data & 0x1fffffff;
                if (this->actionFunc != redeadDance)
                {
                    Animation_MorphToLoop(&this->skelAnime, dances[type - 5], -6.f);
                    this->timer = 0x1112;
                    this->actionFunc = redeadDance;
                }
            }
            else
            {
                redeadIdle(this);
                this->actionFunc = redeadDanceWait;
            }
            this->action = REDEAD_ACTION_DANCE;
            actor->speed = 0.f;
            actor->world.rot.y = actor->shape.rot.y;
            this->isMourning = 0;
        }
        else if (dancing)
        {
            redeadIdle(this);
        }
    }
    ((ActorFunc)actorAddr(REDEAD_ID, REDEAD_UPDATE))(actor, play);
    gSegments[0x0a] = savedSegment;
}

PATCH_VROM_U32(0xcd9678, EnRd_DanceUpdate);
PATCH_VROM_U32(0xcd99d8, 0);
EXPORT_SYMBOL(ENEMY_NATIVE_DANCING_REDEAD_OOT_V1, EnRd_DanceUpdate);
