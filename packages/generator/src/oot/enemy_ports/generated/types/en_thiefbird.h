#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_THIEFBIRD_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_THIEFBIRD_H

#include <combo.h>

#ifndef TAKKURI_LIMB_BODY
#define TAKKURI_LIMB_BODY 1
#endif
#ifndef TAKKURI_LIMB_EYES
#define TAKKURI_LIMB_EYES 13
#endif
#ifndef TAKKURI_LIMB_FEET
#define TAKKURI_LIMB_FEET 15
#endif
#ifndef TAKKURI_LIMB_HEAD
#define TAKKURI_LIMB_HEAD 9
#endif
#ifndef TAKKURI_LIMB_LEFT_EAR
#define TAKKURI_LIMB_LEFT_EAR 11
#endif
#ifndef TAKKURI_LIMB_LEFT_WING_BASE
#define TAKKURI_LIMB_LEFT_WING_BASE 2
#endif
#ifndef TAKKURI_LIMB_LEFT_WING_MIDDLE
#define TAKKURI_LIMB_LEFT_WING_MIDDLE 3
#endif
#ifndef TAKKURI_LIMB_LEFT_WING_TIP
#define TAKKURI_LIMB_LEFT_WING_TIP 4
#endif
#ifndef TAKKURI_LIMB_LEGS
#define TAKKURI_LIMB_LEGS 14
#endif
#ifndef TAKKURI_LIMB_LOWER_BEAK
#define TAKKURI_LIMB_LOWER_BEAK 12
#endif
#ifndef TAKKURI_LIMB_MAX
#define TAKKURI_LIMB_MAX 17
#endif
#ifndef TAKKURI_LIMB_NECK
#define TAKKURI_LIMB_NECK 8
#endif
#ifndef TAKKURI_LIMB_NONE
#define TAKKURI_LIMB_NONE 0
#endif
#ifndef TAKKURI_LIMB_RIGHT_EAR
#define TAKKURI_LIMB_RIGHT_EAR 10
#endif
#ifndef TAKKURI_LIMB_RIGHT_WING_BASE
#define TAKKURI_LIMB_RIGHT_WING_BASE 5
#endif
#ifndef TAKKURI_LIMB_RIGHT_WING_MIDDLE
#define TAKKURI_LIMB_RIGHT_WING_MIDDLE 6
#endif
#ifndef TAKKURI_LIMB_RIGHT_WING_TIP
#define TAKKURI_LIMB_RIGHT_WING_TIP 7
#endif
#ifndef TAKKURI_LIMB_STOLEN_ITEM
#define TAKKURI_LIMB_STOLEN_ITEM 16
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_TakkuriLimb
#define COMBO_ENEMY_ASSET_ENUM_TakkuriLimb
typedef s32 TakkuriLimb;
#endif
#include <combo/common/actors/En_Item00.h>

typedef Actor_EnItem00 EnItem00;

struct EnThiefbird;

typedef void (*EnThiefbirdActionFunc)(struct EnThiefbird*, PlayState*);

typedef struct {
    Vec3f unk_00;
    Vec3f unk_0C;
    f32 unk_18;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20;
    s16 unk_22;
} EnThiefbirdUnkStruct;

typedef enum EnThiefbirdBodyPart {
    EN_THIEFBIRD_BODYPART_0,
    EN_THIEFBIRD_BODYPART_1,
    EN_THIEFBIRD_BODYPART_2,
    EN_THIEFBIRD_BODYPART_3,
    EN_THIEFBIRD_BODYPART_4,
    EN_THIEFBIRD_BODYPART_5,
    EN_THIEFBIRD_BODYPART_6,
    EN_THIEFBIRD_BODYPART_7,
    EN_THIEFBIRD_BODYPART_8,
    EN_THIEFBIRD_BODYPART_9,
    EN_THIEFBIRD_BODYPART_10,
    EN_THIEFBIRD_BODYPART_MAX
} EnThiefbirdBodyPart;

typedef struct EnThiefbird {
    Actor actor;
    SkelAnime skelAnime;
    EnThiefbirdActionFunc actionFunc;
    u8 drawDmgEffType;
    s16 unk_18E;
    s16 unk_190;
    s16 unk_192;
    s16 unk_194;
    s16 unk_196[6];
    Vec3s jointTable[TAKKURI_LIMB_MAX];
    Vec3s morphTable[TAKKURI_LIMB_MAX];
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[3];
    Vec3f bodyPartsPos[EN_THIEFBIRD_BODYPART_MAX];
    f32 drawDmgEffAlpha;
    f32 drawDmgEffScale;
    f32 drawDmgEffFrozenSteamScale;
    f32 unk_3E0;
    Gfx* unk_3E4;
    Gfx* unk_3E8;
    EnItem00* unk_3EC;
    EnThiefbirdUnkStruct unk_3F0[40];
} EnThiefbird;

#endif
