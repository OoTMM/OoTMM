#ifndef COMBO_ENEMY_PORT_MOBLIN_H
#define COMBO_ENEMY_PORT_MOBLIN_H

#include <combo.h>

struct EnMb;

typedef void (*EnMbActionFunc)(struct EnMb*, struct PlayState*);

typedef enum EnMbState {
    ENMB_STATE_SPEAR_SPEARPATH_DAMAGED,
    ENMB_STATE_CLUB_DEAD,
    ENMB_STATE_CLUB_KNEELING_DAMAGED,
    ENMB_STATE_CLUB_KNEELING,
    ENMB_STATE_STUNNED = 5,
    ENMB_STATE_IDLE,
    ENMB_STATE_WALK = 9,
    ENMB_STATE_ATTACK,
    ENMB_STATE_ATTACK_END
} EnMbState;

typedef struct EnMb {
    Actor actor;
    Vec3s bodyPartsPos[10];
    u8 damageEffect;
    SkelAnime skelAnime;
    Vec3s jointTable[28];
    Vec3s morphTable[28];
    EnMbState state;
    EnMbActionFunc actionFunc;
    s16 iceEffectTimer;
    s16 timer1;
    s16 timer2;
    s16 timer3;
    s16 attack;
    u8 chargeHit;
    Vec3f effSpawnPos;
    f32 maxHomeDist;
    f32 playerDetectionRange;
    ColliderCylinder bodyCollider;
    ColliderQuad attackCollider;
    ColliderTris frontShieldingCollider;
    ColliderTrisElement frontShieldingColliderElements[2];
} EnMb;

#endif
