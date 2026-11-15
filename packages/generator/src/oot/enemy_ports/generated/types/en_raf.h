#include <combo.h>

typedef enum CarnivorousLilyPadLimb {
    CARNIVOROUS_LILY_PAD_LIMB_NONE,
    CARNIVOROUS_LILY_PAD_LIMB_FLOWER,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_LOWER_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_MIDDLE_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_UPPER_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_LOWER_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_MIDDLE_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_UPPER_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_LOWER_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_MIDDLE_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT,
    CARNIVOROUS_LILY_PAD_LIMB_ROOTS,
    CARNIVOROUS_LILY_PAD_LIMB_MAX
} CarnivorousLilyPadLimb;
#ifndef Z_EN_RAF_H
#define Z_EN_RAF_H

#define CARNIVOROUS_LILY_GET_TYPE(thisx) (((thisx)->params >> 12) & 0xF)
#define CARNIVOROUS_LILY_GET_REVIVE_TIMER(thisx) (((thisx)->params >> 7) & 0x1F)
#define CARNIVOROUS_LILY_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)

#define CARNIVOROUS_LILY_SWITCH_FLAG_NONE 0x7F

typedef enum CarnivorousLilyType {
    CARNIVOROUS_LILY_TYPE_NORMAL,
    CARNIVOROUS_LILY_TYPE_DORMANT,
    CARNIVOROUS_LILY_TYPE_NO_WATER_INTERACTIONS
} CarnivorousLilyType;

struct EnRaf;

typedef void (*EnRafActionFunc)(struct EnRaf*, PlayState*);

typedef struct {
    u8 isEnabled;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    Vec3s rot;
    f32 scale;
    s16 timer;
} CarnivorousLilyEffect;

#define CARNIVOROUS_LILY_EFFECT_COUNT 31

typedef struct EnRaf {
    DynaPolyActor dyna;
    SkelAnime skelAnime;
    Vec3s jointTable[CARNIVOROUS_LILY_PAD_LIMB_MAX];
    Vec3s morphTable[CARNIVOROUS_LILY_PAD_LIMB_MAX];
    EnRafActionFunc actionFunc;
    Vec3f limbScale[CARNIVOROUS_LILY_PAD_LIMB_MAX];
    Vec3f targetLimbScale[CARNIVOROUS_LILY_PAD_LIMB_MAX];
    Vec3s chewLimbRot[CARNIVOROUS_LILY_PAD_LIMB_MAX];
    s16 grabTarget;
    u8 isCurrentlyInRidingMovingState;
    f32 animEndFrame;
    f32 chewScale;
    f32 bobOffset;
    f32 heightDiffFromPlayer;
    f32 bobPhase;
    s16 timer;
    s16 dissolveTimer;
    s16 rippleTimer;
    s16 reviveTimer;
    s16 playerRotYWhenGrabbed;
    s16 mainType;
    s16 switchFlag;
    s16 petalScaleType;
    s16 chewCount;
    s16 action;
    s16 petalClearPixelFirstPassIndex;
    s16 teethClearPixelFirstPassIndex;
    s16 petalClearPixelSecondPassIndex;
    s16 teethClearPixelSecondPassIndex;
    ColliderCylinder collider;
    CarnivorousLilyEffect effects[CARNIVOROUS_LILY_EFFECT_COUNT];
} EnRaf;

#endif
