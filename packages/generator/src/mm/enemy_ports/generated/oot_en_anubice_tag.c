#include "oot_compat.h"
#include "types/en_anubice_tag.h"
#undef ACTOR_EN_ANUBICE
#define ACTOR_EN_ANUBICE 0x00e0
#undef ACTOR_EN_ANUBICE_TAG
#define ACTOR_EN_ANUBICE_TAG 0x00f6
#undef OBJECT_GAMEPLAY_KEEP
#define OBJECT_GAMEPLAY_KEEP 0x1001

#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_SpawnAsChild
extern Actor* FOREIGN_ENEMY_API_SPAWN_CHILD(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId,
                                            f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_SpawnAsChild FOREIGN_ENEMY_API_SPAWN_CHILD
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 fraction, f32 step))0x800ffdf8)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x800fed44)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)

#include "types/en_anubice.h"
void comboAnubisMirrorX(float*, float, float, float);
void comboAnubisMirrorZ(float*, float, float, float);

#define FLAGS ACTOR_FLAG_UPDATE_CULLING_DISABLED

void EnAnubiceTag_Init(Actor* thisx, PlayState* play);
void EnAnubiceTag_Destroy(Actor* thisx, PlayState* play);
void EnAnubiceTag_Update(Actor* thisx, PlayState* play);
void EnAnubiceTag_Draw(Actor* thisx, PlayState* play);

void EnAnubiceTag_SpawnAnubis(EnAnubiceTag* this, PlayState* play);
void EnAnubiceTag_ManageAnubis(EnAnubiceTag* this, PlayState* play);

ActorProfile En_Anubice_Tag_Profile = {
    0xcf6,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnAnubiceTag),
    EnAnubiceTag_Init,
    EnAnubiceTag_Destroy,
    EnAnubiceTag_Update,
    EnAnubiceTag_Draw,
};

void EnAnubiceTag_Init(Actor* thisx, PlayState* play) {
    EnAnubiceTag* this = (EnAnubiceTag*)thisx;

    if (this->actor.params < -1) {
        this->actor.params = 0;
    }
    if (this->actor.params != 0) {
        this->extraTriggerRange = this->actor.params * 40.0f;
    }
    this->actionFunc = EnAnubiceTag_SpawnAnubis;
}

void EnAnubiceTag_Destroy(Actor* thisx, PlayState* play) {}

void EnAnubiceTag_SpawnAnubis(EnAnubiceTag* this, PlayState* play) {
    this->anubis = (EnAnubice*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_ANUBICE,
                                                  this->actor.world.pos.x, this->actor.world.pos.y,
                                                  this->actor.world.pos.z, 0, this->actor.yawTowardsPlayer, 0, 0);

    if (this->anubis != NULL) {
        this->actionFunc = EnAnubiceTag_ManageAnubis;
    }
}

void EnAnubiceTag_ManageAnubis(EnAnubiceTag* this, PlayState* play) {
    EnAnubice* anubis;
    Vec3f offset;

    Actor* child = NULL;
    for (int category = 0; category < ARRAY_COUNT(play->actorCtx.actors) && !child; ++category) {
        for (Actor* candidate = play->actorCtx.actors[category].first; candidate; candidate = candidate->next) {
            if (candidate == (Actor*)this->anubis) {
                child = candidate;
                break;
            }
        }
    }
    if (!child || child->id != 0xce0 || !child->update || child->parent != &this->actor) {
        this->anubis = NULL;
        this->actor.child = NULL;
        Actor_Kill(&this->actor);
        return;
    }
    anubis = (EnAnubice*)child;

    if (anubis->deathTimer != 0) {
        Actor_Kill(&this->actor);
        return;
    }

    if (this->actor.xzDistToPlayer < (200.0f + this->extraTriggerRange)) {
        if (!anubis->isPlayerOutOfRange && !anubis->isKnockedback) {
            anubis->isMirroringPlayer = true;
            offset.x = -Math_SinS(this->actor.yawTowardsPlayer) * this->actor.xzDistToPlayer;
            offset.z = -Math_CosS(this->actor.yawTowardsPlayer) * this->actor.xzDistToPlayer;
            comboAnubisMirrorX(&anubis->actor.world.pos.x, this->actor.world.pos.x + offset.x, 0.3f, 10.0f);
            comboAnubisMirrorZ(&anubis->actor.world.pos.z, this->actor.world.pos.z + offset.z, 0.3f, 10.0f);
        }
    } else if (anubis->isMirroringPlayer) {
        anubis->isPlayerOutOfRange = true;
    }
}

void EnAnubiceTag_Update(Actor* thisx, PlayState* play) {
    EnAnubiceTag* this = (EnAnubiceTag*)thisx;

    this->actionFunc(this, play);
}

void EnAnubiceTag_Draw(Actor* thisx, PlayState* play) {}

OVL_INFO_NATIVE_ENEMY(0xf6, En_Anubice_Tag_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_TAG_INIT, EnAnubiceTag_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_TAG_DESTROY, EnAnubiceTag_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_TAG_UPDATE, EnAnubiceTag_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_TAG_DRAW, EnAnubiceTag_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_TAG_PROFILE, En_Anubice_Tag_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0xf6, 0x1, 0x596432a8 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_TAG_ORIGIN, kNativePortOrigin);
