#ifndef COMBO_ENEMY_GARO_H
#define COMBO_ENEMY_GARO_H

#include <combo.h>

typedef enum GaroLimb {
    GARO_LIMB_NONE,
    GARO_LIMB_ROOT,
    GARO_LIMB_TORSO,
    GARO_LIMB_LEFT_ARM,
    GARO_LIMB_LEFT_SWORD,
    GARO_LIMB_RIGHT_ARM,
    GARO_LIMB_RIGHT_SWORD,
    GARO_LIMB_ROBE_TOP,
    GARO_LIMB_ROBE_BACK,
    GARO_LIMB_ROBE_LEFT,
    GARO_LIMB_ROBE_RIGHT,
    GARO_LIMB_ROBE_FRONT,
    GARO_LIMB_HEAD,
    GARO_LIMB_LOWER_BODY_ROOT,
    GARO_LIMB_RIGHT_THIGH,
    GARO_LIMB_RIGHT_SHIN,
    GARO_LIMB_RIGHT_FOOT,
    GARO_LIMB_LEFT_THIGH,
    GARO_LIMB_LEFT_SHIN,
    GARO_LIMB_LEFT_FOOT,
    GARO_LIMB_MAX
} GaroLimb;

struct EnJso;

#define EN_JSO_AFTERIMAGE_COUNT 20

typedef void (*EnJsoActionFunc)(struct EnJso*, PlayState*);

typedef enum EnJsoHintType {
    EN_JSO_HINT_FREEZE_OCTOROKS,
    EN_JSO_HINT_FLATS_LOCATION,
    EN_JSO_HINT_VARIABLE_1,
    EN_JSO_HINT_VARIABLE_2,
    EN_JSO_HINT_VARIABLE_3,
    EN_JSO_HINT_VARIABLE_4,
    EN_JSO_HINT_DANCING_REDEADS,
    EN_JSO_HINT_DESTRUCTIBLE_CEILING_IN_CASTLE,
    EN_JSO_HINT_SECOND_ENTRANCE_TO_CASTLE,
    EN_JSO_HINT_KING_WEAK_TO_MIRROR_SHIELD
} EnJsoHintType;

typedef enum EnJsoBodyPart {
    EN_JSO_BODYPART_LEFT_SWORD,
    EN_JSO_BODYPART_RIGHT_SWORD,
    EN_JSO_BODYPART_ROBE_TOP,
    EN_JSO_BODYPART_ROBE_BACK,
    EN_JSO_BODYPART_ROBE_LEFT,
    EN_JSO_BODYPART_ROBE_RIGHT,
    EN_JSO_BODYPART_ROBE_FRONT,
    EN_JSO_BODYPART_HEAD,
    EN_JSO_BODYPART_RIGHT_THIGH,
    EN_JSO_BODYPART_RIGHT_FOOT,
    EN_JSO_BODYPART_LEFT_THIGH,
    EN_JSO_BODYPART_LEFT_FOOT,
    EN_JSO_BODYPART_MAX
} EnJsoBodyPart;

typedef struct EnJso {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[GARO_LIMB_MAX];
    Vec3s morphTable[GARO_LIMB_MAX];
    EnJsoActionFunc actionFunc;
    s16 action;
    s16 circlingAngle;
    s16 circlingAngularVelocity;
    s16 attackMovementTimer;
    u8 unk_284[0x2];
    s16 attackTimer;
    s16 timer;
    s16 isGuarding;
    s16 isPlayerLockedOn;
    s16 isAttacking;
    Vec3s robeRightRot;
    Vec3s targetRobeRightRot;
    Vec3s rightArmRot;
    Vec3s targetRightArmRot;
    s16 drawDmgEffAlpha;
    s16 drawDmgEffType;
    f32 drawDmgEffScale;
    f32 drawDmgEffFrozenSteamScale;
    Vec3f bodyPartsPos[EN_JSO_BODYPART_MAX];
    s16 bodyPartIndex;
    s16 disableAnimations;
    s16 textIndex;
    s16 hintType;
    u8 disableBlure;
    u8 swordState;
    u8 slashHitSomething;
    f32 animEndFrame;
    f32 scale;
    u8 unk_358[0x4];
    ColliderCylinder bodyCollider;
    ColliderQuad rightSwordCollider;
    ColliderQuad leftSwordCollider;
    s32 rightSwordBlureIndex;
    s32 leftSwordBlureIndex;
    s16 afterimageIndex;
    s32 afterimageCount;
    s16 csId;
    u32 cutsceneTimer;
    s16 cutsceneState;
    s16 subCamId;
    u8 unk_4C4[0x4];
    Vec3f subCamEye;
    Vec3f subCamAt;
    Vec3f subCamUp;
    Vec3f subCamEyeNext;
    Vec3f subCamAtNext;
    u8 unk_504[0x10];
    s16 introCsType;
    Vec3f afterimagePos[EN_JSO_AFTERIMAGE_COUNT];
    Vec3s afterimageRot[EN_JSO_AFTERIMAGE_COUNT];
    Vec3s afterimageJointTable[EN_JSO_AFTERIMAGE_COUNT][GARO_LIMB_MAX];
    Vec3f knockbackVelocity;
#if defined(GAME_OOT)
    struct {
        Vec3f pos, velocity;
        Vec3s rot;
        Gfx* display;
        s16 life;
    } swords[2];
#endif
} EnJso;

static inline int comboEnemyGaroInRange(Actor* actor, PlayState* play, f32 range, f32 height)
{
    if (actor->isTargeted)
        return 1;
    return actor->xzDistToPlayer < range &&
        fabsf(GET_PLAYER(play)->actor.world.pos.y - actor->world.pos.y) < height;
}

static inline int comboEnemyGaroCanRest(EnJso* garo, PlayState* play)
{
    return garo->actor.colChkInfo.health && !garo->isAttacking &&
        (garo->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
        !(garo->bodyCollider.base.acFlags & AC_HIT) &&
        !comboEnemyGaroInRange(&garo->actor, play, 600.f, 60.f);
}

#if defined(GAME_MM)
_Static_assert(sizeof(EnJso) == 0xfec, "native Garo size");
_Static_assert(__builtin_offsetof(EnJso, actionFunc) == 0x278, "native Garo action");
#endif

#endif
