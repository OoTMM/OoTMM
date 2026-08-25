#ifndef COMBO_MM_EN_DEATH_H
#define COMBO_MM_EN_DEATH_H

#include <combo/actor.h>
#include <combo/common/z_anim_mat.h>
#include <combo/mm/actors/En_Minideath.h>

typedef enum GomessSkelLimb {
    /* 0x00 */ GOMESS_LIMB_NONE,
    /* 0x01 */ GOMESS_LIMB_1,
    /* 0x02 */ GOMESS_LIMB_CORE_POS,
    /* 0x03 */ GOMESS_LIMB_LOWER_BODY,
    /* 0x04 */ GOMESS_LIMB_TORSO,
    /* 0x05 */ GOMESS_LIMB_COLLAR,
    /* 0x06 */ GOMESS_LIMB_HEAD_LOWER,
    /* 0x07 */ GOMESS_LIMB_HEAD_UPPER,
    /* 0x08 */ GOMESS_LIMB_8,
    /* 0x09 */ GOMESS_LIMB_9,
    /* 0x0A */ GOMESS_LIMB_10,
    /* 0x0B */ GOMESS_LIMB_11,
    /* 0x0C */ GOMESS_LIMB_CLOAK_UPPER,
    /* 0x0D */ GOMESS_LIMB_CLOAK_LOWER,
    /* 0x0E */ GOMESS_LIMB_UPPER_ARM_LEFT,
    /* 0x0F */ GOMESS_LIMB_FOREARM_LEFT,
    /* 0x10 */ GOMESS_LIMB_HAND_LEFT,
    /* 0x11 */ GOMESS_LIMB_UPPER_ARM_RIGHT,
    /* 0x12 */ GOMESS_LIMB_FOREARM_RIGHT,
    /* 0x13 */ GOMESS_LIMB_HAND_RIGHT,
    /* 0x14 */ GOMESS_LIMB_SCYTHE_HANDLE,
    /* 0x15 */ GOMESS_LIMB_SCYTHE_BLADE,
    /* 0x16 */ GOMESS_LIMB_MAX
} GomessSkelLimb;

struct EnDeath;

typedef void (*EnDeathActionFunc)(struct EnDeath*, PlayState*);

typedef struct EnDeath {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnDeathActionFunc actionFunc;
    /* 0x18C */ u8 unk_18C;
    /* 0x18D */ u8 coreGuarded;
    /* 0x18E */ u8 holdsScythe;
    /* 0x18F */ u8 dmgEffect;
    /* 0x190 */ s8 numScytheAfterImages;
    /* 0x191 */ u8 matAnimStep; // animated material color step?
    /* 0x192 */ u8 inEarlyIntro;
    /* 0x193 */ u8 noDrawLimbs[GOMESS_LIMB_MAX];
    /* 0x1A9 */ u8 flameAlphas[56];
    /* 0x1E2 */ Vec3s jointTable[GOMESS_LIMB_MAX];
    /* 0x266 */ Vec3s morphTable[GOMESS_LIMB_MAX];
    /* 0x2EA */ s16 floatTimer;
    /* 0x2EC */ s16 weaponColliderLastUpdateTime;
    /* 0x2EE */ s16 actionTimer;
    /* 0x2F0 */ s16 unk_2F0;
    /* 0x2F2 */ s16 cloakUpperRotationModifier;
    /* 0x2F4 */ s16 cloakLowerRotationModifier;
    /* 0x2F6 */ s16 coreRotation;
    /* 0x2F8 */ s16 explosiveDamageTimer;
    /* 0x2FA */ s16 camId;
    /* 0x2FC */ s16 lightArrowDamageTimer;
    /* 0x300 */ s32 effectIndex;
    /* 0x304 */ f32 scytheScale;
    /* 0x308 */ f32 coreVelocity;
    /* 0x30C */ f32 camEyeSpeed;
    /* 0x310 */ f32 camAtSpeed;
    /* 0x314 */ f32 dmgEffectScale;
    /* 0x318 */ f32 dmgEffectSteamScale;
    /* 0x31C */ f32 dmgEffectAlpha;
    /* 0x320 */ Vec3f corePos;
    /* 0x32C */ Vec3f scytheWorldPos;
    /* 0x338 */ Vec3f scytheScreenPos;
    /* 0x344 */ Vec3f camEyeTarget;
    /* 0x350 */ Vec3f camAtTarget;
    /* 0x35C */ Vec3f bodyPartsPos[14];
    /* 0x404 */ Vec3f sparklePositions[56];
    /* 0x6A4 */ MtxF scytheMtxF;
    /* 0x6E4 */ EnMinideath* miniDeaths[20];
    /* 0x734 */ AnimatedMaterial* bodyMatAnim;
    /* 0x738 */ AnimatedMaterial* coreMatAnim;
    /* 0x73C */ ColliderCylinder bodyCollider;
    /* 0x788 */ ColliderQuad weaponCollider;
    /* 0x808 */ ColliderSphere coreCollider;
    /* 0x860 */ ColliderTris weaponSpinningCollider;
    /* 0x880 */ ColliderTrisElement weaponSpinningColliderElements[2];
} EnDeath; // size = 0x938

#endif
