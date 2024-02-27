#ifndef COMBO_OOT_DEMO_EFFECT_H
#define COMBO_OOT_DEMO_EFFECT_H

#include <combo/common/actor.h>
#include <combo/common/actor_init.h>

typedef struct Actor_DemoEffect Actor_DemoEffect;

typedef void (*DemoEffectFunc)(Actor_DemoEffect*, GameState_Play*);

typedef struct {
    /* 0x00 */ u8 timer;
} DemoEffectFireBall;

typedef struct {
    /* 0x00 */ u8 alpha;
    /* 0x01 */ u8 scale;
    /* 0x02 */ u8 pad;
    /* 0x04 */ s16 rotation;
} DemoEffectBlueOrb;

typedef struct {
    /* 0x00 */ u8 alpha;
    /* 0x01 */ u8 scaleFlag;
    /* 0x02 */ u8 flicker;
    /* 0x04 */ s16 rotation;
} DemoEffectLight;

typedef struct {
    /* 0x00 */ u8 alpha;
} DemoEffectLgtShower;

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 lightRingSpawnDelay;
    /* 0x02 */ u8 rotation;
    /* 0x04 */ s16 lightRingSpawnTimer;
} DemoEffectGodLgt;

typedef struct {
    /* 0x00 */ u8 timerIncrement;
    /* 0x01 */ u8 alpha;
    /* 0x02 */ u8 pad;
    /* 0x04 */ s16 timer;
} DemoEffectLightRing;

typedef struct {
    /* 0x00 */ u8 triforceSpotOpacity;
    /* 0x01 */ u8 lightColumnOpacity;
    /* 0x02 */ u8 crystalLightOpacity;
    /* 0x04 */ s16 rotation;
} DemoEffectTriforceSpot;

typedef struct {
    /* 0x00 */ u8 isPositionInit;
    /* 0x01 */ u8 isLoaded;
    /* 0x02 */ u8 drawId;
    /* 0x04 */ s16 rotation;
} DemoEffectGetItem;

typedef struct {
    /* 0x00 */ u8 pad;
    /* 0x01 */ u8 pad2;
    /* 0x02 */ u8 pad3;
    /* 0x04 */ s16 shrinkTimer;
} DemoEffectTimeWarp;

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 isPositionInit;
    /* 0x02 */ u8 alpha;
    /* 0x04 */ s16 timer;
} DemoEffectJewel;

typedef struct {
    /* 0x00 */ u8 timer;
} DemoEffectDust;

typedef struct Actor_DemoEffect {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ SkelCurve skelCurve;
    /* 0x016C */ u8 requiredObjectSlot;
    /* 0x0170 */ Gfx* jewelDisplayList;
    /* 0x0174 */ Gfx* jewelHolderDisplayList;
    /* 0x0178 */ u8 primXluColor[3];
    /* 0x017B */ u8 envXluColor[3];
    /* 0x017E */ u8 primOpaColor[3];
    /* 0x0181 */ u8 envOpaColor[3];
    /* 0x0184 */ union {
        DemoEffectFireBall fireBall;
        DemoEffectBlueOrb blueOrb;
        DemoEffectLight light;
        DemoEffectLgtShower lgtShower;
        DemoEffectGodLgt godLgt;
        DemoEffectLightRing lightRing;
        DemoEffectTriforceSpot triforceSpot;
        DemoEffectGetItem getItem;
        DemoEffectTimeWarp timeWarp;
        DemoEffectJewel jewel;
        DemoEffectDust dust;
    };
    /* 0x018A */ s16 effectFlags;
    /* 0x018C */ s16 cueChannel;
    /* 0x018E */ Vec3s jewelCsRotation;
    /* 0x0194 */ DemoEffectFunc initUpdateFunc;
    /* 0x0198 */ ActorFunc initDrawFunc;
    /* 0x019C */ DemoEffectFunc updateFunc;
} Actor_DemoEffect; /* size = 0x01A0 */

/* These names come from the objects that correspond to this actor type. */
#define DEMO_EFFECT_CRYSTAL_LIGHT               0x00
#define DEMO_EFFECT_FIRE_BALL                   0x01
/* Object is in GAMEPLAY_KEEP. Not a name from object. It's a blue orb */
#define DEMO_EFFECT_BLUE_ORB                    0x02
#define DEMO_EFFECT_LGT_SHOWER                  0x03
#define DEMO_EFFECT_GOD_LGT_DIN                 0x04
#define DEMO_EFFECT_GOD_LGT_NAYRU               0x05
#define DEMO_EFFECT_GOD_LGT_FARORE              0x06
#define DEMO_EFFECT_LIGHTRING_EXPANDING         0x07
#define DEMO_EFFECT_TRIFORCE_SPOT               0x08
#define DEMO_EFFECT_MEDAL_FIRE                  0x09
#define DEMO_EFFECT_MEDAL_WATER                 0x0A
#define DEMO_EFFECT_MEDAL_FOREST                0x0B
#define DEMO_EFFECT_MEDAL_SPIRIT                0x0C
#define DEMO_EFFECT_MEDAL_SHADOW                0x0D
#define DEMO_EFFECT_MEDAL_LIGHT                 0x0E
#define DEMO_EFFECT_TIMEWARP_MASTERSWORD        0x0F
#define DEMO_EFFECT_LIGHTRING_SHRINKING         0x10
#define DEMO_EFFECT_LIGHTRING_TRIFORCE          0x11
#define DEMO_EFFECT_LIGHT                       0x12
#define DEMO_EFFECT_JEWEL_KOKIRI                0x13
#define DEMO_EFFECT_JEWEL_GORON                 0x14
#define DEMO_EFFECT_JEWEL_ZORA                  0x15
/* Object is jewel, but this is really the dust in the ToT light arrow cutscene */
#define DEMO_EFFECT_DUST                        0x16
#define DEMO_EFFECT_LIGHTARROW                  0x17
#define DEMO_EFFECT_TIMEWARP_TIMEBLOCK_LARGE    0x18
#define DEMO_EFFECT_TIMEWARP_TIMEBLOCK_SMALL    0x19
#define DEMO_EFFECT_MAX_TYPE                    0x1A

#define DEMO_EFFECT_LIGHT_RED       0x00
#define DEMO_EFFECT_LIGHT_BLUE      0x01
#define DEMO_EFFECT_LIGHT_GREEN     0x02
#define DEMO_EFFECT_LIGHT_ORANGE    0x03
#define DEMO_EFFECT_LIGHT_YELLOW    0x04
#define DEMO_EFFECT_LIGHT_PURPLE    0x05
#define DEMO_EFFECT_LIGHT_GREEN2    0x06

#define GOD_LGT_DIN     0x00
#define GOD_LGT_NAYRU   0x01
#define GOD_LGT_FARORE  0x02

/* params info */
/* type: (params & 0x00FF) */
/* light size: ((params & 0x0F00) >> 8) */
/* light color: ((params & 0xF000) >> 12) */

#endif
