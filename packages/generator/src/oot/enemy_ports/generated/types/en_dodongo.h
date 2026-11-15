#ifndef Z_EN_DODONGO_H
#define Z_EN_DODONGO_H

struct EnDodongo;

typedef void (*EnDodongoActionFunc)(struct EnDodongo*, PlayState*);

typedef enum DodongoBodyPart {
    DODONGO_BODYPART_0,
    DODONGO_BODYPART_1,
    DODONGO_BODYPART_2,
    DODONGO_BODYPART_3,
    DODONGO_BODYPART_4,
    DODONGO_BODYPART_5,
    DODONGO_BODYPART_6,
    DODONGO_BODYPART_7,
    DODONGO_BODYPART_8,
    DODONGO_BODYPART_MAX
} DodongoBodyPart;

typedef struct EnDodongo {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[OBJECT_DODONGO_LIMB_MAX];
    Vec3s morphTable[OBJECT_DODONGO_LIMB_MAX];
    EnDodongoActionFunc actionFunc;
    u8 drawDmgEffType;
    s16 timer;
    s16 unk_304;
    s16 unk_306;
    Vec3f unk_308;
    Vec3f unk_314;
    Vec3f unk_320;
    Color_RGBA8 unk_32C;
    Color_RGBA8 unk_330;
    f32 unk_334;
    s32 unk_338;
    f32 drawDmgEffAlpha;
    f32 drawDmgEffScale;
    f32 drawDmgEffFrozenSteamScale;
    Vec3f bodyPartsPos[DODONGO_BODYPART_MAX];
    ColliderJntSph collider1;
    ColliderJntSphElement collider1Elements[10];
    ColliderJntSph collider2;
    ColliderJntSphElement collider2Elements[3];
    ColliderJntSph collider3;
    ColliderJntSphElement collider3Elements[3];
} EnDodongo;

#endif
